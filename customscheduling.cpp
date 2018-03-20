#include "customscheduling.h"
#include "ui_customscheduling.h"

CustomScheduling::CustomScheduling(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::CustomScheduling){
   ui->setupUi(this);
}

CustomScheduling::~CustomScheduling(){
   delete ui;
}
