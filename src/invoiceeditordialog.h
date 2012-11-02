#ifndef INVOICEEDITORDIALOG_H
#define INVOICEEDITORDIALOG_H

#include <QDialog>

namespace Ui {
    class InvoiceEditorDialog;
}

class InvoiceEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InvoiceEditorDialog(QWidget *parent = 0);
    ~InvoiceEditorDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::InvoiceEditorDialog *ui;
};

#endif // INVOICEEDITORDIALOG_H
