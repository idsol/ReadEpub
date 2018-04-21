#include "NetworkReply.h"
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include "NetworkAccessManager.h"
#include "EpubReader.h"
#include "EpubMimeTypes.h"
#include "Util.h"
#include "Error.h"

/*!
	\class NetworkReply
	
	Inherits QNetworkReply to handle 'file' protocol GET request from WebView,
	in EPUB domain, it means GET requesting a file entry in EPUB zip file.
	
	\sa NetworkAccessManager
*/

NetworkReply::NetworkReply(const QUrl& url, QNetworkAccessManager::Operation op, QObject* parent)
	: QNetworkReply(parent)
	, _offset(0)
{
    setUrl(url);
    setOperation(op);
    open(QNetworkReply::ReadOnly | QNetworkReply::Unbuffered);

	// We queue an emission of the readyRead() signal. This tells the browser
	// that the reply's data can be read. We need to queue the signal rather
	// than emit it directly because, when the reply is being created, the
	// browser does not yet know about it.
	// Similarly, we queue an emission of the finishied() signal to tell the
	// browser that all the data is available for unbuffered reading.
	//
    QTimer::singleShot(0, this, SIGNAL(readyRead()));
    QTimer::singleShot(0, this, SIGNAL(finished()));

    replyUrl(url);
}

/*!
	\internal
	Populates _data according to \a url, from the EPUB package.
*/
void NetworkReply::replyUrl(const QUrl& url)
{
	if (url.scheme() == "file") {
		NetworkAccessManager* nam = static_cast<NetworkAccessManager*>(parent());
		if (nam) {
			EpubReader* epub = nam->epubReader();
			if (epub) {
				QString uri = Util::fileUrlToUri(url.toString());
				QString mimeType = EpubMimeTypes::query(Util::removeFragment(uri));

				if (mimeType == EpubMimeTypesConstants::CSS && !epub->optionKeepCss()) {
					// ignore CSS files
				}
				else {
					setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
					try {
						_data = epub->readEntryContent(uri);
					}
					catch (Error& error) {
						setError(ContentNotFoundError, error.message());
					}
				}

				static bool verbose = false;
				if (verbose) {
					qDebug() << QString("[NetworkReply.replyUrl] uri = %1, mimeType = %2, size = %3")
						.arg(uri)
						.arg(mimeType)
						.arg(_data.size());
				}
			}
			else {
				setError(ConnectionRefusedError, tr("[NetworkReply.replyUrl] Error! EpubReader is invalid (null)."));
			}
		}
		else {
			setError(ConnectionRefusedError, tr("[NetworkReply.replyUrl] Error! NetworkAccessManager is invalid (null)."));
		}
    }

	setHeader(QNetworkRequest::ContentLengthHeader, QVariant(_data.size()));
}

/// \reimp
qint64 NetworkReply::readData(char* data, qint64 maxSize)
{
	if (_offset >= _data.size()) {
		return -1;
	}

	qint64 count = qMin(maxSize, _data.size() - _offset);
	memcpy(data, _data.constData() + _offset, count);
	_offset += count;
    return count;
}
