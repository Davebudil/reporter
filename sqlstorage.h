#ifndef SQLSTORAGE_H
#define SQLSTORAGE_H

#include <QObject>

class SQLStorage : public QObject
{
    Q_OBJECT
public:
    explicit SQLStorage(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SQLSTORAGE_H