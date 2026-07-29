#include "Connection.h"

#include <QSerialPortInfo>
#include <QDebug>

Connection::Connection(QObject *parent)
    : QObject(parent)
{
    portIsOpen=false;
    connect(&serial,
            &QSerialPort::errorOccurred,
            this,
            &Connection::handleError);
}

Connection::~Connection()
{
    close();
}

bool Connection::findDevice()
{
    const auto ports = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &info : ports)
    {
        qDebug() << "Checking" << info.portName();

        serial.setPort(info);

        serial.setBaudRate(QSerialPort::Baud115200);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);
        if (!serial.open(QIODevice::ReadWrite)){
            qDebug() << "Open failed:"
             << serial.error()
             << serial.errorString();
            continue;
        }
        serial.clear();
        //portIsOpen=true;
        //return true;
        uint8_t Handshake[] =
            {
                0xAA,
                0x01,
                0x00
            };
        if (!sendBytes(Handshake, 3))
        {
            serial.close();
            continue;
        }
        QByteArray rx = readBytes();
        if (!rx.isEmpty()){
            const uint8_t *data =
                reinterpret_cast<const uint8_t*>(rx.constData());

            //qDebug() << "RX:" << rx.toHex(' ').toUpper();
            if (rx.size() >= 6)
            {
            if (data[0] == 0xAA &&
                data[1] == 0x55)
            {
                portIsOpen=true;
                qDebug() << "Подключение установлено";
                if (data[2] == 0xEE)
                {
                    emit errorMsg(data[6]);
                }

                return true;
            }
            }
        }

        serial.close();
    }

    return false;
}
bool Connection::setResolution(const QString &resolution){
    uint8_t resolutionByte;
    if (resolution == "80x60")
        resolutionByte = 0x01;
    else if (resolution == "160x120")
        resolutionByte = 0x03;
    else if (resolution == "128x128")
        resolutionByte = 0x09;
    else if (resolution == "128x96")
        resolutionByte = 0x0B;
    else {
        qDebug() << "Неверное разрешение: " << resolution;
        emit errorMsg(0x00);
        return false;
    }
    uint8_t command[] =
        {
            0xAA,
            0x02,
            resolutionByte
        };
    sendBytes(command, 3);
    //if(sendBytes(command, 3))
        //qDebug() << "TX: AA 02 " << QString("%1").arg(resolutionByte, 2, 16, QChar('0')).toUpper();
    QByteArray rx = readBytes();
    if (!rx.isEmpty()){
        const uint8_t *data =
            reinterpret_cast<const uint8_t*>(rx.constData());
        //qDebug() << "RX:" << rx.toHex(' ').toUpper();
        if (rx.size() >= 6)
        {
            if (data[0] == 0xAA &&
                data[1] == 0x55)
            {
                if (data[2] == 0x82){
                    qDebug() << "Разрешение установлено";
                    return true;
                }
                if (data[2] == 0xEE)
                {
                    qDebug() << "Ошибка при установке разрешения" << data[6];
                    emit errorMsg(data[6]);
                    return false;
                }
            }
        }
    }
    emit errorMsg(0x00);
    return false;
}

bool Connection::sendBytes(const uint8_t *data, int size)
{
    if (!serial.isOpen())
        return false;
    QByteArray ba = QByteArray::fromRawData(reinterpret_cast<const char*>(data), size);
    qDebug() << "TX:" << ba.toHex(' ').toUpper();
    serial.clear(QSerialPort::Input);
    qint64 written =
        serial.write(reinterpret_cast<const char*>(data), size);

    if (written != size)
        return false;
    return serial.waitForBytesWritten(300);
}

QByteArray Connection::readBytes()
{
    if (!serial.isOpen())
        return {};

    if (!serial.waitForReadyRead(10500))
        return {};

    QByteArray buffer = serial.readAll();

    while (serial.waitForReadyRead(200))
    {
        buffer += serial.readAll();
    }

    qDebug() << "RX:" << buffer .toHex(' ').toUpper();

    return buffer;
}
std::vector<uint8_t> Connection::getData(const QByteArray &buffer){
    std::vector<uint8_t> photo;
    if (buffer.size() < 6)
        return {};
    //qDebug() << "1";
    if (static_cast<uint8_t>(buffer[2])==0xEE){
        emit errorMsg(static_cast<uint8_t>(buffer[6]));
        //qDebug() << "2";
        return {};
    }
    int index = 9;
    size_t currentSize=0;
    uint16_t length = static_cast<uint8_t>(buffer[4]) | (static_cast<uint8_t>(buffer[5]) << 8);
    uint32_t photoLength = static_cast<uint8_t>(buffer[6]) | (static_cast<uint8_t>(buffer[7]) << 8)| (static_cast<uint8_t>(buffer[8]) << 16)| (static_cast<uint8_t>(buffer[9]) << 24);
    photo.resize(photoLength);
    //qDebug() << "3";
    while(static_cast<uint8_t>(buffer[index+ 3])==0x84){
        //qDebug() << "4";
        length = static_cast<uint8_t>(buffer[index+5]) | (static_cast<uint8_t>(buffer[index+6]) << 8);
        memcpy(photo.data()+currentSize,
               buffer.constData() +  index + 7,
               length);
        currentSize+=length;
        index+=length+6;
        //qDebug() << "4_2";
    }
    //qDebug() << "5";
    if (currentSize!=photoLength){
        qDebug() << currentSize << photoLength;
        emit errorMsg(0x60);}
    //qDebug() << "6";
    return photo;
}

void Connection::close()
{
    if (serial.isOpen())
        serial.close();
}
void Connection::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError)
    {
        portIsOpen = false;

        emit portDisconnected();
    }
}