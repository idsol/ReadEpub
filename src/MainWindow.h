#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
    class MainWindow;
}

class MainWindowPrivate;
class TocItem;
class SpineItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    virtual ~MainWindow();

private:
	void setupUi();

protected:
	/// \group reimp
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);

public:
	void openFile(const QString& epubFilePath);

private:
	void loadUri(const QString& uri);
	QString currentUri() const;

private slots:
	/// \group slots
	void fileOpenAction_triggered();
	void toggleStyleAction_triggered();
	void fileInfoAction_triggered();
	void fileQuitAction_triggered();
	void aboutAction_triggered();

	void navPrevAction_triggered();
	void navNextAction_triggered();
	void navFirstAction_triggered();
	void navLastAction_triggered();

	void viewZoomInAction_triggered();
	void viewZoomOutAction_triggered();
	void viewZoomResetAction_triggered();

    void tocTreeView_clicked(const QModelIndex& index);

	void nam_scrollToAnchorRequested(const QString& anchor);
	void webView_urlChanged(const QUrl& url);
	void webViewEventFilter_zoomChanged(qreal oldZoomFactor, qreal zoomFactor);

private:
    Ui::MainWindow* ui;
    MainWindowPrivate* d;
};

#endif // MAINWINDOW_H
