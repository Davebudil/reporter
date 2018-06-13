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
   auto w = new QTreeWidget(this);
   w->setMinimumWidth(461);
   w->setMinimumHeight(461);
   w->setHeaderLabel("Database Detail");
   for(auto it : dbInfo){
      auto t = new QTreeWidgetItem(QStringList() << it[0]);
      it.removeFirst();
      for(auto & itRest : it){
         auto i = new QTreeWidgetItem(QStringList() << itRest);
         t->addChild(i);
      }
      w->addTopLevelItem(t);
   }
   w->show();
}
