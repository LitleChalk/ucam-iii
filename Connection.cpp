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

        if (!serial.open(QIODevice::ReadWrite))
            continue;
        serial.clear();
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
        QByteArray rx;
        if (readBytes(rx)){
            const uint8_t *data =
                reinterpret_cast<const uint8_t*>(rx.constData());

            qDebug() << "RX:" << rx.toHex(' ').toUpper();
            if (rx.size() >= 3)
            {
            if (data[0] == 0xAA &&
                data[1] == 0x55 &&
                data[2] == 0x81)
            {
                portIsOpen=true;
                qDebug() << "Подключение установлено";
                return true;
            }
            if (data[0] == 0xAA &&
                data[1] == 0x55)
            {
                portIsOpen=true;
                qDebug() << "Подключение установлено, камера не подключена";
                return true;
            }
            }
        }

        serial.close();
    }

    return false;
}

bool Connection::sendBytes(const uint8_t *data, int size)
{
    if (!serial.isOpen())
        return false;
    qDebug() << "отправляем1";
    serial.clear(QSerialPort::Input);
    qint64 written =
        serial.write(reinterpret_cast<const char*>(data), size);

    if (written != size)
        return false;

    qDebug() << "отправляем2";
    return serial.waitForBytesWritten(300);
}

bool Connection::readBytes(QByteArray &buffer)
{
    if (!serial.isOpen())
        return false;

    buffer.clear();

    if (!serial.waitForReadyRead(10500))
        return false;

    buffer += serial.readAll();
    while (serial.waitForReadyRead(200))
    {
        buffer += serial.readAll();
    }
    qDebug() << buffer.toHex(' ').toUpper();
    return !buffer.isEmpty();
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