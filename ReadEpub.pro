#-------------------------------------------------
#
# ReadEpub
# Epub Reader
# Copyright 2016 sol (idsol at 163.com)
#
#-------------------------------------------------

QT += core xml gui webkit network
TEMPLATE = app

TARGET = ReadEpub
CONFIG(release, debug|release) {
	DESTDIR = build/release
} else {
	DESTDIR = build/debug
}

# intermediate files (*.o, moc_*, qrc_*, ui_*)
release {
	OBJECTS_DIR = build/obj/release
}
debug {
	OBJECTS_DIR = build/obj/debug
}
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui

INCLUDEPATH += . src src/epub lib/quazip
LIBS += ./lib/quazip/libquazip.lib

HEADERS += \
	src/Error.h \
	src/MainWindow.h \
	src/FileInfoDialog.h \
	src/FileInfoModel.h \
	src/NetworkAccessManager.h \
	src/NetworkReply.h \
	src/TocTreeModel.h \
	src/Util.h \
	src/WebViewEventFilter.h \
	src/epub/OpfReader.h \
	src/epub/NcxReader.h \
	src/epub/OcfReader.h \
	src/epub/TocItem.h \
	src/epub/EpubMimeTypes.h \
	src/epub/EpubReader.h \
	src/epub/SpineItem.h \
	src/epub/ManifestItem.h \
	src/epub/MetadataItem.h \
	src/epub/MetadataKnownNames.h

SOURCES += \
	src/main.cpp \
	src/MainWindow.cpp \
	src/FileInfoDialog.cpp \
	src/FileInfoModel.cpp \
	src/NetworkAccessManager.cpp \
	src/NetworkReply.cpp \
	src/TocTreeModel.cpp \
	src/Util.cpp \
	src/WebViewEventFilter.cpp \
	src/epub/OpfReader.cpp \
	src/epub/NcxReader.cpp \
	src/epub/OcfReader.cpp \
	src/epub/TocItem.cpp \
	src/epub/EpubMimeTypes.cpp \
	src/epub/EpubReader.cpp \
	src/epub/SpineItem.cpp \
	src/epub/ManifestItem.cpp \
	src/epub/MetadataItem.cpp \
	src/epub/MetadataKnownNames.cpp

OTHER_FILES += deploy.cmd

FORMS += \
	src/MainWindow.ui \
	src/FileInfoDialog.ui

RESOURCES += res/app.qrc
RC_FILE = res/icon.rc

TRANSLATIONS = \
	translations/readepub_zh.ts \
	translations/readepub_zh_TW.ts
