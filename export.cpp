#include "export.h"
#include "log.h"

Export::Export(){

}

Export::~Export(){

}

ExportXLS Export::getXLS() const{
   return m_XLS;
}
ExportCSV Export::getCSV() const{
   return m_CSV;
}
ExportHTML Export::getHTML() const{
   return m_HTML;
}
void Export::handleExport(const QQueue<Scheduling> & intervalsToHandle){
   QDateTime currentTime = QDateTime().currentDateTime();

   for(auto & it : intervalsToHandle){
      if(it.m_Shift.generateShiftData(currentTime)){
         m_generateShift(it.getShift());
      }
      if(it.m_Daily.generateDailyData(currentTime)){
         m_generateDaily(it.getDaily());
      }
      if(it.m_Weekly.generateWeeklyData(currentTime)){
         m_generateWeekly(it.getWeekly());
      }
      if(it.m_Monthly.generateMonthlyData(currentTime)){
         m_generateMonthly(it.getMonthly());
      }
   }
}

void Export::m_generateShift(const ShiftSchedule & shift){
   if(shift.getDone0()){

   }else if(shift.getDone1()){

   }else if(shift.getDone2()){

   }
}
void Export::m_generateDaily(const DailySchedule & daily){

}
void Export::m_generateWeekly(const WeeklySchedule & weekly){

}
void Export::m_generateMonthly(const MonthlySchedule & monthly){

}
