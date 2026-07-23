#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QSerialPort>

class Connection : public QObject
{
    Q_OBJECT

public:
    explicit Connection(QObject *parent = nullptr);
    ~Connection();

    bool findDevice();

    bool sendBytes(const uint8_t *data, int size);

    QByteArray readBytes();
    std::vector<uint8_t> getData(const QByteArray &buffer);

    void close();

    bool portIsOpen;
signals:
    void portDisconnected();
    void errorMsg(uint8_t errorCode);
private slots:
    void handleError(QSerialPort::SerialPortError error);
private:
    QSerialPort serial;
};

#endif // CONNECTION_H
