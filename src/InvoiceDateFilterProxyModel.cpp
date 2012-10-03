#include <QDebug>
#include <QDate>
#include "config.h"
#include "InvoiceDateFilterProxyModel.h"
#include "InvoiceItem.h"

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
   // QModelIndex date_idx = sourceModel()->index(sourceRow, InvoiceItem::opt_date, sourceParent);

    QModelIndex symbol_idx = sourceModel()->index(sourceRow, InvoiceItem::opt_symbol, sourceParent);
    QModelIndex buyer_idx = sourceModel()->index(sourceRow, InvoiceItem::opt_buyer, sourceParent);
    QModelIndex tic_idx = sourceModel()->index(sourceRow, InvoiceItem::opt_tic, sourceParent);


    return (sourceModel()->data(symbol_idx).toString().contains(filterRegExp())
            || sourceModel()->data(buyer_idx).toString().contains(filterRegExp())
            || sourceModel()->data(tic_idx).toString().contains(filterRegExp()));

         //  && dateInRange(sourceModel()->data(date_idx).toDate());
}

bool InvoiceDateFilterProxyModel::dateInRange(const QDate &date) const
{
    F_TRACE;
    return (!minDate.isValid() || date >= minDate)
           && (!maxDate.isValid() || date <= maxDate);
}

