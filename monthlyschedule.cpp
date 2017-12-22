#include "monthlyschedule.h"

MonthlySchedule::MonthlySchedule()
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
MonthlySchedule::~MonthlySchedule(){

}

QString MonthlySchedule::getCsvTemplatePath() const{
    return m_csvTemplatePath;
}
QString MonthlySchedule::getXlsTemplatePath() const{
    return m_xlsTemplatePath;
}
QString MonthlySchedule::getSubjName() const{
    return m_SubjName;
}
QString MonthlySchedule::getAttachName() const{
    return m_AttachName;
}
QTime MonthlySchedule::getTime() const{
    return m_Time;
}
qint32 MonthlySchedule::getDay() const{
    return m_Day;
}
bool MonthlySchedule::getActive() const{
    return m_Active;
}
QStringList MonthlySchedule::prepareSerialization(){
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
   valueList.append(QString::number(m_Day));
   return valueList;
}
void MonthlySchedule::setCsvTemplatePath(const QString & csvTemplatePath){
    m_csvTemplatePath = csvTemplatePath;
}
void MonthlySchedule::setXlsTemplatePath(const QString & xlsTemplatePath){
    m_xlsTemplatePath = xlsTemplatePath;
}
void MonthlySchedule::setSubjName(const QString & SubjName){
    m_SubjName = SubjName;
}
void MonthlySchedule::setAttachName(const QString & AttachName){
    m_AttachName = AttachName;
}
void MonthlySchedule::setTime(const QTime & Time){
    m_Time = Time;
}
void MonthlySchedule::setDay(const qint32 & Day){
    m_Day = Day;
}
void MonthlySchedule::setActive(bool Active){
    m_Active = Active;
}
bool MonthlySchedule::getXlsAttach() const{
    return m_xlsAttach;
}
void MonthlySchedule::setXlsAttach(bool xlsAttach){
    m_xlsAttach = xlsAttach;
}
bool MonthlySchedule::getCsvAttach() const{
    return m_csvAttach;
}
void MonthlySchedule::setCsvAttach(bool csvAttach){
    m_csvAttach = csvAttach;
}
QString MonthlySchedule::getEmailTemplatePath() const{
   return m_emailTemplatePath;
}
void MonthlySchedule::setEmailTemplatePath(const QString & emailTemplatePath){
   m_emailTemplatePath = emailTemplatePath;
}