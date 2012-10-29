#ifndef INVOICESLISTFORM_H
#define INVOICESLISTFORM_H

#include <QWidget>
#include "InvoicesModel.h"
#include "DateFilterProxyModel.h"

namespace Ui {
class CustomTableView;
}

class CustomTableView : public QWidget
{
    Q_OBJECT
    
public:
    explicit CustomTableView(FilterAwareTableModel *model, QWidget *parent = 0);
    ~CustomTableView();

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::CustomTableView *ui;
    void addFilter(WidgetProxyModel* filter);

};

#endif // INVOICESLISTFORM_H
