#include "TocTreeModel.h"
#include <QDebug>

/*!
  \class TocTreeModel

  Model of TocItem tree, for QTreeView. Inherits QAbstractItemModel.
*/

enum Column
{
    LabelColumn = 0,
	UriColumn = 1,
	ColumnCount = 2
};

TocTreeModel::TocTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
	, _root(0)
{
}

TocTreeModel::~TocTreeModel()
{
}

/// \reimp
Qt::ItemFlags TocTreeModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags res = QAbstractItemModel::flags(index);
    if (index.isValid()) {
        res |= Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
    return res;
}

/// \reimp
QVariant TocTreeModel::data(const QModelIndex& index, int role) const
{
	if (!_root || !index.isValid() || index.column() < 0
		|| index.column() >= ColumnCount) {
        return QVariant();
	}

    if (TocItem* item = itemForIndex(index)) {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            switch (index.column()) {
				case LabelColumn: return item->label();
				case UriColumn: return item->uri();
				default: Q_ASSERT(false);
            }
        }

        if (role == Qt::TextAlignmentRole) {
            return static_cast<int>(Qt::AlignVCenter | Qt::AlignLeft);
        }

        if (role == Qt::DecorationRole && index.column() == LabelColumn) {
            return item->hasChildren() ? _folderIcon : _fileIcon;
        }
    }

    return QVariant();
}

/// \reimp
QVariant TocTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == LabelColumn) {
            return tr("Label");
        }
		else if (section == UriColumn) {
			return tr("Uri");
		}
    }
    return QVariant();
}

/// \reimp
int TocTreeModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid() && parent.column() != 0) {
        return 0;
	}

    TocItem* parentItem = itemForIndex(parent);
    return parentItem ? parentItem->childCount() : 0;
}

/// \reimp
int TocTreeModel::columnCount(const QModelIndex& parent) const
{
	return parent.isValid() && parent.column() != 0 ? 0 : 1;
}

/// \reimp
QModelIndex TocTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!_root || row < 0 || column < 0 || column >= ColumnCount
        || (parent.isValid() && parent.column() != 0)) {
        return QModelIndex();
    }

	TocItem* parentItem = itemForIndex(parent);
	Q_ASSERT(parentItem);
    if (TocItem* item = parentItem->childAt(row)) {
        return createIndex(row, column, item);
    }

    return QModelIndex();
}

/// \reimp
QModelIndex TocTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    if (TocItem* childItem = itemForIndex(index)) {
        if (TocItem* parentItem = childItem->parent()) {
			if (parentItem == _root) {
                return QModelIndex();
			}
            if (TocItem* grandParentItem = parentItem->parent()) {
                int row = grandParentItem->rowOfChild(parentItem);
                return createIndex(row, 0, parentItem);
            }
        }
    }

    return QModelIndex();
}

/// \reimp
bool TocTreeModel::hasChildren(const QModelIndex& parent) const
{
	// virtual root
	if (!parent.isValid()) {
		return true;
	}

	TocItem* parentItem = itemForIndex(parent);
	if (!parentItem) {
		return false;
	}

	return parentItem->childCount() > 0;
}

void TocTreeModel::clear()
{
	beginResetModel();
	if (_root) {
		_root->clear();
	}
	endResetModel();
}

void TocTreeModel::setFolderIcon(const QIcon& value)
{
    _folderIcon = value;
}

void TocTreeModel::setFileIcon(const QIcon& value)
{
    _fileIcon = value;
}

void TocTreeModel::bind(TocItem* root)
{
	if (root == _root) {
		return;
	}

	beginResetModel();
	_root = root;
	endResetModel();
}

TocItem* TocTreeModel::itemForIndex(const QModelIndex& index) const
{
    if (index.isValid()) {
        if (TocItem* item = static_cast<TocItem*>(index.internalPointer())) {
            return item;
        }
    }
	return _root;
}
