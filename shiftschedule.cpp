#include "shiftschedule.h"
#include <log.h>

ShiftSchedule::ShiftSchedule()
              :m_Active(false),
               m_csvAttach(false),
               m_xlsAttach(false),
               m_Done0(false),
               m_Done1(false),
               m_Done2(false){
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
   valueList.append(QString(m_time0.toString()));
   valueList.append(QString(m_time1.toString()));
   valueList.append(QString(m_time2.toString()));
   valueList.append(QString(m_time0.toString()));
   for(qint32 i = 0; i < 7; ++i){
      valueList.append(QString::number(m_Days[i]));
   }
   valueList.append(QString::number((m_emailAdresses.count())));
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
   m_time0 = QTime::fromString(list.at(8));
   m_time1 = QTime::fromString(list.at(9));
   m_time2 = QTime::fromString(list.at(10));
   m_timeTMP = QTime::fromString(list.at(11));
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
   return m_time0;
}
void ShiftSchedule::setFrom0(const QTime & From0){
   m_time0 = From0;
}
QTime ShiftSchedule::getTo0() const{
   return m_time1;
}
void ShiftSchedule::setTo0(const QTime & To0){
   m_time1 = To0;
}
QTime ShiftSchedule::getFrom1() const{
   return m_time2;
}
void ShiftSchedule::setFrom1(const QTime & From1){
   m_time2 = From1;
}
QString ShiftSchedule::getEmailTemplatePath() const{
   return m_emailTemplatePath;
}
void ShiftSchedule::setEmailTemplatePath(const QString & emailTemplatePath){
   m_emailTemplatePath = emailTemplatePath;
}

void ShiftSchedule::generateShiftData(const QDateTime & currentTime){
   if(m_Days[currentTime.date().dayOfWeek() - 1]){
      m_checkDoneInterval(currentTime.time());
   }
}

void ShiftSchedule::m_checkDoneInterval(const QTime & currentTime){
   if((currentTime > m_time0) && (currentTime < m_time1) && !m_Done2){
      m_Done0 = false;
      m_Done1 = false;
      m_Done2 = true;
      m_completed = false;
      //first interval
   }else if((currentTime > m_time1) && (currentTime < m_time2) && !m_Done0){
      m_Done0 = true;
      m_Done1 = false;
      m_Done2 = false;
      m_completed = false;
      //second interval
   }else if((currentTime > m_time2) && (currentTime < m_time0) && !m_Done1){
      m_Done0 = false;
      m_Done1 = true;
      m_Done2 = false;
      m_completed = false;
      //third interval
   }else{
      m_completed = true;
   }
}

void ShiftSchedule::m_timeInterval0(){
   // m_time2 -> m_time0 interval
   m_completed = true;
}
void ShiftSchedule::m_timeInterval1(){
   // m_time0 -> m_time1 interval
   m_completed = true;
}
void ShiftSchedule::m_timeInterval2(){
   // m_time1 -> m_time2 interval
   m_completed = true;
}

bool ShiftSchedule::getCompleted() const{
   return m_completed;
}

void ShiftSchedule::setCompleted(bool completed){
   m_completed = completed;
}

bool ShiftSchedule::getDone0() const{
   return m_Done0;
}

void ShiftSchedule::setDone0(bool Done0){
   m_Done0 = Done0;
}

bool ShiftSchedule::getDone1() const{
   return m_Done1;
}

void ShiftSchedule::setDone1(bool Done1){
   m_Done1 = Done1;
}

bool ShiftSchedule::getDone2() const
{
   return m_Done2;
}

void ShiftSchedule::setDone2(bool Done2)
{
   m_Done2 = Done2;
}

QTime ShiftSchedule::getTimeTMP() const
{
   return m_timeTMP;
}

void ShiftSchedule::setTimeTMP(const QTime & timeTMP)
{
   m_timeTMP = timeTMP;
}

QMap<QString, QString> & ShiftSchedule::getEmailAdresses(){
   return m_emailAdresses;
}
