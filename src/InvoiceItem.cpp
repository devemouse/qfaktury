#include "InvoiceItem.h"
#include "Settings.h"
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QFile>
#include <QtXmlPatterns/QXmlQuery>
#include <QDomDocument>
#include <QDomNodeList>
#include <QDomNode>

InvoiceItem::InvoiceItem(QObject *parent) :
    QObject(parent)
{
}

InvoiceItem::InvoiceItem(QString filename, QObject *parent) :
    QObject(parent)
{
    data << filename;

    QFile invoice_file(sett().getInvoicesDir() + filename);

    if (!invoice_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << sett().getInvoicesDir() + filename << "file doesn't exist";
    }

    QDomDocument productsDOMDocument;
    productsDOMDocument.setContent("" + runQuerry(&invoice_file, "//product") + "");
    products = productsDOMDocument.elementsByTagName("product");

    data << runQuerry(&invoice_file, "/invoice/@no/data(.)");
    data << runQuerry(&invoice_file, "/invoice/@issueDate/data(.)");
    data << runQuerry(&invoice_file, "/invoice/@type/data(.)");
    data << runQuerry(&invoice_file, "/invoice/buyer/@name/data(.)");
    data << runQuerry(&invoice_file, "/invoice/buyer/@tic/data(.)");

    data << "Kwota";

    qDebug() << data;
    invoice_file.close();
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

QString InvoiceItem::runQuerry(QFile *file, QString querry)
{
    QXmlQuery query;
    QString res;

    file->seek(0);
    query.setFocus(file);
    query.setQuery(querry);
    if ( ! query.isValid())
    {
        qDebug() << "Invalid querry" << querry;
    }

    query.evaluateTo(&res);
    return res;
}
