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

namespace Ui{
   class Reporter;
}
/*Main class of the application
 *Contains all other classes
 * */
class Reporter : public QMainWindow{
      Q_OBJECT

   public:
      //Constructor
      explicit Reporter(QWidget *parent = 0);
      //Destructor
      ~Reporter();
      //Sets up default settings of the app
      void defaultSettings();

   private slots:
      //Shows or hides application
      void m_showHide();
      //Function to connect to DB triggered by click connect button
      void on_dbConnect_clicked();
      //Function to Generate selected query and print results to table
      void on_toolButton_clicked();
      //Function to add new query
      void on_newQuery_clicked();
      //Function to save the edit of current query
      void on_saveQuery_clicked();
      //Function to clear current text
      void on_clearQuery_clicked();

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

      void on_paramTest_clicked();

      void on_tableNames_clicked();

      void timerInterval();

      void on_toolButton_4_clicked();

   private:
      Ui::Reporter *ui;
      SQLControl m_mainSQL;
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
      QTimer* m_Timer;

      bool m_queryActive;
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
      //Serialization
      void m_serializeQueries();
      void m_serializeParameters();
      //Deserialization
      void m_Deserialize();
      //loads .ini settings
      void m_Settings();
      //connects to db
      void m_ConnectDB();
      //utility function to get text from parameters input boxes
      void m_createParamList(QStringList & tmp, qint32 & tmpCount);
      QStringList m_loadParameters(QVector<qint32> & count);
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
      QStringList m_getColumnNames(const QString & tableName);
      void m_SetTimer(qint32 interval);
      //simple debug function used during development
      void m_debugNotification(const QString & toDisplay);
};

#endif // REPORTER_H
