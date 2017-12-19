#ifndef SCHEDULING_H
#define SCHEDULING_H
#include <shiftschedule.h>
#include <dailyschedule.h>
#include <weeklyschedule.h>
#include <monthlyschedule.h>
#include <customschedule.h>

class Scheduling{

   public:
      Scheduling();
      ~Scheduling();
      ShiftSchedule & getShift();
      DailySchedule & getDaily();
      WeeklySchedule & getWeekly();
      MonthlySchedule & getMonthly();
      CustomSchedule & getCustom();

   private:
      ShiftSchedule m_Shift;
      DailySchedule m_Daily;
      WeeklySchedule m_Weekly;
      MonthlySchedule m_Monthly;
      CustomSchedule m_Custom;
};

#endif // SCHEDULING_H
