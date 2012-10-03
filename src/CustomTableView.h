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
    explicit CustomTableView(QAbstractTableModel *model, QWidget *parent = 0);
    ~CustomTableView();

    void addFilter(QSortFilterProxyModel* newModel);
    
private:
    Ui::CustomTableView *ui;
  //  void addMonth(int month);

    QAbstractTableModel *sourceModel;
    QSortFilterProxyModel *proxyModel;

private slots:
   // void on_prev_month_button_clicked();
   // void on_next_month_button_clicked();
    //void goToToday(void);
    void updateFilter(void);
};

#endif // INVOICESLISTFORM_H
