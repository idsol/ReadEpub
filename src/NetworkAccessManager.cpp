#include "NetworkAccessManager.h"
#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>
#include "NetworkReply.h"
#include "EpubReader.h"
#include "EpubMimeTypes.h"
#include "Util.h"

/*!
  \class NetworkAccessManager

  Inherits QNetworkAccessManager to handle 'file' protocol GET request from WebView,
  in EPUB domain, it means GET requesting a file entry in EPUB zip file.

  \sa NetworkReply
*/

NetworkAccessManager::NetworkAccessManager(QNetworkAccessManager* oldManager, QObject* parent)
    : QNetworkAccessManager(parent)
	, _epubReader(0)
{
    setCache(oldManager->cache());
    setCookieJar(oldManager->cookieJar());
    setProxy(oldManager->proxy());
    setProxyFactory(oldManager->proxyFactory());
}

/*!
  \reimp
  Returns a new QNetworkReply object to handle the operation op and request req.
*/
QNetworkReply* NetworkAccessManager::createRequest(Operation op, const QNetworkRequest& req, QIODevice* outgoingData)
{
	if (req.url().scheme() == "file" && op == GetOperation) {
		NetworkReply* reply = new NetworkReply(req.url(), QNetworkAccessManager::GetOperation, this);

		if (reply->hasError()) {
			QMessageBox::critical(0, tr("Error"), reply->errorString());
		}

		//
		// if request url has fragment, request manually scrolling to the anchor specified by fragment.
		// FIXME it does not work as expected - authough url sent to WebView.load(url) DOES contains
		// fragment, it was removed by WebView for some unknown reason.
		//
//		qDebug() << QString("[NetworkAccessManager.createRequest] url = %1, fragment = %2").arg(req.url()).arg(req.url().fragment());
		QString fragment = req.url().hasFragment() ? req.url().fragment() : QString();
		if (!fragment.isEmpty()) {
			QString uri = Util::fileUrlToUri(req.url().toString());
			QString mimeType = EpubMimeTypes::query(Util::removeFragment(uri));
			if (mimeType == EpubMimeTypesConstants::HTML) {
				emit scrollToAnchorRequested(fragment);
			}
		}

        return reply;
    }

    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}

EpubReader* NetworkAccessManager::epubReader() const
{
	return _epubReader;
}

void NetworkAccessManager::epubReader(EpubReader* epubReader)
{
	_epubReader = epubReader;
}
