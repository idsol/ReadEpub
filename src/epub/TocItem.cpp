#include "TocItem.h"

/*!
  \class TocItem

  A toc item in NCX document (/ncx/navMap/navPoint).
*/

TocItem::TocItem(const QString& id, const QString& label, const QString& href, const QString& uri,
				 int order, TocItem* parent)
	: _id(id)
	, _label(label)
	, _href(href)
	, _uri(uri)
	, _order(order)
    , _parent(parent)
{
    if (_parent) {
        _parent->addChild(this);
    }
}

TocItem::~TocItem()
{
	clear();
}

void TocItem::clear()
{
	qDeleteAll(_children);
	_children.clear();
}

TocItem* TocItem::takeChild(int row)
{
    TocItem* item = _children.takeAt(row);
    Q_ASSERT(item);
    item->_parent = 0;
    return item;
}
