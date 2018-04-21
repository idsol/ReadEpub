#ifndef EPUBREADER_H
#define EPUBREADER_H

#include <QObject>

class EpubReaderPrivate;
class OpfReader;
class NcxReader;

class EpubReader : public QObject
{
	Q_OBJECT
	
public:
	EpubReader(QObject* parent = 0);
	virtual ~EpubReader();

public:
	void read(const QString& epubFilePath);

	QString epubFilePath() const;

	OpfReader* opf() const;
	NcxReader* ncx() const;
	
	bool optionKeepCss() const;
	void optionKeepCss(bool value);

	QByteArray readEntryContent(const QString& uri);

signals:
	void readStarted(int maximum);
	void readProgress(int progress, const QString& text);
	void readFinished(bool ok);

private:
	friend class EpubReaderPrivate;
	EpubReaderPrivate* d;
};

#endif // EPUBREADER_H
