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

      void on_saveEmailAdress_clicked();

      void on_toolButton_2_clicked();

      void on_toolButton_3_clicked();

   private:
      Ui::Reporter *ui;
      SQLControl m_mainSQL;
      Setup m_Setup;
      Log m_Log;
      Export m_Export;
      Scheduling m_Schedule;
      QString m_nameKey;
      qint32 m_selectedParam;
      qint32 m_paramKey;
      QToolButton * tmp;
      qint32 m_daysWeeklyIndex;
      bool m_queryActive;
      //Print query result to the table
      void m_displaySQLResult(const QString & name);
      //used to generate result from current selected query
      void m_generateQuery(const QString & name);
      //function to execute query
      void m_executeQuery(const QString & name);
      //function to add query
      void m_addQuery(const QString & queryText, const QString & queryName, const QString & paramName, bool mode, bool active);
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
      //Serialization
      void m_serializeQueries();
      void m_serializeParameters();
      //Deserialization
      void m_Deserialize();
      //loads .ini settings
      void m_Settings();
      //connects to db
      void m_ConnectDB();
      //add Parameters
      void m_addParameters(const QStringList & params, const qint32 & count);
      //utility function to get text from parameters input boxes
      void m_createParamList(QStringList & tmp, qint32 & tmpCount);
      QStringList m_loadParameters(QVector<qint32> & count);
      void m_loadMaster();
      void m_clearParam();
      void m_clearQuery();
      void m_loadSchedule();
      void m_displayShift();
      void m_displayDay();
      void m_displayWeekly();
      void m_displayMonthly();
      void m_displayCustom();
      void m_editShift();
      void m_editDay();
      void m_editWeekly();
      void m_editMonthly();
      void m_editCustom();
      void m_loadActiveShiftDays();
      void m_loadActiveDailyDays();
      void m_loadWeeklyDays();
      void m_editWeeklyDays();
      void m_editActiveShiftDays();
      void m_editActiveDailyDays();
      void m_serializeSchedule();
      void m_deserializeSchedule();
      void m_serializeGlobal();
      void m_saveSchedule();
      void m_generateXLS();
      void m_generateTemplateXLS();
      void m_testingQueryGen();
};

#endif // REPORTER_H
