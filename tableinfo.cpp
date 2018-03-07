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
   QString tableView;
   for(auto & it : dbInfo){
      QStringList tmp = it;
      tmp[0] = "Table name: " + tmp[0];
      tableView += tmp.join("\n");
      tableView += "\n";
      tableView += "\n";
   }
   ui->tablesVIew->setText(tableView);
}
