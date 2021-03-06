#include "monthlyschedule.h"
#include "log.h"

MonthlySchedule::MonthlySchedule()
                :m_Active(false),
                 m_csvAttach(false),
                 m_xlsAttach(false),
                 m_Done(false),
                 m_lastMonth(QDateTime::currentDateTime()),
                 m_Day(0),
                 m_AttachName(" "),
                 m_SubjName(" "),
                 m_xlsTemplatePath(" "),
                 m_csvTemplatePath(" "),
                 m_emailTemplatePath(" "){
}
MonthlySchedule::~MonthlySchedule() = default;

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
   valueList.append(QString::number(m_Done));
   valueList.append(m_lastMonth.toString());
   valueList.append(QString::number((m_emailAdresses.count())));
   for(auto & it : m_emailAdresses){
      valueList.append(it);
   }
   return valueList;
}
void MonthlySchedule::deserializeList(const QStringList & list){
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
   m_lastMonth = QDateTime::fromString(list.at(11));
   QStringList::const_iterator constIterator;
   for(constIterator = list.constBegin() + 12; constIterator != list.constEnd(); ++constIterator){
      m_emailAdresses.insert(*constIterator, *constIterator);
   }
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
QMap<QString, QString> & MonthlySchedule::getEmailAdresses(){
   return m_emailAdresses;
}

QStringList MonthlySchedule::emailAdresses(){
   QStringList tmp;
   for(auto & it : m_emailAdresses){
      tmp.append(it);
   }

   return tmp;
}
void MonthlySchedule::setEmailTemplatePath(const QString & emailTemplatePath){
   m_emailTemplatePath = emailTemplatePath;
}

bool MonthlySchedule::generateMonthlyData(const QDateTime & currentData){
   return m_checkDoneInterval(currentData);
}

void MonthlySchedule::fixParameters(QSharedPointer<SQLParameter> & param, QDateTime & currentTime){
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

bool MonthlySchedule::m_checkDoneInterval(const QDateTime & currentDate){
//   qInfo(logInfo()) << "TestMonthly " + QString::number(currentDate.date().day()) + " " + QString::number(m_Day) + " " + m_Done;
   if(currentDate.date().day() < m_Day + 1 && m_Done
      && m_lastMonth.date().month() != currentDate.date().month()){
      m_Done = false;
      //reset interval
   }else if((currentDate.date().day() == currentDate.date().daysInMonth()) && m_Day == 0 && m_Done
            && m_lastMonth.date().month() != currentDate.date().month()){
      m_Done = false;
   }
   if((currentDate.time() > m_Time) && (currentDate.date().day() >= m_Day + 1) && !m_Done){
      m_Done = true;
      m_lastMonth = currentDate;
      m_monthlyInterval();
      //right interval
      return true;
   }
   return false;
}

void MonthlySchedule::m_monthlyInterval(){
   //Interval from last month m_Day m_Time -> to todays m_Day and m_Time
}

bool MonthlySchedule::getDone() const{
   return m_Done;
}
void MonthlySchedule::setDone(bool Done){
   m_Done = Done;
}
