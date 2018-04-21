#include "shiftschedule.h"
#include <log.h>

ShiftSchedule::ShiftSchedule()
   :m_Active(false),
     m_csvAttach(false),
     m_xlsAttach(false),
     m_Done0(false),
     m_Done1(false),
     m_Done2(false),
     m_AttachName(" "),
     m_SubjName(" "),
     m_emailTemplatePath(" "),
     m_xlsTemplatePath(" "),
     m_csvTemplatePath(" "){
   QVector<bool> falseIni = {false,false,false,false,false,false,false};
   m_Days = falseIni;
}
ShiftSchedule::~ShiftSchedule(){
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
QVector<bool> ShiftSchedule::getDays() const{
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
   valueList.append(QString::number(m_Done0));
   valueList.append(QString::number(m_Done1));
   valueList.append(QString::number(m_Done2));
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
   m_Done0 = (list.at(12) == "0" ? false : true);
   m_Done1 = (list.at(13) == "0" ? false : true);
   m_Done2 = (list.at(14) == "0" ? false : true);
   for(qint32 i = 0; i < 7; ++i){
      m_Days[i] = (list.at(15+i) == "0" ? false : true);
   }
   QStringList::const_iterator constIterator;
   for(constIterator = list.constBegin() + 22; constIterator != list.constEnd(); ++constIterator){
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

bool ShiftSchedule::generateShiftData(const QDateTime & currentTime){
   if(m_Days[currentTime.date().dayOfWeek() - 1]){
      return m_checkDoneInterval(currentTime);
   }
   return false;
}

bool ShiftSchedule::m_checkDoneInterval(const QDateTime & currentTime){
   if((currentTime > m_Date1) && (currentTime < m_Date2) && !m_Done2){
      m_Done0 = false;
      m_Done1 = false;
      m_Done2 = true;
      return true;
      //first interval
   }
   if((currentTime > m_Date2) && (currentTime < m_Date3) && !m_Done0){
      m_Done0 = true;
      m_Done1 = false;
      m_Done2 = false;
      return true;
      //second interval
   }
   if((currentTime > m_Date3) && (currentTime <= m_Date0) && !m_Done1){
      m_Done0 = false;
      m_Done1 = true;
      m_Done2 = false;
      return true;
      //third interval
   }
   return false;
}

void ShiftSchedule::m_timeInterval0(){
   // m_time2 -> m_time0 interval
}
void ShiftSchedule::m_timeInterval1(){
   // m_time0 -> m_time1 interval
}
void ShiftSchedule::m_timeInterval2(){
   // m_time1 -> m_time2 interval
}

QDateTime ShiftSchedule::getDate4() const
{
   return m_Date4;
}

void ShiftSchedule::setDate4(const QDateTime & Date4)
{
   m_Date4 = Date4;
}

QDateTime ShiftSchedule::getDate3() const{
   return m_Date3;
}

void ShiftSchedule::setDate3(const QDateTime & Date3){
   m_Date3 = Date3;
}

QDateTime ShiftSchedule::getDate0() const{
   return m_Date0;
}

void ShiftSchedule::setDate0(const QDateTime & Date0){
   m_Date0 = Date0;
}

void ShiftSchedule::fixParameters(SQLParameter & param, QDateTime & currentTime){
   QStringList tmp = param.getParameters();
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
QDateTime ShiftSchedule::getDate1() const{
   return m_Date1;
}

void ShiftSchedule::setDate1(const QDateTime & Date1){
   m_Date1 = Date1;
}

QDateTime ShiftSchedule::getDate2() const{
   return m_Date2;
}

void ShiftSchedule::setDate2(const QDateTime & Date2){
   m_Date2 = Date2;
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

bool ShiftSchedule::getDone2() const{
   return m_Done2;
}

void ShiftSchedule::setDone2(bool Done2){
   m_Done2 = Done2;
}

QTime ShiftSchedule::getTimeTMP() const{
   return m_timeTMP;
}

void ShiftSchedule::setTimeTMP(const QTime & timeTMP){
   m_timeTMP = timeTMP;
}

QMap<QString, QString> & ShiftSchedule::getEmailAdresses(){
   return m_emailAdresses;
}

QStringList ShiftSchedule::emailAdresses(){
   QStringList tmp;
   for(auto & it : m_emailAdresses){
      tmp.append(it);
   }
   return tmp;
}
