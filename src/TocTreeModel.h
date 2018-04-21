#ifndef TOCTREEMODEL_H
#define TOCTREEMODEL_H

#include <QAbstractItemModel>
#include <QIcon>
#include "TocItem.h"

QT_BEGIN_NAMESPACE
class QXmlStreamReader;
class QXmlStreamWriter;
QT_END_NAMESPACE

class TocTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TocTreeModel(QObject* parent = 0);
	virtual ~TocTreeModel();

public:
	/// \group reimp
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
	bool hasChildren(const QModelIndex& parent) const;

public:
	void bind(TocItem* root);

	TocItem* itemForIndex(const QModelIndex& index) const;

    void clear();

public:
    void setFolderIcon(const QIcon& value = QIcon());
    void setFileIcon(const QIcon& value = QIcon());

private:
	TocItem* _root;
    QIcon _folderIcon;
    QIcon _fileIcon;
};

#endif // TOCTREEMODEL_H
