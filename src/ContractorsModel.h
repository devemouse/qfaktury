#ifndef CONTRACTORSMODEL_H
#define CONTRACTORSMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariant>
#include "FilterAwareTableModel.h"

class ContractorsModel : public FilterAwareTableModel
{
    Q_OBJECT
public:
    explicit ContractorsModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    WidgetProxyModel * getTextFilter();

    enum {
        summary_name,
        summary_city,
        summary_city_code,
        summary_address,
        summary_tic,
        summary_account,
        summary_contractor_type,
        summary_telephone,
        summary_email,
        summary_website,
        summary_num_of_items
    };
signals:
    
public slots:

private:
    QList< QList<QVariant> > contractors;
    
};

#endif // CONTRACTORSMODEL_H
