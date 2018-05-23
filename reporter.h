#ifndef REPORTER_H
#define REPORTER_H

#include <QMainWindow>
#include <QDebug>
#include <QMap>
#include <QWidget>
#include <QBoxLayout>
#include <QToolButton>
#include <QFont>
#include <QtSql>
#include <QDateTime>
#include <QMenuBar>
#include <QMenu>
#include <QList>
#include <sqlcontrol.h>
#include <setup.h>
#include <scheduling.h>
#include <log.h>
#include <export.h>
#include <QFileDialog>
#include <QDir>
#include <QPair>
#include <xlsxdocument.h>
#include <QDesktopServices>
#include <QHotkey>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QMultiMap>
#include "tableinfo.h"
#include "customscheduling.h"
#include <QCloseEvent>
#include "customparametersquery.h"
#include "QtConcurrent"
#include "sqlparameter.h"
#include "QThread"
#include "QFuture"

namespace Ui{
   class Reporter;
} // namespace Ui
/*Main class of the application
 *Contains all other classes
 * */
class Reporter : public QMainWindow{
      Q_OBJECT

   public:
      //Constructor
      explicit Reporter(QWidget *parent = nullptr);
      //Destructor
      ~Reporter() override;
      void closeEvent(QCloseEvent * event) override;
      //Sets up default settings of the app
      void defaultSettings();

   private slots:
      //Shows or hides application
      void m_showHide();
      //Function to Generate selected query and print results to table
      void on_toolButton_clicked();
      //Function to add new query
      void on_newQuery_clicked();
      //Function to save the edit of current query
      void on_saveQuery_clicked();
      //Function to clear current text

      void on_paramNew_clicked();

      void on_paramEdit_clicked();

      void on_paramDelete_clicked();

      void on_queryDelete_clicked();

      void on_queryActive_stateChanged(int state);

      void on_tabWidget_2_tabBarClicked(int index);

      void on_saveScheduling_clicked();

      void on_shiftBrEMAIL_clicked();

      void on_shiftBrXLS_clicked();

      void on_shiftBrCSV_clicked();

      void on_dailyBrEMAIL_clicked();

      void on_dailyBrXLS_clicked();

      void on_dailyBrCSV_clicked();

      void on_weeklyBrEMAIL_clicked();

      void on_weeklyBrXLS_clicked();

      void on_weeklyBrCSV_clicked();

      void on_monthlyBrEMAIL_clicked();

      void on_monthlyBrXLS_clicked();

      void on_monthlyBrCSV_clicked();

      void on_toolButton_2_clicked();

      void on_toolButton_3_clicked();

      void on_newScheduling_clicked();

      void on_deleteScheduling_clicked();

      void on_shiftnewEmailAdress_clicked();

      void on_dailynewEmailAdress_clicked();

      void on_saveEmailAdress_7_clicked();

      void on_monthlynewEmailAdress_clicked();

      void on_shiftdeleteEmailAdress_clicked();

      void on_dailydeleteEmailAdress_clicked();

      void on_weeklydeleteEmailAdress_clicked();

      void on_monthlydeleteEmailAdress_clicked();

      void on_tableNames_clicked();

      void timerInterval();

      void on_toolButton_4_clicked();

      void on_param1_textEdited(const QString & arg1);

      void on_param2_textEdited(const QString & arg1);

      void on_param3_textEdited(const QString & arg1);

      void on_param4_textEdited(const QString & arg1);

      void on_param5_textEdited(const QString & arg1);

      void on_shiftActive_clicked();

      void on_shiftMondayActive_clicked();

      void on_shiftTuesdayActive_clicked();

      void on_shiftWednesdayActive_clicked();

      void on_shiftThursdayActive_clicked();

      void on_shiftFridayActive_clicked();

      void on_shiftSaturdayActive_clicked();

      void on_shiftSundayActive_clicked();

      void on_shiftFrom_userTimeChanged(const QTime & time);

      void on_shiftTo_userTimeChanged(const QTime & time);

      void on_shiftFrom_2_userTimeChanged(const QTime & time);

      void on_shiftAttach_textEdited(const QString & arg1);

      void on_shiftSubj_textEdited(const QString & arg1);

      void on_shiftEmail_textEdited(const QString & arg1);

      void on_shiftXLS_textEdited(const QString & arg1);

      void on_shiftCSV_textEdited(const QString & arg1);

      void on_shiftAttachXLS_clicked();

      void on_shiftAttachCSV_clicked();

      void on_shiftemailAdress_textEdited(const QString & arg1);

      void on_dailyActive_clicked();

      void on_dailyTime_userTimeChanged(const QTime & time);

      void on_dailyEmailAdress_textEdited(const QString & arg1);

      void on_dailyMondayActive_clicked();

      void on_dailyTuesdayActive_clicked();

      void on_dailyWednesdayActive_clicked();

      void on_dailyThursdayActive_clicked();

      void on_dailyFridayActive_clicked();

      void on_dailySaturdayActive_clicked();

      void on_dailySundayActive_clicked();

      void on_dailyAttach_textEdited(const QString & arg1);

      void on_dailySubj_textEdited(const QString & arg1);

      void on_dailyEmail_textEdited(const QString & arg1);

      void on_dailyXLS_textEdited(const QString & arg1);

      void on_dailyCSV_textEdited(const QString & arg1);

      void on_dailyAttachXLS_clicked();

      void on_dailyAttachCSV_clicked();

      void on_weeklyActive_clicked();

      void on_weeklyTime_userTimeChanged(const QTime & time);

      void on_weeklyDays_currentIndexChanged(int index);

      void on_weeklyAttach_textEdited(const QString & arg1);

      void on_weeklySubj_textEdited(const QString & arg1);

      void on_weeklyEmail_textEdited(const QString & arg1);

      void on_weeklyXLS_textEdited(const QString & arg1);

      void on_weeklyCSV_textEdited(const QString & arg1);

      void on_weeklyemailAdress_textEdited(const QString & arg1);

      void on_weeklyAttachXLS_clicked();

      void on_weeklyAttachCSV_clicked();

      void on_monthlyActive_clicked();

      void on_monthlyTIme_userTimeChanged(const QTime & time);

      void on_monthlyEmailAdress_textEdited(const QString & arg1);

      void on_monthlyAttach_textEdited(const QString & arg1);

      void on_monthlySubj_textEdited(const QString & arg1);

      void on_monthlyEmail_textEdited(const QString & arg1);

      void on_monthlyXLS_textEdited(const QString & arg1);

      void on_monthlyCSV_textEdited(const QString & arg1);

      void on_monthlyAttachXLS_clicked();

      void onMonthlyattachcsvClicked();

      void on_startTImer_clicked();

      void on_shiftGenerate_clicked();

      void on_dailyGenerate_clicked();

      void on_weeklyGenerate_clicked();

      void on_monthlyGenerate_clicked();

      void on_customParameters_clicked();

      void on_queryNameEdit_textEdited(const QString &arg1);

      void on_queryEdit_textChanged();

      void on_scheduleName_textEdited(const QString &arg1);

      void on_queryNameEdit_editingFinished();

      void on_queryParamEdit_editingFinished();

      void displaySQL();

   private:
      Ui::Reporter *ui;
      SQLControl m_mainSQL;
      QFuture<void> m_displayWatcher;
      Setup m_Setup;
      Export m_Export;
      QMap<qint32, Scheduling*> m_Schedule;
      QString m_nameKey;
      QString m_emailKey;
      qint32 m_selectedParam;
      qint32 m_paramKey;
      qint32 m_scheduleKey;
      qint32 m_scheduleCount;
      QToolButton * tmp;
      QHotkey * m_shwHide;
      qint32 m_daysWeeklyIndex;
      QTimer * m_Timer;
      qint32 m_TIMERINTERVAL;
      qint32 m_CUSTOMINTERVAL;
      QString m_generatedBy;
      QSharedPointer<CustomScheduling> instantSchedule;
      quint32 m_daysSinceCleanUp;
      QDate m_lastDay;
      QDateTime m_progressFrom;
      QDateTime m_progressTo;
      QMap<QString, QString> m_CustomParameters;
      QDateTime m_CustomParametersFrom;
      QDateTime m_CustomParametersTo;
      bool m_queryActive;
      bool m_generate;
      bool m_FinishedQueryDisplay;

      //Print query result to the table
      void m_displaySQLResult(const QString & name);
      //used to generate result from current selected query
      void m_generateQuery(const QString & name);
      //function to execute query
      void m_executeQuery(const QString & name);
      //functions to add
      void m_addQuery(const QString & queryText, const QString & queryName, const QString & paramName, bool mode, bool active);
      void m_addSchedule(const QString & name);
      void m_addParameters(const QStringList & params, const qint32 & count);
      void m_addShiftScheduleEmail(const QString & email);
      void m_addDailyScheduleEmail(const QString & email);
      void m_addWeeklyScheduleEmail(const QString & email);
      void m_addMonthlyScheduleEmail(const QString & email);
      //functions to determine which button was clicked and select the stored data based on it
      void m_scrollQueryClicked();
      void m_scrollParamClicked();
      //Function to load default query
      void m_defaultQuery();
      //Function to save selected data
      void m_saveQuery();
      void m_saveParameter();
      //functions to delete selected data
      void m_deleteParam();
      void m_deleteQuery();
      void m_deleteSchedule();
      void m_deleteEmails();
      void m_deleteParameters();
      //Serialization
      void m_serializeQueries();
      //Deserialization
      void m_Deserialize();
      //loads .ini settings
      void m_Settings();
      //displays selected button
      void m_loadColorSchedule();
      void m_loadColorScheduleEmail();
      void m_loadColorScheduleParam();
      void m_loadColorQueries();
      //connects to db
      void m_ConnectDB();
      //utility function to get text from parameters input boxes
      void m_createParamList(QStringList & tmp, qint32 & tmpCount);
      void m_loadMaster();
      void m_loadEmails();
      void m_clearParam();
      void m_clearQuery();
      void m_clearSchedule();
      void m_clearShift();
      void m_clearDaily();
      void m_clearWeekly();
      void m_clearMonthly();
      void m_loadSchedule();
      void m_loadShiftEmail();
      void m_loadDailyEmail();
      void m_loadWeeklyEmail();
      void m_loadMonthlyEmail();
      void m_loadScheduleParameters();
      void m_loadIndividualParameters(const qint32 & paramCount);
      void m_displayShift(qint32 keyString);
      void m_displayDay(qint32 keyString);
      void m_displayWeekly(qint32 keyString);
      void m_displayMonthly(qint32 keyString);
      void m_displayCustom(qint32 keyString);
      void m_editShift(qint32 keyString);
      void m_editDay(qint32 keyString);
      void m_editWeekly(qint32 keyString);
      void m_editMonthly(qint32 keyString);
      void m_editCustom();
      void m_loadActiveShiftDays(qint32 keyString);
      void m_loadActiveDailyDays(qint32 keyString);
      void m_editActiveShiftDays(qint32 keyString);
      void m_editActiveDailyDays(qint32 keyString);
      void m_serializeSchedule();
      void m_deserializeSchedule();
      void m_serializeGlobal();
      void m_saveSchedule();
      void m_generateCSV();
      void m_generateXLS();
      void m_generateTemplateXLS();
      void m_testingQueryGen();
      bool m_noSchedule();
      bool m_validateEmail(const QString & email);
      bool m_validateScheduleName(const QString & name);
      bool m_validateScheduleNameSaving(const QString & name);
      QStringList m_getColumnNames(const QString & tableName);
      void m_SetTimer(qint32 interval);
      //simple debug function used during development
      void m_debugNotification(const QString & toDisplay);
      bool m_firstQuery;
      void m_PauseTimer();
      bool m_validateQuerySelected();
};

#endif // REPORTER_H
