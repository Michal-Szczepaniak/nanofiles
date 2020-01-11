#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>

class FileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    FileSystemModel();

    Q_INVOKABLE QModelIndex setPath(QString path);
};

#endif // FILESYSTEMMODEL_H
