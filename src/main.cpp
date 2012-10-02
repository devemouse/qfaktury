#include <QApplication>
#include <QTimer>
#include <QResource>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QTranslator>
#include <QWaitCondition>

#include "old_MainWindow.h"
#include "MainWindow.h"
#include "Settings.h"

// do splasha

int main(int argc, char **argv) {
	QApplication a(argc, argv);
   a.setApplicationName("QFaktury");
	a.setOrganizationName("www.e-linux.pl");


	// multilangage
    a.installTranslator(sett().getTranslation());


	QResource::registerResource("qfaktury.rcc"); // using the rcc file so it's more portable
	// Q_INIT_RESOURCE(qfaktury);


	QRect screen = QApplication::desktop()->screenGeometry();

	QSplashScreen splash(QPixmap(":/res/icons/splash.png"));

    old_MainWindow w(0);
    MainWindow w2(0);
	w.move(screen.center() - QPoint(w.width() / 2, w.height() / 2));
    w2.move(screen.center() - QPoint(w.width() / 2, w.height() / 2));


        //if (a.arguments().contains("--nosplash")) {
		w.show();
        w2.show();
        /*} else {
		splash.show();

		a.processEvents();

		QTimer::singleShot(5000, &w, SLOT(show()));
		QTimer::singleShot(4960, &splash, SLOT(close()));
        }*/

	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

	QIcon icon;
	icon.addPixmap(QPixmap(":/res/icons/qfaktury_48.png"), QIcon::Normal, QIcon::Off);
	a.setWindowIcon(icon);
	return a.exec();
}

#ifdef WIN32
#if CMAKE_BUILD_TYPE == Release
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {
    return main(0,0);
}
#endif
#endif

