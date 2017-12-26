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
bool ExportXLS::readResult(){
   QFile loadFile("/home/dave/Documents/sielaff/project/reporter/reporter/ask_ attachment_final.txt");
   if(loadFile.open(QIODevice::ReadOnly)){
      QDataStream in(&loadFile);
      QStringList tmp;
      while(!in.atEnd()){
         in >> tmp;
      }
      if(tmp.at(0) == "0"){
         QMessageBox::information(0,QObject::tr("Export Result"),QObject::tr("Export to xls file successful."));
         return true;
      }else{
         QMessageBox::information(0,QObject::tr("Export Error"),QObject::tr("Export to xls file failed."));
         return false;
      }
   }
   QMessageBox::critical(0,QObject::tr("File Error"),QObject::tr("Failed to generate output file."));
   return false;
}
