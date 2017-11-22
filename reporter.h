#ifndef REPORTER_H
#define REPORTER_H

#include <QMainWindow>

namespace Ui {
class Reporter;
}

class Reporter : public QMainWindow
{
    Q_OBJECT

public:
    explicit Reporter(QWidget *parent = 0);
    ~Reporter();

private:
    Ui::Reporter *ui;
};

#endif // REPORTER_H
