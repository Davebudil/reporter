#ifndef SQLCONTROL_H
#define SQLCONTROL_H

#include <QObject>

class SQLControl : public QObject
{
    Q_OBJECT
public:
    explicit SQLControl(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SQLCONTROL_H