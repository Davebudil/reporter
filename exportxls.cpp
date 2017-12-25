#include "exportxls.h"

ExportXLS::ExportXLS(){

}

bool ExportXLS::generateFile(const QString & templatePath,
                             const QString & genPath,
                             const QList<std::pair<QString, QString>> constants,
                             const QList<QStringList> queries){
   QFile loadFile("/home/dave/Documents/sielaff/project/reporter/reporter/ask_attachment_source.txt");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QTextStream out(&loadFile);
      out << templatePath << "\n";
      out << genPath << "\n";
      out << QString::number(constants.size()) << "\n";
      for(auto & it : constants){
         out << QString(it.first) << "\n";
         out << QString(it.second) << "\n";
      }
      out << QString::number(queries.size()) << "\n";
      for(auto it : queries){
         out << it.at(0) << "\n";
         out << it.at(1) << "\n";
         out << it.at(2) << "\n";
      }
      return true;
   }
   return false;
}
