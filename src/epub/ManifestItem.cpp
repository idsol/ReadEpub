#include "ManifestItem.h"

/*!
  \class ManifestItem

  A manifest item in OPF document (/package/manifest/item).
*/

ManifestItem::ManifestItem()
{
}

ManifestItem::ManifestItem(const QString& id, const QString& href, const QString& uri,
						   const QString& mediaType)
	: _id(id)
	, _href(href)
	, _uri(uri)
	, _mediaType(mediaType)
{
}

ManifestItem::ManifestItem(const ManifestItem& other)
	: _id(other.id())
	, _href(other.href())
	, _uri(other.uri())
	, _mediaType(other.mediaType())
{
}
