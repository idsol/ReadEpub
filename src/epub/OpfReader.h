#ifndef OPFREADER_H
#define OPFREADER_H

#include <QObject>

class OpfReaderPrivate;
class MetadataItem;
class ManifestItem;
class SpineItem;

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
class QIODevice;
QT_END_NAMESPACE

class OpfReader : public QObject
{
    Q_OBJECT

public:
	OpfReader();
    virtual ~OpfReader();

public:
	void read(QIODevice* dev, const QString& opfUri);

	bool hasError() const;
    QString errorString() const;

	QString ncxUri() const;

	QList<MetadataItem>* metadata() const;
	QList<ManifestItem>* manifest() const;
	QList<SpineItem>* spine() const;

private:
    OpfReaderPrivate* d;
};

#endif // OPFREADER_H
