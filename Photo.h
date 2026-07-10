#ifndef PHOTO_H
#define PHOTO_H

#include <QString>
#include <chrono>
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

    bool sendRequest(QString &requestType);

    bool savePhoto(QString &format, const QString &resolution);
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

#endif // PHOTO_H
