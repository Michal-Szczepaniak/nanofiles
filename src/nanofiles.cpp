#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>
#include "filesystemmodel.h"
#include "qt-folderlistmodel/qquickfolderlistmodel.h"
#include <QApplication>
#include <QtQml/qqml.h>

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QSharedPointer<QQuickView> view(SailfishApp::createView());
    QApplication qapp(argc, argv);

    FileSystemModel* model = new FileSystemModel;
    model->setRootPath("/home/nemo");

    view->rootContext()->setContextProperty("fsmodel",model);
    view->rootContext()->setContextProperty("fsindex",model->index(model->rootPath()));

    QString uri = "com.verdanditeam.FolderListModel";
    qmlRegisterType<QQuickFolderListModel>(uri, 1, 0, "FolderListModel");

    qmlRegisterTypesAndRevisions<QQuickFolderListModel>(uri, 2);

    qmlRegisterModule(uri, 2, 15);

    view->setSource(SailfishApp::pathTo("qml/nanofiles.qml"));
    view->show();

    return app->exec();
}
