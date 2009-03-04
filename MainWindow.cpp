#include "MainWindow.moc"
#include <QTextCodec>
#include <QMessageBox>
#include <QApplication>
#include <QEvent>
#include <QUrl>
#include <qprocess.h>
#include <Qt/qdom.h>
#include <QTextStream>
#include <QtDebug>
#include <QDesktopServices>

#include "Ustawienia.h"
#include "Uzytkownik.h"
#include "Towary.h"
#include "Faktura.h"
#include "Korekta.h"
#include "Kontrahenci.h"



/** Constructor
 */
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    setupUi(this);
    init();
}

/**
 * init() method
 */
void MainWindow::init() {

	workingDir = sett().getWorkingDir();

	// first run
	if (firstRun()) {
		setupDir();
		// towary/uslugi - wymiary
		tableT->setColumnWidth(0, 50);
		tableT->setColumnWidth(1, 140);
		tableT->setColumnWidth(3, 40);
		tableT->setColumnWidth(4, 60);
		tableT->setColumnWidth(5, 50);
		tableT->setColumnWidth(6, 55); // netto1
		tableT->setColumnWidth(7, 55); // netto2
		tableT->setColumnWidth(8, 55); // netto3
		tableT->setColumnWidth(9, 55); // netto4
		tableT->setColumnWidth(10, 55);

		// qDebug("firstRun");
		saveAllSettAsDefault();
	} else {
		// qDebug() << sett().getValueAsDate("filtrStart")
		//		<< sett().getValueAsDate("filtrEnd");

		filtrStart->setDisplayFormat(sett().getDateFormat());
		filtrStart->setDate(sett().getValueAsDate("filtrStart"));
		filtrEnd->setDisplayFormat(sett().getDateFormat());
		filtrEnd->setDate(sett().getValueAsDate("filtrEnd"));
	}

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
	icon.addPixmap(QPixmap(":/res/share/qfaktury/icons/qfaktury_48.png"),
			QIcon::Normal, QIcon::Off);
	this->setWindowIcon(icon);


	this->setWindowTitle( sett().getVersion(qAppName() ));

	// connect slots
	connect(actionForum, SIGNAL (activated ()), this, SLOT(forum()));
	connect(applyFiltrBtn, SIGNAL (clicked()), this, SLOT(rereadHist()));
	connect(plikDane_firmyAction, SIGNAL(activated()), this, SLOT(daneFirmyClick()));
	connect(plikKoniecAction, SIGNAL(activated()), this, SLOT(close()));
	connect(kontrahenciDodajAction, SIGNAL(activated()), this, SLOT(kontrClick()));
	connect(kontrahenciUsunAction, SIGNAL(activated()), this, SLOT(kontrDel()));
	connect(kontrahenciEdycjaAction, SIGNAL(activated()), this, SLOT(kontrEd()));
	connect(fakturyDodajAction, SIGNAL(activated()), this, SLOT(newFra()));
	connect(fakturyUsunAction, SIGNAL(activated()), this, SLOT(delFHist()));
	connect(fakturyEdAction, SIGNAL(activated()), this, SLOT(editFHist()));
	connect(pomocO_QtAction, SIGNAL(activated()), this, SLOT(aboutQt()));
	connect(pomocO_programieAction, SIGNAL(activated()), this, SLOT(oProg()));
	connect(plikUstawieniaAction, SIGNAL(activated()), this, SLOT(settClick()));
	connect(tabWidget2, SIGNAL(currentChanged(QWidget*)), this, SLOT(tabChanged(QWidget*)));
	connect(pomocPomocAction, SIGNAL(activated()), this, SLOT(pomoc()));
	connect(towaryDodajAction, SIGNAL(activated()), this, SLOT(towaryDodaj()));
	connect(towaryEdycjaAction, SIGNAL(activated()), this, SLOT(towaryEdycja()));
	connect(towaryUsunAction, SIGNAL(activated()), this, SLOT(towaryUsun()));
	connect(fakturyKorektaAction, SIGNAL(activated()), this, SLOT(newKor()));
	connect(fakturyPFormaAction, SIGNAL(activated()), this, SLOT(newPForm()));
	connect(prevPageAction, SIGNAL(activated()), this, SLOT(prevPage()));
	connect(nextPageAction, SIGNAL(activated()), this, SLOT(nextPage()));
	connect(tableH, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editFHist()));
	connect(tableK, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(kontrEd()));
	connect(tableT, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(towaryEdycja()));

	tabChanged(tabWidget2);

	readKontr(workingDir);
	readHist(workingDir);
	readTw(workingDir);
}

/**
 * firstRun setup()
 */
bool MainWindow::firstRun() {
	bool ok = sett().value("firstrun", false).toBool();
	if (ok) {
		sett().checkSettings();
		// set dates for filter
		filtrStart->setDate(QDate::currentDate());
		filtrEnd->setDate(QDate::currentDate());
		return true;
	} else {
		sett().checkSettings();
		return false;
	}

}

/** save sett() before quit
 * save column width
 */
void MainWindow::saveColumnWidth() {
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
void MainWindow::saveAllSettAsDefault() {
	sett().resetSettings();
}


/** Saves all sett()
 */
void MainWindow::saveAllSett() {
	// save filtr
	sett().setValue("filtrStart", filtrStart->text());
	sett().setValue("filtrEnd", filtrEnd->text());

	saveColumnWidth();

	// save unsaved
	sett().sync();
}


/** Clears content of the QTableWidget passed in the input
 *  @param QTableWidget
 */
void MainWindow::tableClear(QTableWidget * tab) {
	// should be a better method to do that
	tab->setRowCount(0);
}

/** Used while adding new row
 */
void MainWindow::insertRow(QTableWidget *t, int row) {
	t->insertRow(row);
	for (int i = 0; i < t->columnCount(); i++) {
		t->setItem(row, i, new QTableWidgetItem());
	}
}

/** Used while loading history, returns true if XML with the invoice should be read or not.
 *  Only files which names are in between the dates from filer on the UI should be read.
 *  @param QString - name of the file.
 */
bool MainWindow::applyFiltr(QString nameToCheck) {
	QString tmp = nameToCheck;
	tmp = tmp.remove("h");
	tmp = tmp.left(10);
	tmp = tmp.remove("-");

	// not very flexible
	// assumption is that date comes as yyyymmdd
	// if its otherwise order of remove methods has to be changed
	int year = tmp.left(4).toInt();
	tmp = tmp.remove(0, 4);
	int month = tmp.left(2).toInt();
	tmp = tmp.remove(0, 2);
	int day = tmp.left(2).toInt();
	tmp = tmp.remove(0, 2);
	QDate tmpDate(year, month, day);

	// if debugOn()
	// qDebug() << __FUNCTION__ << __LINE__ << nameToCheck
	//		<< filtrStart->date().toString()
	//		<< year << month << day
	//		<< tmpDate.toString()
	//		<< filtrEnd->date().toString();

	if (tmpDate < filtrStart->date()) {
		return false;
	}

	if (tmpDate > filtrEnd->date()) {
		return false;
	}

	// default true?
	return true;
}

/** Reads the invoices from the directory passed in the input.
 *  @param QString - directory from where the invoices should be read
 */
void MainWindow::readHist(QString progDir) {
	/*!
	 * step one: get list of files from directory
	 */
	QDir allFiles;
	QString text;

	QDomDocument doc(sett().getInoiveDocName());
	QDomElement root;
	QDomElement nadawca;
	QDomElement odbiorca;

	allFiles.setPath(sett().getInvoicesDir());
	allFiles.setFilter(QDir::Files);
	QStringList filters;
	filters << "h*.xml";
	allFiles.setNameFilters(filters);
	QStringList files = allFiles.entryList();
	int i, max = files.count();
	for (i = 0; i < max; ++i) {
		if (applyFiltr(files[i])) {
			// qDebug() << files[i];
			insertRow(tableH, tableH->rowCount());
			text = files[i];
			tableH->item(tableH->rowCount() - 1, 0)->setText(text);

			QFile file(sett().getInvoicesDir() + files[i]);

			if (!file.open(QIODevice::ReadOnly)) {
				qDebug() << "File" << file.fileName() << "doesn't exists";
				return;
			} else {
				QTextStream stream(&file);

				if (!doc.setContent(stream.readAll())) {
					// qDebug ("can not set content ");
					file.close();
					// return;
				}
			}

			root = doc.documentElement();
			tableH->item(tableH->rowCount() - 1, 1)->setText(root.attribute(
					"no", "NULL"));
			tableH->item(tableH->rowCount() - 1, 2)->setText(root.attribute(
					"sellingDate", "NULL"));
			tableH->item(tableH->rowCount() - 1, 3)->setText(root.attribute(
					"type", "NULL"));
			QDomNode nab;
			nab = root.firstChild();
			nab = nab.toElement().nextSibling();
			tableH->item(tableH->rowCount() - 1, 4)->setText(
					nab.toElement().attribute("name", "NULL"));
			tableH->item(tableH->rowCount() - 1, 5)->setText(
					nab.toElement().attribute("tic", "NULL"));
		}
	}
}

/** Reads customers from the XML
 */
void MainWindow::readKontr(QString progDir) {
	QDomDocument doc(sett().getCustomersDocName());
	QDomElement root;
	QDomElement urzad;
	QDomElement firma;

	QFile file(sett().getCustomersXml());
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "File" << file.fileName() << "doesn't exists";
		return;
	} else {
		QTextStream stream(&file);
		if (!doc.setContent(stream.readAll())) {
			qDebug("can not set content ");
			file.close();
			return;
		} else {
			root = doc.documentElement();
			urzad = root.firstChild().toElement();
			firma = root.lastChild().toElement();
		}
		QString text;

		for (QDomNode n = firma.firstChild(); !n.isNull(); n = n.nextSibling()) {
			insertRow(tableK, tableK->rowCount());
			text = n.toElement().attribute("name");
			tableK->item(tableK->rowCount() - 1, 0)->setText(text);
			text = sett().getCompanyName();
			tableK->item(tableK->rowCount() - 1, 1)->setText(text);
			text = n.toElement().attribute("place");
			tableK->item(tableK->rowCount() - 1, 2)->setText(text);
			text = n.toElement().attribute("address");
			tableK->item(tableK->rowCount() - 1, 3)->setText(text);
			text = n.toElement().attribute("telefon");
			tableK->item(tableK->rowCount() - 1, 4)->setText(text);
		}

		for (QDomNode n = urzad.firstChild(); !n.isNull(); n = n.nextSibling()) {
			insertRow(tableK, tableK->rowCount());
			text = n.toElement().attribute("name");
			tableK->item(tableK->rowCount() - 1, 0)->setText(text);
			text = sett().getOfficeName();
			tableK->item(tableK->rowCount() - 1, 1)->setText(text);
			text = n.toElement().attribute("place");
			tableK->item(tableK->rowCount() - 1, 2)->setText(text);
			text = n.toElement().attribute("address");
			tableK->item(tableK->rowCount() - 1, 3)->setText(text);
			text = n.toElement().attribute("telefon");
			tableK->item(tableK->rowCount() - 1, 4)->setText(text);
		}
	}
}

/** Reads goods from the XML
 */
void MainWindow::readTw(QString progDir) {
	QDomDocument doc(sett().getProdutcsDocName());
	QDomElement root;
	QDomElement products;
	QDomElement services;

	QFile file(sett().getProductsXml());
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "File" << file.fileName() << "doesn't exists";
		return;
	} else {
		QTextStream stream(&file);
		if (!doc.setContent(stream.readAll())) {
			qDebug("can't set content ");
			file.close();
			return;
		} else {
			root = doc.documentElement();
			products = root.firstChild().toElement();
			services = root.lastChild().toElement();
		}
		QString text;

		for (QDomNode n = products.firstChild(); !n.isNull(); n = n.nextSibling()) {

			insertRow(tableT, tableT->rowCount());
			text = n.toElement().attribute("idx");
			tableT->item(tableT->rowCount() - 1, 0)->setText(text);
			// text = "towar";
			text = n.toElement().attribute("name");
			tableT->item(tableT->rowCount() - 1, 1)->setText(text);
			text = n.toElement().attribute("desc");
			tableT->item(tableT->rowCount() - 1, 2)->setText(text);
			text = n.toElement().attribute("code");
			tableT->item(tableT->rowCount() - 1, 3)->setText(text);
			text = n.toElement().attribute("pkwiu");
			tableT->item(tableT->rowCount() - 1, 4)->setText(text);
			text = trUtf8("Towar");
			tableT->item(tableT->rowCount() - 1, 5)->setText(text);
			text = n.toElement().attribute("curr");
			tableT->item(tableT->rowCount() - 1, 6)->setText(text);
			text = n.toElement().attribute("netto1");
			tableT->item(tableT->rowCount() - 1, 7)->setText(text);
			text = n.toElement().attribute("netto2");
			tableT->item(tableT->rowCount() - 1, 8)->setText(text);
			text = n.toElement().attribute("netto3");
			tableT->item(tableT->rowCount() - 1, 9)->setText(text);
			text = n.toElement().attribute("netto4");
			tableT->item(tableT->rowCount() - 1, 10)->setText(text);
			text = n.toElement().attribute("vat");
			tableT->item(tableT->rowCount() - 1, 11)->setText(text);
		}

		for (QDomNode n = services.firstChild(); !n.isNull(); n = n.nextSibling()) {
			insertRow(tableT, tableT->rowCount());
			text = n.toElement().attribute("idx");
			tableT->item(tableT->rowCount() - 1, 0)->setText(text);
			// text = "towar";
			text = n.toElement().attribute("name");
			tableT->item(tableT->rowCount() - 1, 1)->setText(text);
			text = n.toElement().attribute("desc");
			tableT->item(tableT->rowCount() - 1, 2)->setText(text);
			text = n.toElement().attribute("code");
			tableT->item(tableT->rowCount() - 1, 3)->setText(text);
			text = n.toElement().attribute("pkwiu");
			tableT->item(tableT->rowCount() - 1, 4)->setText(text);
			text = trUtf8("Usługa");
			tableT->item(tableT->rowCount() - 1, 5)->setText(text);
			text = n.toElement().attribute("curr");
			tableT->item(tableT->rowCount() - 1, 6)->setText(text);
			text = n.toElement().attribute("netto1");
			tableT->item(tableT->rowCount() - 1, 7)->setText(text);
			text = n.toElement().attribute("netto2");
			tableT->item(tableT->rowCount() - 1, 8)->setText(text);
			text = n.toElement().attribute("netto3");
			tableT->item(tableT->rowCount() - 1, 9)->setText(text);
			text = n.toElement().attribute("netto4");
			tableT->item(tableT->rowCount() - 1, 10)->setText(text);
			text = n.toElement().attribute("vat");
			tableT->item(tableT->rowCount() - 1, 11)->setText(text);
		}
	}
}

/** Creates directories if required
 */
void MainWindow::setupDir() {
	QDir dir(workingDir);
	if (!dir.exists()) {
		dir.mkdir(workingDir);
		dir.mkdir(workingDir + sett().getDataDir());
	}

	if (!dir.exists(workingDir + sett().getDataDir())) {
		dir.mkdir(workingDir + sett().getDataDir());
	}
}


// ----------------------------------------  SLOTS ---------------------------------//

/** Slot which enables/disables menu. It's possible to add/remove goods/customers
 *  only if this is the current tab.
 */
void MainWindow::tabChanged(QWidget * qwdt) {

	int tabNo = tabWidget2->indexOf(qwdt);

	switch (tabNo) {
	case 0: {
		// historia
		kontrahenciDodajAction->setEnabled(false);
		kontrahenciEdycjaAction->setEnabled(false);
		kontrahenciUsunAction->setEnabled(false);
		towaryDodajAction->setEnabled(false);
		towaryEdycjaAction->setEnabled(false);
		towaryUsunAction->setEnabled(false);
		fakturyEdAction->setEnabled(true);
		fakturyUsunAction->setEnabled(true);
		break;
	}
	case 1: {
		// kontrahenci
		kontrahenciDodajAction->setEnabled(true);
		kontrahenciEdycjaAction->setEnabled(true);
		kontrahenciUsunAction->setEnabled(true);
		towaryDodajAction->setEnabled(false);
		towaryEdycjaAction->setEnabled(false);
		towaryUsunAction->setEnabled(false);
		fakturyEdAction->setEnabled(false);
		fakturyUsunAction->setEnabled(false);
		break;
	}
	case 2: {
		// towary
		kontrahenciDodajAction->setEnabled(false);
		kontrahenciEdycjaAction->setEnabled(false);
		kontrahenciUsunAction->setEnabled(false);
		towaryDodajAction->setEnabled(true);
		towaryEdycjaAction->setEnabled(true);
		towaryUsunAction->setEnabled(true);
		fakturyEdAction->setEnabled(false);
		fakturyUsunAction->setEnabled(false);
		break;
	}
	default: {
		// historia
		kontrahenciDodajAction->setEnabled(false);
		kontrahenciEdycjaAction->setEnabled(false);
		kontrahenciUsunAction->setEnabled(false);
		towaryDodajAction->setEnabled(false);
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
void MainWindow::rereadHist() {
	//  qDebug( __FUNCTION__ );
	tableClear(tableH);
	readHist(workingDir);
}

/** Slot used to display aboutQt informations.
 */
void MainWindow::aboutQt() {
	QMessageBox::aboutQt(this, sett().getVersion(qAppName()));
}

/** Slot used to display information about QFaktury
 */
void MainWindow::oProg() {
	QMessageBox::about(
			this,
			"O programie",
			trUtf8("Program do wystawiania faktur.\nWersja ") + sett().getVersion(qAppName()) +
			trUtf8(" \n\nProgramista:\n\tRafał‚ Rusin (rafal.rusin@gmail.com)\n\nSupport: rafal.rusin@gmail.com\n\nPoprzednio pracowali:\nKoordynator projektu: \n\tGrzegorz Rękawek www.e-linux.pl\nProgramista:\n\tTomasz 'moux' Pielech\nGrafika:\n\tDariusz Arciszewski"));
}

/** Slot used to edit the invoice from list of invoices.
 */
void MainWindow::editFHist() {
	// qDebug() <<  __FUNCTION__;

	int row;
	QList<QTableWidgetItem *> selected = tableH->selectedItems();
	row = selected[0]->row();

	if (tableH->item(row, 3)->text() == "korekta") {
		// QMessageBox::information( this, trUtf8("QFaktury"), "Jeszcze nie ma", QMessageBox::Ok );
		Korekta *korWindow = new Korekta(this);
		korWindow->progDir2 = workingDir;
		korWindow->readData(tableH->item(row, 0)->text());
		if (korWindow->exec() == QDialog::Accepted) {
			QStringList rowTxt = korWindow->ret.split("|");
			tableH->item(row, 0)->setText(rowTxt[0]); // file name
			tableH->item(row, 1)->setText(rowTxt[1]); // symbol
			tableH->item(row, 2)->setText(rowTxt[2]); // date
			tableH->item(row, 3)->setText(rowTxt[3]); // type
			tableH->item(row, 4)->setText(rowTxt[4]); // nabywca
			tableH->item(row, 5)->setText(rowTxt[5]); // NIP
		}
	}

	if ((tableH->item(row, 3)->text() == "FVAT")
			|| (tableH->item(row, 3)->text() == "FPro")) {
		// qDebug ("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
		Faktura *fraWindow = new Faktura(this);
		fraWindow->progDir2 = workingDir;
		// qDebug() << pdGlob;
		int co = 0;
		if (tableH->item(row, 3)->text() == "FVAT")
			co = 0;
		else
			co = 1;
		fraWindow->readData(tableH->item(row, 0)->text(), co);
		if (fraWindow->exec() == QDialog::Accepted) {
			// tableH->insertRows (tableH->rowCount (), 1);
			QStringList rowTxt = fraWindow->ret.split("|");
			tableH->item(row, 0)->setText(rowTxt[0]); // file name
			tableH->item(row, 1)->setText(rowTxt[1]); // symbol
			tableH->item(row, 2)->setText(rowTxt[2]); // date
			tableH->item(row, 3)->setText(rowTxt[3]); // type
			tableH->item(row, 4)->setText(rowTxt[4]); // nabywca
			tableH->item(row, 5)->setText(rowTxt[5]); // NIP
		}
	}
}

/** Slot used to delete invoices
 */
void MainWindow::delFHist() {
	if (QMessageBox::warning(this, sett().getVersion(qAppName()), trUtf8("Czy napewno chcesz usnąć tą fakturę z historii?"), trUtf8("Tak"),
			trUtf8("Nie"), 0, 0, 1) == 0) {
		QString name = tableH->item (tableH->currentRow(), 0)->text();

		QFile file(sett().getInvoicesDir() + name);
		if (file.exists())
			file.remove();
		tableH->removeRow(tableH->currentRow());
	}

}

/** Slot used to edit data of the current company
 */
void MainWindow::daneFirmyClick() {
	// qDebug("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
	Uzytkownik *daneFirmyWindow = new Uzytkownik(this);
	daneFirmyWindow->show();
}

/** Slot used to edit edit sett()
 */
void MainWindow::settClick() {
	// qDebug ("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
	Ustawienia *settWindow = new Ustawienia(this);
	settWindow->show();
}

//void MainWindow::kretorClick ()
//{
//  qDebug ("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
////     QMessageBox::information( this, trUtf8("QFaktury"), "Funkcja jeszcze nie gotowa. Uzyj menu faktury->Nowy", QMessageBox::Ok );
//  Form3 *kreatorWindow = new Form3;
//  if (kreatorWindow->exec () == QDialog::Accepted)
//    {
//      kreatorWindow->print ();
//      // tableClear (tableH);
//      // readHist (pdGlob);
//      /*
//         QStringList row = QStringList::split( "|",  kreatorWindow->ret );
//         tableH->insertRows (tableH->rowCount (), 1);
//         tableH->setText (tableH->rowCount () - 1, 0, row[0]); // nazwa pliku
//         tableH->setText (tableH->rowCount () - 1, 1, row[1]);
//         tableH->setText (tableH->rowCount () - 1, 2, row[2]);
//         tableH->setText (tableH->rowCount () - 1, 4, row[3]);
//         tableH->setText (tableH->rowCount () - 1, 3, row[4]);
//       */
//    }
//  // delete kreatorWindow;
//}

/** Slot used to add new customer
 */
void MainWindow::kontrClick() {
	Kontrahenci *kontrWindow;
	kontrWindow = new Kontrahenci(this, 0);
	//qDebug ("%s %s:%d", __FUNCTION__, __FILE__, __LINE__);
	if (kontrWindow->exec() == QDialog::Accepted) {
		// qDebug() << progDir;
		// readKontr (progDir);
		insertRow(tableK, tableK->rowCount());
		QStringList row = kontrWindow->ret.split("|");
		tableK->item(tableK->rowCount() - 1, 0)->setText(row[0]); // name
		tableK->item(tableK->rowCount() - 1, 1)->setText(row[1]); // type
		tableK->item(tableK->rowCount() - 1, 2)->setText(row[2]); // place
		tableK->item(tableK->rowCount() - 1, 3)->setText(row[3]); // address
		tableK->item(tableK->rowCount() - 1, 4)->setText(row[4]); // telefon
	}
}

/** Slot used to delete current customer
 */
void MainWindow::kontrDel() {
	if (QMessageBox::warning(this, trUtf8("QFaktury"), trUtf8("Czy napewno chcesz usunąć kontrahenta: ") + tableK->item(tableK->currentRow(), 0)->text() + trUtf8(" ?"), trUtf8("Tak"), trUtf8("Nie"), 0, 0, 1) == 0) {
		QDomDocument doc("kontrahenci");
		QDomElement root;
		QDomElement urzad;
		QDomElement firma;
		int row = tableK->currentRow();

		QFile file(sett().getCustomersXml());
		if (!file.open(QIODevice::ReadOnly)) {
			qDebug() << "File" << file.fileName() << "doesn't exists";
			return;
		} else {
			QTextStream stream(&file);
			if (!doc.setContent(stream.readAll())) {
				qDebug("can not set content ");
				file.close();
				return;
			} else {
				root = doc.documentElement();
				urzad = root.firstChild().toElement();
				firma = root.lastChild().toElement();
			}
			QString text;

			for (QDomNode n = firma.firstChild(); !n.isNull(); n
					= n.nextSibling()) {
				if (n.toElement().attribute("name"). compare(tableK->item(row,
						0)->text()) == 0) {
					firma.removeChild(n);
					break;
				}
			}

			for (QDomNode n = urzad.firstChild(); !n.isNull(); n
					= n.nextSibling()) {
				// qDebug("aaa");
				if (n.toElement().attribute("name"). compare(tableK->item(row,
						0)->text()) == 0) {
					urzad.removeChild(n);
					break;
				}
			}

			QString xml = doc.toString();
			file.close();
			file.open(QIODevice::WriteOnly);
			QTextStream ts(&file);
			ts << xml;

			file.close();
			tableK->removeRow(row);
		}

	}
}

/** Slot used to edit customer
 */
void MainWindow::kontrEd() {
	int row = tableK->selectedItems()[0]->row();
	// qDebug ()<<tableK->item(row, 0)->text();

	Kontrahenci *kontrWindow = new Kontrahenci(this, 1);
	kontrWindow->readData(tableK->item(row, 0)->text(),
			sett().getCustomerType(tableK->item(row, 1)->text()));
	if (kontrWindow->exec() == QDialog::Accepted) {
		QStringList rowTxt = kontrWindow->ret.split("|");
		tableK->item(row, 0)->setText(rowTxt[0]); // name
		tableK->item(row, 1)->setText(rowTxt[1]); // type
		tableK->item(row, 2)->setText(rowTxt[2]); // place
		tableK->item(row, 3)->setText(rowTxt[3]); // address
		tableK->item(row, 4)->setText(rowTxt[4]); // telefon
	}
}

/** Slot used for creating new invoices
 */
void MainWindow::newFra() {
	Faktura *fraWindow = new Faktura(this);
	fraWindow->progDir2 = workingDir;
	fraWindow->pforma = false;
	if (fraWindow->exec() == QDialog::Accepted) {
		insertRow(tableH, tableH->rowCount());
		QStringList row = fraWindow->ret.split("|");
		tableH->item(tableH->rowCount() - 1, 0)->setText(row[0]); // file name
		tableH->item(tableH->rowCount() - 1, 1)->setText(row[1]); // symbol
		tableH->item(tableH->rowCount() - 1, 2)->setText(row[2]); // date
		tableH->item(tableH->rowCount() - 1, 3)->setText(row[3]); // type
		tableH->item(tableH->rowCount() - 1, 4)->setText(row[4]); // nabywca
		tableH->item(tableH->rowCount() - 1, 5)->setText(row[5]); // NIP
	}
}

/** Slot used to create new ProForma Invoice
 */
void MainWindow::newPForm() {
	Faktura *fraWindow = new Faktura(this);
	fraWindow->progDir2 = workingDir;
	fraWindow->pforma = true;
	fraWindow->setWindowTitle("Faktura Pro Forma");
	fraWindow->backBtnClick();
	if (fraWindow->exec() == QDialog::Accepted) {
		insertRow(tableH, tableH->rowCount());
		QStringList row = fraWindow->ret.split("|");
		tableH->item(tableH->rowCount() - 1, 0)->setText(row[0]); // file name
		tableH->item(tableH->rowCount() - 1, 1)->setText(row[1]); // symbol
		tableH->item(tableH->rowCount() - 1, 2)->setText(row[2]); // date
		tableH->item(tableH->rowCount() - 1, 3)->setText(row[3]); // type
		tableH->item(tableH->rowCount() - 1, 4)->setText(row[4]); // nabywca
		tableH->item(tableH->rowCount() - 1, 5)->setText(row[5]); // NIP
	}
}

/** Slot used to create new Korkta
 */
void MainWindow::newKor() {
	int row = tableH->selectedItems()[0]->row();

	if ((tableH->item(row, 3)->text() == "FVAT")) {
		Korekta *korWindow = new Korekta(this);
		// qDebug( pdGlob );
		korWindow->progDir2 = workingDir;
		korWindow->readDataNewKor(tableH->item(row, 0)->text());
		if (korWindow->exec() == QDialog::Accepted) {
			insertRow(tableH, tableH->rowCount());
			QStringList row = korWindow->ret.split("|");
			tableH->item(tableH->rowCount() - 1, 0)->setText(row[0]); // file name
			tableH->item(tableH->rowCount() - 1, 1)->setText(row[1]); // symbol
			tableH->item(tableH->rowCount() - 1, 2)->setText(row[2]); // date
			tableH->item(tableH->rowCount() - 1, 3)->setText(row[3]); // type
			tableH->item(tableH->rowCount() - 1, 4)->setText(row[4]); // nabywca
			tableH->item(tableH->rowCount() - 1, 5)->setText(row[5]); // NIP
		}
	}

	if ((tableH->item(row, 3)->text() == "korekta")) {
		QMessageBox::information(this, "QFaktury",
				trUtf8("Do korekt nie wystawiamy korekt"), QMessageBox::Ok);
	}
	if ((tableH->item(row, 3)->text() == "FPro")) {
		QMessageBox::information(this, "QFaktury",
				trUtf8("Do faktur Pro Forma nie wystawiamy korekt"), QMessageBox::Ok);
	}
}

/** Slot used to add goods
 */
void MainWindow::towaryDodaj() {
	Towary *towWindow = new Towary(this, 0);
	if (towWindow->exec() == QDialog::Accepted) {
		insertRow(tableT, tableT->rowCount());
		QStringList row = towWindow->ret.split("|");
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
	}
}

/** Slot used to delete goods
 */
void MainWindow::towaryUsun() {
	int row = tableT->currentRow();

	if (QMessageBox::warning(this, trUtf8("QFaktury"), trUtf8("Czy napewno chcesz usunąć towar ") + tableT->item(row, 0)->text()
			+ "/" + tableT->item(row, 1)->text() + "?", trUtf8("Tak"), trUtf8("Nie"), 0, 0, 1)
			== 0) {

		QDomDocument doc(sett().getProdutcsDocName());
		QDomElement root;
		QDomElement products;
		QDomElement services;

		QFile file(sett().getProductsXml());
		if (!file.open(QIODevice::ReadOnly)) {
			qDebug() << "File" << file.fileName() << "doesn't exists";
			return;
		} else {
			QTextStream stream(&file);
			if (!doc.setContent(stream.readAll())) {
				qDebug("can not set content ");
				file.close();
				return;
			} else {
				root = doc.documentElement();
				products = root.firstChild().toElement();
				services = root.lastChild().toElement();
			}
			QString text;

			for (QDomNode n = services.firstChild(); !n.isNull(); n
					= n.nextSibling()) {
				// qDebug("aaa");
				if (n.toElement().attribute("idx"). compare(
						tableT->item(row, 0)->text()) == 0) {
					services.removeChild(n);
					break;
				}
			}

			for (QDomNode n = products.firstChild(); !n.isNull(); n
					= n.nextSibling()) {
				// qDebug("aaa");
				if (n.toElement().attribute("idx"). compare(
						tableT->item(row, 0)->text()) == 0) {
					products.removeChild(n);
					break;
				}
			}

			QString xml = doc.toString();
			file.close();
			file.open(QIODevice::WriteOnly);
			QTextStream ts(&file);
			ts << xml;

			file.close();
			tableT->removeRow(row);
		}

	}

}

/** Slot used for editing goods
 */
void MainWindow::towaryEdycja() {
	int row = tableT->selectedItems()[0]->row();

	Towary *towWindow = new Towary(this, 1);
	towWindow->readData(tableT->item(row, 0)->text(),
			sett().getProductType(tableT->item(row, 5)->text()));
	if (towWindow->exec() == QDialog::Accepted) {
		QStringList rowTxt = towWindow->ret.split("|");
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
	}

}

/** Slot close
 */
bool MainWindow::close() {
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
void MainWindow::pomoc() {
	QDesktopServices::openUrl(QUrl("http://www.e-linux.pl/modules/qfaktury/index.php"));
}

/** Slot forum
 */
void MainWindow::forum() {
	QMessageBox::information(this, tr("Forum e-linux.pl"), tr(
			"http://forum.e-linux.pl"));
	QString program = "kfmclient";
	QStringList arguments;
	QProcess *myProcess = new QProcess(this);

#if defined Q_OS_UNIX
	arguments << "exec" << "http://forum.e-linux.pl/";
	// add ifdef for windows
	myProcess->start(program, arguments);
#endif

#if defined Q_WS_WIN
	// it may need to be changed to something more universal
	program = "c:\\Program Files\\Internet Explorer\\iexplore.exe";
	arguments << "http://forum.e-linux.pl";
	myProcess->start(program, arguments);
#endif
}

// ----------------------------------------  SLOTS ---------------------------------//



//-------------------- not used >??? ------------------------------------------
void MainWindow::nextPage()
{
 if ( tabWidget2->count() != tabWidget2->currentIndex() )
 tabWidget2->setCurrentIndex( tabWidget2->currentIndex() + 1 );
}


void MainWindow::prevPage()
{
 if ( tabWidget2->currentIndex() !=  0 )
 tabWidget2->setCurrentIndex( tabWidget2->currentIndex() - 1 );
}

