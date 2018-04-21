#ifndef TOCITEM_H
#define TOCITEM_H

#include <QObject>

class TocItem
{
public:
	explicit TocItem(
		const QString& id = QString(), const QString& label = QString(),
		const QString& href = QString(), const QString& uri = QString(), int order = 0,
		TocItem* parent = 0
	);
	virtual ~TocItem();

public:
	/// xpath: /navMap/navPoint/@id
	QString id() const { return _id; }
	void id(const QString& value) { _id = value; }

	/// xpath: /navMap/navPoint/navLabel/text
	QString label() const { return _label; }
	void label(const QString& value) { _label = value; }

	/// xpath: /navMap/navPoint/content/@src
	QString href() const { return _href; }
	void href(const QString& value) { _href = value; }

	/// The uri relative to package root.
	QString uri() const { return _uri; }
	void uri(const QString& value) { _uri = value; }

	/// xpath: /navMap/navPoint/@playOrder
	int order() const { return _order; }
	void order(int value) { _order = value; }

	TocItem* parent() const { return _parent; }

	TocItem* childAt(int row) const { return _children.value(row); }
	int rowOfChild(TocItem* child) const { return _children.indexOf(child); }
	int childCount() const { return _children.count(); }
	bool hasChildren() const { return !_children.isEmpty(); }
	QList<TocItem*> children() const { return _children; }
	void clear();

    void insertChild(int row, TocItem* item) {
        item->_parent = this;
        _children.insert(row, item);
    }
    void addChild(TocItem* item) {
        item->_parent = this;
        _children << item;
    }
	void swapChildren(int oldRow, int newRow) {
		_children.swap(oldRow, newRow);
	}
    TocItem* takeChild(int row);

private:
	QString _id;
    QString _label;
	QString _href;
	QString _uri;
	int _order;

    TocItem* _parent;
    QList<TocItem*> _children;
};

#endif // TOCITEM_H
