#ifndef SPINEITEM_H
#define SPINEITEM_H

#include <QString>

class ManifestItem;

class SpineItem
{
public:
	SpineItem();
	SpineItem(const QString& id, const QString& href, const QString& uri,
			  const QString& mediaType, bool primary = true);
	SpineItem(const SpineItem& other);
	SpineItem(const ManifestItem& manifestItem, bool primary = true);

public:
	QString id() const { return _id; }
	void id(const QString& value) { _id = value; }

	QString href() const { return _href; }
	void href(const QString& value) { _href = value; }

	QString uri() const { return _uri; }
	void uri(const QString& value) { _uri = value; }

	QString mediaType() const { return _mediaType; }
	void mediaType(const QString& value) { _mediaType = value; }

	bool primary() const { return _primary; }
	void primary(bool value) { _primary = value; }

	void readFromManifestItem(const ManifestItem& manifestItem);

public:
	bool isValid() const { return !_id.isEmpty(); }

private:
	QString _id;
	QString _href;
	QString _uri;
	QString _mediaType;
	bool _primary;
};

#endif // SPINEITEM_H
