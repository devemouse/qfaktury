#include "CustomTableView.h"
#include "ui_CustomTableView.h"
#include "DateFilterProxyModel.h"

CustomTableView::CustomTableView(QAbstractTableModel *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomTableView)
{
    ui->setupUi(this);

    sourceModel = model;
    ui->invoices_tableView->setModel(sourceModel);

    ui->invoices_tableView->verticalHeader()->setVisible(false);
}

CustomTableView::~CustomTableView()
{
    delete ui;
}


void CustomTableView::updateFilter()
{
   // QRegExp regExp(ui->filter_lineEdit->text(), Qt::CaseInsensitive, QRegExp::Wildcard);
   // proxyModel->setFilterRegExp(regExp);
}

void CustomTableView::addFilter(QSortFilterProxyModel *newModel)
{
    proxyModel = newModel;
    proxyModel->setSourceModel(sourceModel);
    proxyModel->setDynamicSortFilter(true);

    ui->invoices_tableView->setModel(proxyModel);
    ui->topBarLayout->addWidget(static_cast<DateFilterProxyModel *>(newModel)->getWidget());

/*    connect(ui->filter_lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateFilter()));*/
}
