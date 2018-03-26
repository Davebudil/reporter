#include "setup.h"
#include "log.h"
//constructor
Setup::Setup(QObject *parent)
           : QObject(parent){
}
//Destructor
Setup::~Setup(){}
//loads .ini settings file
void Setup::loadIni(){
   loadSettings();
   qInfo(logInfo()) << "Succesfully loaded ini settings.";
}
void Setup::saveIni(){
   saveSettings();
   qInfo(logInfo()) << "Succesfully saved ini settings.";
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
bool Setup::serializeSchedule(const QList<QStringList> & serializeData, const QStringList & scheduleName){
   QString tmp;
   QFile loadFile(QDir::currentPath() + "/ReporterSchedule.dat");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      qint32 scheduleIterator = 0;
      for(qint32 i = 0; i + 3 < serializeData.count(); i+=4){
         out << scheduleName[scheduleIterator++];
         for(auto & it : serializeData[i]){
            tmp = it;
            out << tmp;
         }
         for(auto & it : serializeData[i+1]){
            tmp = it;
            out << tmp;
         }
         for(auto & it : serializeData[i+2]){
            tmp = it;
            out << tmp;
         }
         for(auto & it : serializeData[i+3]){
            tmp = it;
            out << tmp;
         }
      }
      return true;
   }
   return false;
}
bool Setup::serializeGlobal(const QStringList & global){
   QFile loadFile(QDir::currentPath() + "/ReporterGlobal.dat");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      for(auto & it : global){
         QString tmp = it;
         out << tmp;
      }
      return true;
   }
   return false;
}
bool Setup::m_serializeParameters(const QStringList & param, const QVector<qint32> & count){
   QFile loadFile(QDir::currentPath() + "/ReporterParameters.dat");
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

SPreferences Setup::getSettings(){
   return m_Settings;
}

void Setup::loadSettings(){
   QSettings settings(QDir::currentPath() +  "/ReporterSettings.ini", QSettings::IniFormat);
   m_Settings.databaseType = settings.value("DatabaseType", "QMYSQL").toString();
   m_Settings.host = settings.value("Host", "localhost").toString();
   m_Settings.port = settings.value("Port", 3306).toInt();
   m_Settings.databaseName = settings.value("DatabaseName", "budil").toString();
   m_Settings.userName = settings.value("Username", "root").toString();
   m_Settings.userPassword = settings.value("UserPassword", "Blizazrd5").toString();
   m_Settings.hotKey = settings.value("Hotkey", "ctrl+alt+Q").toString();
   m_Settings.timerInterval = settings.value("TimerIntervalMS", 1800000).toInt();
   m_Settings.customInterval = settings.value("CustomIntervalMS", 1800000).toInt();
}

void Setup::saveSettings(){
   QSettings settings(QDir::currentPath() +  "/ReporterSettings.ini", QSettings::IniFormat);
   settings.setValue("DatabaseType", m_Settings.databaseType);
   settings.setValue("Host", m_Settings.host);
   settings.setValue("Port", m_Settings.port);
   settings.setValue("DatabaseName", m_Settings.databaseName);
   settings.setValue("Username", m_Settings.userName);
   settings.setValue("UserPassword", m_Settings.userPassword);
   settings.setValue("Hotkey", m_Settings.hotKey);
   settings.setValue("TimerIntervalMS", m_Settings.timerInterval);
   settings.setValue("CustomIntervalMS", m_Settings.customInterval);
}

bool Setup::m_serializeQueries(const QStringList & queries){
   QFile loadFile(QDir::currentPath() + "/ReporterQueries.dat");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      for(auto & it : queries){
         QString tmp = it;
         out << tmp;
      }
      return true;
   }
   return false;
}
//deserializes
bool Setup::m_deserializeQueries(QStringList & queries){
   QFile loadFile(QDir::currentPath() + "/ReporterQueries.dat");
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
   QFile loadFile(QDir::currentPath() + "/ReporterParameters.dat");
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
//DODELAT TODO
bool Setup::deserializeSchedule(QList<QStringList> & deserializeData, QStringList & scheduleName){
   QFile loadFile(QDir::currentPath() + "/ReporterSchedule.dat");
   if(loadFile.open(QIODevice::ReadOnly)){
      QDataStream in(&loadFile);
      while(!in.atEnd()){
         QString tmp;
         QStringList shift, day, weekly, monthly;
         qint32 emailCount;
         in >> tmp;
         scheduleName.push_back(tmp);
         for(qint32 i = 0; i < 22; ++i){
            in >> tmp;
            shift << tmp;
         }
         in >> tmp;
         emailCount = tmp.toInt();
         for(qint32 i = 0; i < emailCount; ++i){
            in >> tmp;
            shift << tmp;
         }
         for(qint32 i = 0; i < 17; ++i){
            in >> tmp;
            day << tmp;
         }
         in >> tmp;
         emailCount = tmp.toInt();
         for(qint32 i = 0; i < emailCount; ++i){
            in >> tmp;
            day << tmp;
         }
         for(qint32 i = 0; i < 11; ++i){
            in >> tmp;
            weekly << tmp;
         }
         in >> tmp;
         emailCount = tmp.toInt();
         for(qint32 i = 0; i < emailCount; ++i){
            in >> tmp;
            weekly << tmp;
         }
         for(qint32 i = 0; i < 11; ++i){
            in >> tmp;
            monthly << tmp;
         }
         in >> tmp;
         emailCount = tmp.toInt();
         for(qint32 i = 0; i < emailCount; ++i){
            in >> tmp;
            monthly << tmp;
         }
         deserializeData.push_back(shift);
         deserializeData.push_back(day);
         deserializeData.push_back(weekly);
         deserializeData.push_back(monthly);
      }
      return true;
   }
   return false;
}
bool Setup::deserializeGlobal(QStringList & global){
   QFile loadFile(QDir::currentPath() + "/ReporterGlobal.dat");
   if(loadFile.open(QIODevice::ReadOnly)){
      QDataStream in(&loadFile);
      while(!in.atEnd()){
         QString tmp;
         in >> tmp;
         global << tmp;
      }
      return true;
   }
   return false;
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
