#include "reporter.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>
#include <QSettings>

QScopedPointer<QFile> m_logFile;
void loadDirectories();

void messageHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg);

qint32 main(qint32 argc, char *argv[]){
   QApplication reporterApp(argc, argv);
   //Handles settings
   reporterApp.setOrganizationName("Sielaff Bohemia");
   reporterApp.setOrganizationDomain("sielaff.cz");
   reporterApp.setApplicationName("Reporter");
   //Handles directories
   loadDirectories();

   //Functions that handle logging
   m_logFile.reset(new QFile(QDir::currentPath() + "/logs/logFile.txt"));
   m_logFile.data()->open(QFile::Append | QFile::Text);
   qInstallMessageHandler(messageHandler);

   //Handles application
   Reporter App;
   App.show();
   App.defaultSettings();

   return reporterApp.exec();
}

//Function that handles log writing
void messageHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg){
   QTextStream out(m_logFile.data());

   out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
   out << QString("%1.%2.%3 ").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD);
   switch(type){
      case QtInfoMsg:
         out << "INF ";
         break;
      case QtDebugMsg:
         out << "DBG ";
         break;
      case QtWarningMsg:
         out << "WRN ";
         break;
      case QtCriticalMsg:
         out << "CRT ";
         break;
      case QtFatalMsg:
         out << "FTL ";
         break;
   }
   out << context.category << ": " << msg << endl;
   out.flush();
}

void loadDirectories(){
   if(!QDir("export").exists()){
      QDir().mkdir("export");
   }
   if(!QDir("export/tmp").exists()){
      QDir().mkdir("export/tmp");
   }
   if(!QDir("export/csv").exists()){
      QDir().mkdir("export/csv");
   }
   if(!QDir("export/xls").exists()){
      QDir().mkdir("export/xls");
   }
   if(!QDir("export/html").exists()){
      QDir().mkdir("export/html");
   }
   if(!QDir("data").exists()){
      QDir().mkdir("data");
   }
   if(!QDir("settings").exists()){
      QDir().mkdir("settings");
   }
   if(!QDir("logs").exists()){
      QDir().mkdir("logs");
   }
}
