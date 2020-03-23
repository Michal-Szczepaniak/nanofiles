#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include <QObject>
#include <QMimeDatabase>

class FilesManager : public QObject
{
    Q_OBJECT
public:
    FilesManager();

    Q_INVOKABLE QString mimeType( const QString &filePath ){ return QMimeDatabase().mimeTypeForFile( filePath ).name(); }
};

#endif // FILESMANAGER_H
