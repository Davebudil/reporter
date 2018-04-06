#ifndef EXPORTHTML_H
#define EXPORTHTML_H
#include <QObject>
#include <QtSql>
#include <QFile>

class ExportHTML{
   public:
      ExportHTML();
      bool generateFile(QSqlQuery & dataHTML, const QString & htmlName);
};

#endif // EXPORTHTML_H
