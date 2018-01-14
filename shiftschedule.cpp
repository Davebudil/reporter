#include "shiftschedule.h"

ShiftSchedule::ShiftSchedule()
              :m_Active(false),
               m_csvAttach(false),
               m_xlsAttach(false),
               m_AttachName("default"),
               m_SubjName("default"),
               m_emailTemplatePath("default"),
               m_xlsTemplatePath("default"),
               m_csvTemplatePath("default"){
   m_Days = new bool[7];
   std::fill_n(m_Days, 7, false);
}
ShiftSchedule::~ShiftSchedule(){
   delete[] m_Days;
}
QString ShiftSchedule::getAttachName() const{
   return m_AttachName;
}
QString ShiftSchedule::getXlsTemplatePath() const{
   return m_xlsTemplatePath;
}
QString ShiftSchedule::getCsvTemplatePath() const{
   return m_csvTemplatePath;
}
bool ShiftSchedule::getActive() const{
   return m_Active;
}
bool * ShiftSchedule::getDays() const{
   return m_Days;
}
void ShiftSchedule::setAttachName(const QString & AttachName){
   m_AttachName = AttachName;
}
void ShiftSchedule::setActive(bool Active){
   m_Active = Active;
}
void ShiftSchedule::setXlsTemplatePath(const QString & xlsTemplatePath){
   m_xlsTemplatePath = xlsTemplatePath;
}
void ShiftSchedule::setCsvTemplatePath(const QString & csvTemplatePath){
   m_csvTemplatePath = csvTemplatePath;
}
void ShiftSchedule::setDays(const qint32 day, bool value){
   m_Days[day] = value;
}
bool ShiftSchedule::getXlsAttach() const{
   return m_xlsAttach;
}

QStringList ShiftSchedule::prepareSerialization(){
   QStringList valueList;
   valueList.append(QString::number(m_Active));
   valueList.append(QString::number(m_csvAttach));
   valueList.append(QString::number(m_xlsAttach));
   valueList.append(m_AttachName);
   valueList.append(m_SubjName);
   valueList.append(m_xlsTemplatePath);
   valueList.append(m_csvTemplatePath);
   valueList.append(m_emailTemplatePath);
   valueList.append(QString(m_From0.toString()));
   valueList.append(QString(m_To0.toString()));
   valueList.append(QString(m_From1.toString()));
   valueList.append(QString(m_To1.toString()));
   for(qint32 i = 0; i < 7; ++i){
      valueList.append(QString::number(m_Days[i]));
   }
   for(auto & it : m_emailAdresses){
      valueList.append(it);
   }
   return valueList;
}
void ShiftSchedule::deserializeList(const QStringList & list){
   m_Active = (list.at(0) == "0" ? false : true);
   m_csvAttach = (list.at(1) == "0" ? false : true);
   m_xlsAttach = (list.at(2) == "0" ? false : true);
   m_AttachName = list.at(3);
   m_SubjName = list.at(4);
   m_xlsTemplatePath = list.at(5);
   m_csvTemplatePath = list.at(6);
   m_emailTemplatePath = list.at(7);
   m_From0 = QTime::fromString(list.at(8));
   m_To0 = QTime::fromString(list.at(9));
   m_From1 = QTime::fromString(list.at(10));
   m_To1 = QTime::fromString(list.at(11));
   for(qint32 i = 0; i < 7; ++i){
      m_Days[i] = (list.at(12+i) == "0" ? false : true);
   }
   QStringList::const_iterator constIterator;
   for(constIterator = list.constBegin() + 19; constIterator != list.constEnd(); ++constIterator){
      m_emailAdresses.insert(*constIterator, *constIterator);
   }
}
void ShiftSchedule::setXlsAttach(bool xlsAttach){
    m_xlsAttach = xlsAttach;
}
bool ShiftSchedule::getCsvAttach() const{
    return m_csvAttach;
}
void ShiftSchedule::setCsvAttach(bool csvAttach){
    m_csvAttach = csvAttach;
}
QString ShiftSchedule::getSubjName() const{
   return m_SubjName;
}
void ShiftSchedule::setSubjName(const QString & SubjName){
   m_SubjName = SubjName;
}
QTime ShiftSchedule::getFrom0() const{
   return m_From0;
}
void ShiftSchedule::setFrom0(const QTime & From0){
   m_From0 = From0;
}
QTime ShiftSchedule::getTo0() const{
   return m_To0;
}
void ShiftSchedule::setTo0(const QTime & To0){
   m_To0 = To0;
}
QTime ShiftSchedule::getFrom1() const{
   return m_From1;
}
void ShiftSchedule::setFrom1(const QTime & From1){
   m_From1 = From1;
}
QTime ShiftSchedule::getTo1() const{
   return m_To1;
}
void ShiftSchedule::setTo1(const QTime & To1){
   m_To1 = To1;
}
QString ShiftSchedule::getEmailTemplatePath() const{
   return m_emailTemplatePath;
}
void ShiftSchedule::setEmailTemplatePath(const QString & emailTemplatePath){
   m_emailTemplatePath = emailTemplatePath;
}

QMap<QString, QString> & ShiftSchedule::getEmailAdresses(){
    return m_emailAdresses;
}
