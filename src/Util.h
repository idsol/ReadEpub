#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QList>
#include <QKeySequence>

class Util
{
public:
	static QString fileUrlToUri(const QString& fileUrl);
	static QString removeFragment(const QString& url);
	static QString makeRelative(const QString& relFile, const QString& relBaseFile);
	static QString shortcutsToString(const QList<QKeySequence>& shortcuts);

private:
	Util() { /* prohibited */ }
};

#endif // UTIL_H
