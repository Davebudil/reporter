#include "setup.h"
//constructor
Setup::Setup(QObject *parent)
   : QObject(parent){
}
//Destructor
Setup::~Setup(){}
//loads .ini settings file
bool Setup::loadIni(){

}
//serializes
bool Setup::serialize(const QStringList & queries){
   QFile loadFile("/home/dave/Documents/sielaff/project/reporter/reporter/ReporterQueries.dat");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      for(int i = 0; i < queries.size(); ++i){
         QString tmp = queries.at(i);
         out << tmp;
      }
      return true;
   }
   return false;
}
//deserializes
bool Setup::deserialize(QStringList & queries){
   QFile loadFile("/home/dave/Documents/sielaff/project/reporter/reporter/ReporterQueries.dat");
   if(loadFile.open(QIODevice::ReadOnly)){
      QDataStream in(&loadFile);
      while(!in.atEnd()){
         QString tmp;
         in >> tmp;
         queries << tmp;
      }
      return true;
   }
   return false;
}
//cleans up
bool Setup::cleanUp(){

}
//setter
void Setup::setFilePath(const QString & path){
   m_filePath = path;
}
//getter
QString Setup::getFilePath(){
   return m_filePath;
}
