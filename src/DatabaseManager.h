#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    DatabaseManager(QObject *parent = 0);
    ~DatabaseManager(){}

    bool openDB();
    bool deleteDB();
    bool createTables();
    QStringList getTables();
    QSqlError lastError();

private:
    QSqlDatabase db;

signals:

public slots:

};

#endif // DATABASEMANAGER_H
