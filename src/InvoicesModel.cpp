#include "InvoicesModel.h"
#include <QDir>
#include <QStringList>
#include "Settings.h"
#include "InvoiceItem.h"


InvoicesModel::InvoicesModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    QDir invoices_dir;

    invoices_dir.setPath(sett().getInvoicesDir());
    invoices_dir.setFilter(QDir::Files);
    QStringList filters;
    filters << "h*.xml" << "k*.xml";
    invoices_dir.setNameFilters(filters);
    foreach(QString file, invoices_dir.entryList())
    {
        allFiles << new InvoiceItem(file, this);
    }
}

int InvoicesModel::rowCount(const QModelIndex &parent) const
{
    F_TRACE;
    return allFiles.size();
}

int InvoicesModel::columnCount(const QModelIndex &parent) const
{
    F_TRACE;
    return InvoiceItem::columnCount();
}

QVariant InvoicesModel::data(const QModelIndex &index, int role) const
{
    F_TRACE;
    if (!index.isValid() || role != Qt::DisplayRole)
             return QVariant();
    return allFiles.at(index.row())->at(index.column());
}

QVariant InvoicesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    F_TRACE;
    if (role != Qt::DisplayRole)
         return QVariant();

     if (orientation == Qt::Horizontal)
         return InvoiceItem::columnHeader(section);
     else
         return QVariant();
}
