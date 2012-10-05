#ifndef INVOICESMODEL_H
#define INVOICESMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QList>
#include "InvoiceItem.h"
#include "FilterAwareTableModel.h"

class QSortFilterProxyModel;

class InvoicesModel : public FilterAwareTableModel
{
    Q_OBJECT
public:
    explicit InvoicesModel(QObject *parent = 0);
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    WidgetProxyModel * getDateFilter();
    WidgetProxyModel * getTextFilter();


signals:
    
public slots:
    
private:
    QList<InvoiceItem *> allFiles;
};

#endif // INVOICESMODEL_H
