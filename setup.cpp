#include "setup.h"
//constructor
Setup::Setup(QObject *parent)
           : QObject(parent){
}
//Destructor
Setup::~Setup(){}
//loads .ini settings file
bool Setup::loadIni(){
   return true; //TMP
}
bool Setup::saveIni(){
   return true;
}

bool Setup::serializeQueries(const QStringList &queries){
   return m_serializeQueries(queries);
}
bool Setup::deserializeQueries(QStringList &queries){
   return m_deserializeQueries(queries);
}
bool Setup::serializeParameters(const QStringList &parameters, const QVector<qint32> & count){
   return m_serializeParameters(parameters, count);
}

bool Setup::serializeSchedule(const QStringList & shift,
                              const QStringList & day,
                              const QStringList & weekly,
                              const QStringList & monthly){
   QString tmp;
   QFile loadFile("/home/dave/Documents/sielaff/project/reporter/reporter/ReporterSchedule.dat");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      for(auto & it : shift){
         tmp = it;
         out << tmp;
      }
      for(auto & it : day){
         tmp = it;
         out << tmp;
      }
      for(auto & it : weekly){
         tmp = it;
         out << tmp;
      }
      for(auto & it : monthly){
         tmp = it;
         out << tmp;
      }
      return true;
   }
   return false;
}
bool Setup::m_serializeParameters(const QStringList & param, const QVector<qint32> & count){
   QFile loadFile("/home/dave/Documents/sielaff/project/reporter/reporter/ReporterParameters.dat");
   if(loadFile.open(QIODevice::ReadWrite | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      qint32 drivingI = 0;
      for(auto it : count){
         qint32 tmpInt = it;
         out << tmpInt;
         for(qint32 i = it; i > 0; i--){
            QString tmpString = param.at(drivingI);
            out << tmpString;
            drivingI++;
         }
      }
      return true;
   }
   return false;
}

bool Setup::m_serializeQueries(const QStringList & queries){
   QFile loadFile("/home/dave/Documents/sielaff/project/reporter/reporter/ReporterQueries.dat");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      for(qint32 i = 0; i < queries.size(); ++i){
         QString tmp = queries.at(i);
         out << tmp;
      }
      return true;
   }
   return false;
}
//deserializes
bool Setup::m_deserializeQueries(QStringList & queries){
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
bool Setup::deserializeParameters(QStringList & parameters, QVector<qint32> & count){
   QFile loadFile("/home/dave/Documents/sielaff/project/reporter/reporter/ReporterParameters.dat");
   if(loadFile.open(QIODevice::ReadOnly)){
      QDataStream in(&loadFile);
      qint32 tmpInt;
      while(!in.atEnd()){
         in >> tmpInt;
         count.append(tmpInt);
         for(qint32 i = 0; i < tmpInt; ++i){
            QString tmpString;
            in >> tmpString;
            parameters << tmpString;
         }
      }
      return true;
   }
   return false;
}

bool Setup::deserializeSchedule(QStringList & shift, QStringList & day, QStringList & weekly, QStringList & monthly)
{

}
//cleans up
bool Setup::cleanUp(){
   return true; //TMP
}
//setter
void Setup::setFilePath(const QString & path){
   m_filePath = path;
}
//getter
QString Setup::getFilePath() const{
   return m_filePath;
}
