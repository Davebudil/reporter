#ifndef EXPORT_H
#define EXPORT_H
#include <QObject>
#include <QQueue>
#include <QDateTime>
#include "exportxls.h"
#include "exportcsv.h"
#include "exporthtml.h"
#include "sqlquery.h"
#include "sqlparameter.h"
#include "scheduling.h"

class Export{
   public:
      Export();
      ~Export();
      ExportXLS getXLS() const;
      ExportHTML getHTML() const;
      ExportCSV getCSV() const;
      void handleExport(QQueue<Scheduling*> & intervalsToHandle,
                        QQueue<SQLquery> & queries,
                        QQueue<SQLParameter> & parameters,
                        QSqlDatabase & db);

   private:
      void m_generateShift(const ShiftSchedule & shift,
                           QQueue<SQLquery> & queries,
                           SQLParameter & param,
                           QSqlDatabase & db,
                           QDateTime & currentTime,
                           quint32 & count);
      void m_generateDaily(const DailySchedule & daily,
                           QQueue<SQLquery> & queries,
                           SQLParameter & param,
                           QSqlDatabase & db,
                           QDateTime & currentTime,
                           quint32 & count);
      void m_generateWeekly(const WeeklySchedule & weekly,
                            QQueue<SQLquery> & queries,
                            SQLParameter & param,
                            QSqlDatabase & db,
                            QDateTime & currentTime,
                            quint32 & count);
      void m_generateMonthly(const MonthlySchedule & monthly,
                             QQueue<SQLquery> & queries,
                             SQLParameter & param,
                             QSqlDatabase & db,
                             QDateTime & currentTime,
                             quint32 & count);
      ExportXLS m_XLS;
      ExportCSV m_CSV;
      ExportHTML m_HTML;
};

#endif // EXPORT_H
