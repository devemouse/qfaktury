#include <QLineEdit>
#include "TextFilterProxyModel.h"

TextFilterProxyModel::TextFilterProxyModel(QList<int> columns, QObject *parent) :
    WidgetProxyModel(parent), lineEdit(new QLineEdit), columns(columns)
{
    lineEdit->setMinimumWidth(200);
    connect(lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(updateFilter(QString)));
}

TextFilterProxyModel::~TextFilterProxyModel()
{
    delete lineEdit;
}

QWidget *TextFilterProxyModel::getWidget()
{
    return lineEdit;
}

bool TextFilterProxyModel::filterAcceptsRow(int sourceRow,
                                            const QModelIndex &sourceParent) const
{
    bool ret = false;
    foreach (int col, columns)
    {
        QModelIndex idx = sourceModel()->index(sourceRow, col, sourceParent);
        bool tmp = sourceModel()->data(idx).toString().contains(filterRegExp());
        ret |= tmp;
    }
    return ret;
}

void TextFilterProxyModel::updateFilter(QString rexp)
{
    QRegExp regExp(rexp, Qt::CaseInsensitive, QRegExp::Wildcard);
    setFilterRegExp(regExp);
}
