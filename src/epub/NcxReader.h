#ifndef NCXREADER_H
#define NCXREADER_H

#include <QObject>

class NcxReaderPrivate;
class TocItem;

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
class QIODevice;
QT_END_NAMESPACE

class NcxReader : public QObject
{
    Q_OBJECT

public:
    NcxReader();
    virtual ~NcxReader();

public:
	void read(QIODevice* dev, const QString& ncxUri);

	QString uri() const;

	TocItem* toc() const;

private:
    NcxReaderPrivate* d;
};

#endif // NCXREADER_H
