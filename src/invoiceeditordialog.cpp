#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include "invoiceeditordialog.h"
#include "ui_invoiceeditordialog.h"

InvoiceEditorDialog::InvoiceEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InvoiceEditorDialog)
{
    ui->setupUi(this);
}

InvoiceEditorDialog::~InvoiceEditorDialog()
{
    delete ui;
}

void InvoiceEditorDialog::on_pushButton_clicked()
{
    QPrinter *printer = new QPrinter;
    QPrintDialog *printDialog = new QPrintDialog(printer, this);
    if (printDialog->exec() == QDialog::Accepted) {
        QPainter p(printer);
        QPixmap pm = QPixmap::grabWidget(this);
        p.drawPixmap(0, 0, pm);
    }
}
