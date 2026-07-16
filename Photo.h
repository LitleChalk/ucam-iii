#ifndef PHOTO_H
#define PHOTO_H

#include <QString>
#include <chrono>

#include <random>
// Форматы сохраняемого изображения
enum class ImageFormat
{
    Raw,
    Gray8,
    CrYCbY,
    RGB565,
    JPEG
};

// Типы запросов к камере
enum class RequestType
{
    Capture,
    GetStatus,
    GetImage,
    Reset
};
class Photo
{
public:
    Photo(int id);
    ~Photo();

    bool displayPhoto();
    bool sendRequest(QString &requestType);

    // get
    int getId() const;
    int getCameraId() const;
    std::chrono::system_clock::time_point getTime() const;
    QString getBatch() const;
    bool getCap() const;
    bool getChanged() const;
    //set
    void setCameraId(int cameraId);
    void setBatch(const QString &batch);
    void setCap(bool cap);
    void setChanged(bool changed);

    // отправка запроса камере
    bool sendRequest(const QString &requestType);

    // ответ от камеры
    bool receiveResponse();

    // сохранение изображения.
    bool savePhoto(const QString &format,
                   const QString &resolution);

    // сохранение информации о фото
    bool saveToCsv();

private:

    int id;
    int camera_id;

    std::chrono::system_clock::time_point time;

    QString batch;

    bool cap;
    bool changed;


};
#include <vector>
#include <cstdint>
#include <cstring>
#include <QFile>
#include <QDir>

class RawImage
{
public:
    RawImage(uint16_t Width, uint16_t Height, uint8_t BytesPerPixel = 1);
    // Возвращает false, если пакет не помещается.
    bool addPacket(const uint8_t* data, size_t size);

    bool isComplete() const;

    bool SaveRawImage(const QString& path,
                      const QString& fileName,
                      const uint8_t* data,
                      size_t size);
    void GenerateRawImage();
    void GenerateRawImage2();
//private:
    uint16_t width{};
    uint16_t height{};
    uint8_t  bytesPerPixel{1};

    size_t expectedSize{};
    size_t received{};

    std::vector<uint8_t> buffer;
};
#endif // PHOTO_H
