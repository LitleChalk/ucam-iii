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