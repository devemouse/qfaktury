#ifndef DATEWIDGETPROXYMODEL_H
#define DATEWIDGETPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QWidget>

class WidgetProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit WidgetProxyModel(QObject *parent = 0):
        QSortFilterProxyModel(parent){}
    virtual QWidget *getWidget(void) {return 0;}
    
signals:
    
public slots:
    
};

#endif // DATEWIDGETPROXYMODEL_H
