#ifndef DATEFILTERPROXYMODEL_H
#define DATEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>
#include <QWidget>
#include "WidgetProxyModel.h"
class DateFilterProxyModelWidget;


class DateFilterProxyModel : public WidgetProxyModel
{
    Q_OBJECT
public:
    explicit DateFilterProxyModel(int column, QObject *parent = 0);
    QWidget *getWidget(void);

signals:

public slots:
    void setFilterMinimumDate(const QDate &date);
    void setFilterMaximumDate(const QDate &date);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    bool dateInRange(const QDate &date) const;

    int column;
    QDate minDate;
    QDate maxDate;
    DateFilterProxyModelWidget *widget;
};

#endif // DATEFILTERPROXYMODEL_H
