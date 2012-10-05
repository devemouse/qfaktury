#ifndef TEXTFILTERPROXYMODEL_H
#define TEXTFILTERPROXYMODEL_H

#include <QLineEdit>
#include <QList>
#include "WidgetProxyModel.h"

class TextFilterProxyModel : public WidgetProxyModel
{
    Q_OBJECT
public:
    explicit TextFilterProxyModel(QList<int> columns, QObject *parent = 0);
    ~TextFilterProxyModel();
    QWidget *getWidget(void);
    
signals:
    
public slots:

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private slots:
    void updateFilter(QString rexp);

private:
    QLineEdit *lineEdit;
    QList<int> columns;
    
};

#endif // TEXTFILTERPROXYMODEL_H
