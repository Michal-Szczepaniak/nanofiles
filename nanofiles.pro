include(vendor/vendor.pri)

TARGET = nanofiles

QT += widgets

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

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/nanofiles-de.ts

HEADERS += \
    src/fileprocess.h \
    src/filesmanager.h \
    src/fileengine.h \
    src/filelist.h \
    src/clipboard.h \
    src/util.h \
    src/worker.h
