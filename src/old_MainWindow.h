#ifndef MainWindow_H
#define MainWindow_H
#include <QMainWindow>
#include <QKeyEvent>
#include "Settings.h"
#include "ui_old_MainWindow.h"

#include "IDataLayer.h"


class old_MainWindow: public QMainWindow, public Ui::old_MainWindow {

Q_OBJECT
public:
    old_MainWindow(QWidget *parent);
    ~old_MainWindow();
	static void insertRow(QTableWidget *t, int row);

	IDataLayer *dl;
public slots:
	void tableClear(QTableWidget * tab);
	void tabChanged(QWidget * aaa);
	void rereadHist();
	void resetFilter();
	void aboutQt();
	void oProg();
	void editFHist();
	void delFHist();
	void daneFirmyClick();
	void settClick();
	void kontrClick();
	void kontrDel();
	void kontrEd();
	void newFra();
	void newPForm();
	void newKor();
	void newDuplikat();
	void newFBrutto();
	void newFRachunek();
	bool close();
	void pomoc();
	void reportBug();
	void towaryDodaj();
	void towaryUsun();
	void towaryEdycja();
	void mainUpdateStatus(QTableWidgetItem *item);
	void showTableMenuT(QPoint p);
	void showTableMenuK(QPoint p);
	void showTableMenuH(QPoint p);
	void pluginSlot();
	void pluginInfoSlot();
	void keyPressEvent(QKeyEvent * event);
private:
	QString workingDir; // should be deprecated
	QMap<int, QString> customActions;
	void saveColumnWidth();
	bool applyFiltr(QString);
	void init();
	bool firstRun();
	void saveAllSett();
	void saveAllSettAsDefault();
	void setupDir();
	void readHist();
	void readKontr();
	void readTw();
protected:
	virtual void loadPlugins();
private slots:
    void on_PrevMonthButton_clicked();
};
#endif
