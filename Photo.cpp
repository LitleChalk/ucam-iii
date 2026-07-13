#include "Photo.h"

#include <QFile>
#include <QTextStream>

Photo::Photo(int id)
    : id(id),
    camera_id(-1),
    time(std::chrono::system_clock::now()),
    batch(""),
    cap(false),
    changed(false)
{

}

Photo::~Photo()
{

}
//get
int Photo::getId() const
{
    return id;
}

int Photo::getCameraId() const
{
    return camera_id;
}

std::chrono::system_clock::time_point Photo::getTime() const
{
    return time;
}

QString Photo::getBatch() const
{
    return batch;
}

bool Photo::getCap() const
{
    return cap;
}

bool Photo::getChanged() const
{
    return changed;
}
//set
void Photo::setCameraId(int cameraId)
{
    camera_id = cameraId;
}

void Photo::setBatch(const QString &batch)
{
    this->batch = batch;
}

void Photo::setCap(bool cap)
{
    this->cap = cap;
}

void Photo::setChanged(bool changed)
{
    this->changed = changed;
}
//камера

bool Photo::sendRequest(const QString &requestType)
{

    return true;
}

bool Photo::receiveResponse()
{
    return true;
}

bool Photo::savePhoto(const QString &format,
                               const QString &resolution)
{

    return true;
}

bool Photo::saveToCsv()
{
    const QString &fileName="";
    QFile file(fileName);

    if (!file.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&file);

    file.close();

    return true;
}

//------------------------------------------------ RAW ------------------------------------------------

RawImage::RawImage(uint16_t Width, uint16_t Height, uint8_t BytesPerPixel)
{
    width = Width;
    height = Height;
    bytesPerPixel = BytesPerPixel;
    expectedSize = static_cast<size_t>(width) * height * bytesPerPixel;
    buffer.resize(expectedSize);

    received = 0;
}
void RawImage::GenerateRawImage(RawImage& image)
{
    constexpr int WIDTH = 80;
    constexpr int HEIGHT = 60;

    uint8_t packet[8];
    size_t index = 0;

    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            packet[index++] = static_cast<uint8_t>(x * 255 / (WIDTH - 1));

            if (index == sizeof(packet))
            {
                if (!image.addPacket(packet, sizeof(packet)))
                    return;

                index = 0;
            }
        }
    }

    if (index != 0)
    {
        image.addPacket(packet, index);
    }
}
// Возвращает false, если пакет не помещается.
bool RawImage::addPacket(const uint8_t* data, size_t size)
{
    if (received + size > expectedSize)
        return false;

    std::memcpy(buffer.data() + received, data, size);
    received += size;

    return true;
}

bool RawImage::isComplete() const
{
    return received == expectedSize;
}

bool RawImage::SaveRawImage(const QString& path,
                  const QString& fileName,
                  const uint8_t* data,
                  size_t size)
{
    QDir dir(path);

    if (!dir.exists())
        dir.mkpath(".");

    QFile file(dir.filePath(fileName));

    if (!file.open(QIODevice::WriteOnly))
        return false;

    file.write(reinterpret_cast<const char*>(data),
               static_cast<qint64>(size));

    file.close();

    return true;
}
//------------------------------------------------ JPEG ------------------------------------------------