#ifndef SCHEDULING_H
#define SCHEDULING_H
#include <QObject>
#include <QDate>
#include <QTime>
#include <QDateTime>
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
      QString getName() const;
      void setName(const QString & Name);
      void serializeGlobal(const QStringList & global);
      void deserializeGlobal(const QStringList & global);

   private:
      QString m_Name;
      ShiftSchedule m_Shift;
      DailySchedule m_Daily;
      WeeklySchedule m_Weekly;
      MonthlySchedule m_Monthly;
      CustomSchedule m_Custom;
};

#endif // SCHEDULING_H
