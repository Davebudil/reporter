#include "customscheduling.h"
#include "ui_customscheduling.h"
#include "log.h"

CustomScheduling::CustomScheduling(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::CustomScheduling){
   ui->setupUi(this);
   ui->dateFrom->setDateTime(QDateTime::currentDateTime());
   ui->dateTo->setDateTime(QDateTime::currentDateTime());
}

CustomScheduling::~CustomScheduling(){
   delete ui;
}

void CustomScheduling::on_toolButton_clicked(){
   ui->ShiftXLStemplate->setText(QFileDialog::getOpenFileName(this, tr("Select XLS template")));
}
void CustomScheduling::on_toolButton_3_clicked(){
   ui->dailyXLSTemplate->setText(QFileDialog::getOpenFileName(this, tr("Select XLS template")));
}
void CustomScheduling::on_toolButton_4_clicked(){
   ui->weeklyXLStemplate->setText(QFileDialog::getOpenFileName(this, tr("Select XLS template")));
}
void CustomScheduling::on_toolButton_5_clicked(){
   ui->monthlyXLSTemplate->setText(QFileDialog::getOpenFileName(this, tr("Select XLS template")));
}
void CustomScheduling::on_buttonBox_accepted(){
   m_Shift = ui->shiftCHeck->isChecked();
   m_Daily = ui->dailyCheck->isChecked();
   m_Weekly = ui->weeklyCheck->isChecked();
   m_Monthly = ui->monthlyCheck->isChecked();
   m_Email = ui->email->text();
   m_useParameters = ui->parametersCheck->isChecked();
   m_From = ui->dateFrom->dateTime();
   m_To = ui->dateTo->dateTime();

   if(m_Shift){
      m_loadShift();
   }
   if(m_Daily){
      m_loadDaily();
   }
   if(m_Weekly){
      m_loadWeekly();
   }
   if(m_Monthly){
      m_loadMonthly();
   }
   if(m_useParameters){
      parameter0 = ui->lineEdit->text();
      parameter1 = ui->lineEdit_2->text();
      parameter2 = ui->lineEdit_3->text();
      parameter3 = ui->lineEdit_4->text();
      parameter4 = ui->lineEdit_5->text();
   }
}

void CustomScheduling::m_loadShift(){
   m_shiftDays[0] = ui->checkBox->isChecked();
   m_shiftDays[1] = ui->checkBox_2->isChecked();
   m_shiftDays[2] = ui->checkBox_3->isChecked();
   m_shiftDays[3] = ui->checkBox_4->isChecked();
   m_shiftDays[4] = ui->checkBox_5->isChecked();
   m_shiftDays[5] = ui->checkBox_11->isChecked();
   m_shiftDays[6] = ui->checkBox_12->isChecked();
   m_ShiftxlsTemplatePath = ui->ShiftXLStemplate->text();
   m_ShiftAttachName = ui->shiftAttach->text();
   m_ShiftSubjectName = ui->shiftSubj->text();
   m_Shift1 = ui->shiftTime1->time();
   m_Shift2 = ui->shiftTime2->time();
   m_Shift3 = ui->shiftTim3->time();
   m_shiftXLS = ui->shiftAttachXLS->isChecked();
   m_shiftCSV = ui->shiftAttachCSV->isChecked();
}

void CustomScheduling::m_loadDaily(){
   m_dailyDays[0] = ui->dailyMon->isChecked();
   m_dailyDays[1] = ui->dailyTue->isChecked();
   m_dailyDays[2] = ui->dailyWed->isChecked();
   m_dailyDays[3] = ui->dailyThu->isChecked();
   m_dailyDays[4] = ui->dailyFri->isChecked();
   m_dailyDays[5] = ui->dailySat->isChecked();
   m_dailyDays[6] = ui->dailySun->isChecked();
   m_dailyXLS = ui->dailyAttachXLS->isChecked();
   m_dailyCSV = ui->dailyAttachCSV->isChecked();
   m_DailyxlsTemplatePath = ui->dailyXLSTemplate->text();
   m_DailyAttachName = ui->dailyAttach->text();
   m_DailySubjectName = ui->dailySubj->text();
   m_Daily1 = ui->dailyTime1->time();
}

void CustomScheduling::m_loadWeekly(){
   m_weeklyXLS = ui->weeklyAttachXLS->isChecked();
   m_weeklyCSV = ui->weeklyAttachCSV->isChecked();
   m_weeklyDay = ui->weeklyDay->currentIndex();
   m_WeeklyxlsTemplatePath = ui->weeklyXLStemplate->text();
   m_WeeklyAttachName = ui->weeklyAttach->text();
   m_WeeklySubjectName = ui->weeklySubj->text();
   m_Weekly1 = ui->weeklyTime1->time();
}

void CustomScheduling::m_loadMonthly(){
   m_monthlyXLS = ui->monthlyXLS->isChecked();
   m_monthlyCSV = ui->monthlyCSV->isChecked();
   m_monthlyDay = ui->monthlyDays->currentIndex();
   m_MonthlyxlsTemplatePath = ui->monthlyXLSTemplate->text();
   m_MonthlyAttachName = ui->monthlyAttach->text();
   m_MonthlySubjectName = ui->monthlySubject->text();
   m_Monthly1 = ui->monthlyTime1->time();
}

//validates the input email
void CustomScheduling::on_email_textEdited(const QString &arg1){
   if(!arg1.isNull() || arg1.isEmpty()){
      if(m_validateEmail(arg1)){
         ui->label_18->setStyleSheet("color: green");
      }else{
         ui->label_18->setStyleSheet("color: red");
      }
   }
}

bool CustomScheduling::m_validateEmail(const QString & email){
   QRegularExpression regex("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
   return regex.match(email).hasMatch();
}
