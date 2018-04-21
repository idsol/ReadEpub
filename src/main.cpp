#include <QtGui/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // core: codec
    QTextCodec* zhCN = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForCStrings(zhCN);
    QTextCodec::setCodecForTr(zhCN);

	// translator
	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(QString("%1/translations/readepub_%2").arg(QApplication::applicationDirPath()).arg(locale));
	app.installTranslator(&translator);

    // decoration: font
    QFont appFont(app.font());
    appFont.setFamily("Tahoma,Î¢ÈíÑÅºÚ,ËÎÌå");
    appFont.setPointSize(10);
    app.setFont(appFont);

    // decoration: style
	app.setStyle("CleanLooks");

    MainWindow mw;
	if (argc == 2) {
		mw.openFile(argv[1]);
	}
    mw.show();

    return app.exec();
}
