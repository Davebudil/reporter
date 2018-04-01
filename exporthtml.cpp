#include "exporthtml.h"
#include "log.h"

ExportHTML::ExportHTML() = default;

bool ExportHTML::generateFile(QSqlQuery & dataHTML,
                              const QString & htmlName){
   QString filePath = QDir::currentPath() + "/" + htmlName + ".html";
   QFile fileHtml(filePath);

   if(dataHTML.first()){
      if(fileHtml.open(QFile::WriteOnly|QFile::Truncate)){
      QTextStream stream(&fileHtml);
      QString tmpString;
      QSqlRecord columns = dataHTML.record();
      stream << "<!DOCTYPE htm>\n";
      stream << "<html>\n";
      stream << "<head>\n";
      stream << "<style>\n";
      stream << "table { font-family: arial, sans-serif; border-collapse: collapse; width: 100%;}";
      stream << "td, th { border: 1px solid #dddddd; text-align: left; padding: 8px; }";
      stream << "tr:nth-child(even) { background-color: #dddddd;}";
      stream << "</style>";
      stream << "</head>";
      stream << "<body>";

      tmpString = "<table style=\"width:100%\">";
      stream << tmpString;
      stream << "\n";

      tmpString = "<tr>";
      stream << tmpString;
      for(qint32 i = 0; i < columns.count(); ++i){
         tmpString = "<td>" + columns.fieldName(i) + "</td>";
         stream << tmpString;
      }
      tmpString = "</tr>";
      stream << tmpString;
      stream << "\n";

      while(dataHTML.next()){
         qint32 x = 0;
         stream << "<tr>";
         for(qint32 i = 0; i < dataHTML.record().count(); ++i){
            tmpString = "<td>" + dataHTML.value(x++).toString() + "</td>";
            stream << tmpString;
         }
         stream << "</tr>";
         stream << "\n";
      }
      stream << "</body>\n";
      stream << "</html>\n";
      qInfo(logInfo()) << "Succesfully generated html file.";
      return true;
      }
      qInfo(logInfo()) << "Failed to generate html file.";
      return false;
   }
   qInfo(logInfo()) << "Failed to generate html file.";
   return false;
}
