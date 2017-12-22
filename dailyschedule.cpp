#include "dailyschedule.h"

DailySchedule::DailySchedule()
              :m_Active(false),
               m_csvAttach(false),
               m_xlsAttach(false),
               m_AttachName("default"),
               m_SubjName("default"),
               m_xlsTemplatePath("default"),
               m_csvTemplatePath("default"),
               m_emailTemplatePath("default"){
   m_Days = new bool[7];
   std::fill_n(m_Days, 7, false);
}
DailySchedule::~DailySchedule(){
   delete m_Days;
}

QString DailySchedule::getCsvTemplatePath() const{
    return m_csvTemplatePath;
}
QString DailySchedule::getXlsTemplatePath() const{
    return m_xlsTemplatePath;
}
QString DailySchedule::getSubjName() const{
    return m_SubjName;
}
QString DailySchedule::getAttachName() const{
    return m_AttachName;
}
QTime DailySchedule::getTime() const{
    return m_Time;
}
bool DailySchedule::getActive() const{
    return m_Active;
}
bool * DailySchedule::getDays() const{
   return m_Days;
}
void DailySchedule::setCsvTemplatePath(const QString & csvTemplatePath){
    m_csvTemplatePath = csvTemplatePath;
}
void DailySchedule::setXlsTemplatePath(const QString & xlsTemplatePath){
    m_xlsTemplatePath = xlsTemplatePath;
}
void DailySchedule::setSubjName(const QString & SubjName){
    m_SubjName = SubjName;
}
void DailySchedule::setAttachName(const QString & AttachName){
    m_AttachName = AttachName;
}
void DailySchedule::setTime(const QTime & Time){
    m_Time = Time;
}
void DailySchedule::setActive(bool Active){
    m_Active = Active;
}
void DailySchedule::setDays(const qint32 day, bool value){
   m_Days[day] = value;
}
bool DailySchedule::getXlsAttach() const{
    return m_xlsAttach;
}
void DailySchedule::setXlsAttach(bool xlsAttach){
    m_xlsAttach = xlsAttach;
}
bool DailySchedule::getCsvAttach() const{
    return m_csvAttach;
}
void DailySchedule::setCsvAttach(bool csvAttach){
    m_csvAttach = csvAttach;
}
QString DailySchedule::getEmailTemplatePath() const{
   return m_emailTemplatePath;
}
void DailySchedule::setEmailTemplatePath(const QString & emailTemplatePath){
   m_emailTemplatePath = emailTemplatePath;
}
