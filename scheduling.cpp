#include "scheduling.h"

Scheduling::Scheduling() : m_paramCount(0){

}
Scheduling::~Scheduling(){
   qDeleteAll(m_Parameters);
   m_Parameters.clear();
}
ShiftSchedule & Scheduling::getShift(){
   return m_Shift;
}
DailySchedule & Scheduling::getDaily(){
   return m_Daily;
}
WeeklySchedule & Scheduling::getWeekly(){
   return m_Weekly;
}
MonthlySchedule & Scheduling::getMonthly(){
   return m_Monthly;
}
CustomSchedule & Scheduling::getCustom(){
   return m_Custom;
}
QString Scheduling::getName() const{
    return m_Name;
}
void Scheduling::setName(const QString & Name){
   m_Name = Name;
}
QMap<qint32, SQLParameter *> Scheduling::getParameters(){
   return m_Parameters;
}

QQueue<SQLParameter> Scheduling::getQueueParameters(){
   QQueue<SQLParameter> tmpResultQ;
   for(auto & it : m_Parameters){
      tmpResultQ.append(*it);
   }
   return tmpResultQ;
}

bool Scheduling::addParam(const QStringList & param, const qint32 & count, const qint32 & id){
   auto newParameter = new SQLParameter(param, count);
   m_Parameters[id] = newParameter;
   //TODO
   return true;
}

void Scheduling::setParameters(const QMap<qint32, SQLParameter *> & Parameters){
   m_Parameters = Parameters;
}

qint32 Scheduling::getParamCount() const
{
   return m_paramCount;
}

void Scheduling::setParamCount(const qint32 & paramCount)
{
   m_paramCount = paramCount;
}

