#include "reporter.h"
#include "ui_reporter.h"

Reporter::Reporter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Reporter)
{
    ui->setupUi(this);
}

Reporter::~Reporter()
{
    delete ui;
}
