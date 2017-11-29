#ifndef REPORTER_H
#define REPORTER_H

#include <QMainWindow>
#include <QList>
#include <sqlcontrol.h>
#include <setup.h>
#include <QDebug>
#include <QMap>
#include <QWidget>
#include <QBoxLayout>
#include <QToolButton>
#include <QFont>
#include <QtSql>

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

   private:
      Ui::Reporter *ui;
      SQLControl m_mainSQL;
      Setup m_Setup;
      QString m_nameKey;
      qint32 m_selectedParam;
      qint32 m_paramKey;
      QToolButton * tmp;
      //Print query result to the table
      void m_displaySQLResult(const QString & name);
      //used to generate result from current selected query
      void m_generateQuery(const QString & name);
      //function to add query
      void m_addQuery(const QString & queryText, const QString & queryName, const QString & paramName);
      //functions to determine which button was clicked and select the stored data based on it
      void m_scrollQueryClicked();
      void m_scrollParamClicked();
      //Function to load default query
      void m_defaultQuery();
      //Function to save selected data
      void m_saveQuery();
      void m_saveParameter();
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
      void m_clearParam();
};

#endif // REPORTER_H
