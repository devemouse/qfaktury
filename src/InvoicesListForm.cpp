#include "InvoicesListForm.h"
#include "ui_invoiceslistform.h"

InvoicesListForm::InvoicesListForm(InvoicesModel *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InvoicesListForm)
{
    ui->setupUi(this);

    proxyModel = new InvoiceDateFilterProxyModel;
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSourceModel(model);

    ui->invoices_tableView->setModel(proxyModel);
    ui->invoices_tableView->verticalHeader()->setVisible(false);

    connect(ui->from_dateEdit, SIGNAL(dateChanged(QDate)),
            proxyModel, SLOT(setFilterMinimumDate(QDate)));
    connect(ui->to_dateEdit, SIGNAL(dateChanged(QDate)),
            proxyModel, SLOT(setFilterMaximumDate(QDate)));
    connect(ui->goToToday_pushButton, SIGNAL(clicked()),
            this, SLOT(goToToday()));

    goToToday();
}

InvoicesListForm::~InvoicesListForm()
{
    delete ui;
}

void InvoicesListForm::on_prev_month_button_clicked()
{
    addMonth(-1);
}

void InvoicesListForm::on_next_month_button_clicked()
{
    addMonth(1);
}

void InvoicesListForm::addMonth(int month)
{
    QDate from = ui->from_dateEdit->date().addMonths(month);

    ui->from_dateEdit->setDate(QDate(from.year(), from.month(), 1));
    ui->to_dateEdit->setDate(QDate(from.year(), from.month(), from.daysInMonth()));
}

void InvoicesListForm::goToToday()
{
    QDate now = QDate::currentDate();
    ui->to_dateEdit->setDate(now);
    ui->from_dateEdit->setDate(QDate(now.year(), now.month(), 1));
}
