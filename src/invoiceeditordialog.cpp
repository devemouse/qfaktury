#include "src/invoiceeditordialog.h"
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
