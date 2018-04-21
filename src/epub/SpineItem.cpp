#include "SpineItem.h"
#include "ManifestItem.h"

/*!
  \class SpineItem

  A spine item in OPF document (/package/spine/itemref).

  SPINE: An arrangement of documents providing a linear reading order.
*/

SpineItem::SpineItem()
	: _primary(true)
{
}

SpineItem::SpineItem(const QString& id, const QString& href, const QString& uri, const QString& mediaType, bool primary)
	: _id(id)
	, _href(href)
	, _uri(uri)
	, _mediaType(mediaType)
	, _primary(primary)
{
}

SpineItem::SpineItem(const SpineItem& other)
	: _id(other.id())
	, _href(other.href())
	, _uri(other.uri())
	, _mediaType(other.mediaType())
	, _primary(other.primary())
{
}

SpineItem::SpineItem(const ManifestItem& manifestItem, bool primary)
	: _id(manifestItem.id())
	, _href(manifestItem.href())
	, _uri(manifestItem.uri())
	, _mediaType(manifestItem.mediaType())
	, _primary(primary)
{
}

void SpineItem::readFromManifestItem(const ManifestItem& manifestItem)
{
	id(manifestItem.id());
	href(manifestItem.href());
	uri(manifestItem.uri());
	mediaType(manifestItem.mediaType());
}
