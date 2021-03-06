#include "setup.h"
#include "log.h"
//constructor
Setup::Setup(QObject *parent)
   : QObject(parent){
}
//Destructor
Setup::~Setup() = default;
//loads .ini settings file
void Setup::loadIni(){
   loadSettings();
   qInfo(logInfo()) << "Succesfully loaded ini settings.";
}
void Setup::saveIni(){
   saveSettings();
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
   QFile loadFile(QDir::currentPath() + "/data/ReporterSchedule.dat");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      qint32 scheduleIterator = 0;
      for(qint32 i = 0; i + 4 < serializeData.count(); i+=5){
         out << scheduleName[scheduleIterator++];
         for(auto & it : serializeData[i]){
            tmp = it;
            out << tmp;
//            qInfo(logInfo()) << tmp + " SHIFT";
         }
         for(auto & it : serializeData[i+1]){
            tmp = it;
            out << tmp;
//            qInfo(logInfo()) << tmp + " DAILY";
         }
         for(auto & it : serializeData[i+2]){
            tmp = it;
            out << tmp;
//            qInfo(logInfo()) << tmp + " WEEKLY";
         }
         for(auto & it : serializeData[i+3]){
            tmp = it;
            out << tmp;
//            qInfo(logInfo()) << tmp + " MONTHLY";
         }
         for(auto & it : serializeData[i+4]){
            tmp = it;
            out << tmp;
         }
      }
      return true;
   }
   return false;
}
bool Setup::serializeGlobal(const QStringList & global){
   QFile loadFile(QDir::currentPath() + "/data/ReporterGlobal.dat");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      for(auto & it : global){
         out << it;
      }
      return true;
   }
   return false;
}
bool Setup::m_serializeParameters(const QStringList & param, const QVector<qint32> & count){
   QFile loadFile(QDir::currentPath() + "/data/ReporterParameters.dat");
   if(loadFile.open(QIODevice::ReadWrite | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      qint32 drivingI = 0;
      for(auto it : count){
         qint32 tmpInt = it;
         out << tmpInt;
         for(qint32 i = it; i > 0; i--){
            const QString & tmpString = param.at(drivingI);
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
   m_Settings.databaseType = "QMYSQL";
   QSettings settings(QDir::currentPath() +  "/settings/ReporterSettings.ini", QSettings::IniFormat);
   m_Settings.databaseName = settings.value("DatabaseName", "budil").toString();
   m_Settings.host = settings.value("Host", "localhost").toString();
   m_Settings.port = settings.value("Port", 3306).toInt();
   m_Settings.userName = settings.value("Username", "root").toString();
   m_Settings.userPassword = settings.value("UserPassword", "Blizazrd5").toString();
   m_Settings.hotKey = settings.value("Hotkey", "ctrl+alt+Q").toString();
   m_Settings.timerInterval = settings.value("TimerIntervalMS", 1800000).toInt();
   m_Settings.customInterval = settings.value("CustomIntervalMS", 1800000).toInt();
   m_Settings.generatedByUser = settings.value("GeneratedByUser", "David Budil").toString();
}

void Setup::saveSettings(){
   QSettings settings(QDir::currentPath() +  "/settings/ReporterSettings.ini", QSettings::IniFormat);
   settings.setValue("GeneratedByUser", m_Settings.generatedByUser);
   settings.setValue("Hotkey", m_Settings.hotKey);
   settings.setValue("DatabaseName", m_Settings.databaseName);
   settings.setValue("Host", m_Settings.host);
   settings.setValue("Port", m_Settings.port);
   settings.setValue("Username", m_Settings.userName);
   settings.setValue("UserPassword", m_Settings.userPassword);
   settings.setValue("TimerIntervalMS", m_Settings.timerInterval);
   settings.setValue("CustomIntervalMS", m_Settings.customInterval);
}

bool Setup::m_serializeQueries(const QStringList & queries){
   QFile loadFile(QDir::currentPath() + "/data/ReporterQueries.dat");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QDataStream out(&loadFile);
      for(auto & it : queries){
         const QString & tmp = it;
         out << tmp;
      }
      return true;
   }
   return false;
}
//deserializes
bool Setup::m_deserializeQueries(QStringList & queries){
   QFile loadFile(QDir::currentPath() + "/data/ReporterQueries.dat");
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
   QFile loadFile(QDir::currentPath() + "/data/ReporterParameters.dat");
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
bool Setup::deserializeSchedule(QList<QStringList> & deserializeData,
                                QStringList & scheduleName,
                                QMap<QString, QVector<QStringList>> & parameters){
   QFile loadFile(QDir::currentPath() + "/data/ReporterSchedule.dat");
   if(loadFile.open(QIODevice::ReadOnly)){
      QDataStream in(&loadFile);
      qint32 scheduleCount = 0;
      while(!in.atEnd()){
         QString tmp;
         QStringList shift, day, weekly, monthly;
         QVector<QStringList> allParameters;
         qint32 emailCount;
         qint32 totalParamCount, paramCount;
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
         for(qint32 i = 0; i < 18; ++i){
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
         for(qint32 i = 0; i < 12; ++i){
            in >> tmp;
            monthly << tmp;
         }
         in >> tmp;
         emailCount = tmp.toInt();
         for(qint32 i = 0; i < emailCount; ++i){
            in >> tmp;
            monthly << tmp;
         }
         in >> tmp;
         totalParamCount = tmp.toInt();
         for(qint32 i = 0; i < totalParamCount; ++i){
            QStringList tmpList;
            in >> tmp;
            paramCount = tmp.toInt();
            for(qint32 y = 0; y < paramCount; ++y){
               in >> tmp;
               tmpList << tmp;
            }
            allParameters.push_back(tmpList);
         }

         parameters[scheduleName[scheduleCount++]] = allParameters;
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
   QFile loadFile(QDir::currentPath() + "/data/ReporterGlobal.dat");
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
   QFile loadFile(QDir::currentPath() + "/logs/logFile.txt");
   QFile loadTMPFile(QDir::currentPath() + "/logs/tmp.txt");
   QDate currentDate = QDate::currentDate();

   if(loadFile.open(QIODevice::ReadOnly | QIODevice::Text)
      && loadTMPFile.open(QIODevice::WriteOnly | QIODevice::Text)){
      QTextStream in(&loadFile);
      QTextStream out(&loadTMPFile);

      while(!in.atEnd()){
         QString tmpDate = "";
         QString tmpLine = in.readLine();

         for(qint32 i = 0; i < 10; ++i){
            tmpDate += tmpLine[i];
         }

         QDate tmp = QDate().fromString(tmpDate);
         tmp = tmp.addDays(14);

         if(tmp < currentDate){
            out << tmpLine << " - Log cleanup completed." << "\n";
         }
      }
      loadFile.remove();
      loadTMPFile.rename(loadFile.fileName());
      return true;
   }else{
      return false;
   }
}
//setter
void Setup::setFilePath(const QString & path){
   m_filePath = path;
}
//getter
QString Setup::getFilePath() const{
   return m_filePath;
}
