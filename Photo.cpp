#include "Photo.h"

#include <QFile>
#include <QTextStream>

Photo::Photo()
    : id(0),
    camera_id(0),
    time(std::chrono::system_clock::now()),
    cap(false),
    changed(false)
{
}

Photo::Photo(int id, int cameraId, const QString &batch)
    : id(id),
    camera_id(cameraId),
    time(std::chrono::system_clock::now()),
    batch(batch),
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
void Photo::setId(int Id)
{
    id = Id;
}
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

bool Photo::saveToCsv(const QString &format,
                      const QString &resolution)
{
    QString dirPath = QString("saved_info/camera_%1/batch_%2")
    .arg(camera_id)
        .arg(batch);

    QDir dir;
    if (!dir.mkpath(dirPath))
        return false;

    QFile file(dirPath + "/info.csv");
    bool newFile = !file.exists();

    if (!file.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&file);

    if (newFile)
        out << "id,time,cap,changed,format,resolution\n";

    //auto tt = std::chrono::system_clock::to_time_t(time);
    //QString timeStr = QDateTime::fromSecsSinceEpoch(tt).toString(Qt::ISODate);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  time.time_since_epoch()).count();

    QString timeStr =
        QDateTime::fromMSecsSinceEpoch(ms).toString(Qt::ISODateWithMs);
    out << id << ','
        << timeStr << ','
        << (cap ? 1 : 0) << ','
        << (changed ? 1 : 0) << ','
        << format << ','
        << resolution << '\n';

    return true;
}
bool Photo::loadFromCsv(const QString &dirPath, int id)
{
    QFile file(dirPath + "/info.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&file);

    // пропускаем заголовок
    in.readLine();

    while (!in.atEnd())
    {
        QString line = in.readLine();

        QStringList data = line.split(',');

        // некорректная строка
        if (data.size() < 6)
            continue;

        // ищем нужный id
        if (data[0].toInt() == id)
        {
            this->id = id;

            QDateTime dt =
                QDateTime::fromString(data[1], Qt::ISODateWithMs);

            if (!dt.isValid())
                return false;

            time = std::chrono::system_clock::time_point(
                std::chrono::milliseconds(dt.toMSecsSinceEpoch()));

            cap = data[2].toInt();
            changed = data[3].toInt();

            return true;
        }
    }

    // запись с таким id не найдена
    return false;
}
//------------------------------------------------ RAW ------------------------------------------------

RawImage::RawImage(uint16_t Width, uint16_t Height, uint8_t BytesPerPixel)
{
    width = Width;
    height = Height;
    bytesPerPixel = BytesPerPixel;
    expectedSize = static_cast<size_t>(width) * height * bytesPerPixel;
    buffer.resize(expectedSize);
    time=std::chrono::system_clock::now();

    received = 0;
}
void RawImage::GenerateRawImage()
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
                if (!this->addPacket(packet, sizeof(packet)))
                    return;

                index = 0;
            }
        }
    }

    if (index != 0)
    {
        this->addPacket(packet, index);
    }
}
void RawImage::GenerateRawImage2()
{
    constexpr int WIDTH = 80;
    constexpr int HEIGHT = 60;

    uint8_t packet[8];
    size_t index = 0;

    // Новый seed при каждом вызове
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 255);

    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            // Каждый пиксель будет случайным
            packet[index++] = static_cast<uint8_t>(dist(gen));

            if (index == sizeof(packet))
            {
                if (!this->addPacket(packet, sizeof(packet)))
                    return;

                index = 0;
            }
        }
    }

    if (index != 0)
    {
        this->addPacket(packet, index);
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
bool RawImage::SaveRawImage(int cameraId,
                            const QString& batch,
                            int id,
                            const uint8_t* data,
                            size_t size)
{
    QString dirPath =
        QString("saved_info/camera_%1/batch_%2")
            .arg(cameraId)
            .arg(batch);


    QString fileName =
        QString("Photo_%1.raw")
            .arg(id);


    return SaveRawImage(
        dirPath,
        fileName,
        data,
        size
        );
}
//камера

bool RawImage::sendRawRequest()
{

    return true;
}

bool RawImage::receiveRawResponse()
{
    return true;
}
//------------------------------------------------ JPEG ------------------------------------------------