#ifndef INVOICEDATEFILTERPROXYMODEL_H
#define INVOICEDATEFILTERPROXYMODEL_H

#include <QDate>
#include <QSortFilterProxyModel>

class InvoiceDateFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    InvoiceDateFilterProxyModel(QObject *parent = 0);


    QDate filterMinimumDate() const { return minDate; }


    QDate filterMaximumDate() const { return maxDate; }

    
signals:
    
public slots:
    void setFilterMinimumDate(const QDate &date);
    void setFilterMaximumDate(const QDate &date);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    bool dateInRange(const QDate &date) const;

    QDate minDate;
    QDate maxDate;

};

#endif // INVOICEDATEFILTERPROXYMODEL_H
