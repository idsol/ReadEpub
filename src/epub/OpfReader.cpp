#include "OpfReader.h"
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include "MetadataItem.h"
#include "ManifestItem.h"
#include "SpineItem.h"
#include "Error.h"
#include "Util.h"
#include "EpubMimeTypes.h"

/*!
  \class OpfReader

  Reads OPF document which stores metadata, manifest, spine, guide, etc.
*/

class OpfReaderPrivate
{
public:
	OpfReader* q;

	QString opfUri; // opf uri
	QString ncxUri; // ncx uri

	QList<MetadataItem> metadata;
	QList<ManifestItem> manifest;
	QList<SpineItem> spine;

	QDomDocument xml;
	bool hasError;
	QString errorMsg;
	int errorLine, errorColumn;

	bool verbose; // show verbose debug message or not

public:
	OpfReaderPrivate(OpfReader* q)
		: q(q)
		, hasError(false)
		, errorLine(0)
		, errorColumn(0)
		, verbose(false)
	{
	}

public:
	void readOpf()
	{
		QDomElement docElem = xml.documentElement();

		QDomElement e = docElem.firstChildElement();
		while (!e.isNull()) {
			if (e.tagName() == "metadata") {
				readMetadata(e);
			}
			else if (e.tagName() == "manifest") {
				readManifest(e);
			}
			else if (e.tagName() == "spine") {
				readSpine(e);
			}
			else if (e.tagName() == "guide") {
				readGuide(e);
			}
			e = e.nextSiblingElement();
		}
	}

	void readMetadata(const QDomElement& elem)
	{
		metadata.clear();
		QDomElement e = elem.firstChildElement();
		while (!e.isNull()) {
			QString name = e.tagName().trimmed();
			QString value = e.text().trimmed();
			if (verbose) {
				qDebug() <<
					QString("[OpfReaderPrivate.readMetadata] metadata = { name = %1, value = %2 }")
					.arg(name)
					.arg(value);
			}
			if (!value.isEmpty()) {
				MetadataItem item(name, value);
				metadata.append(item);
			}
			e = e.nextSiblingElement();
		}
	}

	void readManifest(const QDomElement& elem)
	{
		//
		// read manifest items
		//
		// EXAMPLE
		// <manifest>
		//   <item id="chapter1"  href="chapter1.html"  media-type="application/xhtml+xml"/>
		// </manifest>
		//
		manifest.clear();
		QDomElement e = elem.firstChildElement();
		while (!e.isNull()) {
			if (e.tagName() == "item") {
				QString id = e.attribute("id");
				QString href = e.attribute("href");
				QString uri = Util::makeRelative(e.attribute("href"), opfUri);
				QString mediaType = e.attribute("media-type");

				ManifestItem item(id, href, uri, mediaType);
				manifest.append(item);

				if (verbose) {
					qDebug() <<
						QString("[OpfReaderPrivate.readManifest] manifest = { id = %1, href = %2, media-type = %3 }")
						.arg(item.id())
						.arg(item.href())
						.arg(item.mediaType());
				}

				//
				// read ncx uri. note that this path is relative to opf file itself.
				//
				if (e.hasAttribute("media-type")
					&& e.attribute("media-type") == EpubMimeTypesConstants::NCX) {
					QString ncxHref = e.attribute("href");
					ncxUri = Util::makeRelative(ncxHref, opfUri);
					if (verbose) {
						qDebug() <<
							QString("[OpfReader.readManifest] NCX uri found: href = %1, opfUri = %2, ncxUri = %3")
							.arg(ncxHref)
							.arg(opfUri)
							.arg(ncxUri);
					}
				}
			}
			e = e.nextSiblingElement();
		}
	}

	void readSpine(const QDomElement& elem)
	{
		//
		// read ncx-uri in case it's not determined in readManifest()
		//
		if (ncxUri.isEmpty() && elem.hasAttribute("toc")) {
			QString ncxId = elem.attribute("toc");
			foreach (const ManifestItem& manifestItem, manifest) {
				if (manifestItem.id() == ncxId) {
					ncxUri = manifestItem.uri();
					break;
				}
			}
		}

		//
		// read spine items
		//
		// EXAMPLE
		// <spine toc="ncx">
		//   <itemref idref="coverpage" linear="yes"/>
		// </spine>
		//
		spine.clear();
		QDomElement e = elem.firstChildElement();
		while (!e.isNull()) {
			if (e.tagName() == "itemref") {
				QString idref = e.attribute("idref");

				SpineItem item;
				foreach (const ManifestItem& manifestItem, manifest) {
					if (manifestItem.id() == idref) {
						item.readFromManifestItem(manifestItem);
						break;
					}
				}

				if (item.isValid()) {
					bool isPrimary = e.hasAttribute("linear") && e.attribute("linear") == "yes";
					item.primary(isPrimary);
					spine.append(item);
				}

				if (verbose) {
					qDebug() <<
						QString("[OpfReaderPrivate.readSpine] spine = { id = %1, href = %2, media-type = %3, primary = %4 }")
						.arg(item.id())
						.arg(item.href())
						.arg(item.mediaType())
						.arg(item.primary());
				}
			}
			e = e.nextSiblingElement();
		}
	}

	void readGuide(const QDomElement& elem)
	{
		// TODO: imp
		// \note  Guide is OPTIONAL for reading system implementation
		Q_UNUSED(elem);
	}
};

OpfReader::OpfReader()
{
	d = new OpfReaderPrivate(this);
}

OpfReader::~OpfReader()
{
    delete d;
}

/*!
  Reads and stores ncx-uri which can be retrieved later via ncxUri().

  \throws  If \a dev is invalid or not open.
  \throws  If OPF document specified by \a dev is malformed.
*/
void OpfReader::read(QIODevice* dev, const QString& opfUri)
{
	if (!dev) {
		throw Error(tr("[OpfReader.read] dev = null, uri = %1").arg(opfUri));
	}

	if (!dev->isOpen()) {
		throw Error(tr("[OpfReader.read] dev is NOT open, uri = %1").arg(opfUri));
	}

	d->hasError = d->xml.setContent(dev, true, &d->errorMsg, &d->errorLine, &d->errorColumn);

	QDomElement docElem = d->xml.documentElement();
	if (docElem.isNull()
		|| docElem.tagName() != "package"
		|| !docElem.hasAttribute("version") || docElem.attribute("version") != "2.0") {

		d->hasError = true;
		d->errorMsg = tr("The file is NOT an EPUB 2.0 OPF file");
		d->errorLine = 0;
		d->errorColumn = 0;

		throw Error(tr("[OpfReader.read] %1, uri = %2").arg(d->errorMsg).arg(opfUri));
	}

	d->opfUri = opfUri;
	d->readOpf();
}

bool OpfReader::hasError() const
{
	return d->hasError;
}

QString OpfReader::errorString() const
{
	return tr("%1\nLine %2, column %3")
		.arg(d->errorMsg)
		.arg(d->errorLine)
		.arg(d->errorColumn);
}

/*!
  Returns ncx uri retrieved from former read() operation.
*/
QString OpfReader::ncxUri() const
{
	return d->ncxUri;
}

/*!
  Returns the metadata item collection.
*/
QList<MetadataItem>* OpfReader::metadata() const
{
	return &d->metadata;
}

/*!
  Returns the manifest item collection.
*/
QList<ManifestItem>* OpfReader::manifest() const
{
	return &d->manifest;
}

/*!
  Returns the spine item collection (content flow).
*/
QList<SpineItem>* OpfReader::spine() const
{
	return &d->spine;
}
