#include "customparametersquery.h"
#include "ui_customparametersquery.h"

customParametersQuery::customParametersQuery(QWidget *parent,
                                             QMap<QString, QString> parameters)
   : QDialog(parent),
     m_Parameters(parameters),
     m_paramCount(0),
     ui(new Ui::customParametersQuery){
     ui->setupUi(this);
     ui->parameter0->setText(m_Parameters["#PARAMETER1"]);
     ui->parameter1->setText(m_Parameters["#PARAMETER2"]);
     ui->parameter2->setText(m_Parameters["#PARAMETER3"]);
     ui->parameter3->setText(m_Parameters["#PARAMETER4"]);
     ui->parameter4->setText(m_Parameters["#PARAMETER5"]);
}

customParametersQuery::~customParametersQuery(){
   delete ui;
}

void customParametersQuery::on_buttonBox_accepted(){
   if(!ui->parameter0->text().isEmpty()){
      m_Parameters["#PARAMETER1"] = ui->parameter0->text();
      m_paramCount++;
   }
   if(!ui->parameter1->text().isEmpty()){
      m_Parameters["#PARAMETER2"] = ui->parameter1->text();
      m_paramCount++;
   }
   if(!ui->parameter2->text().isEmpty()){
      m_Parameters["#PARAMETER3"] = ui->parameter2->text();
      m_paramCount++;
   }
   if(!ui->parameter3->text().isEmpty()){
      m_Parameters["#PARAMETER4"] = ui->parameter3->text();
      m_paramCount++;
   }
   if(!ui->parameter4->text().isEmpty()){
      m_Parameters["#PARAMETER5"] = ui->parameter4->text();
      m_paramCount++;
   }
}
