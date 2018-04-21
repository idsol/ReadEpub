#include "EpubReader.h"
#include <QtCore>
#include "quazip.h"
#include "quazipfile.h"
#include "quazipfileinfo.h"
#include "NcxReader.h"
#include "OpfReader.h"
#include "OcfReader.h"
#include "OpfReader.h"
#include "NcxReader.h"
#include "Error.h"
#include "EpubMimeTypes.h"
#include "Util.h"

/*!
	\class EpubReader
	
	Reads contents of EPUB file.
	
	Provides OCF, OPF, and NCX models for use in reading system.
*/

class EpubReaderPrivate
{
public:
	EpubReader* q;

	QString epubFilePath; // epub file as input file
	QuaZip* epubFile;

	OcfReader* ocf;
	OpfReader* opf;
	NcxReader* ncx;

	int progress;
	int maxProgress;

	bool hasError;
	QString errorText;

	QDateTime execStartTime;
	QDateTime execEndTime;

	bool verbose; // show debug info or not
	
	bool optionKeepCss;

public:
	EpubReaderPrivate(EpubReader* q)
		: q(q)
		, epubFile(new QuaZip())
		, ocf(new OcfReader())
		, opf(new OpfReader())
		, ncx(new NcxReader())
		, progress(0)
		, maxProgress(0)
		, hasError(false)
		, execStartTime(QDateTime::currentDateTime())
		, execEndTime(QDateTime::currentDateTime())
		, verbose(false)
		, optionKeepCss(false)
	{
		epubFile->setFileNameCodec("GBK");
		epubFile->setCommentCodec("GBK");
	}

	~EpubReaderPrivate() {
		delete ncx;
		ncx = 0;

		delete opf;
		opf = 0;

		delete ocf;
		ocf = 0;

		if (epubFile && epubFile->isOpen()) {
			epubFile->close();
		}
		delete epubFile;
		epubFile = 0;
	}

	void setError(const QString& value) {
		errorText = value;
		hasError = true;
		execEndTime = QDateTime::currentDateTime();
		emit q->readFinished(false);
	}

private:
	EpubReaderPrivate(); /* prohibited */
};

EpubReader::EpubReader(QObject* parent)
	: QObject(parent)
{
	d = new EpubReaderPrivate(this);
}

EpubReader::~EpubReader()
{
	delete d;
}

/*!
  Reads the EPUB file specified by \a epubFilePath.
*/
void EpubReader::read(const QString& epubFilePath)
{
	emit readStarted(100);

	if (!QFileInfo(epubFilePath).exists()) {
		throw Error(tr("[EpubReader.read] File does NOT exist: %1.").arg(epubFilePath));
	}

	if (d->epubFile->isOpen()) {
		d->epubFile->close();
	}
	d->epubFilePath = epubFilePath;
	d->epubFile->setZipName(epubFilePath);
	d->epubFile->open(QuaZip::mdUnzip);

	if (!d->epubFile->isOpen()) {
		throw Error(tr("[EpubReader.read] FAILED to open file: %1.").arg(epubFilePath));
	}

	//
	// 1) reads opf-uri from ocf
	//
	QuaZipFile ocfEntry(d->epubFile);
	d->epubFile->setCurrentFile(OcfReader::uri());
	ocfEntry.open(QIODevice::ReadOnly | QIODevice::Text);
	d->ocf->read(&ocfEntry);
	ocfEntry.close();
	readProgress(33, QString());

	//
	// 2) reads metadata/manifest/spine/guide/ncx-uri from opf
	//
	QuaZipFile opfEntry(d->epubFile);
	d->epubFile->setCurrentFile(d->ocf->opfUri());
	opfEntry.open(QIODevice::ReadOnly | QIODevice::Text);
	d->opf->read(&opfEntry, d->ocf->opfUri());
	opfEntry.close();
	readProgress(66, QString());

	//
	// 3) reads toc from ncx
	//
	QuaZipFile ncxEntry(d->epubFile);
	d->epubFile->setCurrentFile(d->opf->ncxUri());
	ncxEntry.open(QIODevice::ReadOnly | QIODevice::Text);
	d->ncx->read(&ncxEntry, d->opf->ncxUri());
	ncxEntry.close();
	readProgress(100, QString());

	emit readFinished(true);
}

/*!
	Returns the epub filepath formerly set via read(epubFilePath).
*/
QString EpubReader::epubFilePath() const
{
	return d->epubFilePath;
}

OpfReader* EpubReader::opf() const
{
	return d->opf;
}

NcxReader* EpubReader::ncx() const
{
	return d->ncx;
}

bool EpubReader::optionKeepCss() const
{
	return d->optionKeepCss;
}

void EpubReader::optionKeepCss(bool value)
{
	d->optionKeepCss = value;
}

QByteArray EpubReader::readEntryContent(const QString& uri)
{
	if (!d->epubFile->isOpen()) {
		return QByteArray();
	}

	QByteArray res = QByteArray();
	QString entryName = Util::removeFragment(uri);

	QuaZipFile entry(d->epubFile);
	d->epubFile->setCurrentFile(entryName);
	entry.open(QIODevice::ReadOnly);
	if (entry.isOpen()) {
		res = entry.readAll();
		entry.close();
	}
	else {
		QString mimeType = EpubMimeTypes::query(entryName);
		if (mimeType == EpubMimeTypesConstants::HTML) {
			throw Error(tr("Read failed: %1").arg(uri));
		}
		else {
			// tolerant - allow auxiliary files (.js, .css, etc.) to be absent
		}
		return QByteArray();
	}

	return res;
}
