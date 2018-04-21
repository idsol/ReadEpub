#include "FileInfoDialog.h"
#include "ui_FileInfoDialog.h"
#include "FileInfoModel.h"

/*!
  \class FileInfoDialog

  Shows metadata (book info) of current EPUB file.
*/

FileInfoDialog::FileInfoDialog(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::FileInfoDialog)
{
    ui->setupUi(this);
}

FileInfoDialog::~FileInfoDialog()
{
    delete ui;
}

void FileInfoDialog::bind(FileInfoModel* model)
{
	ui->tableView->setModel(model);

	QHeaderView* header = ui->tableView->horizontalHeader();
	header->setResizeMode(0, QHeaderView::ResizeToContents);
	header->setResizeMode(1, QHeaderView::Stretch);
}
