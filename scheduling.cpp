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
QString Scheduling::getName() const{
    return m_Name;
}
void Scheduling::setName(const QString & Name){
   m_Name = Name;
}
//Checks if its time to generate data tables
void Scheduling::checkTimeInterval(QDateTime & currentDateTime){
   m_Shift.generateShiftData(currentDateTime);
   m_Daily.generateDailyData(currentDateTime);
   m_Weekly.generateWeeklyData(currentDateTime);
   m_Monthly.generateMonthlyData(currentDateTime);
}
