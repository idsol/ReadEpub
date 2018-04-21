#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class EpubReader;

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    NetworkAccessManager(QNetworkAccessManager* oldManager, QObject* parent = 0);

public:
	EpubReader* epubReader() const;
	void epubReader(EpubReader* epubReader);

signals:
	void scrollToAnchorRequested(const QString& anchor);

protected:
	/// \reimp
	QNetworkReply* createRequest(Operation op, const QNetworkRequest& request, QIODevice* outgoingData = 0);

private:
	EpubReader* _epubReader;
};

#endif // NETWORKACCESSMANAGER_H
