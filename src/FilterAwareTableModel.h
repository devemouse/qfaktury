#ifndef FILTERAWARETABLEMODEL_H
#define FILTERAWARETABLEMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include "WidgetProxyModel.h"

class FilterAwareTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FilterAwareTableModel(QObject *parent = 0):
        QAbstractTableModel(parent){}
    virtual WidgetProxyModel * getDateFilter() {return new WidgetProxyModel;}
    virtual WidgetProxyModel * getTextFilter() {return new WidgetProxyModel;}
    
signals:
    
public slots:
    
};

#endif // FILTERAWARETABLEMODEL_H
