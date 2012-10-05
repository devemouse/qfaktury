#include <QtGui>
#include "CustomTableView.h"
#include "ui_CustomTableView.h"
#include "WidgetProxyModel.h"


CustomTableView::CustomTableView(FilterAwareTableModel *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomTableView)
{
    ui->setupUi(this);

    ui->tableView->setModel(model);

    addFilter(model->getDateFilter());
    ui->topBarLayout->addStretch(9999);
    addFilter(model->getTextFilter());

    ui->topBarLayout->setSpacing(0);


    ui->tableView->verticalHeader()->setVisible(false);
}

CustomTableView::~CustomTableView()
{
    delete ui;
}

void CustomTableView::addFilter(WidgetProxyModel *filter)
{
    QWidget *tempWidget;

    filter->setSourceModel(ui->tableView->model());
    filter->setDynamicSortFilter(true);
    tempWidget = filter->getWidget();
    if (tempWidget != 0)
        ui->topBarLayout->addWidget(tempWidget,1,Qt::AlignLeft);

    ui->tableView->setModel(filter);
}
