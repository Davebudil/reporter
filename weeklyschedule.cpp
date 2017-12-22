#include "weeklyschedule.h"

WeeklySchedule::WeeklySchedule()
               :m_Active(false),
                m_csvAttach(false),
                m_xlsAttach(false),
                m_Day(0),
                m_AttachName("default"),
                m_SubjName("default"),
                m_xlsTemplatePath("default"),
                m_csvTemplatePath("default"),
                m_emailTemplatePath("default"){
}
WeeklySchedule::~WeeklySchedule(){

}
QString WeeklySchedule::getAttachName() const{
   return m_AttachName;
}
QString WeeklySchedule::getSubjName() const{
   return m_SubjName;
}
QString WeeklySchedule::getXlsTemplatePath() const{
   return m_xlsTemplatePath;
}
QString WeeklySchedule::getCsvTemplatePath() const{
   return m_csvTemplatePath;
}
QTime WeeklySchedule::getTime() const{
   return m_Time;
}
qint32 WeeklySchedule::getDay() const{
   return m_Day;
}
bool WeeklySchedule::getActive() const{
   return m_Active;
}
void WeeklySchedule::setAttachName(const QString & AttachName){
   m_AttachName = AttachName;
}
void WeeklySchedule::setSubjName(const QString & SubjName){
   m_SubjName = SubjName;
}
void WeeklySchedule::setXlsTemplatePath(const QString & xlsTemplatePath){
   m_xlsTemplatePath = xlsTemplatePath;
}
void WeeklySchedule::setCsvTemplatePath(const QString & csvTemplatePath){
   m_csvTemplatePath = csvTemplatePath;
}
void WeeklySchedule::setTime(const QTime & Time){
   m_Time = Time;
}
void WeeklySchedule::setDay(const qint32 & Day){
   m_Day = Day;
}
void WeeklySchedule::setActive(bool Active){
   m_Active = Active;
}
bool WeeklySchedule::getXlsAttach() const{
   return m_xlsAttach;
}
QStringList WeeklySchedule::prepareSerialization(){
   QStringList valueList;
   valueList.append(QString::number(m_Active));
   valueList.append(QString::number(m_csvAttach));
   valueList.append(QString::number(m_xlsAttach));
   valueList.append(m_AttachName);
   valueList.append(m_SubjName);
   valueList.append(m_xlsTemplatePath);
   valueList.append(m_csvTemplatePath);
   valueList.append(m_emailTemplatePath);
   valueList.append(QString(m_Time.toString()));
   return valueList;
}
void WeeklySchedule::setXlsAttach(bool xlsAttach){
    m_xlsAttach = xlsAttach;
}
bool WeeklySchedule::getCsvAttach() const{
    return m_csvAttach;
}
void WeeklySchedule::setCsvAttach(bool csvAttach){
    m_csvAttach = csvAttach;
}
QString WeeklySchedule::getEmailTemplatePath() const{
   return m_emailTemplatePath;
}
void WeeklySchedule::setEmailTemplatePath(const QString & emailTemplatePath){
   m_emailTemplatePath = emailTemplatePath;
}
