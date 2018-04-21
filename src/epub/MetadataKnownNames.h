#ifndef METADATAKNOWNNAMES_H
#define METADATAKNOWNNAMES_H

#include <QString>
#include <QHash>

class MetadataKnownNames : public QObject
{
	Q_OBJECT

private:
	MetadataKnownNames() { /* prohibited */ }

public:
	static QString query(const QString& name);

private:
	static void initialize();

private:
	static QHash<QString,QString> __knownNames; // key: name, value: known-name
};

#endif // METADATAKNOWNNAMES_H
