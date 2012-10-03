#include "src/DateFilterProxyModelWidget.h"
#include "ui_DateFilterProxyModelWidget.h"

DateFilterProxyModelWidget::DateFilterProxyModelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DateFilterProxyModelWidget)
{
    ui->setupUi(this);

    connect(ui->goToToday_pushButton, SIGNAL(clicked()),
            this, SLOT(goToToday()));
    connect(ui->from_dateEdit, SIGNAL(dateChanged(QDate)),
            this, SIGNAL(startDateChanged(QDate)));
    connect(ui->to_dateEdit, SIGNAL(dateChanged(QDate)),
            this, SIGNAL(endDateChanged(QDate)));
}

DateFilterProxyModelWidget::~DateFilterProxyModelWidget()
{
    delete ui;
}

void DateFilterProxyModelWidget::on_prev_month_button_clicked()
{
    addMonth(-1);
}

void DateFilterProxyModelWidget::on_next_month_button_clicked()
{
    addMonth(1);
}

void DateFilterProxyModelWidget::goToToday()
{
    QDate now = QDate::currentDate();
    ui->to_dateEdit->setDate(now);
    ui->from_dateEdit->setDate(QDate(now.year(), now.month(), 1));
}

void DateFilterProxyModelWidget::addMonth(int month)
{
    QDate from = ui->from_dateEdit->date().addMonths(month);

    ui->from_dateEdit->setDate(QDate(from.year(), from.month(), 1));
    ui->to_dateEdit->setDate(QDate(from.year(), from.month(), from.daysInMonth()));
}
