#include <QtQuick>

#include <sailfishapp.h>
#include "filesmanager.h"
#include "fileprocess.h"
#include "fileengine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setSetuidAllowed(true);
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QSharedPointer<QQuickView> view(SailfishApp::createView());
    QApplication qapp(argc, argv);

    FilesManager* fmanager = new FilesManager;
    FileEngine fileEngine;
    view->rootContext()->setContextProperty("engine", &fileEngine);
    view->rootContext()->setContextProperty("fprocess", fileEngine.fileProcess);
    view->rootContext()->setContextProperty("clipboard", fileEngine.clipboard);

    view->rootContext()->setContextProperty("fmanager",fmanager);

     QObject::connect((QObject*)view->engine(), SIGNAL(quit()), app.data(), SLOT(quit()));

    view->setSource(SailfishApp::pathTo("qml/nanofiles.qml"));
    view->show();

    return app->exec();
}
