#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "InvoiceDateFilterProxyModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void on_actionRun_old_MainWindow_triggered();
    /*void on_prev_month_button_clicked();
    void on_next_month_button_clicked();
    void goToToday(void);
    */

private:
    //void addMonth(int month);

    //InvoiceDateFilterProxyModel *proxyModel;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
