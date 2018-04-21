#ifndef EPUBMIMETYPES_H
#define EPUBMIMETYPES_H

#include <QString>
#include <QHash>

class EpubMimeTypes
{
private:
    EpubMimeTypes() { /* prohibited */ }

public:
    static QString query(const QString& filename);

private:
    static void initialize();

private:
    static QHash<QString,QString> __mimeTypes; // key: suffix, value: mimetype
};

namespace EpubMimeTypesConstants
{
    static const char* const HTML = "application/xhtml+xml";
    static const char* const CSS = "text/css";
    static const char* const JPG = "image/jpeg";
    static const char* const GIF = "image/gif";
    static const char* const PNG = "image/png";
    static const char* const NCX = "application/x-dtbncx+xml";
}

#endif // EPUBMIMETYPES_H
