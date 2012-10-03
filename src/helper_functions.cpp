#include <QXmlQuery>
#include <QDebug>
#include "config.h"
#include "helper_functions.h"

QString runXmlQuerry(QFile *file, QString querry)
{
    F_TRACE;
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
    return res.simplified();
}

