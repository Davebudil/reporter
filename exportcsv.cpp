#include "exportcsv.h"

ExportCSV::ExportCSV(){

}

bool ExportCSV::generateFile(const QString & templatePathCSV,
                             const QString & attachNameCSV,
                             QSqlQuery & dataCSV){
   //TMP TEST add Template
   QString filePath = QDir::currentPath() + "/" + attachNameCSV + ".csv";
   QFile fileCSV(filePath);

   if(fileCSV.open(QFile::WriteOnly|QFile::Truncate)){
      QTextStream stream(&fileCSV);

      while(dataCSV.next()){
         qint32 x = 0;
         for(qint32 i = 0; i < dataCSV.record().count(); ++i){
            stream << dataCSV.value(x++).toString() << ";";
         }
         stream << "\n";
      }

      fileCSV.close();
      qDebug() << filePath;
      QDesktopServices::openUrl(QUrl(filePath));
      return true;
   }else{
      return false;
   }
}

