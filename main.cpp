#include "reporter.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QScopedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QLoggingCategory>

QScopedPointer<QFile> m_logFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

qint32 main(qint32 argc, char *argv[]){
   QApplication reporterApp(argc, argv);
   m_logFile.reset(new QFile("/home/dave/Documents/sielaff/project/reporter/reporter/logFile.txt"));
   m_logFile.data()->open(QFile::Append | QFile::Text);
   qInstallMessageHandler(messageHandler);
   Reporter App;
   App.show();
   qInfo(logInfo()) << "Interface loaded successfuly.";
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
