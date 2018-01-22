#include "tableinfo.h"
#include "ui_tableinfo.h"

TableInfo::TableInfo(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::TableInfo){
   ui->setupUi(this);
}

TableInfo::~TableInfo(){
   delete ui;
}

void TableInfo::getInfo(const QVector<QStringList> & dbInfo){
   for(auto & it : dbInfo){
      qDebug() << it[0];
   }
}
