#include "FileInfoModel.h"
#include "MetadataItem.h"
#include "MetadataKnownNames.h"

/*!
  \class FileInfoModel

  Metadata model to EPUB file (book info).
*/

enum Column
{
	NameColumn = 0,
	ValueColumn = 1,
	ColumnCount = 2
};

FileInfoModel::FileInfoModel(QObject* parent)
	: QAbstractTableModel(parent)
	, _items(0)
{
}

/// \reimp
Qt::ItemFlags FileInfoModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags res = QAbstractTableModel::flags(index);
	if (index.isValid()) {
		res |= Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		if (index.column() == ValueColumn) {
			res |= Qt::ItemIsEditable;
		}
	}
	return res;
}

/// \reimp
QVariant FileInfoModel::data(const QModelIndex& index, int role) const
{
	if (!_items || !index.isValid() || index.column() < 0
		|| index.column() >= ColumnCount) {
		return QVariant();
	}

	if (MetadataItem* item = itemForIndex(index)) {
		if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole) {
			switch (index.column()) {
				case NameColumn: {
					QString knownName = MetadataKnownNames::query(item->name());
					return !knownName.isEmpty() ? knownName : item->name();
				}
				case ValueColumn: {
					return item->value();
				}
				default: Q_ASSERT(false);
			}
		}

		if (role == Qt::TextAlignmentRole) {
			return static_cast<int>(Qt::AlignVCenter | Qt::AlignLeft);
		}
	}

	return QVariant();
}

/// \reimp
QVariant FileInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal) {
		if (role == Qt::DisplayRole) {
			if (section == NameColumn) {
				return tr("Name");
			}
			else if (section == ValueColumn) {
				return tr("Value");
			}
		}
	}
	return QVariant();
}

/// \reimp
int FileInfoModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	if (!_items) {
		return 0;
	}

	return _items->count();
}

/// \reimp
int FileInfoModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return ColumnCount;
}

void FileInfoModel::bind(QList<MetadataItem>* items)
{
	beginResetModel();
	_items = items;
	endResetModel();
}

MetadataItem* FileInfoModel::itemForIndex(const QModelIndex& index) const
{
	if (index.isValid() && _items) {
		return const_cast<MetadataItem*>(&_items->at(index.row()));
	}
	return 0;
}
