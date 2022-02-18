include(vendor/vendor.pri)

TARGET = nanofiles

QT += widgets qml quick

CONFIG += sailfishapp

SOURCES += src/nanofiles.cpp \
    src/fileprocess.cpp \
    src/filesmanager.cpp \
    src/fileengine.cpp \
    src/filelist.cpp \
    src/clipboard.cpp \
    src/util.cpp \
    src/worker.cpp

DISTFILES += qml/nanofiles.qml \
    qml/components/ErrorDialog.qml \
    qml/cover/CoverPage.qml \
    qml/js/moment.min.js \
    qml/pages/FileInfo.qml \
    qml/pages/Main.qml \
    qml/pages/About.qml \
    qml/pages/Places.qml \
    qml/pages/ShareFilesPage.qml \
    qml/components/NewFilesDialog.qml \
    qml/components/FileRenameDialog.qml \
    qml/components/MenuBar.qml \
    rpm/nanofiles.spec \
    translations/*.ts \
    nanofiles.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

RESOURCES += \
    qml/resources/resources.qrc

CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/nanofiles-es.ts \
                translations/nanofiles-sv.ts \
                translations/nanofiles-zh_CN.ts

HEADERS += \
    src/fileprocess.h \
    src/filesmanager.h \
    src/fileengine.h \
    src/filelist.h \
    src/clipboard.h \
    src/util.h \
    src/worker.h
