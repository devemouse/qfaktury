#include <QDebug>
#include <QDate>
#include "config.h"
#include "InvoiceDateFilterProxyModel.h"

InvoiceDateFilterProxyModel::InvoiceDateFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void InvoiceDateFilterProxyModel::setFilterMinimumDate(const QDate &date)
{
    F_TRACE;
    minDate = date;
    invalidateFilter();
}

void InvoiceDateFilterProxyModel::setFilterMaximumDate(const QDate &date)
{
    F_TRACE;
    maxDate = date;
    invalidateFilter();
}

bool InvoiceDateFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    F_TRACE;
    QModelIndex date_index = sourceModel()->index(sourceRow, 2, sourceParent);

    return (dateInRange(sourceModel()->data(date_index).toDate()));
}

bool InvoiceDateFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    F_TRACE;
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    if (leftData.type() == QVariant::DateTime) {
        return leftData.toDateTime() < rightData.toDateTime();
    } else {
        return false;
    }
}

bool InvoiceDateFilterProxyModel::dateInRange(const QDate &date) const
{
    F_TRACE;
    return (!minDate.isValid() || date >= minDate)
           && (!maxDate.isValid() || date <= maxDate);
}

