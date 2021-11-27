#include "storage.h"

Storage::Storage(){};

Storage::Storage(const QString extension, const QString directoty)
{
    this->extension = extension;
    this->path = QDir::currentPath();
    this->directoty = directoty;

    QDir dir(path);
    if (!dir.exists(directoty)){
      dir.mkpath(directoty);
    }

    qInfo()<<"QmlStorage: loaded!";

    //dir.setNameFilters(QStringList()<<extension);
}

QDataStream *Storage::openDataStream(QString filename, QFile **ptrfile) const
{
    qInfo()<<"Storage: trying to open a data stream for " << filename << "...";

    *ptrfile = new QFile(path + "/" + directoty + "/" + filename + "." + extension);
    if(!(**ptrfile).open(QIODevice::ReadWrite))
    {
        qWarning() << "Storage: cant open a file";
        return nullptr;
    }
    qInfo()<<"Storage: opened new filestream...";
    return new QDataStream(&(**ptrfile)); // after usage close the file
}

void Storage::closeDataStream(QDataStream *datastream)
{
    qInfo()<<"Storage: trying to close a data stream...";
    qInfo()<<"Storage: TEMP DISABLED...";
};

QVector<QString> Storage::loadFromFile(QString filename)
{
    QVector<QString> result;
    qInfo()<<"Storage: trying to read a file...";

    QFile file(path + "/" + directoty + "/" + filename + "." + extension);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Storage: cant open a file";
        return result;
    }
    QTextStream textStream(&file);
    while (!textStream.atEnd()) {
        result.append(textStream.readLine());
    }

    file.close();

    qInfo()<<"Storage: file readed succesfully";
    return result;
}
bool Storage::saveToFile(QString filename, QVector<QString> data)
{
    qInfo()<<"Storage: trying to wtite to file...";

    QFile file(path + "/" + directoty + "/" + filename + "." + extension);
    if(!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Storage: cant open a file";
        return false;
    }
    QTextStream wr(&file); //(write to file)
    for (const QString &line : data)
        wr << line << "\n";

    file.close();

    qInfo()<<"Storage: file writted succesfully";
    return true;
}
bool Storage::deleteFile(QString filename)
{
    QFile file(path + "/" + directoty + "/" + filename + "." + extension);
    if (!file.exists())
        return false;

    qInfo()<<"Storage: try delete " << filename;
    //return file.moveToTrash();
}
bool Storage::copyFile(QString filename, QString newname)
{
    QFile file(path + "/" + directoty + "/" + filename + "." + extension);
    if (!file.exists())
        return false;

    qInfo()<<"Storage: try copy " << filename;
    return file.copy(newname);
}

bool Storage::renameFile(QString filename, QString newname)
{
    newname = path + "/" + directoty + "/" + newname + "." + extension;
    QFile file(path + "/" + directoty + "/" + filename + "." + extension);
    if (!file.exists())
        return false;

    if (QFile(newname).exists())
    {
        qInfo()<< "Storage: rename error -- already exists!";
        return false;
    }
    qInfo()<<"Storage: try rename to " << newname;
    return file.rename(newname);
}

