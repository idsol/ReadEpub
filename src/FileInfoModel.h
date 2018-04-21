#ifndef FILEINFOMODEL_H
#define FILEINFOMODEL_H

#include <QAbstractTableModel>
#include "MetadataItem.h"

class FileInfoModel : public QAbstractTableModel
{
    Q_OBJECT

public:
	explicit FileInfoModel(QObject* parent = 0);

public:
	/// \group reimp
	Qt::ItemFlags flags(const QModelIndex& index) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;

public:
	void bind(QList<MetadataItem>* items);
	MetadataItem* itemForIndex(const QModelIndex& index) const;

private:
	QList<MetadataItem>* _items;
};

#endif // FILEINFOMODEL_H
