#include <QFile>
#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>

#include "config.h"
#include "Settings.h"
#include "helper_functions.h"
#include "ContractorsModel.h"

ContractorsModel::ContractorsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    QFile contractor_file(sett().getCustomersXml());
    qDebug() << "Loading" << contractor_file.fileName();

    if (!contractor_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << sett().getCustomersXml() << "file doesn't exist";
    }

    QDomDocument contractorsDOMDocument;
    contractorsDOMDocument.setContent("<root>" + runXmlQuerry(&contractor_file, "//company[@name]") + "</root>");

    QDomNodeList m_entryNodes = contractorsDOMDocument.elementsByTagName("company");
    for (int i = 0; i < m_entryNodes.size(); i++)
    {
        QDomElement contr_elem = m_entryNodes.at(i).toElement();
        QList<QVariant> contractor;
        contractor << contr_elem.attribute("name");
        contractor << contr_elem.attribute("place");
        contractor << contr_elem.attribute("code");
        contractor << contr_elem.attribute("address");
        contractor << contr_elem.attribute("tic");
        contractor << contr_elem.attribute("account");
        contractor << tr("Company");
        contractor << contr_elem.attribute("phone");
        contractor << contr_elem.attribute("email");
        contractor << contr_elem.attribute("www");

        contractors << contractor;
    }
    qDebug() << contractors;
}

int ContractorsModel::rowCount(const QModelIndex &parent) const
{
    F_TRACE;
    return contractors.size();
}

int ContractorsModel::columnCount(const QModelIndex &parent) const
{
    F_TRACE;
    return summary_num_of_items;
}

QVariant ContractorsModel::data(const QModelIndex &index, int role) const
{
    F_TRACE;
    if (!index.isValid() || role != Qt::DisplayRole)
             return QVariant();
    return contractors.at(index.row()).at(index.column());
}

QVariant ContractorsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    F_TRACE;
    const QString head_data[] = {"Name", "City", "City_code", "Address",
                                 "Tic", "Account", "Contractor_type",
                                 "Telephone", "Email", "Website"};

    if (role != Qt::DisplayRole)
         return QVariant();

    if ((orientation == Qt::Horizontal)
            && section < sizeof(head_data)/sizeof(head_data[0]))
         return head_data[section];
     else
         return QVariant();

}
