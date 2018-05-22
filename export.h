#ifndef EXPORT_H
#define EXPORT_H
#include <QObject>
#include <QQueue>
#include <QDateTime>
#include <QFile>
#include "exportxls.h"
#include "exportcsv.h"
#include "exporthtml.h"
#include "sqlquery.h"
#include "sqlparameter.h"
#include "scheduling.h"
#include <QProcess>
#include <customscheduling.h>

class Export{
   public:
      Export();
      ~Export();
      ExportXLS & getXLS();
      ExportHTML & getHTML();
      ExportCSV & getCSV();
      void handleExport(QQueue<Scheduling*> & intervalsToHandle,
                        QQueue<SQLquery> & queries,
                        QQueue<QSharedPointer<SQLParameter>> & parameters,
                        QSqlDatabase & db,
                        QString & generatedBy);
      void customExport(CustomScheduling & exportData,
                        QQueue<SQLquery> & queries,
                        QQueue<QSharedPointer<SQLParameter>> & parameters,
                        QSqlDatabase & db,
                        qint32 & customInterval,
                        QString & generatedBy);
      void m_createTempScheduling(CustomScheduling & exportData,
                                  ShiftSchedule & shift,
                                  DailySchedule & daily,
                                  WeeklySchedule & weekly,
                                  MonthlySchedule & monthly);
      bool m_generateShift(ShiftSchedule shift,
                           QQueue<SQLquery> queries,
                           QSharedPointer<SQLParameter> param,
                           QSqlDatabase & db,
                           QDateTime & currentTime,
                           quint32 & count,
                           QString & generatedBy,
                           bool showInfo);
      bool m_generateDaily(DailySchedule daily,
                           QQueue<SQLquery> queries,
                           QSharedPointer<SQLParameter> param,
                           QSqlDatabase & db,
                           QDateTime & currentTime,
                           quint32 & count,
                           QString & generatedBy,
                           bool showInfo);
      bool m_generateWeekly(WeeklySchedule weekly,
                            QQueue<SQLquery> queries,
                            QSharedPointer<SQLParameter> param,
                            QSqlDatabase & db,
                            QDateTime & currentTime,
                            quint32 & count,
                            QString & generatedBy,
                            bool showInfo);
      bool m_generateMonthly(MonthlySchedule monthly,
                             QQueue<SQLquery> queries,
                             QSharedPointer<SQLParameter> param,
                             QSqlDatabase & db,
                             QDateTime & currentTime,
                             quint32 & count,
                             QString & generatedBy,
                             bool showInfo);
      void m_shiftDayReset(ShiftSchedule & shift, const QDateTime & current);
   private:
      bool validateQuery(QQueue<SQLquery> & query, SQLquery & currentQuery);
      ExportXLS m_XLS;
      ExportCSV m_CSV;
      ExportHTML m_HTML;

};

#endif // EXPORT_H
