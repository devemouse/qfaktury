#include <QApplication>
#include <QtCore>
#include <QTimer>
#include <QResource>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QTranslator>
#include <QWaitCondition>

#include "old_MainWindow.h"
#include "MainWindow.h"
#include "Settings.h"
#include "DatabaseManager.h"

// do splasha

class MyThread : public QThread
{
public:
    explicit MyThread(QObject *parent = 0)
        :QThread(parent){}
    ~MyThread(){}

    void run()
    {
        QRect screen = QApplication::desktop()->screenGeometry();
        old_MainWindow w(0);
        w.move(screen.center() - QPoint(w.width() / 2, w.height() / 2));
        w.show();
    }
};


int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    a.setApplicationName("QFaktury");
    a.setOrganizationName("www.e-linux.pl");

    DatabaseManager test;
    if (test.openDB())
        qDebug() << "openned";
    else
        qDebug() << "failed to open";

    qDebug() << test.getTables();
    test.createTables();

    // multilangage
    a.installTranslator(sett().getTranslation());


    QResource::registerResource("qfaktury.rcc"); // using the rcc file so it's more portable
    // Q_INIT_RESOURCE(qfaktury);


    QRect screen = QApplication::desktop()->screenGeometry();

    QSplashScreen splash(QPixmap(":/res/icons/splash.png"));


    MainWindow w(0);
    w.move(screen.center() - QPoint(w.width() / 2, w.height() / 2));
    w.show();



#if 1
    old_MainWindow old_w(0);
    old_w.move(screen.center() - QPoint(old_w.width() / 2, old_w.height() / 2));
    old_w.show();
#endif

    a.connect(&w, SIGNAL(closingWindow), &old_w, SLOT(close));

    //if (a.arguments().contains("--nosplash")) {


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

