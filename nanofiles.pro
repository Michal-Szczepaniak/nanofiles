# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = nanofiles

QT += core-private qml-private widgets

CONFIG += sailfishapp

SOURCES += src/nanofiles.cpp \
    src/filesystemmodel.cpp \
    qt-folderlistmodel/qquickfolderlistmodel.cpp \
    qt-folderlistmodel/plugin.cpp \
    qt-folderlistmodel/fileinfothread.cpp

DISTFILES += qml/nanofiles.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    rpm/nanofiles.changes.in \
    rpm/nanofiles.changes.run.in \
    rpm/nanofiles.spec \
    rpm/nanofiles.yaml \
    translations/*.ts \
    nanofiles.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/nanofiles-de.ts

HEADERS += \
    src/filesystemmodel.h \
    qt-folderlistmodel/qquickfolderlistmodel.h \
    qt-folderlistmodel/fileproperty_p.h \
    qt-folderlistmodel/fileinfothread_p.h
