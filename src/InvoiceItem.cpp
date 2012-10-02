#include "src\InvoiceItem.h"
#include <QString>
#include <QStringList>
#include <QObject>

InvoiceItem::InvoiceItem(QObject *parent) :
    QObject(parent)
{
}

InvoiceItem::InvoiceItem(QString filename, QObject *parent) :
    QObject(parent)
{
    data << filename;

    data << "Symbol" << "Data" << "Typ";
    data << "Nabywca" << "Nip" << "Kwota";
}

QVariant InvoiceItem::at(int column)
{
    if (column < data.size())
        return data[column];
    else
        return QVariant();
}

QString InvoiceItem::columnHeader(int column)
{
    QStringList columns;
    columns << "Nazwa Pliku" << "Symbol" << "Data" << "Typ";
    columns << "Nabywca" << "Nip" << "Kwota";
    return columns.at(column);
}
