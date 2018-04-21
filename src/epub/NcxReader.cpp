#include "NcxReader.h"
#include <QIcon>
#include <QStyle>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDomDocument>
#include <QFileInfo>
#include <QDebug>
#include "Error.h"
#include "TocItem.h"
#include "OpfReader.h"
#include "Util.h"

/*!
  \class NcxReader

  Reads toc items from NCX document.
*/

class NcxReaderPrivate
{
public:
	/// The ncx uri
	QString ncxUri;

	/// The virtual root item of all toc items
	TocItem toc;

public:
	NcxReaderPrivate()
    {
    }

public:
    void readNavPointElement(const QDomElement& element, TocItem* parentItem)
    {
		QString id = element.attribute("id");
        QString label = element.firstChildElement("navLabel").firstChildElement("text").text();
		QString href = element.firstChildElement("content").attribute("src");
		QString tocUri = Util::makeRelative(href, ncxUri);
		int order = element.attribute("playOrder").toInt();

		TocItem* item = new TocItem(id, label, href, tocUri, order, parentItem);

        QDomElement child = element.firstChildElement();
        while (!child.isNull()) {
            if (child.tagName() == "navPoint") {
                readNavPointElement(child, item);
            }
            child = child.nextSiblingElement();
        }
    }
};

NcxReader::NcxReader()
{
    d = new NcxReaderPrivate();
}

NcxReader::~NcxReader()
{
    delete d;
}

/*!
  Reads all the toc items.

  \throws  If \a dev is null or not open.
  \throws  If ncx document specified by \a dev is malformed.
*/
void NcxReader::read(QIODevice* dev, const QString& ncxUri)
{
	if (!dev) {
		throw Error(tr("[NcxReader.read] dev = null, uri = %1").arg(ncxUri));
	}

	if (!dev->isOpen()) {
		throw Error(tr("[NcxReader.read] dev is NOT open, uri = %1").arg(ncxUri));
	}

	d->ncxUri = ncxUri;
	d->toc.clear();

	QDomDocument doc;
	QString errStr;
	int errLine, errCol;
	if (!doc.setContent(dev, true, &errStr, &errLine, &errCol)) {
		throw Error(
			tr("[NcxReader.read] Parse error in NCX stream:\n\n%1\nLine %2, column %3")
				.arg(errStr)
				.arg(errLine)
				.arg(errCol)
		);
	}

	QDomElement root = doc.documentElement();
	if (root.tagName() != "ncx") {
		throw Error(
			tr("[NcxReader.read] Specified NCX stream is invalid due to absence of <ncx> root element.")
		);
	}

	QDomElement navMap = root.firstChildElement("navMap");
	if (navMap.isNull()) {
		throw Error(
			tr("[NcxReader.read] Specified NCX stream is invalid due to absence of <navMap> element")
		);
	}

	QDomElement child = navMap.firstChildElement("navPoint");
	while (!child.isNull()) {
		d->readNavPointElement(child, &d->toc);
		child = child.nextSiblingElement("navPoint");
	}
}

/*!
  Returns the ncx uri formerly retrieved by read().
*/
QString NcxReader::uri() const
{
	return d->ncxUri;
}

/*!
  Returns the virtual root item of all toc items formerly retrieved by read().
*/
TocItem* NcxReader::toc() const
{
	return &d->toc;
}
