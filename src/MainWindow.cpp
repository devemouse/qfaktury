#include "src\MainWindow.h"
#include "ui_MainWindow.h"
#include "InvoicesModel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->invoices_tableView->setModel(new InvoicesModel(this));
    ui->invoices_tableView->verticalHeader()->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
