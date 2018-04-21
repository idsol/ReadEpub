#ifndef FILEINFODIALOG_H
#define FILEINFODIALOG_H

#include <QDialog>

namespace Ui {
    class FileInfoDialog;
}

class FileInfoModel;

class FileInfoDialog : public QDialog
{
    Q_OBJECT

public:
	explicit FileInfoDialog(QWidget* parent = 0);
	virtual ~FileInfoDialog();

public:
	void bind(FileInfoModel* model);

private:
	Ui::FileInfoDialog* ui;
};

#endif // FILEINFODIALOG_H
