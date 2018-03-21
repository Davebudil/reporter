#include "export.h"
#include "log.h"

Export::Export(){

}

Export::~Export(){

}

ExportXLS Export::getXLS() const{
   return m_XLS;
}
ExportCSV Export::getCSV() const{
   return m_CSV;
}
ExportHTML Export::getHTML() const{
   return m_HTML;
}
void Export::handleExport(QQueue<Scheduling*> & intervalsToHandle,
                          QQueue<SQLquery> & queries,
                          QQueue<SQLParameter> & parameters,
                          QSqlDatabase & db){
   QDateTime currentTime = QDateTime().currentDateTime();
   quint32 exportCount = 0;

   for(auto & it : intervalsToHandle){
      if(it->getShift().generateShiftData(currentTime) && it->getShift().getActive()){
         for(auto & itPar : parameters){
            //generate all queries for this interval and for all parameters
            m_generateShift(it->getShift(), queries, itPar, db, currentTime, exportCount);
         }
      }
      if(it->getDaily().generateDailyData(currentTime) && it->getDaily().getActive()){
         for(auto & itPar : parameters){
            m_generateDaily(it->getDaily(), queries, itPar, db, currentTime, exportCount);
         }
      }
      if(it->getWeekly().generateWeeklyData(currentTime) && it->getWeekly().getActive()){
         for(auto & itPar : parameters){
            m_generateWeekly(it->getWeekly(), queries, itPar, db, currentTime, exportCount);
         }
      }
      if(it->getMonthly().generateMonthlyData(currentTime) && it->getMonthly().getActive()){
         for(auto & itPar : parameters){
            m_generateMonthly(it->getMonthly(), queries, itPar, db, currentTime, exportCount);
         }
      }
   }
   if(exportCount != 0){
      qInfo(logInfo()) << "Succesfully generated " + QVariant(exportCount).toString() + " queries.";
   }
}

void Export::m_generateShift(const ShiftSchedule & shift,
                             QQueue<SQLquery> & queries,
                             SQLParameter & param,
                             QSqlDatabase & db,
                             QDateTime & currentTime,
                             quint32 & count){
   //need to define which way to format parameters -> using #parameter1 - 5 for now
   for(auto & it : queries){
      QList<std::pair<QString, QString>> genInfo;
      QDateTime tmp(currentTime);
      QDateTime tmp2(currentTime);
      genInfo.append(std::make_pair("CURRENT_DATE", QDate().currentDate().toString("dd.MM.yyyy")));
      for(quint32 i = 0; i < quint32(param.getCount()); ++i){
         QString tmpParam1;
         tmpParam1 = "#PARAMETER" + QString(i+1);
         it.bindParameter(tmpParam1, param.getParameters()[i]);
      }
      if(shift.getDone0()){
         tmp.setTime(shift.getFrom1());
         tmp2.setTime(shift.getFrom0());
         tmp2 = tmp2.addSecs(-1);
         genInfo.append(std::make_pair("DATE_TIME_FROM_TO",
                                       "FROM " + tmp.toString("dd.MM.yy hh:mm") +
                                       " TO " + tmp2.toString("dd.MM.yy hh:mm")));
         it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
         it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));
      }else if(shift.getDone1()){
         tmp.setTime(shift.getFrom0());
         tmp2.setTime(shift.getTo0());
         tmp2 = tmp2.addSecs(-1);
         genInfo.append(std::make_pair("DATE_TIME_FROM_TO",
                                       "FROM " + tmp.toString("dd.MM.yy hh:mm") +
                                       " TO " + tmp2.toString("dd.MM.yy hh:mm")));
         it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
         it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));
      }else if(shift.getDone2()){
         tmp.setTime(shift.getTo0());
         tmp2.setTime(shift.getFrom1());
         tmp2 = tmp2.addSecs(-1);
         genInfo.append(std::make_pair("DATE_TIME_FROM_TO",
                                       "FROM " + tmp.toString("dd.MM.yy hh:mm") +
                                       " TO " + tmp2.toString("dd.MM.yy hh:mm")));
         it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
         it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));
      }
      it.generateQuery(db);
      it.executeQuery();
      //export as CSV or XLSX...
      if(it.getResult().isActive()){
         if(shift.getXlsAttach()){
            //NOT SURE ABOUT THIS LINE
            genInfo.append(std::make_pair("GENERATED_BY", "David Budil"));
            //
            QList<QStringList> finalQueries;
            finalQueries.append(it.queryList());
            m_XLS.generateFile(shift.getXlsTemplatePath(),
                               shift.getAttachName(),
                               genInfo,
                               finalQueries);
         }
         if(shift.getCsvAttach()){
            QSqlQuery resultCSV = it.getResult();
            m_CSV.generateFile(shift.getCsvTemplatePath(),
                               shift.getAttachName(),
                               resultCSV);
         }
         ++count;
      }else{
         qInfo(logInfo()) << "Failed to generate query: " + it.getName() + " : " + it.getResult().lastError().text();
      }
      //SEND TO EMAIL/POSTMAN QUEUE
   }
}
void Export::m_generateDaily(const DailySchedule & daily,
                             QQueue<SQLquery> & queries,
                             SQLParameter & param,
                             QSqlDatabase & db,
                             QDateTime & currentTime,
                             quint32 & count){
   for(auto & it : queries){
      QList<std::pair<QString, QString>> genInfo;
      QDateTime tmp(currentTime);
      QDateTime tmp2(currentTime);
      genInfo.append(std::make_pair("CURRENT_DATE", QDate().currentDate().toString("dd.MM.yyyy")));
      for(quint32 i = 0; i < quint32(param.getCount()); ++i){
         QString tmpParam1;
         tmpParam1 = "#PARAMETER" + QString(i+1);
         it.bindParameter(tmpParam1, param.getParameters()[i]);
      }
      tmp.setTime(daily.getTime());
      tmp2.setTime(daily.getTime());
      tmp = tmp.addDays(-1);
      tmp2 = tmp2.addSecs(-1);
      genInfo.append(std::make_pair("DATE_TIME_FROM_TO",
                                    "FROM " + tmp.toString("dd.MM.yy hh:mm") +
                                    " TO " + tmp2.toString("dd.MM.yy hh:mm")));
      it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
      it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));

      it.generateQuery(db);
      it.executeQuery();
      //export as CSV or XLSX...
      if(it.getResult().isActive()){
         if(daily.getXlsAttach()){
            //NOT SURE ABOUT THIS LINE
            genInfo.append(std::make_pair("GENERATED_BY", "David Budil"));
            //
            QList<QStringList> finalQueries;
            finalQueries.append(it.queryList());
            m_XLS.generateFile(daily.getXlsTemplatePath(),
                               daily.getAttachName(),
                               genInfo,
                               finalQueries);
         }
         if(daily.getCsvAttach()){
            QSqlQuery resultCSV = it.getResult();
            m_CSV.generateFile(daily.getCsvTemplatePath(),
                               daily.getAttachName(),
                               resultCSV);
         }
         ++count;
      }else{
         qInfo(logInfo()) << "Failed to generate query: " + it.getName() + " : " + it.getResult().lastError().text();
      }
      //SEND TO EMAIL/POSTMAN QUEUE
   }
}
void Export::m_generateWeekly(const WeeklySchedule & weekly,
                              QQueue<SQLquery> & queries,
                              SQLParameter & param,
                              QSqlDatabase & db,
                              QDateTime & currentTime,
                              quint32 & count){
   for(auto & it : queries){
      QList<std::pair<QString, QString>> genInfo;
      QDateTime tmp(currentTime);
      QDateTime tmp2(currentTime);
      genInfo.append(std::make_pair("CURRENT_DATE", QDate().currentDate().toString("dd.MM.yyyy")));
      for(quint32 i = 0; i < quint32(param.getCount()); ++i){
         QString tmpParam1;
         tmpParam1 = "#PARAMETER" + QString(i+1);
         it.bindParameter(tmpParam1, param.getParameters()[i]);
      }
      tmp.setTime(weekly.getTime());
      tmp2.setTime(weekly.getTime());
      tmp = tmp.addDays(-7);
      tmp2 = tmp2.addSecs(-1);
      genInfo.append(std::make_pair("DATE_TIME_FROM_TO",
                                    "FROM " + tmp.toString("dd.MM.yy hh:mm") +
                                    " TO " + tmp2.toString("dd.MM.yy hh:mm")));
      it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
      it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));

      it.generateQuery(db);
      it.executeQuery();
      //export as CSV or XLSX...
      if(it.getResult().isActive()){
         if(weekly.getXlsAttach()){
            //NOT SURE ABOUT THIS LINE
            genInfo.append(std::make_pair("GENERATED_BY", "David Budil"));
            //
            QList<QStringList> finalQueries;
            finalQueries.append(it.queryList());
            m_XLS.generateFile(weekly.getXlsTemplatePath(),
                               weekly.getAttachName(),
                               genInfo,
                               finalQueries);
         }
         if(weekly.getCsvAttach()){
            QSqlQuery resultCSV = it.getResult();
            m_CSV.generateFile(weekly.getCsvTemplatePath(),
                               weekly.getAttachName(),
                               resultCSV);
         }
         ++count;
      }else{
         qInfo(logInfo()) << "Failed to generate query: " + it.getName() + " : " + it.getResult().lastError().text();
      }
      //SEND TO EMAIL/POSTMAN QUEUE
   }

}
void Export::m_generateMonthly(const MonthlySchedule & monthly,
                               QQueue<SQLquery> & queries,
                               SQLParameter & param,
                               QSqlDatabase & db,
                               QDateTime & currentTime,
                               quint32 & count){
   for(auto & it : queries){
      QList<std::pair<QString, QString>> genInfo;
      QDateTime tmp(currentTime);
      QDateTime tmp2(currentTime);
      genInfo.append(std::make_pair("CURRENT_DATE", QDate().currentDate().toString("dd.MM.yyyy")));
      for(quint32 i = 0; i < quint32(param.getCount()); ++i){
         QString tmpParam1;
         tmpParam1 = "#PARAMETER" + QString(i+1);
         it.bindParameter(tmpParam1, param.getParameters()[i]);
      }
      tmp.setTime(monthly.getTime());
      tmp2.setTime(monthly.getTime());
      tmp = tmp.addMonths(-1);
      tmp2 = tmp2.addSecs(-1);
      genInfo.append(std::make_pair("DATE_TIME_FROM_TO",
                                    "FROM " + tmp.toString("dd.MM.yy hh:mm") +
                                    " TO " + tmp2.toString("dd.MM.yy hh:mm")));
      it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
      it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));

      it.generateQuery(db);
      it.executeQuery();
      //export as CSV or XLSX...
      if(it.getResult().isActive()){
         if(monthly.getXlsAttach()){
            //NOT SURE ABOUT THIS LINE
            genInfo.append(std::make_pair("GENERATED_BY", "David Budil"));
            //
            QList<QStringList> finalQueries;
            finalQueries.append(it.queryList());
            m_XLS.generateFile(monthly.getXlsTemplatePath(),
                               monthly.getAttachName(),
                               genInfo,
                               finalQueries);
         }
         if(monthly.getCsvAttach()){
            QSqlQuery resultCSV = it.getResult();
            m_CSV.generateFile(monthly.getCsvTemplatePath(),
                               monthly.getAttachName(),
                               resultCSV);
         }
         ++count;
      }else{
         qInfo(logInfo()) << "Failed to generate query: " + it.getName() + " : " + it.getResult().lastError().text();
      }
      //SEND TO EMAIL/POSTMAN QUEUE
   }
}
