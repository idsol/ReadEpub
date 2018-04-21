#include "EpubMimeTypes.h"
#include <QFileInfo>

/*!
  \class EpubMimeTypes

  To query a file's mime type, specific to EPUB specification domain.
*/

QHash<QString,QString> EpubMimeTypes::__mimeTypes;

/// \static
QString EpubMimeTypes::query(const QString& filename)
{
    initialize();

    QString ext = QFileInfo(filename).suffix().toLower();
    if (!ext.isEmpty() && __mimeTypes.contains(ext)) {
        return __mimeTypes[ext];
    }
    return QString();
}

/// \static
void EpubMimeTypes::initialize()
{
    if (__mimeTypes.isEmpty()) {
        __mimeTypes.insert("htm", EpubMimeTypesConstants::HTML);
        __mimeTypes.insert("html", EpubMimeTypesConstants::HTML);
        __mimeTypes.insert("xml", EpubMimeTypesConstants::HTML);
        __mimeTypes.insert("xhtml", EpubMimeTypesConstants::HTML);
        __mimeTypes.insert("css", EpubMimeTypesConstants::CSS);
        __mimeTypes.insert("jpeg", EpubMimeTypesConstants::JPG);
        __mimeTypes.insert("jpg", EpubMimeTypesConstants::JPG);
        __mimeTypes.insert("gif", EpubMimeTypesConstants::GIF);
        __mimeTypes.insert("png", EpubMimeTypesConstants::PNG);
        __mimeTypes.insert("ncx", EpubMimeTypesConstants::NCX);
    }
}
