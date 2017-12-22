#include "scheduling.h"

Scheduling::Scheduling(){

}
Scheduling::~Scheduling(){

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
QString Scheduling::getGlobalEmail() const{
   return m_GlobalEmail;
}
void Scheduling::setGlobalEmail(const QString & GlobalEmail){
   m_GlobalEmail = GlobalEmail;
}
