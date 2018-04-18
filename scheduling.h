#ifndef SCHEDULING_H
#define SCHEDULING_H
#include <QObject>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QQueue>
#include <shiftschedule.h>
#include <dailyschedule.h>
#include <weeklyschedule.h>
#include <monthlyschedule.h>
#include <customschedule.h>
#include <sqlparameter.h>

class Scheduling{
   public:
      Scheduling();
      ~Scheduling();
      ShiftSchedule & getShift();
      DailySchedule & getDaily();
      WeeklySchedule & getWeekly();
      MonthlySchedule & getMonthly();
      CustomSchedule & getCustom();
      QMap<qint32, SQLParameter *> getParameters();
      QQueue<SQLParameter> getQueueParameters();
      qint32 getParamCount() const;
      //DEBUG
      void printParamTest();
      bool addParam(const QStringList & param, const qint32 & count, const qint32 & id);
      QString getName() const;
      void setParamCount(const qint32 & paramCount);
      void setName(const QString & Name);
      void setParameters(const QMap<qint32, SQLParameter *> & Parameters);
      void serializeGlobal(const QStringList & global);
      void deserializeGlobal(const QStringList & global);

   private:
      QString m_Name;
      qint32 m_paramCount;
      ShiftSchedule m_Shift;
      DailySchedule m_Daily;
      WeeklySchedule m_Weekly;
      MonthlySchedule m_Monthly;
      CustomSchedule m_Custom;
      QMap<qint32, SQLParameter*> m_Parameters;

};

#endif // SCHEDULING_H
