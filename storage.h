#ifndef STORAGE_H
#define STORAGE_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QDataStream>

class Storage
{
public:
    Storage();
    Storage(const QString extension, const QString directoty = "");

    QDataStream *openDataStream(QString filename, QFile **file) const;
    void closeDataStream(QDataStream *datastream);

    QVector<QString> loadFromFile(QString filename);
    bool saveToFile(QString filename, QVector<QString> data);
    bool deleteFile(QString filename);
    bool copyFile(QString filename, QString newname);
    bool renameFile(QString filename, QString newname);


private:
    QString extension;
    QString path;
    QString directoty;
};

#endif // STORAGE_H
