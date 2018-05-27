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
#include "database.h"
#include <QtConcurrent>
#include <customscheduling.h>

class Export{
   public:
      Export();
      ~Export();
      ExportXLS & getXLS();
      ExportHTML & getHTML();
      ExportCSV & getCSV();
      void asyncExport(QQueue<QSharedPointer<Scheduling>> & intervalsToHandle,
                        QQueue<SQLquery> & queries,
                        QQueue<QSharedPointer<SQLParameter>> & parameters,
                        Database & db);
      void asyncCustomExport(QSharedPointer<CustomScheduling> exportData,
                             QQueue<SQLquery> & queries,
                             QQueue<QSharedPointer<SQLParameter>> & parameters,
                             Database & db,
                             qint32 & customInterval);
      void m_createTempScheduling(QSharedPointer<CustomScheduling> exportData,
                                  ShiftSchedule & shift,
                                  DailySchedule & daily,
                                  WeeklySchedule & weekly,
                                  MonthlySchedule & monthly);
      void m_shiftDayReset(ShiftSchedule & shift, const QDateTime & current);
      void asyncShiftGeneration(ShiftSchedule shift,
                                QQueue<SQLquery> queries,
                                QSharedPointer<SQLParameter> param,
                                Database & db,
                                QDateTime & currentTime);
      void asyncDailyGeneration(DailySchedule daily,
                                QQueue<SQLquery> queries,
                                QSharedPointer<SQLParameter> param,
                                Database & db,
                                QDateTime & currentTime);
      void asyncWeeklyGeneration(WeeklySchedule weekly,
                                 QQueue<SQLquery> queries,
                                 QSharedPointer<SQLParameter> param,
                                 Database & db,
                                 QDateTime & currentTime);
      void asyncMonthlGeneration(MonthlySchedule monthly,
                                 QQueue<SQLquery> queries,
                                 QSharedPointer<SQLParameter> param,
                                 Database & db,
                                 QDateTime & currentTime);
      void setGeneratedBy(const QString & generatedBy);

   private:
      void handleExport(QQueue<QSharedPointer<Scheduling>> & intervalsToHandle,
                        QQueue<SQLquery> & queries,
                        QQueue<QSharedPointer<SQLParameter>> & parameters,
                        Database & db);
      void customExport(QSharedPointer<CustomScheduling> exportData,
                        QQueue<SQLquery> & queries,
                        QQueue<QSharedPointer<SQLParameter>> & parameters,
                        Database & db,
                        qint32 & customInterval);
      bool m_generateShift(ShiftSchedule shift,
                           QQueue<SQLquery> queries,
                           QSharedPointer<SQLParameter> param,
                           Database & db,
                           QDateTime & currentTime);
      bool m_generateDaily(DailySchedule daily,
                           QQueue<SQLquery> queries,
                           QSharedPointer<SQLParameter> param,
                           Database & db,
                           QDateTime & currentTime);
      bool m_generateWeekly(WeeklySchedule weekly,
                            QQueue<SQLquery> queries,
                            QSharedPointer<SQLParameter> param,
                            Database & db,
                            QDateTime & currentTime);
      bool m_generateMonthly(MonthlySchedule monthly,
                             QQueue<SQLquery> queries,
                             QSharedPointer<SQLParameter> param,
                             Database & db,
                             QDateTime & currentTime);
      bool validateQuery(QQueue<SQLquery> & query, SQLquery & currentQuery);
      QString masterQuery(SQLquery & detail, SQLquery & master);
      ExportXLS m_XLS;
      ExportCSV m_CSV;
      ExportHTML m_HTML;
      QString generatedBy;

};

#endif // EXPORT_H
