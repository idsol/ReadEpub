#ifndef NETWORKREPLY_H
#define NETWORKREPLY_H

#include <QNetworkReply>

class NetworkReply : public QNetworkReply
{
    Q_OBJECT

public:
	NetworkReply(const QUrl& url, QNetworkAccessManager::Operation op, QObject* parent);

protected:
	/// \reimp
    qint64 readData(char* data, qint64 maxSize);
    void abort() { close(); }
	qint64 bytesAvailable() const { return _data.size() - _offset; }
    bool isSequential() const { return true; }

public:
	bool hasError() const { return (error() != NoError); }

private:
	void replyUrl(const QUrl& url);

private:
	QByteArray _data;
	qint64 _offset;
};

#endif // NETWORKREPLY_H
