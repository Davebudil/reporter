#include "exportxls.h"
#include "log.h"
//test
#include <QRandomGenerator>

ExportXLS::ExportXLS(QObject * parent) : QObject(parent),
   generateXLS(nullptr){
}

ExportXLS::~ExportXLS(){
   if(generateXLS){
      delete generateXLS;
   }
}

bool ExportXLS::generateFile(const QString & templatePath,
                             const QString & genPath,
                             const QList<std::pair<QString, QString>> constants,
                             const QList<QStringList> queries){
   //   qint32 randomInt = QRandomGenerator::global()->generate();
   //   QFile loadFile(QDir::currentPath() + "/export/tmp/ask_attachment_source" + QString::number(randomInt) + ".txt");
   QFile loadFile(QDir::currentPath() + "/ASK/ask_ attachment_source.txt");
   if(loadFile.open(QIODevice::WriteOnly | QIODevice::Truncate)){
      QTextStream out(&loadFile);
      out << templatePath << "\r\n";
      out << QDir::currentPath() + "/ASK/export/" + genPath + ".xls" << "\r\n";
      out << QString::number(constants.size()) << "\r\n";
      for(auto & it : constants){
         out << QString(it.first) << "\r\n";
         out << QString(it.second) << "\r\n";
      }
      out << QString::number(queries.size()) << "\r\n";
      for(const auto & it : queries){
         out << it.at(0) << "\r\n";
         out << it.at(1) << "\r\n";
         out << it.at(2) << "\r\n";
      }
      //      qInfo(logInfo()) << "Successfuly generated XLSX file.";
      return true;
   }
   //   qWarning(logWarning()) << "Failed to generate XLSX file.";
   return false;
}
bool ExportXLS::readResult(){
   generateXLS = new QProcess;

   connect(generateXLS, SIGNAL (error(QProcess::ProcessError)), this, SLOT(sdLaunchError(QProcess::ProcessError)));
   connect(generateXLS, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [=](int exitCode, QProcess::ExitStatus exitStatus){
      qInfo(logInfo()) << "Exit Code: " + QVariant(exitCode).toString()
                          + " Status: " + QVariant(exitStatus).toString();});

   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   generateXLS->setProcessEnvironment(env);
   generateXLS->start(QDir::currentPath() + "/ASK/ask_attachment.exe");
   generateXLS->waitForFinished();
   generateXLS->close();

   QFile loadFile(QDir::currentPath() + "/ASK/ask_ attachment_final.txt");
   delete generateXLS;

   if(loadFile.open(QIODevice::ReadOnly)){
      QTextStream in(&loadFile);
      QStringList tmplist;
      QString tmp;
      while(!in.atEnd()){
         in >> tmp;
         tmplist.append(tmp);
      }
      if(tmplist.at(0) == "0"){
         qInfo(logInfo()) << "Export to xlsx file succesful.";
         //         QMessageBox::information(nullptr, QObject::tr("Export Result"),QObject::tr("Export to xls file successful."));
         return true;
      }else{
         QStringList tmpList = tmplist;
         tmpList.removeFirst();
         qWarning(logWarning()) << "Exit Code: " + tmplist.first() + " Export to xlsx file failed: " + tmpList.join(" ");
         //         QMessageBox::information(nullptr, QObject::tr("Export Error"),QObject::tr("Export to xls file failed."));
         return false;
      }
   }else{
      qWarning(logWarning()) << "Failed to open output file.";
      return false;
   }
}

void ExportXLS::sdLaunchError(QProcess::ProcessError error){
   qDebug(logDebug()) << "RUNNING PROCESS ERROR: " + QVariant(error).toString();
}
