#include "MetadataItem.h"

/*!
  \class MetadataItem

  A metadata item in OPF document (children of /package/metadata/).
*/

MetadataItem::MetadataItem()
{
}

MetadataItem::MetadataItem(const QString& name, const QString& value)
	: _name(name)
	, _value(value)
{
}
