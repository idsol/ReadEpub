#ifndef OCFREADER_H
#define OCFREADER_H

#include <QObject>

class OcfReaderPrivate;

QT_BEGIN_NAMESPACE
class QIODevice;
QT_END_NAMESPACE

class OcfReader : public QObject
{
	Q_OBJECT

public:
	OcfReader(QObject* parent = 0);
	virtual ~OcfReader();

public:
	static QString uri();

	void read(QIODevice* dev);

	QString opfUri() const;

private:
	OcfReaderPrivate* d;
};

#endif // OCFREADER_H
