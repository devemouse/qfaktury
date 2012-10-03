#include <QDate>
#include <QtGui>
#include "MainWindow.h"
#include "old_MainWindow.h"
#include "ui_MainWindow.h"
#include "InvoicesModel.h"
#include "ContractorsModel.h"
#include "InvoiceDateFilterProxyModel.h"
#include "InvoicesListForm.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(ui->history_tab);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->history_tab->setLayout(layout);

    InvoicesListForm *form = new InvoicesListForm(new InvoicesModel(this));
    ui->history_tab->layout()->addWidget(form);

    layout = new QHBoxLayout(ui->contractors_tab);
    layout->setSpacing(0);
    layout->setMargin(0);
    ui->contractors_tab->setLayout(layout);

    form = new InvoicesListForm(new ContractorsModel(this));
    ui->contractors_tab->layout()->addWidget(form);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionRun_old_MainWindow_triggered()
{
    //run this in thread
    //QRect screen = QApplication::desktop()->screenGeometry();
   // old_MainWindow w(0);
   // w.move(screen.center() - QPoint(w.width() / 2, w.height() / 2));
    // w.show();
}
