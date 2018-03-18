#ifndef EXPORT_H
#define EXPORT_H
#include <QObject>
#include <QQueue>
#include <exportxls.h>
#include <exportcsv.h>
#include <exporthtml.h>
#include "scheduling.h"

class Export{
   public:
      Export();
      ~Export();
      ExportXLS getXLS() const;
      ExportHTML getHTML() const;
      ExportCSV getCSV() const;
      void handleExport(const QQueue<Scheduling> & intervalsToHandle);

   private:
      void m_generateShift(const ShiftSchedule & shift);
      void m_generateDaily(const DailySchedule & daily);
      void m_generateWeekly(const WeeklySchedule & weekly);
      void m_generateMonthly(const MonthlySchedule & monthly);
      ExportXLS m_XLS;
      ExportCSV m_CSV;
      ExportHTML m_HTML;
};

#endif // EXPORT_H
