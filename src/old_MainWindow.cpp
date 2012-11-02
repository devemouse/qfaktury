#include "moc_old_MainWindow.cpp"
#include <QTextCodec>
#include <QMessageBox>
#include <QApplication>
#include <QEvent>
#include <QMenu>
#include <QUrl>
#include <Qt/qdom.h>
#include <QTextStream>
#include <QtDebug>
#include <QDesktopServices>
#include <QProcess>

#include "Ustawienia.h"
#include "Uzytkownik.h"
#include "Towary.h"
#include "Faktura.h"
#include "Korekta.h"
#include "Duplikat.h"
#include "FakturaBrutto.h"
#include "KorektaBrutto.h"
#include "Rachunek.h"
#include "Kontrahenci.h"
#include "DateWidgetItem.h"

#include "XmlDataLayer.h"



/** Constructor
 */
old_MainWindow::old_MainWindow(QWidget *parent): QMainWindow(parent) {
    setupUi(this);
    init();
}

/** Destructor
 */
old_MainWindow::~old_MainWindow() {
	saveAllSett();
}

/**
 * init() method
 */
void old_MainWindow::init() {

	// first run
	if (firstRun()) {
		// towary/uslugi - wymiary
		saveAllSettAsDefault();
		setupDir();

		if (QMessageBox::warning(this, "QFaktury", trUtf8("Pierwsze uruchomienie programu. Czy chcesz skonfigurować firmę?"), trUtf8("Tak"), trUtf8("Nie"), 0, 0,
				1) == 0) {
			daneFirmyClick();
		}

	} else {
		// qDebug() << sett().getValueAsDate("filtrStart")
		//	 	 << sett().getValueAsDate("filtrEnd");
		setupDir();
		filtrStart->setDisplayFormat(sett().getDateFormat());
		filtrStart->setDate(sett().getValueAsDate("filtrStart"));
		filtrEnd->setDisplayFormat(sett().getDateFormat());
		filtrEnd->setDate(sett().getValueAsDate("filtrEnd"));
	}


	// choose data access mode
	dl = new XmlDataLayer();

	// towary/uslugi - wymiary
	tableT->setColumnWidth(0, sett().value("towCol0", QVariant(50)) .toInt());
	tableT->setColumnWidth(1, sett().value("towCol1", QVariant(140)) .toInt());
	tableT->setColumnWidth(3, sett().value("towCol2", QVariant(40)) .toInt());
	tableT->setColumnWidth(4, sett().value("towCol3", QVariant(60)) .toInt());
	tableT->setColumnWidth(5, sett().value("towCol4", QVariant(50)) .toInt());
	tableT->setColumnWidth(6, sett().value("towCol5", QVariant(55)) .toInt()); // netto1
	tableT->setColumnWidth(7, sett().value("towCol6", QVariant(55)) .toInt()); // netto2
	tableT->setColumnWidth(8, sett().value("towCol7", QVariant(55)) .toInt()); // netto3
	tableT->setColumnWidth(9, sett().value("towCol8", QVariant(55)) .toInt());; // netto4
	tableT->setColumnWidth(10, sett().value("towCol9", QVariant(55)) .toInt());

	tableH->setColumnWidth(0, sett().value("histCol0", QVariant(0)) .toInt());
	tableH->setColumnWidth(1, sett().value("histCol1", QVariant(40)) .toInt());
	tableH->setColumnWidth(3, sett().value("histCol2", QVariant(40)) .toInt());
	tableH->setColumnWidth(4, sett().value("histCol3", QVariant(140)) .toInt());
	tableH->setColumnWidth(5, sett().value("histCol4", QVariant(50)) .toInt());

	tableK->setColumnWidth(0, sett().value("custCol0", QVariant(50)) .toInt());
	tableK->setColumnWidth(1, sett().value("custCol1", QVariant(140)) .toInt());
	tableK->setColumnWidth(3, sett().value("custCol2", QVariant(40)) .toInt());
	tableK->setColumnWidth(4, sett().value("custCol3", QVariant(60)) .toInt());
	tableK->setColumnWidth(5, sett().value("custCol4", QVariant(50)) .toInt());

	// add Icon
	QIcon icon;
	icon.addPixmap(QPixmap(":/res/icons/qfaktury_48.png"),
			QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);


	this->setWindowTitle( sett().getVersion(qAppName() ));

	// connect slots
	connect(actionBug, SIGNAL (activated ()), this, SLOT(reportBug()));
	connect(applyFiltrBtn, SIGNAL (clicked()), this, SLOT(rereadHist()));
	connect(resetFiltrBtn, SIGNAL(clicked()), this, SLOT(resetFilter()));
	connect(plikDane_firmyAction, SIGNAL(activated()), this, SLOT(daneFirmyClick()));
	connect(plikKoniecAction, SIGNAL(activated()), this, SLOT(close()));
	connect(kontrahenciDodajAction, SIGNAL(activated()), this, SLOT(kontrClick()));
	connect(kontrahenciUsunAction, SIGNAL(activated()), this, SLOT(kontrDel()));
	connect(kontrahenciEdycjaAction, SIGNAL(activated()), this, SLOT(kontrEd()));
	connect(fakturyDodajAction, SIGNAL(activated()), this, SLOT(newFra()));
	connect(fakturyUsunAction, SIGNAL(activated()), this, SLOT(delFHist()));
	connect(fakturyEdAction, SIGNAL(activated()), this, SLOT(editFHist()));
	connect(fakturyDuplikatAction, SIGNAL(activated()), this, SLOT(newDuplikat()));
	connect(fakturyBruttoAction, SIGNAL(activated()), this, SLOT(newFBrutto()));
	connect(fakturyRachunekAction, SIGNAL(activated()), this, SLOT(newFRachunek()));
	connect(fakturyKorektaAction, SIGNAL(activated()), this, SLOT(newKor()));
	connect(fakturyPFormaAction, SIGNAL(activated()), this, SLOT(newPForm()));
	connect(towaryDodajAction, SIGNAL(activated()), this, SLOT(towaryDodaj()));
	connect(towaryEdycjaAction, SIGNAL(activated()), this, SLOT(towaryEdycja()));
	connect(towaryUsunAction, SIGNAL(activated()), this, SLOT(towaryUsun()));
	connect(pomocO_QtAction, SIGNAL(activated()), this, SLOT(aboutQt()));
	connect(pomocO_programieAction, SIGNAL(activated()), this, SLOT(oProg()));
	connect(plikUstawieniaAction, SIGNAL(activated()), this, SLOT(settClick()));
	connect(tabWidget2, SIGNAL(currentChanged(QWidget*)), this, SLOT(tabChanged(QWidget*)));
	connect(pomocPomocAction, SIGNAL(activated()), this, SLOT(pomoc()));
	connect(tableH, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editFHist()));
	connect(tableH, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuH(QPoint)));
	connect(tableK, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(kontrEd()));
	connect(tableK, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuK(QPoint)));
	connect(tableT, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(towaryEdycja()));
	connect(tableT, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuT(QPoint)));

	connect(tableH, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(mainUpdateStatus(QTableWidgetItem *)));
	connect(tableK, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(mainUpdateStatus(QTableWidgetItem *)));
	connect(tableT, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(mainUpdateStatus(QTableWidgetItem *)));

	tabChanged(tabWidget2);

	readKontr();
	readHist();
	readTw();

	loadPlugins();
}


/**
 *  Loads PyQt plugins
 */
void old_MainWindow::loadPlugins() {
	QDir allFiles;
	QString text, path;
	path = sett().getWorkingDir() + "/plugins/";
	allFiles.setPath(path);
	allFiles.setFilter(QDir::Files);
	QStringList filters;
	filters << "*.py" << "*.Py" << "*.PY" << "*.pY";
	allFiles.setNameFilters(filters);
	QStringList files = allFiles.entryList();
	int i, max = files.count();
	for (i = 0; i < max; ++i) {
		QFile skrypt(path + allFiles[i]);
		if (!skrypt.open(QIODevice::ReadOnly)) {
			// return;
		} else {
			QTextStream t(&skrypt);
			t.readLine ();
			QAction *action = new QAction(t.readLine ().remove ("# "), this);
			action->setData(QVariant(i));
			connect(action, SIGNAL(triggered()), this, SLOT (pluginSlot()));
			this->menuPlugins->addAction(action);
			customActions[i] = path + allFiles[i];
		}
	}
	menuPlugins->addSeparator();
    menuPlugins->addAction(trUtf8("Informacje"), this, SLOT (pluginInfoSlot()));
}

void old_MainWindow::on_PrevMonthButton_clicked()
{

}

/**
 * firstRun setup()
 */
bool old_MainWindow::firstRun() {
	bool ok = sett().value("firstrun", true).toBool();
	// qDebug() << "firstRun" << ok;
	if (ok) {
		sett().checkSettings();
		// set dates for filter
		filtrStart->setDate(sett().getFirstDayOfYear());
		filtrEnd->setDate(sett().getLastDayOfYear());
		return ok;
	} else {
		sett().checkSettings();
		return ok;
	}

}

/** save sett() before quit
 * save column width
 */
void old_MainWindow::saveColumnWidth() {
	// width of the columns in the towary "goods" tab
	sett().setValue("towCol0", tableT->columnWidth(0));
	sett().setValue("towCol1", tableT->columnWidth(1));
	sett().setValue("towCol2", tableT->columnWidth(2));
	sett().setValue("towCol3", tableT->columnWidth(3));
	sett().setValue("towCol4", tableT->columnWidth(4));
	sett().setValue("towCol5", tableT->columnWidth(5));
	sett().setValue("towCol6", tableT->columnWidth(6));
	sett().setValue("towCol7", tableT->columnWidth(7));
	sett().setValue("towCol8", tableT->columnWidth(8));
	sett().setValue("towCol9", tableT->columnWidth(9));
	sett().setValue("towCol10", tableT->columnWidth(10));
	// width of the columns in the history tab
	sett().setValue("histCol0", tableH->columnWidth(0));
	sett().setValue("histCol1", tableH->columnWidth(1));
	sett().setValue("histCol2", tableH->columnWidth(2));
	sett().setValue("histCol3", tableH->columnWidth(3));
	sett().setValue("histCol4", tableH->columnWidth(4));
	sett().setValue("histCol5", tableH->columnWidth(5));
	// width of the columns in the customers tab
	sett().setValue("custCol0", tableK->columnWidth(0));
	sett().setValue("custCol1", tableK->columnWidth(1));
	sett().setValue("custCol2", tableK->columnWidth(2));
	sett().setValue("custCol3", tableK->columnWidth(3));
	sett().setValue("custCol4", tableK->columnWidth(4));
	sett().setValue("custCol5", tableK->columnWidth(5));
}

/** Saves all sett() as default - first run
 */
void old_MainWindow::saveAllSettAsDefault() {
	sett().resetSettings();
}


/** Saves all sett()
 */
void old_MainWindow::saveAllSett() {
	// save filtr
	sett().setValue("filtrStart", filtrStart->date());
	sett().setValue("filtrEnd", filtrEnd->date());

	saveColumnWidth();

	// save unsaved
	sett().sync();
}


/** Clears content of the QTableWidget passed in the input
 *  @param QTableWidget
 */
void old_MainWindow::tableClear(QTableWidget * tab) {
	// should be a better method to do that
	tab->setRowCount(0);
}

/** Used while adding new row
 */
void old_MainWindow::insertRow(QTableWidget *t, int row) {
	t->insertRow(row);
	for (int i = 0; i < t->columnCount(); i++) {
		t->setItem(row, i, new QTableWidgetItem());
	}
}

/** Reads the invoices from the directory passed in the input.
 *  @param QString - directory from where the invoices should be read
 */
void old_MainWindow::readHist() {
	QVector<InvoiceData> invoicesVec;
	invoicesVec = dl->invoiceSelectAllData(filtrStart->date(), filtrEnd->date());
	tableH->setSortingEnabled(false);
	for (int i = 0; i < invoicesVec.size(); ++i) {
		insertRow(tableH, tableH->rowCount());
		QString text = invoicesVec.at(i).id;
		tableH->item(tableH->rowCount() - 1, 0)->setText(text);
		text = invoicesVec.at(i).frNr;
		tableH->item(tableH->rowCount() - 1, 1)->setText(text);
        tableH -> setItem(tableH->rowCount() - 1, 2, new DateWidgetItem(invoicesVec.at(i).sellingDate));
		text = invoicesVec.at(i).type;
		tableH->item(tableH->rowCount() - 1, 3)->setText(text);
		text = invoicesVec.at(i).custName;
		tableH->item(tableH->rowCount() - 1, 4)->setText(text);
		text = invoicesVec.at(i).custTic;
		tableH->item(tableH->rowCount() - 1, 5)->setText(text);
	}
	tableH->setSortingEnabled(true);
	tableH->resizeColumnsToContents();
}

/** Reads customers from the XML
 */
void old_MainWindow::readKontr() {
	tableClear(tableK);
	tableK->setSortingEnabled(false);
	QVector<KontrData> kontrVec = dl->kontrahenciSelectAllData();
	for (int i = 0; i < kontrVec.size(); ++i) {
		insertRow(tableK, tableK->rowCount());
		QString text = kontrVec.at(i).name;
		tableK->item(tableK->rowCount() - 1, 0)->setText(text);
		text = kontrVec.at(i).type;
		tableK->item(tableK->rowCount() - 1, 1)->setText(text);
		text = kontrVec.at(i).place;
		tableK->item(tableK->rowCount() - 1, 2)->setText(text);
		text = kontrVec.at(i).address;
		tableK->item(tableK->rowCount() - 1, 3)->setText(text);
		text = kontrVec.at(i).phone;
		tableK->item(tableK->rowCount() - 1, 4)->setText(text);
	}
	tableK->setSortingEnabled(true);
	tableK->resizeColumnsToContents();
}

/** Reads goods from the XML
 */
void old_MainWindow::readTw() {
	tableClear(tableT);
	QVector<ProductData> prodVec = dl->productsSelectAllData();
	for (int i = 0; i < prodVec.size(); ++i) {
		insertRow(tableT, tableT->rowCount());
		QString text = QString::number(prodVec.at(i).id);
		tableT->item(tableT->rowCount() - 1, 0)->setText(text);
		text = prodVec.at(i).name;
		tableT->item(tableT->rowCount() - 1, 1)->setText(text);
		text = prodVec.at(i).desc;
		tableT->item(tableT->rowCount() - 1, 2)->setText(text);
		text = prodVec.at(i).code;
		tableT->item(tableT->rowCount() - 1, 3)->setText(text);
		text = prodVec.at(i).pkwiu;
		tableT->item(tableT->rowCount() - 1, 4)->setText(text);
		text = prodVec.at(i).type;
		tableT->item(tableT->rowCount() - 1, 5)->setText(text);
		text = prodVec.at(i).quanType;
		tableT->item(tableT->rowCount() - 1, 6)->setText(text);
		text = sett().numberToString(prodVec.at(i).prices[0]);
		tableT->item(tableT->rowCount() - 1, 7)->setText(text);
		text = sett().numberToString(prodVec.at(i).prices[0]);
		tableT->item(tableT->rowCount() - 1, 8)->setText(text);
		text = sett().numberToString(prodVec.at(i).prices[0]);
		tableT->item(tableT->rowCount() - 1, 9)->setText(text);
		text = sett().numberToString(prodVec.at(i).prices[0]);
		tableT->item(tableT->rowCount() - 1, 10)->setText(text);
		text = QString::number(prodVec.at(i).vat);
		tableT->item(tableT->rowCount() - 1, 11)->setText(text);
	}

	tableT->setSortingEnabled(true);
	tableT->resizeColumnsToContents();
}

/** Creates directories if required
 */
void old_MainWindow::setupDir() {
	workingDir = sett().getWorkingDir();
	QDir dir(workingDir);
	if (!dir.exists(sett().getDataDir()))
		dir.mkpath(workingDir + sett().getDataDir());
}


// ----------------------------------------  SLOTS ---------------------------------//

void old_MainWindow::keyPressEvent(QKeyEvent * event) {
	// qDebug() << __FUNCTION__;
	// for now not really used could be deleted
	if (event->key() == Qt::Key_F5) {
		newFra();
	}
	if (event->key() == Qt::Key_Return) {
		// int tabNo = tabWidget2-
		switch (tabWidget2->currentIndex()) {
		case 0: {
			editFHist();
			break;
		}
		case 1: {
			kontrEd();
			break;
		}
		case 2: {
			towaryEdycja();
			break;
		}

		}
	}
	if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_PageUp) {
		if (tabWidget2->currentIndex() != tabWidget2->count() - 1) {
			tabWidget2->setCurrentIndex(tabWidget2->currentIndex() + 1);
		} else {
			tabWidget2->setCurrentIndex(0);
		}
	}
	if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_PageDown) {
		if (tabWidget2->currentIndex() != 0) {
			tabWidget2->setCurrentIndex(tabWidget2->currentIndex() - 1);
		} else {
			tabWidget2->setCurrentIndex(tabWidget2->count() - 1);
		}
	}
}


/** Slot
 *  Just show the message.
 */
void old_MainWindow::pluginInfoSlot() {
	QMessageBox::information(this, trUtf8("QFaktury"),
			trUtf8("To menu służy do obsługi pluginów pythona, \n np. archiwizacji danych, generowania raportów etc.\n\n") +
			trUtf8("Skrypty pythona sa czytane z folderu \"~/elinux/plugins/\"."),
			trUtf8("Ok"), 0, 0, 1);

}

/** Slot
 *  Used while calling python script from the menu
 */
void old_MainWindow::pluginSlot() {

	QString program = "python";

	QAction *a = static_cast<QAction *> (this->sender());

	int scriptId = a->data().toInt();

	QStringList args;
	args += customActions[scriptId];
    args += QString("%1").arg((size_t)winId(), 0, 10);

	qDebug() << find(winId())->windowTitle();

	QProcess *cmd = new QProcess(this);
	cmd->start(program, args);
	if (!cmd->waitForStarted()) {
		QMessageBox::information(this, trUtf8("QFaktury"), trUtf8(
				"Uruchomienie się nie powiodło."), QMessageBox::Ok);
	}
}

/** Slot
 *  Show context menu
 */
void old_MainWindow::showTableMenuT(QPoint p) {
	// qDebug() << __FUNCTION__ << __LINE__;
	QMenu menuTableT(tableT);
	menuTableT.addAction(towaryDodajAction);
	menuTableT.addAction(towaryUsunAction);
	menuTableT.addAction(towaryEdycjaAction);
	menuTableT.exec(tableT->mapToGlobal(p));
}


/** Slot
 *  Show context menu
 */
void old_MainWindow::showTableMenuK(QPoint p) {
	// qDebug() << __FUNCTION__ << __LINE__;
	QMenu menuTable(tableK);
	menuTable.addAction(kontrahenciDodajAction);
	menuTable.addAction(kontrahenciUsunAction);
	menuTable.addAction(kontrahenciEdycjaAction);
	menuTable.exec(tableK->mapToGlobal(p));
}


/** Slot
 *  Show context menu
 */
void old_MainWindow::showTableMenuH(QPoint p) {
	// qDebug() << __FUNCTION__ << __LINE__;
	QMenu menuTable(tableH);
	menuTable.addAction(fakturyDodajAction);
	menuTable.addAction(fakturyBruttoAction);
	menuTable.addAction(fakturyRachunekAction);
	menuTable.addSeparator();
	menuTable.addAction(fakturyPFormaAction);
	menuTable.addAction(fakturyKorektaAction);
	menuTable.addAction(fakturyDuplikatAction);
	menuTable.addSeparator();
	menuTable.addAction(fakturyEdAction);
	menuTable.addAction(fakturyUsunAction);
	menuTable.exec(tableH->mapToGlobal(p));
}

/** Slot
 *  StatusBar slot
 */
void old_MainWindow::mainUpdateStatus(QTableWidgetItem *item) {

    // cast is required since the names of method and objects inside old_MainWindow class and
    // Qold_MainWindow and Uiold_MainWindow are the same... I guess there is a way to avoid it.
    QStatusBar* stat = dynamic_cast<QMainWindow *>(this)->statusBar();

	QTableWidget *table = item->tableWidget();
	QString message;
	message += table->horizontalHeaderItem(1)->text() + " : " + table->item(item->row(), 1)->text() + ", ";
	message += table->horizontalHeaderItem(2)->text() + " : " + table->item(item->row(), 2)->text() + ", ";
	message += table->horizontalHeaderItem(3)->text() + " : " + table->item(item->row(), 3)->text();

	stat->showMessage(trUtf8("Wybrana pozycja: ") + message);
}

/** Slot which enables/disables menu. It's possible to add/remove goods/customers
 *  only if this is the current tab.
 */
void old_MainWindow::tabChanged(QWidget * qwdt) {

	int tabNo = tabWidget2->indexOf(qwdt);

	// disable Edit and Remove actions _ONLY_
	switch (tabNo) {
	case 0: {
		// historia
		kontrahenciEdycjaAction->setEnabled(false);
		kontrahenciUsunAction->setEnabled(false);
		towaryEdycjaAction->setEnabled(false);
		towaryUsunAction->setEnabled(false);
		fakturyEdAction->setEnabled(true);
		fakturyUsunAction->setEnabled(true);
		break;
	}
	case 1: {
		// kontrahenci
		kontrahenciEdycjaAction->setEnabled(true);
		kontrahenciUsunAction->setEnabled(true);
		towaryEdycjaAction->setEnabled(false);
		towaryUsunAction->setEnabled(false);
		fakturyEdAction->setEnabled(false);
		fakturyUsunAction->setEnabled(false);
		break;
	}
	case 2: {
		// towary
		kontrahenciEdycjaAction->setEnabled(false);
		kontrahenciUsunAction->setEnabled(false);
		towaryEdycjaAction->setEnabled(true);
		towaryUsunAction->setEnabled(true);
		fakturyEdAction->setEnabled(false);
		fakturyUsunAction->setEnabled(false);
		break;
	}
	default: {
		// historia
		kontrahenciEdycjaAction->setEnabled(false);
		kontrahenciUsunAction->setEnabled(false);
		towaryEdycjaAction->setEnabled(false);
		towaryUsunAction->setEnabled(false);
		fakturyEdAction->setEnabled(true);
		fakturyUsunAction->setEnabled(true);
		break;
	}
	}
}

/** Slot used to read the invoices, calls readHist.
 */
void old_MainWindow::rereadHist() {
	//  qDebug( __FUNCTION__ );
	tableClear(tableH);
	tableH->setSortingEnabled(false);
	readHist();
}

/** Slot used to display aboutQt informations.
 */
void old_MainWindow::aboutQt() {
	QMessageBox::aboutQt(this, sett().getVersion(qAppName()));
}

/** Slot used to display information about QFaktury
 */
void old_MainWindow::oProg() {
	QMessageBox::about(
			this,
			trUtf8("O programie"),
			trUtf8("Program do wystawiania faktur.\n\n ") + sett().getVersion(qAppName()) +
			trUtf8("\n\nKoordynator projektu:\n\tGrzegorz Rękawek\t\t\n\nProgramiści: \n\tTomasz Pielech\n\t") +
			trUtf8("Rafał Rusin http://people.apache.org/~rr/\n\tSławomir Patyk \n\nIkony:\n\tDariusz Arciszewski\n\n") +
			trUtf8("UWAGA!!!\n") +
			trUtf8("Ten program komputerowy dostarczany jest przez autora w formie \"takiej, jaki jest\". ") +
			trUtf8("Autor nie udziela żadnej gwarancji oraz rękojmi, że program będzie działał ") +
			trUtf8("prawidłowo, jest odpowiedniej jakości oraz że spełni oczekiwania ") +
			trUtf8("użytkownika. Autor nie odpowiada za jakiekolwiek straty wynikające z użytkowania ") +
			trUtf8("programu, w tym utratą spodziewanych korzyści, danych, informacji ") +
			trUtf8("gospodarczych lub koszt urządzeń lub programów zastępczych."));
}

/** Slot used to edit the invoice from list of invoices.
 */
void old_MainWindow::editFHist() {
	if (tableH->selectedItems().count() <= 0) {
		QMessageBox::information(this, trUtf8("QFaktury"), trUtf8("Faktura nie wybrana. Nie mozna edytować."), trUtf8("Ok"), 0, 0, 1);
		return;
	}

	tableH->setSortingEnabled(false);

	int row;
	QList<QTableWidgetItem *> selected = tableH->selectedItems();
	row = selected[0]->row();

	if (tableH->item(row, 3)->text() == trUtf8("korekta")) {
		// QMessageBox::information( this, trUtf8("QFaktury"), "Jeszcze nie ma", QMessageBox::Ok );
		Korekta korWindow(this, dl);
		korWindow.korektaInit(true);
		korWindow.readCorrData(tableH->item(row, 0)->text());
		if (korWindow.exec() == QDialog::Accepted) {
			// edit window shouln't return anything
			rereadHist();
		}
		if (korWindow.kAdded) readKontr();
	}

	if (tableH->item(row, 3)->text() == trUtf8("kbrutto")) {
		// QMessageBox::information( this, trUtf8("QFaktury"), "Jeszcze nie ma", QMessageBox::Ok );
		KorektaBrutto korWindow(this, dl);
		korWindow.korektaInit(true);
		korWindow.readCorrData(tableH->item(row, 0)->text());
		if (korWindow.exec() == QDialog::Accepted) {
			// edit window shouln't return anything
			rereadHist();
		}
		if (korWindow.kAdded) readKontr();
	}


	if (tableH->item(row, 3)->text() == trUtf8("rachunek")) {
		// QMessageBox::information( this, trUtf8("QFaktury"), "Jeszcze nie ma", QMessageBox::Ok );
		Rachunek raWindow(this, dl);
		raWindow.readData(tableH->item(row, 0)->text(), 0);
		raWindow.rachunekInit();
		raWindow.setWindowTitle(trUtf8("Edytuje Rachunek"));
		if (raWindow.exec() == QDialog::Accepted) {
			// edit window shouln't return anything
			rereadHist();
		}
		if (raWindow.kAdded) readKontr();
	}

	if ((tableH->item(row, 3)->text() == trUtf8("FVAT"))
			|| (tableH->item(row, 3)->text() == trUtf8("FPro"))) {
		// qDebug ("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
		Faktura fraWindow(this, dl);
		// qDebug() << pdGlob;
		int co = 0;
		if (tableH->item(row, 3)->text() == "FVAT")
			co = 0;
		else
			co = 1;
		fraWindow.readData(tableH->item(row, 0)->text(), co);
		if (fraWindow.exec() == QDialog::Accepted) {
			rereadHist();
		}
		if (fraWindow.kAdded) readKontr();
	}

	if ((tableH->item(row, 3)->text() == trUtf8("FBrutto"))
			|| (tableH->item(row, 3)->text() == trUtf8("FBPro"))) {
		// qDebug ("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
		FakturaBrutto fraWindow(this, dl);
		// qDebug() << pdGlob;
		int co = 0;
		if (tableH->item(row, 3)->text() == "FBrutto")
			co = 0;
		else
			co = 1;
		fraWindow.readData(tableH->item(row, 0)->text(), co);
		if (fraWindow.exec() == QDialog::Accepted) {
			rereadHist();
		}
		if (fraWindow.kAdded) readKontr();
	}

	tableH->setSortingEnabled(true);
}

/** Slot used to delete invoices
 */
void old_MainWindow::delFHist() {
	if (tableH->selectedItems().count() <= 0) {
		QMessageBox::information(this, trUtf8("QFaktury"), trUtf8("Faktura nie wybrana. Nie mozna usuwać."), trUtf8("Ok"), 0, 0, 1);
		return;
	}

	if (QMessageBox::warning(this, sett().getVersion(qAppName()), trUtf8("Czy napewno chcesz usnąć tą fakturę z historii?"), trUtf8("Tak"),
			trUtf8("Nie"), 0, 0, 1) == 0) {
		QString name = tableH->item (tableH->currentRow(), 0)->text();
		dl->invoiceDeleteData(name);
		tableH->removeRow(tableH->currentRow());
	}

}

/** Slot used to edit data of the current company
 */
void old_MainWindow::daneFirmyClick() {
	// qDebug("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
	Uzytkownik daneFirmyWindow(this);
	daneFirmyWindow.exec();
}

/** Slot used to edit edit sett()
 */
void old_MainWindow::settClick() {
	// qDebug ("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
	Ustawienia settWindow(this);
	settWindow.exec();
}

/** Slot used to add new customer
 */
void old_MainWindow::kontrClick() {
	Kontrahenci kontrWindow(this, 0, dl);
	//qDebug ("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
	if (kontrWindow.exec() == QDialog::Accepted) {
		tableK->setSortingEnabled(false);
		insertRow(tableK, tableK->rowCount());
		QStringList row = kontrWindow.ret.split("|");
		tableK->item(tableK->rowCount() - 1, 0)->setText(row[0]); // name
		tableK->item(tableK->rowCount() - 1, 1)->setText(row[1]); // type
		tableK->item(tableK->rowCount() - 1, 2)->setText(row[2]); // place
		tableK->item(tableK->rowCount() - 1, 3)->setText(row[3]); // address
		tableK->item(tableK->rowCount() - 1, 4)->setText(row[4]); // telefon
		tableK->setSortingEnabled(true);
	}
}

/** Slot used to delete current customer
 */
void old_MainWindow::kontrDel() {
	if (tableK->selectedItems().count() <= 0) {
		QMessageBox::information(this, trUtf8("QFaktury"), trUtf8("Kontrahent nie wybrany. Nie mozna usuwac."), trUtf8("Ok"), 0, 0, 1);
		return;
	}

	if (QMessageBox::warning(this, trUtf8("QFaktury"), trUtf8("Czy napewno chcesz usunąć kontrahenta: ") +
			tableK->item(tableK->currentRow(), 0)->text() + trUtf8(" ?"), trUtf8("Tak"), trUtf8("Nie"), 0, 0, 1) == 0) {
			dl->kontrahenciDeleteData(tableK->item(tableK->currentRow(), 0)->text());
			tableK->removeRow(tableK->currentRow());
	}
}

/** Slot used to edit customer
 */
void old_MainWindow::kontrEd() {
	if (tableK->selectedItems().count() <= 0) {
		QMessageBox::information(this, trUtf8("QFaktury"), trUtf8("Kontrahent nie wybrany."), trUtf8("Ok"), 0, 0, 1);
		return;
	}

	int row = tableK->selectedItems()[0]->row();

	Kontrahenci kontrWindow(this, 1, dl);
	kontrWindow.selectData(tableK->item(row, 0)->text(),
			sett().getCustomerType(tableK->item(row, 1)->text()));
	if (kontrWindow.exec() == QDialog::Accepted) {
		tableK->setSortingEnabled(false);
		QStringList rowTxt = kontrWindow.ret.split("|");
		tableK->item(row, 0)->setText(rowTxt[0]); // name
		tableK->item(row, 1)->setText(rowTxt[1]); // type
		tableK->item(row, 2)->setText(rowTxt[2]); // place
		tableK->item(row, 3)->setText(rowTxt[3]); // address
		tableK->item(row, 4)->setText(rowTxt[4]); // telefon
		tableK->setSortingEnabled(true);
	}
}


/** Slot used for creating new invoices
 */
void old_MainWindow::newFra() {
	Faktura fraWindow(this, dl);
	fraWindow.pforma = false;
	if (fraWindow.exec() == QDialog::Accepted) {
		tableH->setSortingEnabled(false);
		insertRow(tableH, tableH->rowCount());
		QStringList row = fraWindow.ret.split("|");
		tableH->item(tableH->rowCount() - 1, 0)->setText(row[0]); // file name
		tableH->item(tableH->rowCount() - 1, 1)->setText(row[1]); // symbol
		tableH->item(tableH->rowCount() - 1, 2)->setText(row[2]); // date
		tableH->item(tableH->rowCount() - 1, 3)->setText(row[3]); // type
		tableH->item(tableH->rowCount() - 1, 4)->setText(row[4]); // nabywca
		tableH->item(tableH->rowCount() - 1, 5)->setText(row[5]); // NIP
		tableH->setSortingEnabled(true);
	} else {
		rereadHist();
	}

	if (fraWindow.kAdded) readKontr();
}


/** Slot used for creating new invoices
 */
void old_MainWindow::newFRachunek() {
	Rachunek fraWindow(this, dl);
	fraWindow.pforma = false;
	fraWindow.setWindowTitle(trUtf8("Rachunek"));
	fraWindow.rachunekInit();
	if (fraWindow.exec() == QDialog::Accepted) {
		tableH->setSortingEnabled(false);
		insertRow(tableH, tableH->rowCount());
		QStringList row = fraWindow.ret.split("|");
		tableH->item(tableH->rowCount() - 1, 0)->setText(row[0]); // file name
		tableH->item(tableH->rowCount() - 1, 1)->setText(row[1]); // symbol
		tableH->item(tableH->rowCount() - 1, 2)->setText(row[2]); // date
		tableH->item(tableH->rowCount() - 1, 3)->setText(row[3]); // type
		tableH->item(tableH->rowCount() - 1, 4)->setText(row[4]); // nabywca
		tableH->item(tableH->rowCount() - 1, 5)->setText(row[5]); // NIP
		tableH->setSortingEnabled(true);
	} else {
		rereadHist();
	}

	if (fraWindow.kAdded) readKontr();
}


/** Slot used for creating new invoices
 */
void old_MainWindow::newFBrutto() {
	FakturaBrutto fraWindow(this, dl);
	fraWindow.pforma = false;
	fraWindow.setWindowTitle(trUtf8("Faktura VAT Brutto"));
	if (fraWindow.exec() == QDialog::Accepted) {
		tableH->setSortingEnabled(false);
		insertRow(tableH, tableH->rowCount());
		QStringList row = fraWindow.ret.split("|");
		tableH->item(tableH->rowCount() - 1, 0)->setText(row[0]); // file name
		tableH->item(tableH->rowCount() - 1, 1)->setText(row[1]); // symbol
		tableH->item(tableH->rowCount() - 1, 2)->setText(row[2]); // date
		tableH->item(tableH->rowCount() - 1, 3)->setText(row[3]); // type
		tableH->item(tableH->rowCount() - 1, 4)->setText(row[4]); // nabywca
		tableH->item(tableH->rowCount() - 1, 5)->setText(row[5]); // NIP
		tableH->setSortingEnabled(true);
	} else {
		rereadHist();
	}

	if (fraWindow.kAdded) readKontr();
}


/** Slot used to create new ProForma Invoice
 */
void old_MainWindow::newPForm() {
	Faktura fraWindow(this, dl);
	fraWindow.pforma = true;
	fraWindow.setWindowTitle(trUtf8("Faktura Pro Forma"));
	fraWindow.backBtnClick();
	if (fraWindow.exec() == QDialog::Accepted) {
		tableH->setSortingEnabled(false);
		insertRow(tableH, tableH->rowCount());
		QStringList row = fraWindow.ret.split("|");
		tableH->item(tableH->rowCount() - 1, 0)->setText(row[0]); // file name
		tableH->item(tableH->rowCount() - 1, 1)->setText(row[1]); // symbol
		tableH->item(tableH->rowCount() - 1, 2)->setText(row[2]); // date
		tableH->item(tableH->rowCount() - 1, 3)->setText(row[3]); // type
		tableH->item(tableH->rowCount() - 1, 4)->setText(row[4]); // nabywca
		tableH->item(tableH->rowCount() - 1, 5)->setText(row[5]); // NIP
		tableH->setSortingEnabled(true);
	} else {
		rereadHist();
	}

	if (fraWindow.kAdded) readKontr();
}

/** Slot used to create new Korkta
 */
void old_MainWindow::newKor() {
	if (tableH->selectedItems().count() <= 0) {
		QMessageBox::information(this, trUtf8("QFaktury"), trUtf8("Faktura nie wybrana. Wybierz fakurę, do której chcesz wystawić korektę."), trUtf8("Ok"), 0, 0, 1);
		return;
	}

	int row = tableH->selectedItems()[0]->row();

	QStringList invTypes;
	invTypes << "FVAT" << "FBrutto";

	if (invTypes.contains(tableH->item(row, 3)->text())) {
		tableH->setSortingEnabled(false);

		Korekta *korWindow;

		if (tableH->item(row, 3)->text().contains("FVAT")) {
			korWindow = new Korekta(this, dl);
		} else  {
			korWindow = new KorektaBrutto(this, dl);
		}

		korWindow->korektaInit(false);
		// qDebug( pdGlob );
		korWindow->readData(tableH->item(row, 0)->text(), 2);
		korWindow->setWindowTitle(trUtf8("Nowa korekta"));
		if (korWindow->exec() == QDialog::Accepted) {
			insertRow(tableH, tableH->rowCount());
			// qDebug() << korWindow->ret;
			QStringList row = korWindow->ret.split("|");
			int newRow = tableH->rowCount() - 1;
			tableH->item(newRow, 0)->setText(row[0]); // file name
			tableH->item(newRow, 1)->setText(row[1]); // symbol
			tableH->item(newRow, 2)->setText(row[2]); // date
			tableH->item(newRow, 3)->setText(row[3]); // type
			tableH->item(newRow, 4)->setText(row[4]); // nabywca
			tableH->item(newRow, 5)->setText(row[5]); // NIP
		}
		if (korWindow->kAdded) readKontr();
		delete korWindow;
		korWindow = NULL;
		tableH->setSortingEnabled(true);
	} else {
		QMessageBox::information(this, "QFaktury",
				trUtf8("Do faktury typu ") + tableH->item(row, 3)->text()
				+ (" nie wystawiamy korekt."), QMessageBox::Ok);
	}
}

/** Slot
 *  Creates duplicate
 */
void old_MainWindow::newDuplikat() {
	if (tableH->selectedItems().count() <= 0) {
		QMessageBox::information(this, trUtf8("QFaktury"), trUtf8("Faktura nie wybrana. Wybierz fakurę, do której chcesz wystawić duplikat."), trUtf8("Ok"), 0, 0, 1);
		return;
	}
	int row = tableH->selectedItems()[0]->row();

	// types of invoices for which it's ok to issue a duplicate
	QStringList invTypes;
	invTypes << "FVAT" << "FBrutto";

	if (invTypes.contains(tableH->item(row, 3)->text())) {
		Duplikat dupWindow(this, dl);
		// qDebug( pdGlob );
		dupWindow.readData(tableH->item(row, 0)->text(), 2);
		dupWindow.setWindowTitle(trUtf8("Nowy duplikat"));
		dupWindow.duplikatInit();
		if (dupWindow.exec() == QDialog::Accepted) {
			// not saving duplicate
		}
	} else {
		QMessageBox::information(this, "QFaktury",
				trUtf8("Do faktury typu ") + tableH->item(row, 3)->text()
				+ (" nie wystawiamy duplikatów."), QMessageBox::Ok);
	}

}

/** Slot used to add goods
 */
void old_MainWindow::towaryDodaj() {
	Towary towWindow(this, 0, dl);
	if (towWindow.exec() == QDialog::Accepted) {
		tableT->setSortingEnabled(false);
		insertRow(tableT, tableT->rowCount());
		QStringList row = towWindow.ret.split("|");
		tableT->item(tableT->rowCount() - 1, 0)->setText(row[0]);
		tableT->item(tableT->rowCount() - 1, 1)->setText(row[1]);
		tableT->item(tableT->rowCount() - 1, 2)->setText(row[2]);
		tableT->item(tableT->rowCount() - 1, 3)->setText(row[3]);
		tableT->item(tableT->rowCount() - 1, 4)->setText(row[4]);
		tableT->item(tableT->rowCount() - 1, 5)->setText(row[5]);
		tableT->item(tableT->rowCount() - 1, 6)->setText(row[6]);
		tableT->item(tableT->rowCount() - 1, 7)->setText(row[7]);
		tableT->item(tableT->rowCount() - 1, 8)->setText(row[8]);
		tableT->item(tableT->rowCount() - 1, 9)->setText(row[9]);
		tableT->item(tableT->rowCount() - 1, 10)->setText(row[10]);
		tableT->item(tableT->rowCount() - 1, 11)->setText(row[11]);
		tableT->setSortingEnabled(true);
	}
}

/** Slot used to delete goods
 */
void old_MainWindow::towaryUsun() {
	if (tableT->selectedItems().count() <= 0) {
		QMessageBox::information(this, trUtf8("QFaktury"), trUtf8("Towar nie wybrany. Nie mozna usuwac."), trUtf8("Ok"), 0, 0, 1);
		return;
	}

	int row = tableT->currentRow();

	if (QMessageBox::warning(this, trUtf8("QFaktury"), trUtf8("Czy napewno chcesz usunąć towar ") + tableT->item(row, 0)->text()
			+ "/" + tableT->item(row, 1)->text() + "?", trUtf8("Tak"), trUtf8("Nie"), 0, 0, 1)
			== 0) {
			dl->productsDeleteData(tableT->item(row, 0)->text());
			tableT->removeRow(row);
		}

}

/** Slot used for editing goods
 */
void old_MainWindow::towaryEdycja() {
	if (tableT->selectedItems().count() <= 0) {
		QMessageBox::information(this, trUtf8("QFaktury"), trUtf8("Towar nie wybrany. Nie można edytować."), trUtf8("Ok"), 0, 0, 1);
		return;
	}
	int row = tableT->selectedItems()[0]->row();

	Towary towWindow(this, 1, dl);
	// qDebug() << tableT->item(row, 5)->text() << sett().getProductType(tableT->item(row, 5)->text());
	towWindow.selectData(tableT->item(row, 0)->text(),
			sett().getProductType(tableT->item(row, 5)->text()));
	if (towWindow.exec() == QDialog::Accepted) {
		tableT->setSortingEnabled(false);
		QStringList rowTxt = towWindow.ret.split("|");
		tableT->item(row, 0)->setText(rowTxt[0]);
		tableT->item(row, 1)->setText(rowTxt[1]);
		tableT->item(row, 2)->setText(rowTxt[2]);
		tableT->item(row, 3)->setText(rowTxt[3]);
		tableT->item(row, 4)->setText(rowTxt[4]);
		tableT->item(row, 5)->setText(rowTxt[5]);
		tableT->item(row, 6)->setText(rowTxt[6]);
		tableT->item(row, 7)->setText(rowTxt[7]);
		tableT->item(row, 8)->setText(rowTxt[8]);
		tableT->item(row, 9)->setText(rowTxt[9]);
		tableT->item(row, 10)->setText(rowTxt[10]);
		tableT->item(row, 11)->setText(rowTxt[11]);
		tableT->setSortingEnabled(true);
	}
}

/** Slot close
 */
bool old_MainWindow::close() {
    qDebug() << "close slot";
	if (QMessageBox::question(this, trUtf8("Potwierdź"),
	trUtf8("Czy chcesz wyjść z programu?"), QMessageBox::Yes | QMessageBox::No,
			QMessageBox::Yes) == QMessageBox::Yes) {
		saveAllSett();
        return QMainWindow::close();
	} else {
		return false;
	}
}

/** Slot help
 */
void old_MainWindow::pomoc() {
	QDesktopServices::openUrl(QUrl("http://www.e-linux.pl/"));
}

/** Slot reportBug
 */
void old_MainWindow::reportBug() {
	QDesktopServices::openUrl(QUrl("https://sourceforge.net/tracker2/?func=add&group_id=154610&atid=792471"));
}

// slot for resetting the filters
void old_MainWindow::resetFilter() {
	filtrStart->setDate(sett().getFirstDayOfYear());
	filtrEnd->setDate(sett().getLastDayOfYear());
	rereadHist();
}

// ----------------------------------------  SLOTS ---------------------------------//




