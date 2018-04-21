#include "Util.h"
#include <QUrl>
#include <QDir>

/*!
  \class Util

  Helper methods such as url transforming.
*/

/*!
  Converts FILE protocol url to relative uri to be used for EPUB.

  Removes "file:///" protocol part.
  Removes query items if any.

  \example
  IN	file:///E:/dev/ReadEpub/release/OPS/images/logo-feedbooks-tiny.png
  OUT	OPS/images/logo-feedbooks-tiny.png
*/
QString Util::fileUrlToUri(const QString& fileUrl)
{
	// remove protocol "file:///"
	QString absPath = fileUrl;
	if (absPath.startsWith("file:///")) {
		absPath.remove("file:///");
	}

	//
	// remove any query item
	// \example
	// OPS/images/cover.jpg?t=112010 => OPS/images/cover.jpg
	//
	QUrl canonUri = QUrl(absPath);
	if (canonUri.queryItems().count() > 0) {
		canonUri.setQueryItems(QList<QPair<QString,QString> >());
		absPath = canonUri.toString();
	}

	QString res = QDir::current().relativeFilePath(absPath);
	return res;
}

/*!
  Removes fragment part from \a url and return.

  \example
  IN	OPS/chapter1.xml#figure1
  OUT	OPS/chapter1.xml
*/
QString Util::removeFragment(const QString& url)
{
	return QUrl(url).toString(QUrl::RemoveFragment);
}

/*!
  Makes \a relFile relative to \a relBaseFile, then relative to QDir::currentPath().

  \example
  IN	relFile = fb.ncx, relBaseFile = OPS/fb.opf
  OUT	OPS/fb.ncx

  \example
  IN	relFile = toc.ncx, relBaseFile = content.opf
  OUT	toc.ncx

  \example
  IN	relFile = ../toc.ncx, relBaseFile = OPS/content.opf
  OUT	toc.ncx
*/
QString Util::makeRelative(const QString& relFile, const QString& relBaseFile)
{
	// given QDir::currentPath() = "e:/dev"
	QString absBasePath = QFileInfo(relBaseFile).absolutePath(); // OPS/fb.opf => e:/dev/OPS
	QString absFile = QFileInfo(QDir(absBasePath), relFile).absoluteFilePath(); // toc.ncx => e:/dev/OPS/toc.ncx
	QString res = QDir::current().relativeFilePath(absFile); // e:/dev/OPS/toc.ncx => OPS/toc.ncx
	return res;
}

/// \static
QString Util::shortcutsToString(const QList<QKeySequence>& shortcuts)
{
	QStringList res;
	foreach (QKeySequence shortcut, shortcuts) {
		res.append(shortcut.toString());
	}
	return res.join(" | ");
}
