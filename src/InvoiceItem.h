#ifndef INVOICEITEM_H
#define INVOICEITEM_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QDomNodeList>
#include <QFile>

class InvoiceItem : public QObject
{
    Q_OBJECT
public:
    explicit InvoiceItem(QObject *parent = 0);
    explicit InvoiceItem(QString filename, QObject *parent = 0);

    QString filename(void) { return data[opt_filename].toString();}
    QVariant at(int column);

    static int columnCount(void) {return opt_num_fields;}
    static QString columnHeader(int column);
    
signals:
    
public slots:
    
private:

    QString runQuerry(QFile *file, QString querry);
    QList<QVariant> data;
    QDomNodeList products;

    enum {
        opt_filename,
        opt_symbol,
        opt_data,
        opt_type,
        opt_buyer,
        opt_nip,
        opt_amount,
        opt_num_fields
    };
};

#endif // INVOICEITEM_H
