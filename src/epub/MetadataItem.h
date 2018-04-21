#ifndef METADATAITEM_H
#define METADATAITEM_H

#include <QString>

class MetadataItem
{
public:
	MetadataItem();
	MetadataItem(const QString& name, const QString& value);

public:
	QString name() const { return _name; }
	void name(const QString& value) { _name = value; }

	QString value() const { return _value; }
	void value(const QString& value) { _value = value; }

private:
	QString _name;
	QString _value;
};

#endif // METADATAITEM_H
