#include <QDebug>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QFile>
#include <QtXmlPatterns/QXmlQuery>
#include <QDomDocument>
#include <QDomNodeList>
#include <QDomNode>


#include "Settings.h"
#include "helper_functions.h"
#include "InvoiceItem.h"



InvoiceItem::InvoiceItem(QObject *parent) :
    QObject(parent)
{
}

InvoiceItem::InvoiceItem(QString filename, QObject *parent) :
    QObject(parent)
{
    F_TRACE;
    data << filename;

    QFile invoice_file(sett().getInvoicesDir() + filename);

    qDebug() << "Loading" << invoice_file.fileName();

    if (!invoice_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << sett().getInvoicesDir() + filename << "file doesn't exist";
    }

    QDomDocument productsDOMDocument;
    productsDOMDocument.setContent("" + runXmlQuerry(&invoice_file, "//product") + "");
    products = productsDOMDocument.elementsByTagName("product");

    data << runXmlQuerry(&invoice_file, "/invoice/@no/data(.)");
    data << QDate::fromString(runXmlQuerry(&invoice_file, "/invoice/@issueDate/data(.)"), "dd/MM/yyyy");
    data << runXmlQuerry(&invoice_file, "/invoice/@type/data(.)");
    data << runXmlQuerry(&invoice_file, "/invoice/buyer/@name/data(.)");
    data << runXmlQuerry(&invoice_file, "/invoice/buyer/@tic/data(.)");

    data << "Kwota";

    qDebug() << data;
    invoice_file.close();
}

QVariant InvoiceItem::at(int column)
{
    F_TRACE;
    if (column < data.size())
        return data[column];
    else
        return QVariant();
}

QString InvoiceItem::columnHeader(int column)
{
    F_TRACE;
    QStringList columns;
    columns << "Nazwa Pliku" << "Symbol" << "Data" << "Typ";
    columns << "Nabywca" << "Nip" << "Kwota";
    return columns.at(column);
}
