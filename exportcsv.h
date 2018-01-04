#ifndef EXPORTCSV_H
#define EXPORTCSV_H

#include <QObject>
#include <QtSql>
#include <QDesktopServices>
#include <QFile>

class ExportCSV
{
   public:
      ExportCSV();
      bool generateFile(const QString & templatePathCSV,
                        const QString & attachNameCSV,
                        QSqlQuery & dataCSV);
};

#endif // EXPORTCSV_H
