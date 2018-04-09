#include "exportcsv.h"
#include "log.h"

ExportCSV::ExportCSV() = default;

bool ExportCSV::generateFile(const QString & templatePathCSV,
                             const QString & attachNameCSV,
                             QSqlQuery & dataCSV){
   //TMP TEST add Template -> maybe use TEMPLATE CSV AS INI
   QString filePath = QDir::currentPath() + "/export/csv/" + attachNameCSV + ".csv";
   QFile fileCSV(filePath);
//   loadCSVIni(templatePathCSV);

   if(dataCSV.first()){
      if(fileCSV.open(QFile::WriteOnly|QFile::Truncate)){
         QTextStream stream(&fileCSV);
         QSqlRecord columns = dataCSV.record();

         for(qint32 i = 0; i < columns.count(); ++i){
            QString tmp = columns.fieldName(i);
            stream << tmp << ";";
         }
         stream << "\n";

         while(dataCSV.next()){
            qint32 x = 0;
            for(qint32 i = 0; i < dataCSV.record().count(); ++i){
               stream << dataCSV.value(x++).toString() << ";";
            }
            stream << "\n";
         }

         fileCSV.close();
         //debug function
//         QDesktopServices::openUrl(QUrl(filePath));
//         qInfo(logInfo()) << "Successfuly generated CSV file.";
         return true;
      }else{
//         qWarning(logWarning()) << "Failed to generate CSV file.";
         return false;
      }
   }else{
//      qWarning(logWarning()) << "Failed to generate CSV file.";
      return false;
   }
}

QStringList ExportCSV::loadCSVIni(const QString & iniPath){

}

