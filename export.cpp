#include "export.h"
#include "log.h"

Export::Export() = default;

Export::~Export() = default;

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
      m_shiftDayReset(it->getShift(), currentTime);
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
      qInfo(logInfo()) << "Succesfully generated " + QVariant(exportCount).toString() + " datasets.";
   }
}
void Export::m_shiftDayReset(ShiftSchedule & shift, const QDateTime & current){
   QDateTime tmp = current;
   tmp.setTime(shift.getFrom0());
   shift.setDate0(tmp);
   tmp.setTime(shift.getTo0());
   shift.setDate1(tmp);
   tmp.setTime(shift.getFrom1());
   shift.setDate2(tmp);
   tmp = shift.getDate0();
   tmp = tmp.addDays(1);
   shift.setDate3(tmp);
}
void Export::customExport(CustomScheduling & exportData,
                          QQueue<SQLquery> & queries,
                          QQueue<SQLParameter> & parameters,
                          QSqlDatabase & db,
                          qint32 & customInterval){
   QDateTime from;
   QDateTime to;
   ShiftSchedule shift;
   DailySchedule daily;
   WeeklySchedule weekly;
   MonthlySchedule monthly;
   quint32 exportCount = 0;

   from = exportData.m_From;
   to = exportData.m_To;
   m_createTempScheduling(exportData, shift, daily, weekly, monthly);

   while(!(from > to)){
      m_shiftDayReset(shift, from);
      if(exportData.m_useParameters){
         //CHYBA JE VE 3. INTERVALU, PROBIHA JEN 8:00 -> 16:00 -> 24:00 a "24 -> 8:00" NEPROBEHNE
         //NA SHIFT SE MI CELKOVE NECO NELIBI -> RADSI CELE PREPSAT
         if(exportData.m_Shift && shift.generateShiftData(from)){
            for(auto & it : parameters){
               m_generateShift(shift, queries, it, db, from, exportCount);
            }
         }
         //IS WORKING
         if(exportData.m_Daily && daily.generateDailyData(from)){
            for(auto & it : parameters){
               m_generateDaily(daily, queries, it, db, from, exportCount);
            }
         }
         //IS WORKING
         if(exportData.m_Weekly && weekly.generateWeeklyData(from)){
            for(auto & it : parameters){
               m_generateWeekly(weekly, queries, it, db, from, exportCount);
            }
         }
         //IS WORKING
         if(exportData.m_Monthly && monthly.generateMonthlyData(from)){
            for(auto & it : parameters){
               m_generateMonthly(monthly, queries, it, db, from, exportCount);
            }
         }
      }else{
         //work around to not use any parameters -> use empy parameter as arguement
         SQLParameter tmp(0);

         if(exportData.m_Shift && shift.generateShiftData(from)){
            m_generateShift(shift, queries, tmp, db, from, exportCount);
         }
         if(exportData.m_Daily && daily.generateDailyData(from)){
            m_generateDaily(daily, queries, tmp, db, from, exportCount);
         }
         if(exportData.m_Weekly && weekly.generateWeeklyData(from)){
            m_generateWeekly(weekly, queries, tmp, db, from, exportCount);
         }
         if(exportData.m_Monthly && monthly.generateMonthlyData(from)){
            m_generateMonthly(monthly, queries, tmp, db, from, exportCount);
         }
      }

      from = from.addMSecs(customInterval);
   }
   if(exportCount != 0){
      qInfo(logInfo()) << "Succesfully generated " + QVariant(exportCount).toString() + " datasets.";
   }
}

void Export::runXLSGenerator(){
   auto xlsGenerator = new QProcess;
   QString filePath;
   filePath = QDir::currentPath() + "xlsxGenerator.exe";
   xlsGenerator->start(filePath);
}

void Export::m_createTempScheduling(CustomScheduling & exportData,
                                    ShiftSchedule & shift,
                                    DailySchedule & daily,
                                    WeeklySchedule & weekly,
                                    MonthlySchedule & monthly){
   if(exportData.m_Shift){
      for(qint32 i = 0; i < 7; i++){
         shift.setDays(i, exportData.m_shiftDays[i]);
      }
      shift.setActive(true);
      shift.setAttachName(exportData.m_ShiftAttachName);
      shift.setCsvAttach(exportData.m_shiftCSV);
      shift.setSubjName(exportData.m_ShiftSubjectName);
      shift.setXlsAttach(exportData.m_shiftXLS);
      shift.setXlsTemplatePath(exportData.m_ShiftxlsTemplatePath);
      shift.setFrom0(exportData.m_Shift1);
      shift.setTo0(exportData.m_Shift2);
      shift.setFrom1(exportData.m_Shift3);
   }
   if(exportData.m_Daily){
      for(qint32 i = 0; i < 7; i++){
         daily.setDays(i, exportData.m_dailyDays[i]);
      }
      daily.setActive(true);
      daily.setAttachName(exportData.m_DailyAttachName);
      daily.setCsvAttach(exportData.m_dailyCSV);
      daily.setXlsAttach(exportData.m_dailyXLS);
      daily.setSubjName(exportData.m_DailySubjectName);
      daily.setXlsTemplatePath(exportData.m_ShiftxlsTemplatePath);
      daily.setTime(exportData.m_Daily1);
   }

   if(exportData.m_Weekly){
      weekly.setDay(exportData.m_weeklyDay);
      weekly.setActive(true);
      weekly.setAttachName(exportData.m_WeeklyAttachName);
      weekly.setCsvAttach(exportData.m_weeklyCSV);
      weekly.setXlsAttach(exportData.m_weeklyXLS);
      weekly.setXlsTemplatePath(exportData.m_WeeklyxlsTemplatePath);
      weekly.setSubjName(exportData.m_WeeklySubjectName);
      weekly.setTime(exportData.m_Weekly1);
   }
   if(exportData.m_Monthly){
      monthly.setActive(true);
      monthly.setDay(exportData.m_monthlyDay);
      monthly.setCsvAttach(exportData.m_monthlyCSV);
      monthly.setXlsAttach(exportData.m_monthlyXLS);
      monthly.setAttachName(exportData.m_MonthlyAttachName);
      monthly.setXlsTemplatePath(exportData.m_MonthlyxlsTemplatePath);
      monthly.setSubjName(exportData.m_MonthlySubjectName);
      monthly.setTime(exportData.m_Monthly1);
   }

}

void Export::m_generateShift(ShiftSchedule & shift,
                             QQueue<SQLquery> & queries,
                             SQLParameter & param,
                             QSqlDatabase & db,
                             QDateTime & currentTime,
                             quint32 & count){
   //need to define which way to format parameters . using #parameter1 - 5 for now
   for(auto & it : queries){
      QList<std::pair<QString, QString>> genInfo;
      QDateTime tmp(currentTime);
      QDateTime tmp2(currentTime);
      genInfo.append(std::make_pair("CURRENT_DATE", QDate().currentDate().toString("dd.MM.yyyy")));
      for(qint32 i = 0; i < param.getCount(); ++i){
         QString tmpParam1;
         tmpParam1 = "#PARAMETER" + QString(i+1);
         it.bindParameter(tmpParam1, param.getParameters()[i]);
      }
      if(shift.getDone0()){
//         tmp.setTime(shift.getTo0());
         tmp = shift.getDate1();
//         tmp2.setTime(shift.getFrom1());
         tmp2 = shift.getDate2();
         tmp2 = tmp2.addSecs(-1);
         genInfo.append(std::make_pair("DATE_TIME_FROM_TO",
                                       "FROM " + tmp.toString("dd.MM.yy hh:mm") +
                                       " TO " + tmp2.toString("dd.MM.yy hh:mm")));
         it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
         it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));
         qInfo(logInfo()) << "generating from: " + tmp.toString() + " to " + tmp2.toString();
      }else if(shift.getDone1()){
//         tmp.setTime(shift.getFrom0());
         tmp = shift.getDate2();
//         tmp2.setTime(shift.getTo0());
         tmp2 = shift.getDate3();
         tmp2 = tmp2.addSecs(-1);
         genInfo.append(std::make_pair("DATE_TIME_FROM_TO",
                                       "FROM " + tmp.toString("dd.MM.yy hh:mm") +
                                       " TO " + tmp2.toString("dd.MM.yy hh:mm")));
         it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
         it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));
         qInfo(logInfo()) << "generating from: " + tmp.toString() + " to " + tmp2.toString();
      }else if(shift.getDone2()){
//         tmp.setTime(shift.getFrom0());
         tmp = shift.getDate0();
//         tmp2.setTime(shift.getTo0());
         tmp2 = shift.getDate1();
         if(tmp2.time() == QTime().fromString("23:59")){
            tmp2 = tmp2.addSecs(59);
         }else{
            tmp2 = tmp2.addSecs(-1);
         }
         genInfo.append(std::make_pair("DATE_TIME_FROM_TO",
                                       "FROM " + tmp.toString("dd.MM.yy hh:mm") +
                                       " TO " + tmp2.toString("dd.MM.yy hh:mm")));
         it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
         it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));
         qInfo(logInfo()) << "generating from: " + tmp.toString() + " to " + tmp2.toString();
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
            //            runXLSGenerator();
            m_XLS.readResult();
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
      QStringList emailAdresses = shift.emailAdresses();
      for(const auto & it : emailAdresses){
         //SEND TO EMAIL/POSTMAN QUEUE
      }
   }
}
void Export::m_generateDaily(DailySchedule & daily,
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
      for(qint32 i = 0; i < param.getCount(); ++i){
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
            //            runXLSGenerator();
            m_XLS.readResult();
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
      QStringList emailAdresses = daily.emailAdresses();
      for(auto & it : emailAdresses){
         //SEND TO EMAIL/POSTMAN QUEUE
      }
   }
}
void Export::m_generateWeekly(WeeklySchedule & weekly,
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
      for(qint32 i = 0; i < param.getCount(); ++i){
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
            //            runXLSGenerator();
            m_XLS.readResult();
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
      QStringList emailAdresses = weekly.emailAdresses();
      for(auto & it : emailAdresses){
         //SEND TO EMAIL/POSTMAN QUEUE
      }
   }

}
void Export::m_generateMonthly(MonthlySchedule & monthly,
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
      for(qint32 i = 0; i < param.getCount(); ++i){
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
            //            runXLSGenerator();
            m_XLS.readResult();
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
      QStringList emailAdresses = monthly.emailAdresses();
      for(auto & it : emailAdresses){
         //SEND TO EMAIL/POSTMAN QUEUE
      }
   }
}
