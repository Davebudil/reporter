#include "dailyschedule.h"
#include <log.h>

DailySchedule::DailySchedule()
              :m_Active(false),
               m_csvAttach(false),
               m_xlsAttach(false),
               m_Done(false),
               m_lastDoneDay(0),
               m_AttachName(" "),
               m_SubjName(" "),
               m_xlsTemplatePath(" "),
               m_csvTemplatePath(" "),
               m_emailTemplatePath(" "){
   QVector<bool> falseIni = {false,false,false,false,false,false,false};
   m_Days = falseIni;
}
DailySchedule::~DailySchedule(){
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
QMap<QString, QString> & DailySchedule::getEmailAdresses(){
   return m_emailAdresses;
}

QStringList DailySchedule::emailAdresses(){
   QStringList tmp;
   for(auto & it : m_emailAdresses){
      tmp.append(it);
   }

   return tmp;
}
QTime DailySchedule::getTime() const{
    return m_Time;
}
bool DailySchedule::getActive() const{
    return m_Active;
}
QVector<bool> DailySchedule::getDays() const{
   return m_Days;
}
QStringList DailySchedule::prepareSerialization(){
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
   valueList.append(QString::number(m_Done));
   valueList.append(QString::number(m_lastDoneDay));
   for(qint32 i = 0; i < 7; ++i){
      valueList.append(QString::number(m_Days[i]));
   }
   valueList.append(QString::number((m_emailAdresses.count())));
   for(auto & it : m_emailAdresses){
      valueList.append(it);
   }
   return valueList;
}

void DailySchedule::deserializeList(const QStringList & list){
   m_Active = (list.at(0) == "0" ? false : true);
   m_csvAttach = (list.at(1) == "0" ? false : true);
   m_xlsAttach = (list.at(2) == "0" ? false : true);
   m_AttachName = list.at(3);
   m_SubjName = list.at(4);
   m_xlsTemplatePath = list.at(5);
   m_csvTemplatePath = list.at(6);
   m_emailTemplatePath = list.at(7);
   m_Time = QTime::fromString(list.at(8));
   m_Done = (list.at(9) == "0" ? false : true);
   m_lastDoneDay = QVariant(list.at(10)).toInt();
   for(qint32 i = 0; i < 7; ++i){
      m_Days[i] = (list.at(11+i) == "0" ? false : true);
   }
   QStringList::const_iterator constIterator;
   for(constIterator = list.constBegin() + 18; constIterator != list.constEnd(); ++constIterator){
      m_emailAdresses.insert(*constIterator, *constIterator);
   }
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

bool DailySchedule::generateDailyData(const QDateTime & currentData){
   if(m_Days[currentData.date().dayOfWeek() - 1]){
      return m_checkDoneInterval(currentData);
   }
   return false;
}

void DailySchedule::fixParameters(QSharedPointer<SQLParameter> & param, QDateTime & currentTime){
   QStringList tmp = param->getParameters();
   for(qint32 i = 0; i < tmp.count(); ++i){
      m_AttachName.replace(QString("#PARAMETER" + QVariant(i+1).toString()), tmp[i]);
      m_SubjName.replace(QString("#PARAMETER" + QVariant(i+1).toString()), tmp[i]);
      m_emailTemplatePath.replace(QString("#PARAMETER" + QVariant(i+1).toString()), tmp[i]);
      m_xlsTemplatePath.replace(QString("#PARAMETER" + QVariant(i+1).toString()), tmp[i]);
   }

   if(m_AttachName.contains("#Date", Qt::CaseInsensitive)){
      QString tmpDate;
      QString resultDate;
      qint32 first, last;
      first = m_AttachName.indexOf("#Date(") + 6;
      last = m_AttachName.lastIndexOf(")");
      tmpDate = m_AttachName.mid(first, last - first);
      resultDate = currentTime.toString(tmpDate);
      tmpDate = "#Date(" + tmpDate + ")";
      m_AttachName.replace(tmpDate, resultDate);
   }

   if(m_SubjName.contains("#Date", Qt::CaseInsensitive)){
      QString tmpDate;
      QString resultDate;
      qint32 first, last;
      first = m_SubjName.indexOf("#Date(") + 6;
      last = m_SubjName.lastIndexOf(")");
      tmpDate = m_SubjName.mid(first, last - first);
      resultDate = currentTime.toString(tmpDate);
      tmpDate = "#Date(" + tmpDate + ")";
      m_SubjName.replace(tmpDate, resultDate);
   }
}

bool DailySchedule::m_checkDoneInterval(const QDateTime & currentDate){
   if(m_Done){
      if(m_lastDoneDay != currentDate.date().dayOfWeek()){
         //time rolled over to another day
         m_Done = false;
      }
   }
   if(currentDate.time() > m_Time && !m_Done){
      m_lastDoneDay = currentDate.date().dayOfWeek();
      m_Done = true;
      return true;
   }
   return false;
}

void DailySchedule::m_dailyInterval(){
   //m_Time -> m_Time interval(last day)
}

bool DailySchedule::getDone() const{
    return m_Done;
}

void DailySchedule::setDone(bool Done){
    m_Done = Done;
}
QString DailySchedule::getEmailTemplatePath() const{
    return m_emailTemplatePath;
}
void DailySchedule::setEmailTemplatePath(const QString & emailTemplatePath){
   m_emailTemplatePath = emailTemplatePath;
}
