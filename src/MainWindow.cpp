#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QDebug>
#include <QLabel>
#include <QWebFrame>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "EpubReader.h"
#include "OpfReader.h"
#include "NcxReader.h"
#include "TocTreeModel.h"
#include "TocItem.h"
#include "FileInfoModel.h"
#include "MetadataItem.h"
#include "SpineItem.h"
#include "Error.h"
#include "NetworkAccessManager.h"
#include "Util.h"
#include "FileInfoDialog.h"
#include "WebViewEventFilter.h"

class MainWindowPrivate
{
public:
    MainWindow* q;

	EpubReader epubReader;
    TocTreeModel tocTreeModel;
	FileInfoModel fileInfoModel;
	WebViewEventFilter webViewEventFilter;

public:
    MainWindowPrivate(MainWindow* q)
        : q(q)
    {
        tocTreeModel.setFolderIcon(QIcon(":/icons/folder.png"));
        tocTreeModel.setFileIcon(QIcon(":/icons/file.png"));
    }
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    d = new MainWindowPrivate(this);
    ui->setupUi(this);
	setupUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi()
{
	//
	// setup window
	//
	setWindowState(Qt::WindowMaximized);
	setWindowIcon(QIcon(":/icons/app.png"));

	//
	// setup action states
	//
	ui->fileInfoAction->setEnabled(false);
	ui->navPrevAction->setEnabled(false);
	ui->navNextAction->setEnabled(false);
	ui->navFirstAction->setEnabled(false);
	ui->navLastAction->setEnabled(false);
	ui->viewZoomInAction->setEnabled(false);
	ui->viewZoomOutAction->setEnabled(false);
	ui->viewZoomResetAction->setEnabled(false);

	//
	// setup actions events
	//
	connect(ui->fileOpenAction, SIGNAL(triggered()), this, SLOT(fileOpenAction_triggered()));
	connect(ui->toggleStyleAction, SIGNAL(triggered()), this, SLOT(toggleStyleAction_triggered()));
	connect(ui->fileInfoAction, SIGNAL(triggered()), this, SLOT(fileInfoAction_triggered()));
	connect(ui->fileQuitAction, SIGNAL(triggered()), this, SLOT(fileQuitAction_triggered()));
	connect(ui->aboutAction, SIGNAL(triggered()), this, SLOT(aboutAction_triggered()));
	connect(ui->navPrevAction, SIGNAL(triggered()), this, SLOT(navPrevAction_triggered()));
	connect(ui->navNextAction, SIGNAL(triggered()), this, SLOT(navNextAction_triggered()));
	connect(ui->navFirstAction, SIGNAL(triggered()), this, SLOT(navFirstAction_triggered()));
	connect(ui->navLastAction, SIGNAL(triggered()), this, SLOT(navLastAction_triggered()));
	connect(ui->viewZoomInAction, SIGNAL(triggered()), this, SLOT(viewZoomInAction_triggered()));
	connect(ui->viewZoomOutAction, SIGNAL(triggered()), this, SLOT(viewZoomOutAction_triggered()));
	connect(ui->viewZoomResetAction, SIGNAL(triggered()), this, SLOT(viewZoomResetAction_triggered()));

	//
	// setup tocTreeView
	//
    ui->tocTreeView->setModel(&d->tocTreeModel);
	connect(ui->tocTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(tocTreeView_clicked(QModelIndex)));

	//
	// setup webView
	// hook our custom networkAccessManager to handle resource loading from EPUB container
	//
	QWebView* webView = ui->webView;
	QNetworkAccessManager* oldManager = webView->page()->networkAccessManager();
	NetworkAccessManager* newManager = new NetworkAccessManager(oldManager, webView);
	newManager->epubReader(&d->epubReader);
	webView->page()->setNetworkAccessManager(newManager);
	connect(newManager, SIGNAL(scrollToAnchorRequested(QString)), this, SLOT(nam_scrollToAnchorRequested(QString)));
	connect(webView, SIGNAL(urlChanged(QUrl)), this, SLOT(webView_urlChanged(QUrl)));

	//
	// setup various web settings
	//
	QWebSettings* settings = webView->settings();
	settings->setFontFamily(QWebSettings::StandardFont, "Microsoft Yahei");

	ui->webView->installEventFilter(&d->webViewEventFilter);
	connect(&d->webViewEventFilter, SIGNAL(zoomChanged(qreal,qreal)),
			this, SLOT(webViewEventFilter_zoomChanged(qreal,qreal)));

	//
	// setup custom context menu for web view
	//
	ui->webView->setContextMenuPolicy(Qt::ActionsContextMenu);
	ui->webView->addAction(ui->fileOpenAction);
	ui->webView->addAction(ui->toggleStyleAction);
	ui->webView->addAction(ui->fileInfoAction);
	QAction* sep1 = new QAction("", this);
	sep1->setSeparator(true);
	ui->webView->addAction(sep1);
	ui->webView->addAction(ui->navPrevAction);
	ui->webView->addAction(ui->navNextAction);
	ui->webView->addAction(ui->navFirstAction);
	ui->webView->addAction(ui->navLastAction);
	QAction* sep2 = new QAction("", this);
	sep2->setSeparator(true);
	ui->webView->addAction(sep2);
	ui->webView->addAction(ui->viewZoomOutAction);
	ui->webView->addAction(ui->viewZoomResetAction);
	ui->webView->addAction(ui->viewZoomInAction);
}

/// \reimp
void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	QMainWindow::dragEnterEvent(event);

	if (event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	}
}

/// \reimp
void MainWindow::dropEvent(QDropEvent* event)
{
	QMainWindow::dropEvent(event);

	if (!event->mimeData()->urls().isEmpty()) {
		QString filePath = event->mimeData()->urls().first().toLocalFile();
		openFile(filePath);
		event->acceptProposedAction();
	}
}

void MainWindow::openFile(const QString& epubFilePath)
{
	try {
		//
		// IMPORTANT empty model bindings before reading epub (will implicitly change models),
		// to avoid inconsistency between models and views.
		//
		d->tocTreeModel.bind(0);
		d->fileInfoModel.bind(0);

		//
		// read epub file
		//
		d->epubReader.read(epubFilePath);

		d->tocTreeModel.bind(d->epubReader.ncx()->toc());
		d->fileInfoModel.bind(d->epubReader.opf()->metadata());

		ui->tocTreeView->resizeColumnToContents(0);
		navFirstAction_triggered();

		//
		// reset window title
		//
		QString fileName = QFileInfo(epubFilePath).fileName();
		setWindowTitle(QString("%1 - ReadEpub").arg(fileName));

		//
		// update action states
		//
		ui->fileInfoAction->setEnabled(true);
		ui->navPrevAction->setEnabled(true);
		ui->navNextAction->setEnabled(true);
		ui->navFirstAction->setEnabled(true);
		ui->navLastAction->setEnabled(true);
		ui->viewZoomInAction->setEnabled(true);
		ui->viewZoomOutAction->setEnabled(true);
		ui->viewZoomResetAction->setEnabled(true);
	}
	catch (Error& error) {
		QMessageBox::critical(this, tr("Error"), error.message());
	}
}

void MainWindow::fileOpenAction_triggered()
{
	QString epubFilePath = QFileDialog::getOpenFileName(
		this, tr("Open File"), QDir::currentPath(), tr("EPUB File (*.epub)")
	);
	if (!epubFilePath.isEmpty()) {
		openFile(epubFilePath);
    }
}

void MainWindow::toggleStyleAction_triggered()
{
	d->epubReader.optionKeepCss(ui->toggleStyleAction->isChecked());
	ui->webView->reload();
}

void MainWindow::fileInfoAction_triggered()
{
	FileInfoDialog* dlg = new FileInfoDialog(this);
	dlg->bind(&d->fileInfoModel);
	dlg->show();
}

void MainWindow::fileQuitAction_triggered()
{
	qApp->quit();
}

void MainWindow::aboutAction_triggered()
{
	QString copyrightText = "<b>ReadEpub</b><br/>" +
							tr("A fast, elegant EPUB reader.<br/>") +
							"Copyright 2016 &copy; sol (idsol at 163.com)<br/>";

	QString argumentsText = tr("<b>Arguments:</b><br/>"
							   "&lt;filename&gt;: EPUB file name<br/>");

	QString shortcutsText = tr("<b>Shortcuts:</b><br/>");
	foreach (QAction* action, ui->toolBar->actions()) {
		if (action->isSeparator()) {
			continue;
		}
		shortcutsText += QString("%1: %2<br/>")
			.arg(action->iconText())
			.arg(Util::shortcutsToString(action->shortcuts()));
	}

	QMessageBox::information(
		this,
		tr("About"),
		QString("%1<br/>%2<br/>%3")
		.arg(copyrightText)
		.arg(argumentsText)
		.arg(shortcutsText)
	);
}

void MainWindow::navPrevAction_triggered()
{
	QList<SpineItem>* spine = d->epubReader.opf()->spine();
	int i = 0;
	foreach (const SpineItem& item, *spine) {
		if (Util::removeFragment(item.uri()) == currentUri()) {
			break;
		}
		++i;
	}

	if (i > 0) {
		const SpineItem* prevItem = const_cast<SpineItem*>(&spine->at(--i));
		if (prevItem) {
			loadUri(prevItem->uri());
		}
	}
}

void MainWindow::navNextAction_triggered()
{
	QList<SpineItem>* spine = d->epubReader.opf()->spine();
	int i = 0;
	foreach (const SpineItem& item, *spine) {
		if (Util::removeFragment(item.uri()) == currentUri()) {
			break;
		}
		++i;
	}

	if (i >= 0 && i < spine->count() - 1) {
		SpineItem* nextItem = const_cast<SpineItem*>(&spine->at(++i));
		if (nextItem) {
			loadUri(nextItem->uri());
		}
	}
}

void MainWindow::navFirstAction_triggered()
{
	QList<SpineItem>* spine = d->epubReader.opf()->spine();
	SpineItem* firstSpineItem = !spine->empty() ? &spine->first() : 0;
	if (firstSpineItem) {
		loadUri(firstSpineItem->uri());
	}
}

void MainWindow::navLastAction_triggered()
{
	QList<SpineItem>* spine = d->epubReader.opf()->spine();
	SpineItem* lastSpineItem = !spine->empty() ? &spine->last() : 0;
	if (lastSpineItem) {
		loadUri(lastSpineItem->uri());
	}
}

void MainWindow::viewZoomInAction_triggered()
{
	d->webViewEventFilter.zoomIn(ui->webView);
}

void MainWindow::viewZoomOutAction_triggered()
{
	d->webViewEventFilter.zoomOut(ui->webView);
}

void MainWindow::viewZoomResetAction_triggered()
{
	d->webViewEventFilter.zoomReset(ui->webView);
}

void MainWindow::tocTreeView_clicked(const QModelIndex& index)
{
    if (index.isValid()) {
		TocItem* tocItem = d->tocTreeModel.itemForIndex(index);
		if (Util::removeFragment(tocItem->uri()) != currentUri()) {
			loadUri(tocItem->uri());
		}
    }
}

//
// This happens when loaded url contains fragment.
// It has to be explicitly scrolled to the anchor specified by url-fragment as custom loading
// procedure does not do the scrolling as would standard loading procedure.
//
void MainWindow::nam_scrollToAnchorRequested(const QString& anchor)
{
	qDebug() << QString("[MainWindow.nam_scrollToAnchorRequested] anchor = %1").arg(anchor);
	ui->webView->page()->mainFrame()->scrollToAnchor(anchor);
}

void MainWindow::webView_urlChanged(const QUrl& url)
{
	if (url.scheme() == "file") {
		//
		// synchronize active toc treeview item to loaded content
		// FIXME current implmentation only works for first level nodes
		//
		QString uri = currentUri();

		QModelIndexList matchList = d->tocTreeModel.match(
			d->tocTreeModel.index(0, 1),
			Qt::DisplayRole,
			QVariant::fromValue(uri),
			1,
			Qt::MatchStartsWith | Qt::MatchWrap | Qt::MatchRecursive
		);
		QModelIndex match = !matchList.empty() ? matchList.first() : QModelIndex();
		if (match.isValid()) {
			ui->tocTreeView->setCurrentIndex(match);
		}

		//
		// update action states
		//
		QList<SpineItem>* spine = (d->epubReader.opf() && d->epubReader.opf()->spine()) ? d->epubReader.opf()->spine() : 0;
		bool isFirst = (spine && !spine->empty() && Util::removeFragment(spine->first().uri()) == currentUri());
		bool isLast = (spine && !spine->empty() && Util::removeFragment(spine->last().uri()) == currentUri());
		ui->navFirstAction->setEnabled(!isFirst);
		ui->navLastAction->setEnabled(!isLast);
		ui->navPrevAction->setEnabled(!isFirst);
		ui->navNextAction->setEnabled(!isLast);
	}
}

void MainWindow::webViewEventFilter_zoomChanged(qreal oldZoomFactor, qreal zoomFactor)
{
	QString zoomText = QString("%1%").arg(zoomFactor * 100);
	ui->viewZoomResetAction->setText(zoomText);

	ui->viewZoomInAction->setEnabled(d->webViewEventFilter.canZoomIn(ui->webView));
	ui->viewZoomOutAction->setEnabled(d->webViewEventFilter.canZoomOut(ui->webView));
	ui->viewZoomResetAction->setEnabled(d->webViewEventFilter.canZoomReset(ui->webView));
}

/*!
	Loads \a uri to webView.
*/
void MainWindow::loadUri(const QString& uri)
{
	ui->webView->load(uri);
}

/*!
	Returns the current uri loaded in webView, which could be from webView.load() or link click.
*/
QString MainWindow::currentUri() const
{
	QUrl url = ui->webView->url();
	QString res = Util::fileUrlToUri(url.toString());
	res = Util::removeFragment(res);
	return res;
}
