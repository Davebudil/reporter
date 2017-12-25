#ifndef EXPORTXLS_H
#define EXPORTXLS_H
#include <QObject>
#include <QPair>
#include <QDataStream>
#include <QTextStream>
#include <QFile>

class ExportXLS{
   public:
      ExportXLS();
      bool generateFile(const QString & templatePath,
                        const QString & genPath,
                        const QList<std::pair<QString, QString>> constants,
                        const QList<QStringList> queries);
};

#endif // EXPORTXLS_H
