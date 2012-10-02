/*
 * FakrutaBrutto.cpp
 *
 *  Created on: Apr 2, 2009
 *      Author: TPIELECH
 */

#include "moc_FakturaBrutto.cpp"

#include "TowaryBruttoLista.h"
#include "old_MainWindow.h"

// constructor
FakturaBrutto::FakturaBrutto(QWidget *parent, IDataLayer *dl): Faktura(parent, dl) {

}

void FakturaBrutto::fakturaBruttoInit() {
	// invoiceType = "FB"
}

QString FakturaBrutto::getInvoiceTypeAndSaveNr() {
	QString ftype = "FBrutto";
	sett().setValue("fvat", frNr->text());
	return ftype;
}

/** Caclulate Discount
 */
void FakturaBrutto::calculateOneDiscount(int i) {
	// qDebug() << __FUNCTION__ << __LINE__ << __FILE__;

	double quantity = 0, vat = 0, gross = 0;
	double netto = 0,  price = 0;
	double discountValue = 0, discount;

	price = sett().stringToDouble(tableTow->item(i, 7)->text());
	if (constRab->isChecked()) {
		discount = rabatValue->value() * 0.01;
	} else {
		discount = sett().stringToDouble(tableTow->item(i, 6)->text()) * 0.01;
	}
	quantity = sett().stringToDouble(tableTow->item(i, 4)->text());
	price = price * quantity;
	discountValue = price * discount;

	gross = price - discountValue;
	int vatValue = sett().stringToDouble(tableTow->item(i, 9)->text());
	vat = (gross * vatValue)/(100 + vatValue);

	netto = gross - vat;

	// qDebug() << price << quantity << netto << discount << discountValue << vat << gross;

    tableTow->item(i, 6)->setText(sett().numberToString(discount * 100, 'f', 0)); // discount
	// tableTow->item(i, 7)->setText(price); // price
	tableTow->item(i, 8)->setText(sett().numberToString(netto)); // nett
	// tableTow->item(i, 9)->setText(sett().numberToString(gross - vat)); // vat
	tableTow->item(i, 10)->setText(sett().numberToString(gross)); // gross

}

/** Slot
 *  Add new towar
 */
void FakturaBrutto::addTow() {
	TowaryBruttoLista twWindow(this);
	if (twWindow.exec() == QDialog::Accepted) {
		old_MainWindow::insertRow(tableTow, tableTow->rowCount());
		// qDebug() << twWindow->ret;
		QStringList row = twWindow.ret.split("|");
		int rowNum = tableTow->rowCount() - 1;
		tableTow->item(rowNum, 0)->setText(sett().numberToString(
				tableTow->rowCount())); // id
		tableTow->item(rowNum, 1)->setText(row[0]); // name
		tableTow->item(rowNum, 2)->setText(row[1]); // code
		tableTow->item(rowNum, 3)->setText(row[2]); // pkwiu
		tableTow->item(rowNum, 4)->setText(row[3]); // quantity
		tableTow->item(rowNum, 5)->setText(row[4]); // qType
		tableTow->item(rowNum, 6)->setText(row[5]); // discount
		tableTow->item(rowNum, 7)->setText(row[6]); // price
		tableTow->item(rowNum, 8)->setText(row[7]); // nett
		tableTow->item(rowNum, 9)->setText(row[8]); // vat
		tableTow->item(rowNum, 10)->setText(row[9]); // gross
		saveBtn->setEnabled(true);
		canClose = false;
		if (constRab->isChecked())
			calculateDiscount();
		calculateSum();
	}
}
