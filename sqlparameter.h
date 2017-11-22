#ifndef SQLPARAMETER_H
#define SQLPARAMETER_H

#include <QObject>

class SQLParameter : public QObject
{
    Q_OBJECT
public:
    explicit SQLParameter(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SQLPARAMETER_H