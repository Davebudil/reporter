#ifndef SETUP_H
#define SETUP_H

#include <QObject>

class Setup : public QObject
{
    Q_OBJECT
public:
    explicit Setup(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SETUP_H