#include "weeklyschedule.h"
#include "log.h"

WeeklySchedule::WeeklySchedule()
               :m_Active(false),
                m_csvAttach(false),
                m_xlsAttach(false),
                m_Day(0){
}
WeeklySchedule::~WeeklySchedule() = default;
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
   valueList.append(QString::number(m_Day));
   valueList.append(QString::number((m_emailAdresses.count())));
   valueList.append(QString::number(m_Done));
   for(auto & it : m_emailAdresses){
      valueList.append(it);
   }
   return valueList;
}

void WeeklySchedule::deserializeList(const QStringList & list){
   m_Active = (list.at(0) == "0" ? false : true);
   m_csvAttach = (list.at(1) == "0" ? false : true);
   m_xlsAttach = (list.at(2) == "0" ? false : true);
   m_AttachName = list.at(3);
   m_SubjName = list.at(4);
   m_xlsTemplatePath = list.at(5);
   m_csvTemplatePath = list.at(6);
   m_emailTemplatePath = list.at(7);
   m_Time = QTime::fromString(list.at(8));
   m_Day = QString(list.at(9)).toInt();
   m_Done = (list.at(10) == "0" ? false : true);
   QStringList::const_iterator constIterator;
   for(constIterator = list.constBegin() + 11; constIterator != list.constEnd(); ++constIterator){
      m_emailAdresses.insert(*constIterator, *constIterator);
   }
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

QMap<QString, QString> &WeeklySchedule::getEmailAdresses(){
   return m_emailAdresses;
}

QStringList WeeklySchedule::emailAdresses(){
   QStringList tmp;
   for(auto & it : m_emailAdresses){
      tmp.append(it);
   }

   return tmp;
}
void WeeklySchedule::setEmailTemplatePath(const QString & emailTemplatePath){
   m_emailTemplatePath = emailTemplatePath;
}

bool WeeklySchedule::generateWeeklyData(const QDateTime & currenDate){
   return m_checkDoneInterval(currenDate);
}

bool WeeklySchedule::m_checkDoneInterval(const QDateTime & currentDate){
   if((currentDate.date().dayOfWeek() < m_Day + 1) && m_Done){
      m_Done = false;
   }else if((currentDate.date().dayOfWeek() == 7) && (m_Day == 0) && m_Done){
      m_Done = false;
   }
   if(((currentDate.time() > m_Time) && (currentDate.date().dayOfWeek() >= m_Day + 1) && !m_Done)
      && currentDate.date().dayOfWeek() != 7){
      m_Done = true;
      m_weeklyInterval();
      //right interval
      return true;
   }
   return false;
}

void WeeklySchedule::m_weeklyInterval(){
   //interval from m_Day and m_Time of last week -> m_Day m_Time of today.
}

bool WeeklySchedule::getDone() const{
    return m_Done;
}

void WeeklySchedule::setDone(bool Done){
    m_Done = Done;
}
