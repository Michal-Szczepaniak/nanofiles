#include "filesystemmodel.h"
#include <QListView>
#include <QDebug>
#include <QByteArray>
#include <QMetaProperty>

FileSystemModel::FileSystemModel()
{

}

QModelIndex FileSystemModel::setPath(QString path)
{
    qDebug() << path;
    this->setRootPath(path);
    return index(path);
}


