#include "OcfReader.h"
#include <QFileInfo>
#include <QDir>
#include <QXmlStreamReader>
#include <QDebug>
#include "Error.h"

/*!
  \class OcfReader

  Reads contents of OCF file from epub package, which is fixed named as `/META-INF/container.xml`.

  The sole purpose of ocf file is to store OPF URI, so just read(), then opfUri().
*/

static const char* const OCF_URI = "META-INF/container.xml";

class OcfReaderPrivate
{
public:
	QString opfUri;
};

OcfReader::OcfReader(QObject* parent)
    : QObject(parent)
{
	d = new OcfReaderPrivate();
}

OcfReader::~OcfReader()
{
    delete d;
}

QString OcfReader::uri()
{
	return OCF_URI;
}

/*!
  Reads and stores opf-uri which can be retrieved later via opfUri().

  \throws  If \a dev is null or not open.
  \throws  If OCF document specified by \a dev is malformed.
*/
void OcfReader::read(QIODevice* dev)
{
	if (!dev) {
		throw Error(tr("[OcfReader.read] dev = null, uri = %1.").arg(OCF_URI));
	}

	if (!dev->isOpen()) {
		throw Error(tr("[OcfReader.read] dev is NOT open, uri = %1.").arg(OCF_URI));
	}

	d->opfUri = QString();

	QXmlStreamReader reader;
	reader.setDevice(dev);

	while (!reader.atEnd()) {
		reader.readNext();
		if (reader.isStartElement() && reader.name() == "rootfile") {
			d->opfUri = reader.attributes().value("full-path").toString();
			break;
		}
	}

	if (d->opfUri.isEmpty()) {
		throw Error(tr("[OcfReader.read] NCX document is malformed as it does NOT contain valid OPF uri."));
	}
}

/*!
  Returns opf uri retrieved from former read() operation.
*/
QString OcfReader::opfUri() const
{
	return d->opfUri;
}
