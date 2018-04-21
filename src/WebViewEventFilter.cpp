#include "WebViewEventFilter.h"
#include <QEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QWebView>
#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QDragEnterEvent>
#include <QDropEvent>

/*!
  \class WebViewEventFilter

  Handles misc events of QWebView via QObject.installEventFilter() mechanisim, including:
  QWheelEvent, QKeyEvent, QDragEnterEvent, QDropEvent, etc.
*/

///
/// only listed zoom levels are allowed
///
static const int NUM_ZOOM_LEVELS_ALLOWED = 14;
static const qreal ZOOM_LEVELS_ALLOWED[NUM_ZOOM_LEVELS_ALLOWED] = {
	0.3f, 0.5f, 0.67f, 0.8f, 0.9f,
	1.0f, 1.1f, 1.2f, 1.33f, 1.5f,
	1.7f, 2.0f, 2.4f, 3.0f
};

WebViewEventFilter::WebViewEventFilter(QObject* parent)
	: QObject(parent)
{
}

/// \reimp
bool WebViewEventFilter::eventFilter(QObject* watched, QEvent* event)
{
	if (event->type() == QEvent::Wheel) {
		QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
		bool ctrl = wheelEvent->modifiers() & Qt::ControlModifier;

		//
		// ctrl + wheel = zoom-in/out
		//
		if (ctrl) {
			//
			// numSteps: +1 = push forward = zoom-in, -1 = push backward = zoom-out
			//
			int numDegrees = wheelEvent->delta() / 8;
			int numSteps = numDegrees / 15;

			QWebView* webView = dynamic_cast<QWebView*>(watched);
			if (webView) {
				//
				// zoom-in levels:  110% 120% 133% 150% 170% 200% 240% 300%
				// zoom-out levels: 90% 80% 67% 50% 30%
				//
				bool zoomInWanted = (numSteps == 1);
				if (zoomInWanted) {
					zoomIn(webView);
				}
				else {
					zoomOut(webView);
				}
			}
			return true;
		}

		return false;
	}

	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		bool ctrl = keyEvent->modifiers() & Qt::ControlModifier;

		//
		// ctrl + c = copy selection
		//
		if (ctrl && keyEvent->key() == Qt::Key_C) {
			QWebView* webView = dynamic_cast<QWebView*>(watched);
			if (webView && !webView->selectedText().isEmpty()) {
				QApplication::clipboard()->setText(webView->selectedText());
			}
			return true;
		}

		return false;
	}

	//
	// prevent default file drop handling of WebView, just propagate it to its parent widget
	//
	if (event->type() == QEvent::DragEnter || event->type() == QEvent::Drop) {
		return true;
	}

	// standard event processing
	return QObject::eventFilter(watched, event);
}


void WebViewEventFilter::zoomIn(QWebView* webView)
{
	if (!webView) {
		return;
	}

	qreal oldZoomFactor = webView->zoomFactor();
	qreal newZoomFactor = oldZoomFactor;

	for (int i = 0; i < NUM_ZOOM_LEVELS_ALLOWED; ++i) {
		if (ZOOM_LEVELS_ALLOWED[i] == oldZoomFactor && i < NUM_ZOOM_LEVELS_ALLOWED - 1) {
			newZoomFactor = ZOOM_LEVELS_ALLOWED[i+1];
			break;
		}
	}

	if (oldZoomFactor != newZoomFactor) {
		webView->setZoomFactor(newZoomFactor);
		emit zoomChanged(oldZoomFactor, newZoomFactor);
	}
}

void WebViewEventFilter::zoomOut(QWebView* webView)
{
	if (!webView) {
		return;
	}

	qreal oldZoomFactor = webView->zoomFactor();
	qreal newZoomFactor = oldZoomFactor;

	for (int i = 0; i < NUM_ZOOM_LEVELS_ALLOWED; ++i) {
		if (ZOOM_LEVELS_ALLOWED[i] == oldZoomFactor && i > 0) {
			newZoomFactor = ZOOM_LEVELS_ALLOWED[i-1];
			break;
		}
	}

	if (oldZoomFactor != newZoomFactor) {
		webView->setZoomFactor(newZoomFactor);
		emit zoomChanged(oldZoomFactor, newZoomFactor);
	}
}

void WebViewEventFilter::zoomReset(QWebView* webView)
{
	if (!webView) {
		return;
	}

	qreal oldZoomFactor = webView->zoomFactor();
	qreal newZoomFactor = 1.0f;
	if (oldZoomFactor != newZoomFactor) {
		webView->setZoomFactor(newZoomFactor);
		emit zoomChanged(oldZoomFactor, newZoomFactor);
	}
}

bool WebViewEventFilter::canZoomIn(QWebView* webView)
{
	if (!webView) {
		return false;
	}

	qreal zoomFactor = webView->zoomFactor();
	return (zoomFactor < ZOOM_LEVELS_ALLOWED[NUM_ZOOM_LEVELS_ALLOWED-1]);
}

bool WebViewEventFilter::canZoomOut(QWebView* webView)
{
	if (!webView) {
		return false;
	}

	qreal zoomFactor = webView->zoomFactor();
	return (zoomFactor > ZOOM_LEVELS_ALLOWED[0]);
}

bool WebViewEventFilter::canZoomReset(QWebView* webView)
{
	if (!webView) {
		return false;
	}

	qreal zoomFactor = webView->zoomFactor();
	return (zoomFactor != 1.0f);
}
