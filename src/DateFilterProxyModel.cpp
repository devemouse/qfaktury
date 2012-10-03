#include <QDebug>
#include "config.h"
#include "DateFilterProxyModel.h"
#include "DateFilterProxyModelWidget.h"

DateFilterProxyModel::DateFilterProxyModel(int column, QObject *parent) :
    QSortFilterProxyModel(parent), column(column), widget(0)
{
}

void DateFilterProxyModel::setFilterMinimumDate(const QDate &date)
{
    F_TRACE;
    minDate = date;
    invalidateFilter();
}

void DateFilterProxyModel::setFilterMaximumDate(const QDate &date)
{
    F_TRACE;
    maxDate = date;
    invalidateFilter();
}

bool DateFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    F_TRACE;
    QModelIndex date_idx = sourceModel()->index(sourceRow, column, sourceParent);
    return  dateInRange(sourceModel()->data(date_idx).toDate());
}

bool DateFilterProxyModel::dateInRange(const QDate &date) const
{
    F_TRACE;
    qDebug() << minDate << "<" << date << "<" << maxDate << "?";
    return (!minDate.isValid() || date >= minDate)
            && (!maxDate.isValid() || date <= maxDate);
}

QWidget * DateFilterProxyModel::getWidget()
{
    F_TRACE;
    if (widget == 0)
    {
        widget = new DateFilterProxyModelWidget(0);

        connect(widget, SIGNAL(startDateChanged(QDate)),
                this, SLOT(setFilterMinimumDate(QDate)));
        connect(widget, SIGNAL(endDateChanged(QDate)),
                this, SLOT(setFilterMaximumDate(QDate)));

        widget->goToToday();
    }
    return widget;
}
