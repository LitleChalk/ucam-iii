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

        // Отправляем 0xAA
        uint8_t tx = 0xAA;

        if (serial.write(reinterpret_cast<char*>(&tx), 1) != 1)
        {
            serial.close();
            continue;
        }

        if (!serial.waitForBytesWritten(10))
        {
            serial.close();
            continue;
        }

        if (!serial.waitForReadyRead(300))
        {
            serial.close();
            continue;
        }

        QByteArray rx;

        while (serial.waitForReadyRead(100))
        {
            rx += serial.readAll();
        }

        //const uint8_t* data = reinterpret_cast<const uint8_t*>(rx.constData());


        if (!rx.isEmpty())
        {
            const uint8_t* data =
                reinterpret_cast<const uint8_t*>(rx.constData());

            qDebug() << "Received" << rx.size() << "bytes:";

            for (int i = 0; i < rx.size(); ++i)
            {
                qDebug().noquote()
                << QString("%1")
                        .arg(data[i], 2, 16, QChar('0'))
                        .toUpper();
            }
            portIsOpen=true;
            return true;
        }

        serial.close();
    }

    return false;
}

bool Connection::sendBytes(const uint8_t *data, int size)
{
    if (!serial.isOpen())
        return false;
    serial.clear(QSerialPort::Input);
    qint64 written =
        serial.write(reinterpret_cast<const char*>(data), size);

    if (written != size)
        return false;

    return serial.waitForBytesWritten(300);
}

bool Connection::readBytes(QByteArray &buffer)
{
    if (!serial.isOpen())
        return false;

    buffer.clear();

    // Ждем первый байт ответа
    if (!serial.waitForReadyRead(100))
        return false;

    // Забираем все, что пришло
    while (serial.waitForReadyRead(20))
    {
        buffer += serial.readAll();

    }

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