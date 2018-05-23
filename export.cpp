#include "export.h"
#include "log.h"
#include "sqlstorage.h"

Export::Export() = default;

Export::~Export() = default;

ExportXLS & Export::getXLS(){
   return m_XLS;
}
ExportCSV & Export::getCSV(){
   return m_CSV;
}
ExportHTML & Export::getHTML(){
   return m_HTML;
}
void Export::handleExport(QQueue<QSharedPointer<Scheduling>> & intervalsToHandle,
                          QQueue<SQLquery> & queries,
                          QQueue<QSharedPointer<SQLParameter>> & parameters,
                          QSqlDatabase & db){
   QDateTime currentTime = QDateTime().currentDateTime();
   quint32 exportCount = 0;
   QFile resultFile(QDir::currentPath() + "/ask_attachment_final.txt");
   resultFile.remove();

   for(auto & it : intervalsToHandle){
      if(!it->getParameters().isEmpty()){
         m_shiftDayReset(it->getShift(), currentTime);

         if(it->getShift().generateShiftData(currentTime) && it->getShift().getActive()){
            for(auto & itPar : it->getParameters()){
               //               qInfo(logInfo()) << "Generating with parameters: " + itPar->getParameters().join("-");
               m_generateShift(it->getShift(), queries, itPar, db, currentTime);
            }
         }
         if(it->getDaily().generateDailyData(currentTime) && it->getDaily().getActive()){
            for(auto & itPar : it->getParameters()){
               //               qInfo(logInfo()) << "Generating with parameters: " + itPar->getParameters().join("-");
               m_generateDaily(it->getDaily(), queries, itPar, db, currentTime);
            }
         }
         if(it->getWeekly().generateWeeklyData(currentTime) && it->getWeekly().getActive()){
            for(auto & itPar : it->getParameters()){
               //               qInfo(logInfo()) << "Generating with parameters: " + itPar->getParameters().join("-");
               m_generateWeekly(it->getWeekly(), queries, itPar, db, currentTime);
            }
         }
         if(it->getMonthly().generateMonthlyData(currentTime) && it->getMonthly().getActive()){
            for(auto & itPar : it->getParameters()){
               //               qInfo(logInfo()) << "Generating with parameters: " + itPar->getParameters().join("-");
               m_generateMonthly(it->getMonthly(), queries, itPar, db, currentTime);
            }
         }
      }else{
         QSharedPointer<SQLParameter> tmp = QSharedPointer<SQLParameter>::create(0);
         m_shiftDayReset(it->getShift(), currentTime);

         if(it->getShift().generateShiftData(currentTime) && it->getShift().getActive()){
            m_generateShift(it->getShift(), queries, tmp, db, currentTime);
         }
         if(it->getDaily().generateDailyData(currentTime) && it->getDaily().getActive()){
            m_generateDaily(it->getDaily(), queries, tmp, db, currentTime);
         }
         if(it->getWeekly().generateWeeklyData(currentTime) && it->getWeekly().getActive()){
            m_generateWeekly(it->getWeekly(), queries, tmp, db, currentTime);
         }
         if(it->getMonthly().generateMonthlyData(currentTime) && it->getMonthly().getActive()){
            m_generateMonthly(it->getMonthly(), queries, tmp, db, currentTime);
         }
      }
   }
   if(exportCount != 0){
      qInfo(logInfo()) << "Succesfully generated " + QVariant(exportCount).toString() + " datasets.";
   }
}
void Export::m_shiftDayReset(ShiftSchedule & shift, const QDateTime & current){
   QDateTime tmp = current;

   tmp.setTime(shift.getTo0());
   tmp = tmp.addDays(1);
   shift.setDate0(tmp);

   tmp = current;
   tmp.setTime(shift.getFrom1());
   shift.setDate1(tmp);

   tmp = current;
   tmp.setTime(shift.getFrom0());
   shift.setDate2(tmp);

   tmp = current;
   tmp.setTime(shift.getTo0());
   shift.setDate3(tmp);
}

bool Export::validateQuery(QQueue<SQLquery> & query, SQLquery & currentQuery){
   for(auto & it : query){
      if(currentQuery.getName() == it.getMasterQueryName()){
         return false;
      }
   }
   return true;
}

void Export::setGeneratedBy(const QString & generatedBy){
   this->generatedBy = generatedBy;
}

void Export::asyncShiftGeneration(ShiftSchedule shift,
                                  QQueue<SQLquery> queries,
                                  QSharedPointer<SQLParameter> param,
                                  QSqlDatabase & db,
                                  QDateTime & currentTime){
   QtConcurrent::run(this, &Export::m_generateShift, shift, queries, param, db, currentTime);
}

void Export::asyncDailyGeneration(DailySchedule daily,
                                  QQueue<SQLquery> queries,
                                  QSharedPointer<SQLParameter> param,
                                  QSqlDatabase & db,
                                  QDateTime & currentTime){
   QtConcurrent::run(this, &Export::m_generateDaily, daily, queries, param, db, currentTime);
}

void Export::asyncWeeklyGeneration(WeeklySchedule weekly,
                                   QQueue<SQLquery> queries,
                                   QSharedPointer<SQLParameter> param,
                                   QSqlDatabase & db,
                                   QDateTime & currentTime){
   QtConcurrent::run(this, &Export::m_generateWeekly, weekly, queries, param, db, currentTime);
}

void Export::asyncMonthlGeneration(MonthlySchedule monthly,
                                   QQueue<SQLquery> queries,
                                   QSharedPointer<SQLParameter> param,
                                   QSqlDatabase & db,
                                   QDateTime & currentTime){
   QtConcurrent::run(this, &Export::m_generateMonthly, monthly, queries, param, db, currentTime);
}
void Export::customExport(QSharedPointer<CustomScheduling> exportData,
                          QQueue<SQLquery> & queries,
                          QQueue<QSharedPointer<SQLParameter>> & parameters,
                          QSqlDatabase & db,
                          qint32 & customInterval){
   QDateTime from;
   QDateTime to;
   ShiftSchedule shift;
   DailySchedule daily;
   WeeklySchedule weekly;
   MonthlySchedule monthly;
   quint32 shiftCount = 0;
   quint32 dailyCount = 0;
   quint32 weeklyCount = 0;
   quint32 monthlyCount = 0;
   QFile resultFile(QDir::currentPath() + "/ask_attachment_final.txt");
   QStringList tmpList;
   tmpList.append(exportData->parameter0);
   tmpList.append(exportData->parameter1);
   tmpList.append(exportData->parameter2);
   tmpList.append(exportData->parameter3);
   tmpList.append(exportData->parameter4);
   QSharedPointer<SQLParameter> tmpParameters = QSharedPointer<SQLParameter>::create(tmpList, tmpList.count());

   parameters.append(tmpParameters);
   resultFile.remove();

   from = exportData->m_From;
   to = exportData->m_To;
   m_createTempScheduling(exportData, shift, daily, weekly, monthly);
   //TODO: TEST CUSTOM PARAMETERS

   while(!(from > to)){
      m_shiftDayReset(shift, from);
      if(exportData->m_useParameters){
         //IS WORKING
         if(exportData->m_Shift && shift.generateShiftData(from)){
            for(auto & it : parameters){
               if(m_generateShift(shift, queries, it, db, from)){
                  ++shiftCount;
               }
            }
         }
         //IS WORKING
         if(exportData->m_Daily && daily.generateDailyData(from)){
            for(auto & it : parameters){
               if(m_generateDaily(daily, queries, it, db, from)){
                  ++dailyCount;
               }
            }
         }
         //IS WORKING
         if(exportData->m_Weekly && weekly.generateWeeklyData(from)){
            for(auto & it : parameters){
               if(m_generateWeekly(weekly, queries, it, db, from)){
                  ++weeklyCount;
               }
            }
         }
         //IS WORKING
         if(exportData->m_Monthly && monthly.generateMonthlyData(from)){
            for(auto & it : parameters){
               if(m_generateMonthly(monthly, queries, it, db, from)){
                  ++monthlyCount;
               }
            }
         }
      }else{
         //work around to not use any parameters -> use empty parameter as arguement
         QSharedPointer<SQLParameter> tmp = QSharedPointer<SQLParameter>::create(0);

         if(exportData->m_Shift && shift.generateShiftData(from)){
            if(m_generateShift(shift, queries, tmp, db, from)){
               ++shiftCount;
            }
         }
         if(exportData->m_Daily && daily.generateDailyData(from)){
            if(m_generateDaily(daily, queries, tmp, db, from)){
               ++dailyCount;
            }
         }
         if(exportData->m_Weekly && weekly.generateWeeklyData(from)){
            if(m_generateWeekly(weekly, queries, tmp, db, from)){
               ++weeklyCount;
            }
         }
         if(exportData->m_Monthly && monthly.generateMonthlyData(from)){
            if(m_generateMonthly(monthly, queries, tmp, db, from)){
               ++monthlyCount;
            }
         }
      }
      from = from.addMSecs(customInterval);
   }
   if(shiftCount != 0){
      qInfo(logInfo()) << "Succesfully generated " + QVariant(shiftCount).toString() + " shift datasets.";
   }
   if(dailyCount != 0){
      qInfo(logInfo()) << "Succesfully generated " + QVariant(dailyCount).toString() + " daily datasets.";
   }
   if(weeklyCount != 0){
      qInfo(logInfo()) << "Succesfully generated " + QVariant(weeklyCount).toString() + " weekly datasets.";
   }
   if(monthlyCount != 0){
      qInfo(logInfo()) << "Succesfully generated " + QVariant(monthlyCount).toString() + " monthly datasets.";
   }
}

void Export::m_createTempScheduling(QSharedPointer<CustomScheduling> exportData,
                                    ShiftSchedule & shift,
                                    DailySchedule & daily,
                                    WeeklySchedule & weekly,
                                    MonthlySchedule & monthly){
   if(exportData->m_Shift){
      for(qint32 i = 0; i < 7; i++){
         shift.setDays(i, exportData->m_shiftDays[i]);
      }
      shift.setActive(true);
      shift.setAttachName(exportData->m_ShiftAttachName);
      shift.setCsvAttach(exportData->m_shiftCSV);
      shift.setSubjName(exportData->m_ShiftSubjectName);
      shift.setXlsAttach(exportData->m_shiftXLS);
      shift.setXlsTemplatePath(exportData->m_ShiftxlsTemplatePath);
      shift.setFrom0(exportData->m_Shift1);
      shift.setTo0(exportData->m_Shift2);
      shift.setFrom1(exportData->m_Shift3);
   }
   if(exportData->m_Daily){
      for(qint32 i = 0; i < 7; i++){
         daily.setDays(i, exportData->m_dailyDays[i]);
      }
      daily.setActive(true);
      daily.setAttachName(exportData->m_DailyAttachName);
      daily.setCsvAttach(exportData->m_dailyCSV);
      daily.setXlsAttach(exportData->m_dailyXLS);
      daily.setSubjName(exportData->m_DailySubjectName);
      daily.setXlsTemplatePath(exportData->m_ShiftxlsTemplatePath);
      daily.setTime(exportData->m_Daily1);
   }

   if(exportData->m_Weekly){
      weekly.setDay(exportData->m_weeklyDay);
      weekly.setActive(true);
      weekly.setAttachName(exportData->m_WeeklyAttachName);
      weekly.setCsvAttach(exportData->m_weeklyCSV);
      weekly.setXlsAttach(exportData->m_weeklyXLS);
      weekly.setXlsTemplatePath(exportData->m_WeeklyxlsTemplatePath);
      weekly.setSubjName(exportData->m_WeeklySubjectName);
      weekly.setTime(exportData->m_Weekly1);
   }
   if(exportData->m_Monthly){
      monthly.setActive(true);
      monthly.setDay(exportData->m_monthlyDay);
      monthly.setCsvAttach(exportData->m_monthlyCSV);
      monthly.setXlsAttach(exportData->m_monthlyXLS);
      monthly.setAttachName(exportData->m_MonthlyAttachName);
      monthly.setXlsTemplatePath(exportData->m_MonthlyxlsTemplatePath);
      monthly.setSubjName(exportData->m_MonthlySubjectName);
      monthly.setTime(exportData->m_Monthly1);
   }
}

bool Export::m_generateShift(ShiftSchedule shift,
                             QQueue<SQLquery> queries,
                             QSharedPointer<SQLParameter> param,
                             QSqlDatabase & db,
                             QDateTime & currentTime){
   //need to define which way to format parameters . using #parameter1 - 5 for now
   //   for(auto it : queries){
   //      QList<std::pair<QString, QString>> genInfo;
   //      QDateTime tmp(currentTime);
   //      QDateTime tmp2(currentTime);

   //      genInfo.append(std::make_pair("CURRENT_DATE", QDate().currentDate().toString("dd.MM.yyyy")));

   //      if(!validateQuery(queries, it)){
   //         continue;
   //      }

   //      qInfo(logInfo()) << "Generating shift";

   //      for(qint32 i = 0; i < param.getCount(); ++i){
   //         QString tmpParam1;
   //         tmpParam1 = "#PARAMETER" + QVariant(i+1).toString();
   //         it.bindParameter(tmpParam1, param.getParameters()[i]);
   //      }
   //      shift.fixParameters(param, currentTime);

   //      if(shift.getDone0()){
   //         tmp = shift.getDate1();
   //         tmp2 = shift.getDate2();
   //         tmp2 = tmp2.addSecs(-1);
   //         genInfo.append(std::make_pair("DateTimeFromTo",
   //                                       tmp.toString("dd.MM.yy hh:mm") +
   //                                       " po " + tmp2.toString("dd.MM.yy hh:mm")));
   //         it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
   //         it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy.hh:mm"));
   //      }else if(shift.getDone1()){
   //         tmp = shift.getDate2();
   //         tmp2 = shift.getDate3();
   //         tmp2 = tmp2.addSecs(-1);
   //         genInfo.append(std::make_pair("DateTimeFromTo",
   //                                       tmp.toString("dd.MM.yy hh:mm") +
   //                                       " po " + tmp2.toString("dd.MM.yy hh:mm")));
   //         it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
   //         it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));
   //      }else if(shift.getDone2()){
   //         tmp = shift.getDate0();
   //         tmp = tmp.addDays(-2);
   //         tmp2 = shift.getDate1();
   //         tmp2 = tmp2.addSecs(-1);

   //         genInfo.append(std::make_pair("DateTimeFromTo",
   //                                       tmp.toString("dd.MM.yy hh:mm") +
   //                                       " po " + tmp2.toString("dd.MM.yy hh:mm")));
   //         it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
   //         it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));
   //      }

   //      QString tmpAttachName = shift.getAttachName();
   //      if(shift.getXlsAttach()){
   //         //NOT SURE ABOUT THIS LINE
   //         genInfo.append(std::make_pair("vygeneroval", generatedBy));
   //         //
   //         QList<QStringList> finalQueries;
   //         if(!it.getParam().isEmpty()){
   //            QStringList tmpList;
   //            tmpList.append(it.getParam());
   //            tmpList.append("");
   //            tmpList.append(it.getMasterQueryString());
   //            finalQueries.append(tmpList);
   //         }
   //         finalQueries.append(it.queryList());


   //         //data template path
   //         if(m_XLS.generateFile(shift.getXlsTemplatePath(),
   //                               tmpAttachName,
   //                               genInfo,
   //                               finalQueries) && showInfo){
   //            qInfo(logInfo()) << "Successfuly generated shift XLSX file.";
   //         }else if(showInfo){
   //            qWarning(logWarning()) << "Failed to generate shift XLSX file.";
   //         }

   //         m_XLS.readResult();
   //         //html email template path
   //         QString tmpAttachNameEmail = tmpAttachName + "EMAIL";
   //         if(m_XLS.generateFile(shift.getEmailTemplatePath(),
   //                               tmpAttachNameEmail,
   //                               genInfo,
   //                               finalQueries) && showInfo){
   //            qInfo(logInfo()) << "Successfuly generated shift XLSX html file.";
   //         }else if(showInfo){
   //            qWarning(logWarning()) << "Failed to generate shift XLSX html file.";
   //         }

   //         m_XLS.readResult();
   //      }
   //      //TODO: generate email

   //      it.generateQuery(db);
   //      it.forceExecuteQuery();


   //      //export as CSV or XLSX...
   //      if(it.getResult().isActive()){
   //         QSqlQuery resultCSV = it.getResult();

   //         if(shift.getCsvAttach()){
   //            m_CSV.generateFile(shift.getCsvTemplatePath(),
   //                               tmpAttachName,
   //                               resultCSV);
   //         }
   //         if(shift.getXlsAttach() || shift.getCsvAttach()){
   //            ++count;
   //         }
   //         //         m_HTML.generateFile(resultCSV, tmpAttachName);

   //      }else{
   //         qInfo(logInfo()) << "Failed to generate query: " + it.getName() + " : " + it.getResult().lastError().text();
   //         return false;
   //      }

   //      QStringList emailAdresses = shift.emailAdresses();
   //      for(const auto & it : emailAdresses){
   //         //SEND TO EMAIL/POSTMAN QUEUE
   //         //send first

   //      }
   //   }
   //   return false;
}
bool Export::m_generateDaily(DailySchedule daily,
                             QQueue<SQLquery> queries,
                             QSharedPointer<SQLParameter> param,
                             QSqlDatabase & db,
                             QDateTime & currentTime){
   //   for(auto & it : queries){
   //      QList<std::pair<QString, QString>> genInfo;
   //      QDateTime tmp(currentTime);
   //      QDateTime tmp2(currentTime);


   //      genInfo.append(std::make_pair("CURRENT_DATE", QDate().currentDate().toString("dd.MM.yyyy")));

   //      if(!validateQuery(queries, it)){
   //         continue;
   //      }

   //      qInfo(logInfo()) << "Generating daily";

   //      for(qint32 i = 0; i < param.getCount(); ++i){
   //         QString tmpParam1;
   //         tmpParam1 = "#PARAMETER" + QVariant(i+1).toString();
   //         it.bindParameter(tmpParam1, param.getParameters()[i]);
   //      }
   //      daily.fixParameters(param, currentTime);

   //      tmp.setTime(daily.getTime());
   //      tmp2.setTime(daily.getTime());
   //      tmp = tmp.addDays(-1);
   //      tmp2 = tmp2.addSecs(-1);
   //      genInfo.append(std::make_pair("DateTimeFromTo",
   //                                    tmp.toString("dd.MM.yy hh:mm") +
   //                                    " po " + tmp2.toString("dd.MM.yy hh:mm")));
   //      it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
   //      it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));

   //      QString tmpAttachName = daily.getAttachName();
   //      if(daily.getXlsAttach()){
   //         genInfo.append(std::make_pair("vygeneroval", generatedBy));

   //         QList<QStringList> finalQueries;

   //         if(!it.getParam().isEmpty()){
   //            QStringList tmpList;

   //            tmpList.append(it.getParam());
   //            tmpList.append("");
   //            tmpList.append(it.getMasterQueryString());
   //            finalQueries.append(tmpList);
   //         }

   //         finalQueries.append(it.queryList());

   //         //data template path
   //         if(m_XLS.generateFile(daily.getXlsTemplatePath(),
   //                               tmpAttachName,
   //                               genInfo,
   //                               finalQueries) && showInfo){
   //            qInfo(logInfo()) << "Successfuly generated daily XLSX file.";
   //         }else if(showInfo){
   //            qWarning(logWarning()) << "Failed to generate daily XLSX file.";
   //         }

   //         m_XLS.readResult();
   //         //html email template path
   //         QString tmpAttachNameEmail = tmpAttachName + "EMAIL";
   //         if(m_XLS.generateFile(daily.getEmailTemplatePath(),
   //                               tmpAttachNameEmail,
   //                               genInfo,
   //                               finalQueries) && showInfo){
   //            qInfo(logInfo()) << "Successfuly generated shift XLSX html file.";
   //         }else if(showInfo){
   //            qWarning(logWarning()) << "Failed to generate shift XLSX html file.";
   //         }


   //         m_XLS.readResult();
   //      }

   //      //TODO: generate email

   //      it.generateQuery(db);
   //      it.forceExecuteQuery();
   //      //export as CSV or XLSX...
   //      if(it.getResult().isActive()){
   //         QSqlQuery resultCSV = it.getResult();

   //         if(daily.getCsvAttach()){
   //            m_CSV.generateFile(daily.getCsvTemplatePath(),
   //                               tmpAttachName,
   //                               resultCSV);
   //         }
   //         if(daily.getXlsAttach() || daily.getCsvAttach()){
   //            ++count;
   //         }
   //         //         m_HTML.generateFile(resultCSV, tmpAttachName);

   //      }else{
   //         qInfo(logInfo()) << "Failed to generate query: " + it.getName() + " : " + it.getResult().lastError().text();
   //         return false;
   //      }

   //      QStringList emailAdresses = daily.emailAdresses();
   //      for(auto & it : emailAdresses){
   //         //SEND TO EMAIL/POSTMAN QUEUE
   //      }
   //   }
   //   return false;
}
bool Export::m_generateWeekly(WeeklySchedule weekly,
                              QQueue<SQLquery> queries,
                              QSharedPointer<SQLParameter> param,
                              QSqlDatabase & db,
                              QDateTime & currentTime){
   //   for(auto & it : queries){
   //      QList<std::pair<QString, QString>> genInfo;
   //      QDateTime tmp(currentTime);
   //      QDateTime tmp2(currentTime);
   //      genInfo.append(std::make_pair("CURRENT_DATE", QDate().currentDate().toString("dd.MM.yyyy")));

   //      if(!validateQuery(queries, it)){
   //         continue;
   //      }

   //      qInfo(logInfo()) << "Generating weekly";

   //      for(qint32 i = 0; i < param.getCount(); ++i){
   //         QString tmpParam1;
   //         tmpParam1 = "#PARAMETER" + QVariant(i+1).toString();
   //         it.bindParameter(tmpParam1, param.getParameters()[i]);
   //      }
   //      weekly.fixParameters(param, currentTime);

   //      tmp.setTime(weekly.getTime());
   //      tmp2.setTime(weekly.getTime());
   //      tmp = tmp.addDays(-7);
   //      tmp2 = tmp2.addSecs(-1);
   //      genInfo.append(std::make_pair("DateTimeFromTo",
   //                                    tmp.toString("dd.MM.yy hh:mm") +
   //                                    " po " + tmp2.toString("dd.MM.yy hh:mm")));
   //      it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
   //      it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));

   //      QString tmpAttachName = weekly.getAttachName();
   //      if(weekly.getXlsAttach()){
   //         //NOT SURE ABOUT THIS LINE
   //         genInfo.append(std::make_pair("vygeneroval", generatedBy));
   //         //
   //         QList<QStringList> finalQueries;
   //         if(!it.getParam().isEmpty()){
   //            QStringList tmpList;
   //            tmpList.append(it.getParam());
   //            tmpList.append("");
   //            tmpList.append(it.getMasterQueryString());
   //            finalQueries.append(tmpList);
   //         }
   //         finalQueries.append(it.queryList());

   //         //data template path
   //         if(m_XLS.generateFile(weekly.getXlsTemplatePath(),
   //                               tmpAttachName,
   //                               genInfo,
   //                               finalQueries) && showInfo){
   //            qInfo(logInfo()) << "Successfuly generated weekly XLSX file.";
   //         }else if(showInfo){
   //            qWarning(logWarning()) << "Failed to generate weekly XLSX file.";
   //         }

   //         m_XLS.readResult();
   //         //html email template path
   //         QString tmpAttachNameEmail = tmpAttachName + "EMAIL";
   //         if(m_XLS.generateFile(weekly.getEmailTemplatePath(),
   //                               tmpAttachNameEmail,
   //                               genInfo,
   //                               finalQueries) && showInfo){
   //            qInfo(logInfo()) << "Successfuly generated shift XLSX html file.";
   //         }else if(showInfo){
   //            qWarning(logWarning()) << "Failed to generate shift XLSX html file.";
   //         }

   //         m_XLS.readResult();
   //      }

   //      //TODO: generate email

   //      it.generateQuery(db);
   //      it.forceExecuteQuery();

   //      //export as CSV or XLSX...
   //      if(it.getResult().isActive()){
   //         QSqlQuery resultCSV = it.getResult();

   //         if(weekly.getCsvAttach()){
   //            m_CSV.generateFile(weekly.getCsvTemplatePath(),
   //                               tmpAttachName,
   //                               resultCSV);
   //         }
   //         if(weekly.getXlsAttach() || weekly.getCsvAttach()){
   //            ++count;
   //         }
   //         //         m_HTML.generateFile(resultCSV, tmpAttachName);

   //      }else{
   //         qInfo(logInfo()) << "Failed to generate query: " + it.getName() + " : " + it.getResult().lastError().text();
   //         return false;
   //      }

   //      QStringList emailAdresses = weekly.emailAdresses();
   //      for(auto & it : emailAdresses){
   //         //SEND TO EMAIL/POSTMAN QUEUE
   //      }
   //   }
   //   return false;
}
bool Export::m_generateMonthly(MonthlySchedule monthly,
                               QQueue<SQLquery> queries,
                               QSharedPointer<SQLParameter> param,
                               QSqlDatabase & db,
                               QDateTime & currentTime){
   //   for(auto & it : queries){
   //      QList<std::pair<QString, QString>> genInfo;
   //      QDateTime tmp(currentTime);
   //      QDateTime tmp2(currentTime);
   //      genInfo.append(std::make_pair("CURRENT_DATE", QDate().currentDate().toString("dd.MM.yyyy")));

   //      if(!validateQuery(queries, it)){
   //         continue;
   //      }

   //      qInfo(logInfo()) << "Generating monthly";

   //      for(qint32 i = 0; i < param.getCount(); ++i){
   //         QString tmpParam1;
   //         tmpParam1 = "#PARAMETER" + QVariant(i+1).toString();
   //         it.bindParameter(tmpParam1, param.getParameters()[i]);
   //      }
   //      monthly.fixParameters(param, currentTime);

   //      tmp.setTime(monthly.getTime());
   //      tmp2.setTime(monthly.getTime());
   //      tmp = tmp.addMonths(-1);
   //      tmp2 = tmp2.addSecs(-1);
   //      genInfo.append(std::make_pair("DateTimeFromTo",
   //                                    tmp.toString("dd.MM.yy hh:mm") +
   //                                    " po " + tmp2.toString("dd.MM.yy hh:mm")));
   //      it.bindParameter("#TIMEFROM", tmp.toString("dd.MM.yy hh:mm"));
   //      it.bindParameter("#TIMETO", tmp2.toString("dd.MM.yy hh:mm"));

   //      QString tmpAttachName = monthly.getAttachName();
   //      if(monthly.getXlsAttach()){
   //         //NOT SURE ABOUT THIS LINE
   //         genInfo.append(std::make_pair("vygeneroval", generatedBy));
   //         //
   //         QList<QStringList> finalQueries;
   //         if(!it.getParam().isEmpty()){
   //            QStringList tmpList;
   //            tmpList.append(it.getParam());
   //            tmpList.append("");
   //            tmpList.append(it.getMasterQueryString());
   //            finalQueries.append(tmpList);
   //         }
   //         finalQueries.append(it.queryList());

   //         //data template path
   //         if(m_XLS.generateFile(monthly.getXlsTemplatePath(),
   //                               tmpAttachName,
   //                               genInfo,
   //                               finalQueries) && showInfo){
   //            qInfo(logInfo()) << "Successfuly generated monthly XLSX file.";
   //         }else if(showInfo){
   //            qWarning(logWarning()) << "Failed to generate monthly XLSX file.";
   //         }

   //         m_XLS.readResult();
   //         //html email template path
   //         QString tmpAttachNameEmail = tmpAttachName + "EMAIL";
   //         if(m_XLS.generateFile(monthly.getEmailTemplatePath(),
   //                               tmpAttachNameEmail,
   //                               genInfo,
   //                               finalQueries) && showInfo){
   //            qInfo(logInfo()) << "Successfuly generated shift XLSX html file.";
   //         }else if(showInfo){
   //            qWarning(logWarning()) << "Failed to generate shift XLSX html file.";
   //         }

   //         m_XLS.readResult();
   //      }

   //      it.generateQuery(db);
   //      it.forceExecuteQuery();
   //      //export as CSV or XLSX...
   //      if(it.getResult().isActive()){
   //         QSqlQuery resultCSV = it.getResult();

   //         if(monthly.getCsvAttach()){
   //            m_CSV.generateFile(monthly.getCsvTemplatePath(),
   //                               tmpAttachName,
   //                               resultCSV);
   //         }
   //         if(monthly.getXlsAttach() || monthly.getCsvAttach()){
   //            ++count;
   //         }
   //         //         m_HTML.generateFile(resultCSV, tmpAttachName);

   //      }else{
   //         qInfo(logInfo()) << "Failed to generate query: " + it.getName() + " : " + it.getResult().lastError().text();
   //         return false;
   //      }

   //      QStringList emailAdresses = monthly.emailAdresses();
   //      for(auto & it : emailAdresses){
   //         //SEND TO EMAIL/POSTMAN QUEUE
   //      }
   //   }
   //   return false;
}
