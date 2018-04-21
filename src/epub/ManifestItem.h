#ifndef MANIFESTITEM_H
#define MANIFESTITEM_H

#include <QString>

class ManifestItem
{
public:
    ManifestItem();
	ManifestItem(const QString& id, const QString& href, const QString& uri, const QString& mediaType);
	ManifestItem(const ManifestItem& other);

public:
	QString id() const { return _id; }
	void id(const QString& value) { _id = value; }

	QString href() const { return _href; }
	void href(const QString& value) { _href = value; }

	QString uri() const { return _uri; }
	void uri(const QString& value) { _uri = value; }

	QString mediaType() const { return _mediaType; }
	void mediaType(const QString& value) { _mediaType = value; }

private:
	QString _id;
	QString _href;
	QString _uri;
	QString _mediaType;
};

#endif // MANIFESTITEM_H
