#ifndef WEBVIEWEVENTFILTER_H
#define WEBVIEWEVENTFILTER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QWebView;
QT_END_NAMESPACE

class WebViewEventFilter : public QObject
{
    Q_OBJECT

public:
	explicit WebViewEventFilter(QObject* parent = 0);

public:
	void zoomIn(QWebView* webView);
	void zoomOut(QWebView* webView);
	void zoomReset(QWebView* webView);

	bool canZoomIn(QWebView* webView);
	bool canZoomOut(QWebView* webView);
	bool canZoomReset(QWebView* webView);

protected:
	/// \reimp
	bool eventFilter(QObject* watched, QEvent* event);

signals:
	void zoomChanged(qreal oldZoomFactor, qreal zoomFactor);
};

#endif // WEBVIEWEVENTFILTER_H
