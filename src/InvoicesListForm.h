#ifndef INVOICESLISTFORM_H
#define INVOICESLISTFORM_H

#include <QWidget>
#include "InvoicesModel.h"
#include "InvoiceDateFilterProxyModel.h"

namespace Ui {
class InvoicesListForm;
}

class InvoicesListForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit InvoicesListForm(InvoicesModel *model, QWidget *parent = 0);
    ~InvoicesListForm();
    
private:
    Ui::InvoicesListForm *ui;
    void addMonth(int month);

    InvoiceDateFilterProxyModel *proxyModel;

private slots:
    void on_prev_month_button_clicked();
    void on_next_month_button_clicked();
    void goToToday(void);
};

#endif // INVOICESLISTFORM_H
