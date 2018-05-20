#include "log.h"
#include "reporter.h"
#include "ui_reporter.h"

//Constructor
Reporter::Reporter(QWidget *parent)
   : QMainWindow(parent),
     ui(new Ui::Reporter),
     m_paramKey(0),
     m_scheduleKey(0),
     m_scheduleCount(0),
     tmp(nullptr),
     m_Timer(nullptr),
     m_daysSinceCleanUp(0),
     m_lastDay(QDate::currentDate()),
     m_queryActive(false),
     m_firstQuery(false),
     m_generate(false){
   qInfo(logInfo()) << "Application started.";
   m_Setup.loadIni();
   m_TIMERINTERVAL = m_Setup.getSettings().timerInterval;
   m_CUSTOMINTERVAL = m_Setup.getSettings().customInterval;
   m_generatedBy = m_Setup.getSettings().generatedByUser;
   ui->setupUi(this);
   //   ui->queryNameEdit->setText("Query Name");
   //   ui->queryParamEdit->setText("Master name");
   ui->queryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
   //   QFont newFont("DejaVu Sans Mono", 11, QFont::Normal, true);
   //   setFont(newFont);
   ui->weeklyDays->setMaximumHeight(100);
   ui->monthlyDays->setMaximumHeight(100);
   m_shwHide = new QHotkey(QKeySequence(m_Setup.getSettings().hotKey), true);
   connect(m_shwHide, SIGNAL(activated()), this, SLOT(m_showHide()));
   ui->toolButton_2->setVisible(false);
   ui->paramTest->setVisible(false);
   ui->tabWidget_2->removeTab(4);
   m_lastDay = QDate::currentDate();
   m_daysSinceCleanUp = 0;
   m_selectedParam = -1;
   ui->tabWidget->setStyleSheet("QTabBar::tab { height: 50px; width: 150px; }");
   ui->tabWidget_2->setStyleSheet("QTabBar::tab { height: 30px; width: 120px; }");
   m_Timer = new QTimer;
   //custom is disabled, waiting for future implementation
}
//Destructor
Reporter::~Reporter(){
   m_Setup.saveIni();
   delete ui;
   delete m_shwHide;
   qDeleteAll(m_Schedule);
   delete m_Timer;
   delete instantSchedule;
   qInfo(logInfo()) << "Application shutdown.";
}
//Shows or hides application on key shortcut pressed
void Reporter::m_showHide(){
   if(isVisible()){
      hide();
   }else{
      show();
   }
}
//Function to connect to DB triggered by click connect button
//Print query result to the table
void Reporter::m_displaySQLResult(const QString & name){
   m_mainSQL.setQueryModel(name);
   ui->queryTable->clearSpans();
   ui->queryTable->setModel(m_mainSQL.getResult());
   ui->queryTable->setSortingEnabled(true);
   ui->sqlDataCount->setText(QString::number(m_mainSQL.getStorage().getQueries()[name]->getQueryResultRows()));
}
//Function to Generate selected query and print results to table
void Reporter::on_toolButton_clicked(){
   if(m_validateQuerySelected()){
      if(!m_mainSQL.getDatabase().getDatabase().open()){
         qWarning(logWarning()) << "Can not run SQL query due to no Database connection.";
         QMessageBox::critical(this, QObject::tr("Database error"),
                               "Not connected to database");
      }else{
         m_testingQueryGen();
         m_saveQuery();
      }
   }
}

void Reporter::m_executeQuery(const QString & name){
   //   m_mainSQL.getStorage().executeQuery(name);
}

//used to generate result from current selected query
void Reporter::m_generateQuery(const QString & name){
   //   m_mainSQL.getStorage().generateQuery(name, m_mainSQL.getDatabase().getDatabase());
   //look into this
}
//Function to add new query
void Reporter::on_newQuery_clicked(){
   if(m_mainSQL.getStorage().getQueries().count() == 0){
      m_addQuery(ui->queryEdit->toPlainText(),
                 ui->queryNameEdit->text(),
                 ui->queryParamEdit->text(),
                 true,
                 m_queryActive);
      m_loadColorQueries();
   }else{
      m_saveQuery();
      m_clearQuery();
      m_addQuery("SQL query text",
                 "New Query",
                 ui->queryParamEdit->text(),
                 true,
                 m_queryActive);
      m_nameKey = ui->queryNameEdit->text();
      m_loadColorQueries();
   }
}
//Function to save the edit of current query
void Reporter::on_saveQuery_clicked(){
   m_saveQuery();
}
//Function to save query
void Reporter::m_saveQuery(){
   QString queryText;
   QString queryName;
   QString paramName;
   QString lastName;

   queryText = ui->queryEdit->toPlainText();
   queryName = ui->queryNameEdit->text();
   paramName = ui->queryParamEdit->text();

   if((!m_mainSQL.getStorage().getQueries().contains(paramName)) && (!paramName.isEmpty())){
      QMessageBox::critical(this, QObject::tr("New Query Error"), "No query with specified master param name exists.");
   }else if(m_nameKey.isEmpty()){
      QMessageBox::critical(this, QObject::tr("Query Edit Error"), "No query selected.");
      /*}else if(queryName != m_mainSQL.getStorage().getQueries()[m_nameKey]->getName()
            && m_mainSQL.getStorage().getQueries()){
      QMessageBox::critical(this, QObject::tr("Master query name conflict."), "Another query is using this one as detail query, remove it before renaming this one.")*/
   }else{
      if(m_mainSQL.getStorage().addQuery(queryText, queryName, paramName, m_queryActive, false, false)){
         tmp = ui->scrollAreaWidgetContents->findChild<QToolButton *>(m_nameKey);
         tmp->setObjectName(queryName);
         tmp->setText(queryName);
         lastName = m_mainSQL.getStorage().getQueries()[m_nameKey]->getName();
         m_mainSQL.getStorage().getQueries().remove(m_nameKey);
         m_nameKey = queryName;
      }else{
         m_mainSQL.getStorage().getQueries()[m_nameKey]->setMasterQueryName(paramName);
         m_mainSQL.getStorage().getQueries()[m_nameKey]->setOriginalQuery(queryText);
         m_mainSQL.getStorage().getQueries()[m_nameKey]->setIsActive(m_queryActive);
         ui->queryEdit->document()->setPlainText(queryText);
         lastName = m_mainSQL.getStorage().getQueries()[m_nameKey]->getName();
         ui->queryParamEdit->setText(paramName);
      }
   }
   //fixes master parameter names in detail queries if the name changes
   for(auto & it : m_mainSQL.getStorage().getQueries()){
      if(it->getMasterQueryName() == lastName){
         it->setMasterQueryName(m_mainSQL.getStorage().getQueries()[m_nameKey]->getName());
         it->setMasterQuery(m_mainSQL.getStorage().getQueries()[it->getMasterQueryName()]->getOriginalQuery());
      }
   }
   m_serializeQueries();
   m_loadColorQueries();
}
//function to add query
void Reporter::m_addQuery(const QString & queryText,
                          const QString & queryName,
                          const QString & paramName,
                          bool mode,
                          bool active){
   QString param;
   if(paramName.isEmpty() || paramName.isNull()){
      param = "";
   }else{
      param = paramName;
   }
   if(!queryText.isEmpty() && !queryName.isEmpty()){
      if(m_mainSQL.getStorage().addQuery(queryText, queryName, param, active, true, mode)){
         auto newQuery = new QToolButton;
         newQuery->setText(queryName);
         newQuery->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
         newQuery->setObjectName(queryName);
         ui->scrollLayout->addWidget(newQuery);

         connect(newQuery, &QToolButton::clicked, this, &Reporter::m_scrollQueryClicked);
         //         m_clearQuery();
         m_nameKey = "";
         m_serializeQueries();
         m_loadColorQueries();
      }
   }else{
      QMessageBox::critical(nullptr, QObject::tr("Text error"), "No text entered.");
   }
   m_mainSQL.getStorage().getQueries()[m_nameKey]->setMasterQuery(m_mainSQL.getStorage().getQueries()[
                                                                  m_mainSQL.getStorage().getQueries()[m_nameKey]->getMasterQueryName()]->getOriginalQuery());
}
//Function to add repeated parameters
void Reporter::m_addParameters(const QStringList & params, const qint32 & count){
   m_paramKey = m_Schedule[m_scheduleKey]->getParamCount();

   if(m_Schedule[m_scheduleKey]->addParam(params, count, m_paramKey)){
      auto newParameter = new QToolButton;
      m_selectedParam = m_paramKey;
      newParameter->setText(params.at(0));
      newParameter->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      newParameter->setObjectName(QString::number(m_paramKey));
      ui->scrollLayout_2->addWidget(newParameter);
      m_paramKey++;

      connect(newParameter, &QToolButton::clicked, this, &Reporter::m_scrollParamClicked);
      //TODO serialize parameters in schedule

      qint32 paramCount = m_Schedule[m_scheduleKey]->getParameters()[m_selectedParam]->getCount();
      m_loadIndividualParameters(paramCount);
      m_serializeSchedule();
      m_loadColorScheduleParam();
   }
}
//Function to add schedule
void Reporter::m_addSchedule(const QString & name){
   if(name.isEmpty() || name.isNull()){
      QMessageBox::critical(this, QObject::tr("Text error"), QObject::tr("Enter name for new schedule."));
      return;
   }

   auto tmp = new Scheduling;
   tmp->setName(name);
   m_Schedule.insert(m_scheduleCount, tmp);
   m_scheduleKey = m_scheduleCount++;
   //   m_editDay(m_scheduleKey);
   //   m_editMonthly(m_scheduleKey);
   //   m_editShift(m_scheduleKey);
   //   m_editWeekly(m_scheduleKey);

   auto newSchedule = new QToolButton;
   newSchedule->setText(name);
   newSchedule->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

   newSchedule->setObjectName(QString::number(m_scheduleKey));
   ui->scrollLayour_3->addWidget(newSchedule);
   connect(newSchedule, &QToolButton::clicked, this, &Reporter::m_loadSchedule);
   m_serializeSchedule();

   m_deleteEmails();
   m_displayShift(m_scheduleKey);
   m_displayDay(m_scheduleKey);
   m_displayWeekly(m_scheduleKey);
   m_displayMonthly(m_scheduleKey);
   m_displayCustom(m_scheduleKey);
   ui->scheduleName->setText(m_Schedule[m_scheduleKey]->getName());
   m_loadEmails();
   m_loadScheduleParameters();
   m_loadColorSchedule();
}
//Function to add email adress to shift schedule
void Reporter::m_addShiftScheduleEmail(const QString & email){
   if(email.isEmpty() || email.isNull() || !m_validateEmail(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Enter valid email adress."));
      return;
   }

   if(m_Schedule[m_scheduleKey]->getShift().getEmailAdresses().contains(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Email adress already exists."));
      return;
   }

   auto newEmail = new QToolButton;
   newEmail->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
   newEmail->setText(email);
   newEmail->setObjectName(email);
   ui->shiftEmailLayout->addWidget(newEmail);
   m_emailKey = email;

   connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadShiftEmail);
   m_Schedule[m_scheduleKey]->getShift().getEmailAdresses().insert(email, email);
   m_serializeSchedule();
}
//Function to add email adress to daily schedule
void Reporter::m_addDailyScheduleEmail(const QString & email){
   if(email.isEmpty() || email.isNull() || !m_validateEmail(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Enter valid email adress."));
      return;
   }

   if(m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses().contains(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Email adress already exists."));
      return;
   }

   auto newEmail = new QToolButton;
   newEmail->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
   newEmail->setText(email);
   newEmail->setObjectName(email);
   ui->dailyEmailLayout->addWidget(newEmail);
   m_emailKey = email;

   connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadDailyEmail);
   m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses().insert(email, email);
   m_serializeSchedule();
}
//Function to add email adress to weekly schedule
void Reporter::m_addWeeklyScheduleEmail(const QString & email){
   if(email.isEmpty() || email.isNull() || !m_validateEmail(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Enter valid email adress."));
      return;
   }

   if(m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses().contains(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Email adress already exists."));
      return;
   }

   auto newEmail = new QToolButton;
   newEmail->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
   newEmail->setText(email);
   newEmail->setObjectName(email);
   ui->weeklyEmailLayout->addWidget(newEmail);
   m_emailKey = email;

   connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadWeeklyEmail);
   m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses().insert(email, email);
   m_serializeSchedule();
}
//Function to add email adress to monthly schedules
void Reporter::m_addMonthlyScheduleEmail(const QString & email){
   if(email.isEmpty() || email.isNull() || !m_validateEmail(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Enter valid email adress."));
      return;
   }

   if(m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses().contains(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Email adress already exists."));
      return;
   }

   auto newEmail = new QToolButton;
   newEmail->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
   newEmail->setText(email);
   newEmail->setObjectName(email);
   ui->monthlyEmailLayout->addWidget(newEmail);
   m_emailKey = email;

   connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadMonthlyEmail);
   m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses().insert(email, email);
   m_serializeSchedule();
}
//Function that loads parameters on mouse click
void Reporter::m_scrollParamClicked(){
   QObject * senderObj = sender();
   qint32 paramCount;
   QString senderObjID = senderObj->objectName();

   m_selectedParam = senderObjID.toInt();
   paramCount = m_Schedule[m_scheduleKey]->getParameters()[m_selectedParam]->getCount();
   m_loadIndividualParameters(paramCount);
   m_loadColorScheduleParam();
}
//Function that saves choosen paramaters
void Reporter::m_saveParameter(){
   QStringList parameters;
   qint32 tmpCount = 0;

   m_createParamList(parameters, tmpCount);
   if(tmpCount == 0 || m_selectedParam == -1){
      QMessageBox::critical(this,QObject::tr("Text error"), "No parameter selected.");
   }else{
      m_Schedule[m_scheduleKey]->getParameters()[m_selectedParam]->editInfo(parameters, tmpCount);
      tmp = ui->scrollAreaWidgetContents_2->findChild<QToolButton *>(QString::number(m_selectedParam));
      tmp->setText(parameters.at(0));
      m_serializeSchedule();
   }
   m_loadColorScheduleParam();
}
//Function that deletes selected query
void Reporter::m_deleteQuery(){
   m_mainSQL.getStorage().getQueries().remove(m_nameKey);
   tmp = ui->scrollAreaWidgetContents->findChild<QToolButton *>(m_nameKey);
   delete tmp;
   m_clearQuery();
   m_serializeQueries();
   m_nameKey = "";
}
//Function that deletes selected Schedule
void Reporter::m_deleteSchedule(){
   m_Schedule[m_scheduleKey]->getShift().getEmailAdresses().clear();
   m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses().clear();
   m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses().clear();
   m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses().clear();
   m_deleteEmails();
   m_deleteParameters();
   m_Schedule.remove(m_scheduleKey);
   tmp = ui->scrollSchedule->findChild<QToolButton *>(QString::number(m_scheduleKey));
   delete tmp;
   if(m_noSchedule()){
      m_scheduleKey = 0;
      m_addSchedule("Default");
   }
   m_clearSchedule();
   if(!m_Schedule.isEmpty()){
      m_scheduleKey = m_Schedule.firstKey();
   }else{
      m_scheduleCount = 0;
      m_scheduleKey = 0;
      m_addSchedule("Default");
   }
}
//Function that deletes email buttons so new ones can be loaded
void Reporter::m_deleteEmails(){
   for(auto & it : m_Schedule[m_scheduleKey]->getShift().getEmailAdresses()){
      tmp = ui->shiftEmailWidget->findChild<QToolButton *>(QString(it));
      delete tmp;
      m_emailKey = "";
      ui->shiftemailAdress->clear();
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses()){
      tmp = ui->dailyEmailWidget->findChild<QToolButton *>(QString(it));
      delete tmp;
      m_emailKey = "";
      ui->dailyEmailAdress->clear();
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses()){
      tmp = ui->weeklyEmailWidget->findChild<QToolButton *>(QString(it));
      delete tmp;
      m_emailKey = "";
      ui->weeklyemailAdress->clear();
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses()){
      tmp = ui->monthlyEmailWidget->findChild<QToolButton *>(QString(it));
      delete tmp;
      m_emailKey = "";
      ui->monthlyEmailAdress->clear();
   }
}

void Reporter::m_deleteParameters(){
   tmp = ui->scrollAreaWidgetContents_2->findChild<QToolButton *>();
   while(tmp){
      delete tmp;
      tmp = ui->scrollAreaWidgetContents_2->findChild<QToolButton *>();
   }
   m_clearParam();
}
//Function that deletes parameters
void Reporter::m_deleteParam(){
   m_Schedule[m_scheduleKey]->deleteParameter(m_selectedParam);

   tmp = ui->scrollAreaWidgetContents_2->findChild<QToolButton *>(QString::number(m_selectedParam));
   delete tmp;
   m_selectedParam = -1;
   m_clearParam();
   m_serializeSchedule();
   //   m_serializeParameters ();
}
//Sets up default settings of the app
void Reporter::defaultSettings(){
   m_ConnectDB();
   m_Deserialize();
   m_deserializeSchedule();

   if(m_noSchedule()){
      m_addSchedule("Default");
      ui->scheduleName->setText(m_Schedule[0]->getName());
   }

   //   m_SetTimer(m_TIMERINTERVAL);
   //   m_PauseTimer();
   //   m_PauseTimer();
   m_mainSQL.getStorage().fixMaster();
   m_loadColorSchedule();
   qInfo(logInfo()) << "Settings and data successfuly loaded.";
}

void Reporter::closeEvent(QCloseEvent * event){
   if(event != nullptr){
      m_Setup.saveIni();
      event->accept();
   }
}
//Load the default query
void Reporter::m_defaultQuery(){

}
//function to determine which button was clicked and select the stored query based on it
void Reporter::m_scrollQueryClicked(){
   QObject * senderObj = sender();

   QString senderObjname = senderObj->objectName();
   if(!m_nameKey.isEmpty()){
      m_saveQuery();
   }
   m_nameKey = senderObjname;

   ui->queryEdit->document()->setPlainText(m_mainSQL.getStorage().getQueries()[senderObjname]->getQuery());
   ui->queryNameEdit->setText(m_mainSQL.getStorage().getQueries()[senderObjname]->getName());
   ui->queryParamEdit->setText(m_mainSQL.getStorage().getQueries()[senderObjname]->getParam());
   ui->queryActive->setChecked(m_mainSQL.getStorage().getQueries()[senderObjname]->getActive());
   m_loadColorQueries();
}
//Setup connection to DB
void Reporter::m_ConnectDB(){
   m_mainSQL.getDatabase().getDatabaseConnector().setInfo(m_Setup.getSettings().databaseType,
                                                          m_Setup.getSettings().host,
                                                          m_Setup.getSettings().port,
                                                          m_Setup.getSettings().databaseName,
                                                          m_Setup.getSettings().userName,
                                                          m_Setup.getSettings().userPassword);
   if(!m_mainSQL.getDatabase().createConnection()){
      qCritical(logCritical()) << "Database connection error: " + m_mainSQL.getDatabase().getDatabase().lastError().text();
      //      QMessageBox::warning(this, "Database connection error",
      //                           m_mainSQL.getDatabase().getDatabase().lastError().text());
   }else{
      qInfo(logInfo()) << "Connection to database estabilished successfuly.";
      //      QMessageBox::information(this, "Database connection success", "Connecting to database successful.");
   }
}
//Serializes queries
void Reporter::m_serializeQueries(){
   QStringList tmpSerialize = m_mainSQL.loadList();
   m_Setup.serializeQueries(tmpSerialize);
}
//Serializes schedule
void Reporter::m_serializeSchedule(){
   QList<QStringList> scheduleSerialization;
   QStringList scheduleNames;
   for(auto & it : m_Schedule){
      scheduleNames.push_back(it->getName());
      scheduleSerialization.append(it->getShift().prepareSerialization());
      scheduleSerialization.append(it->getDaily().prepareSerialization());
      scheduleSerialization.append(it->getWeekly().prepareSerialization());
      scheduleSerialization.append(it->getMonthly().prepareSerialization());
      scheduleSerialization.append(it->serializeParameters());
   }
   m_Setup.serializeSchedule(scheduleSerialization, scheduleNames);
}
//Deserialization
void Reporter::m_Deserialize(){
   QStringList tmpDeserializeQueries;
   //Deserialize queries
   m_Setup.deserializeQueries(tmpDeserializeQueries);
   for(qint32 i = 0; i + 3 < tmpDeserializeQueries.size(); i+=4){
      m_addQuery(tmpDeserializeQueries.at(i),
                 tmpDeserializeQueries.at(i+1),
                 tmpDeserializeQueries.at(i+2),
                 false,
                 (tmpDeserializeQueries.at(i+3) == "0" ? false : true));
   }

   QStringList tmpDeserializeParameters;
   QVector<qint32> tmpDeserializeCounts;
   qint32 drivingI = 0;
   //Deserialize parameters
   m_Setup.deserializeParameters(tmpDeserializeParameters,tmpDeserializeCounts);
   for(auto it : tmpDeserializeCounts){
      qint32 tmpInt = it;
      QStringList tmpList;

      for(qint32 i = tmpInt; i > 0; i--){
         tmpList << tmpDeserializeParameters.at(drivingI);
         drivingI++;
      }
      m_addParameters(tmpList, tmpInt);
   }
}

void Reporter::m_loadColorSchedule(){
   qint32 tmpKey;
   for(auto & it : m_Schedule){
      tmpKey = m_Schedule.key(it);
      tmp = ui->scrollSchedule->findChild<QToolButton *>(QString::number(tmpKey));
      if(tmpKey == m_scheduleKey){
         tmp->setStyleSheet("color: red;");
      }else{
         tmp->setStyleSheet(" ");
      }
   }
}

void Reporter::m_loadColorScheduleEmail(){
   for(auto & it : m_Schedule[m_scheduleKey]->getShift().getEmailAdresses()){
      tmp = ui->shiftEmailScrollArea->findChild<QToolButton *>(it);
      if(it == m_emailKey){
         tmp->setStyleSheet("color: red;");
      }else{
         tmp->setStyleSheet(" ");
      }
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses()){
      tmp = ui->dailyEmailScrollArea->findChild<QToolButton *>(it);
      if(it == m_emailKey){
         tmp->setStyleSheet("color: red;");
      }else{
         tmp->setStyleSheet(" ");
      }
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses()){
      tmp = ui->weeklyEmailScrollArea->findChild<QToolButton *>(it);
      if(it == m_emailKey){
         tmp->setStyleSheet("color: red;");
      }else{
         tmp->setStyleSheet(" ");
      }
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses()){
      tmp = ui->monthlyEmailScrollArea->findChild<QToolButton *>(it);
      if(it == m_emailKey){
         tmp->setStyleSheet("color: red;");
      }else{
         tmp->setStyleSheet(" ");
      }
   }
}

void Reporter::m_loadColorScheduleParam(){
   qint32 tmpKey;
   for(auto & it : m_Schedule[m_scheduleKey]->getParameters()){
      tmpKey = m_Schedule[m_scheduleKey]->getParameters().key(it);
      tmp = ui->scrollAreaWidgetContents_2->findChild<QToolButton *>(QString::number(tmpKey));
      if(tmpKey == m_selectedParam){
         tmp->setStyleSheet("color: red;");
      }else{
         tmp->setStyleSheet(" ");
      }
   }
}

void Reporter::m_loadColorQueries(){
   QString tmpKey;
   for(auto & it : m_mainSQL.getStorage().getQueries()){
      tmpKey = m_mainSQL.getStorage().getQueries().key(it);
      tmp = ui->scrollAreaWidgetContents->findChild<QToolButton *>(QString(tmpKey));
      if(tmpKey == m_nameKey){
         tmp->setStyleSheet("color: red;");
      }else{
         tmp->setStyleSheet(" ");
      }
   }
}
//Deserializes Schedule
void Reporter::m_deserializeSchedule(){
   QList<QStringList> scheduleDeserialize;
   QStringList scheduleNames;
   qint32 desCount;
   QMap<QString, QVector<QStringList>> parameters;

   m_Setup.deserializeSchedule(scheduleDeserialize, scheduleNames, parameters);

   desCount = 0;
   m_scheduleKey = 0;
   m_scheduleCount = 0;
   for(auto & it : scheduleNames){
      auto tmp = new Scheduling;
      tmp->setName(it);
      m_Schedule.insert(m_scheduleCount, tmp);
      m_scheduleKey = m_scheduleCount++;
      auto newSchedule = new QToolButton;
      newSchedule->setText(it);
      newSchedule->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      newSchedule->setObjectName(QString::number(m_scheduleKey));
      ui->scrollLayour_3->addWidget(newSchedule);

      connect(newSchedule, &QToolButton::clicked, this, &Reporter::m_loadSchedule);

      m_Schedule[m_scheduleKey]->getShift().deserializeList(scheduleDeserialize[desCount++]);
      m_Schedule[m_scheduleKey]->getDaily().deserializeList(scheduleDeserialize[desCount++]);
      m_Schedule[m_scheduleKey]->getWeekly().deserializeList(scheduleDeserialize[desCount++]);
      m_Schedule[m_scheduleKey]->getMonthly().deserializeList(scheduleDeserialize[desCount++]);
      m_Schedule[m_scheduleKey]->deserializeParameters(parameters[it]);
   }
   m_scheduleKey = 0;
   if(!m_noSchedule()){
      ui->scheduleName->setText(m_Schedule[m_scheduleKey]->getName());
      m_displayShift(m_scheduleKey);
      m_displayDay(m_scheduleKey);
      m_displayWeekly(m_scheduleKey);
      m_displayMonthly(m_scheduleKey);
      m_displayCustom(m_scheduleKey);
      m_loadEmails();
      m_loadScheduleParameters();
   }
}
//Saves selected schedule
void Reporter::m_saveSchedule(){
   if(!m_validateScheduleNameSaving(ui->scheduleName->text())){
      return;
   }
   tmp = ui->scrollSchedule->findChild<QToolButton *>(QString::number(m_scheduleKey));
   //   delete tmp;
   //   auto newSchedule = new QToolButton;
   //   TODO: not sure about this yet, test this more, seems like its working
   tmp->setText(ui->scheduleName->text());
   tmp->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
   tmp->setObjectName(QString::number(m_scheduleKey));
   //   ui->scrollLayour_3->addWidget(newSchedule);

   connect(tmp, &QToolButton::clicked, this, &Reporter::m_loadSchedule);
   m_Schedule[m_scheduleKey]->setName(ui->scheduleName->text());
   m_editShift(m_scheduleKey);
   m_editDay(m_scheduleKey);
   m_editWeekly(m_scheduleKey);
   m_editMonthly(m_scheduleKey);
   m_editCustom();
   //   m_saveParameter();
   m_serializeSchedule();
   m_loadColorSchedule();
}
//DEBUG
//Generates CSV, mostly for testing
void Reporter::m_generateCSV(){
   //TESTING FOR SHIFT
   QSqlQuery generateCSV;
   generateCSV = m_mainSQL.getStorage().getQueries()[m_nameKey]->getResult();
   m_Export.getCSV().generateFile(m_Schedule[m_scheduleKey]->getShift().getCsvTemplatePath(),
                                  m_Schedule[m_scheduleKey]->getShift().getAttachName(),
                                  generateCSV);
}

//DEBUG
//Generates XLS, mostly for testing
void Reporter::m_generateXLS(){
   //TESTING FOR SHIFT
   QList<std::pair<QString, QString>> tmp;
   QList<QStringList> tmpQueries;
   m_generateQuery(m_nameKey);
   m_executeQuery(m_nameKey);
   tmp.append(std::make_pair("CURRENT_DATE","15.12.2017"));
   tmp.append(std::make_pair("DateTimeFromTo", "Od datumu A po datum B"));
   tmp.append(std::make_pair("vygeneroval", "David Budil"));
   //TOHLE ASI NIC NEDELA -> OTESTOVAT
   if(!m_mainSQL.getStorage().getQueries()[m_nameKey]->getResult().first()){
      return;
   }else{
      tmpQueries.append(m_mainSQL.getStorage().getQueries()[m_nameKey]->queryList());
      if(!m_mainSQL.getStorage().getQueries()[m_nameKey]->getIsMaster()){
         m_generateQuery(m_mainSQL.getStorage().getQueries()[m_nameKey]->getParam());
         m_executeQuery(m_mainSQL.getStorage().getQueries()[m_nameKey]->getParam());
         tmpQueries.push_front(m_mainSQL.getStorage().getQueries()[m_mainSQL.getStorage().getQueries()[m_nameKey]->getParam()]->queryList());
      }

      m_Export.getXLS().generateFile(m_Schedule[m_scheduleKey]->getShift().getXlsTemplatePath(),
                                     m_Schedule[m_scheduleKey]->getShift().getAttachName(),
                                     tmp,
                                     tmpQueries);
   }
}
//Generates XLS template
void Reporter::m_generateTemplateXLS(){
   m_testingQueryGen();
   QXlsx::Document xlsx;
   QSqlQuery tmpModel = m_mainSQL.getStorage().getQueries()[m_nameKey]->getResult();
   QSqlRecord rec = tmpModel.record();

   for(qint32 i = 0; i < rec.count(); ++i){
      QString tmp = rec.fieldName(i);
      xlsx.write(1, i + 1, tmp);
      tmpModel.next();
      xlsx.write(2, i + 1, tmpModel.value(i).toString());
   }

   xlsx.saveAs(QDir::currentPath() + "/templateFieldNamesXLSX.xls");
   QDesktopServices::openUrl(QUrl(QDir::currentPath() + "/templateFieldNamesXLSX.xls"));
   m_mainSQL.getStorage().getQueries()[m_nameKey]->finishQuery();
}
//Generates query data model that is displayed in table in application
void Reporter::m_testingQueryGen(){
   if(m_Timer->isActive()){
      m_Timer->stop();
   }
   if(m_firstQuery){
      m_mainSQL.getModel()->clear();
      m_mainSQL.getModel()->query().clear();
      //TODO: test this, crashes app
      //      for(auto & it : m_mainSQL.getStorage().getQueries()){
      //         if(it != m_mainSQL.getStorage().getQueries()[m_nameKey]){
      //            it->clearQueries();
      //         }
      //      }
   }
   if(!m_mainSQL.getDatabase().getDatabase().open()){
      qWarning(logWarning()) << "Can not run SQL query due to no Database connection.";
      QMessageBox::critical(this, QObject::tr("Database error"),
                            "Not connected to database");
   }else{
      //TODO: add custom parameters
      m_loadMaster();
      m_generateQuery(m_nameKey);
      m_executeQuery(m_nameKey);
      m_displaySQLResult(m_nameKey);
      //TODO: tmp workaround, try to fix this in next version
      m_mainSQL.getStorage().getQueries()[m_nameKey]->clearQueries();
      m_firstQuery = true;
   }
   if(!m_Timer->isActive()){
      m_Timer->start(m_TIMERINTERVAL);
   }
}
//Adds first schedule item
bool Reporter::m_noSchedule(){
   return (ui->scrollLayour_3->count() == 0 ? true : false);
}
//Validates the email syntax
bool Reporter::m_validateEmail(const QString & email){
   QRegularExpression regex("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
   return regex.match(email).hasMatch();
}

bool Reporter::m_validateScheduleName(const QString & name){
   for(const auto & it : m_Schedule){
      if(name == it->getName()){
         QMessageBox::critical(this, QObject::tr("New schedule error"),
                               "Schedule with this name already exists.");
         return false;
      }
   }
   return true;
}

bool Reporter::m_validateScheduleNameSaving(const QString & name){
   for(const auto & it : m_Schedule){
      if(name == it->getName() && it != m_Schedule[m_scheduleKey]){
         QMessageBox::critical(this, QObject::tr("New schedule error"),
                               "Schedule with this name already exists.");

         ui->scheduleName->setText(m_Schedule[m_scheduleKey]->getName());
         return false;
      }
   }
   return true;
}

QStringList Reporter::m_getColumnNames(const QString & tableName){
   QSqlRecord dbRecord = m_mainSQL.getDatabase().getDatabase().driver()->record(tableName);
   qint32 colCount = dbRecord.count();
   QStringList result;
   for(qint32 i = 0; i < colCount; ++i){
      result.append(dbRecord.field(i).name());
   }
   return result;
}

void Reporter::m_SetTimer(qint32 interval){
   connect(m_Timer, SIGNAL(timeout()), this, SLOT(timerInterval()));
   m_Timer->start(interval);
   qInfo(logInfo()) << "Timer with interval: " + QVariant(interval).toString() + " ms successfully started.";
}

void Reporter::m_debugNotification(const QString & toDisplay){
   QMessageBox::information(this, "debug", toDisplay);
}

void Reporter::m_PauseTimer(){
   if(m_Timer->isActive()){
      m_Timer->stop();
   }else{
      m_Timer->start(m_TIMERINTERVAL);
   }
}

bool Reporter::m_validateQuerySelected(){
   if(m_nameKey.isEmpty() || m_nameKey.isNull()){
      QMessageBox::critical(this, QObject::tr("Query Error"), "No query selected.");
      return false;
   }else{
      return true;
   }
}
//Loads schedule on mouse click
void Reporter::m_loadSchedule(){
   m_deleteEmails();
   QObject * senderObj = sender();
   QString senderObjname = senderObj->objectName();
   m_scheduleKey = senderObjname.toInt();
   m_displayShift(m_scheduleKey);
   m_displayDay(m_scheduleKey);
   m_displayWeekly(m_scheduleKey);
   m_displayMonthly(m_scheduleKey);
   m_displayCustom(m_scheduleKey);
   ui->scheduleName->setText(m_Schedule[m_scheduleKey]->getName());
   m_loadEmails();
   m_loadScheduleParameters();
   m_loadColorSchedule();
}
//Loads shift email on mouse click
void Reporter::m_loadShiftEmail(){
   QObject * senderObj = sender();
   m_emailKey = senderObj->objectName();
   ui->shiftemailAdress->setText(m_Schedule[m_scheduleKey]->getShift().getEmailAdresses()[m_emailKey]);
   m_loadColorScheduleEmail();
}
//Loads daily email on mouse click
void Reporter::m_loadDailyEmail(){
   QObject * senderObj = sender();
   m_emailKey = senderObj->objectName();
   ui->dailyEmailAdress->setText(m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses()[m_emailKey]);
   m_loadColorScheduleEmail();
}
//Loads weekly email on mouse click
void Reporter::m_loadWeeklyEmail(){
   QObject * senderObj = sender();
   m_emailKey = senderObj->objectName();
   ui->weeklyemailAdress->setText(m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses()[m_emailKey]);
   m_loadColorScheduleEmail();
}
//Loads monthly email on mouse click
void Reporter::m_loadMonthlyEmail(){
   QObject * senderObj = sender();
   m_emailKey = senderObj->objectName();
   ui->monthlyEmailAdress->setText(m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses()[m_emailKey]);
   m_loadColorScheduleEmail();
}

void Reporter::m_loadScheduleParameters(){
   //delete current toolbuttons to show the selected scheduling parameters
   tmp = ui->scrollAreaWidgetContents_2->findChild<QToolButton *>();
   while(tmp){
      delete tmp;
      tmp = ui->scrollAreaWidgetContents_2->findChild<QToolButton *>();
   }
   m_clearParam();

   for(const auto & it : m_Schedule[m_scheduleKey]->getParameters()){
      auto newParameter = new QToolButton;
      newParameter->setText(it->getParameters().at(0));
      newParameter->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      newParameter->setObjectName(QString::number(m_Schedule[m_scheduleKey]->getParameters().key(it)));
      ui->scrollLayout_2->addWidget(newParameter);

      connect(newParameter, &QToolButton::clicked, this, &Reporter::m_scrollParamClicked);
      m_clearParam();
      m_selectedParam = -1;
   }
   m_loadColorScheduleParam();
}

void Reporter::m_loadIndividualParameters(const qint32 & paramCount){
   m_clearParam();
   if(paramCount > 0){
      ui->param1->setText(m_Schedule[m_scheduleKey]->getParameters()[m_selectedParam]->getParameters().at(0));
   }
   if(paramCount > 1){
      ui->param2->setText(m_Schedule[m_scheduleKey]->getParameters()[m_selectedParam]->getParameters().at(1));
   }
   if(paramCount > 2){
      ui->param3->setText(m_Schedule[m_scheduleKey]->getParameters()[m_selectedParam]->getParameters().at(2));
   }
   if(paramCount > 3){
      ui->param4->setText(m_Schedule[m_scheduleKey]->getParameters()[m_selectedParam]->getParameters().at(3));
   }
   if(paramCount > 4){
      ui->param5->setText(m_Schedule[m_scheduleKey]->getParameters()[m_selectedParam]->getParameters().at(4));
   }
}
//Adds new parameter
void Reporter::on_paramNew_clicked(){
   QStringList tmp;
   qint32 tmpCount = 0;
   if(m_Schedule[m_scheduleKey]->getParamCount() == 0){
      m_createParamList(tmp, tmpCount);
      m_addParameters(tmp, tmpCount);
   }else{
      tmp.append("Parameter1");
      tmpCount = 1;
      m_addParameters(tmp, tmpCount);
   }
}
//Edits selected parameter
void Reporter::on_paramEdit_clicked(){
   m_saveParameter();
}
//Loads master parameter for master/detail system
void Reporter::m_loadMaster(){
   if(!m_mainSQL.getStorage().getQueries()[m_nameKey]->getIsMaster()){
      m_mainSQL.getStorage().masterQuery(m_mainSQL.getStorage().getQueries()[m_nameKey]->getName(),
                                         m_mainSQL.getStorage().getQueries()[m_nameKey]->getParam());
   }
}
//Loads emails to create mail buttons
void Reporter::m_loadEmails(){
   for(auto & it : m_Schedule[m_scheduleKey]->getShift().getEmailAdresses()){
      auto newEmail = new QToolButton;
      newEmail->setText(it);
      newEmail->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      newEmail->setObjectName(it);
      ui->shiftEmailLayout->addWidget(newEmail);

      connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadShiftEmail);
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses()){
      auto newEmail = new QToolButton;
      newEmail->setText(it);
      newEmail->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      newEmail->setObjectName(it);
      ui->dailyEmailLayout->addWidget(newEmail);

      connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadDailyEmail);
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses()){
      auto newEmail = new QToolButton;
      newEmail->setText(it);
      newEmail->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      newEmail->setObjectName(it);
      ui->weeklyEmailLayout->addWidget(newEmail);

      connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadWeeklyEmail);
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses()){
      auto newEmail = new QToolButton;
      newEmail->setText(it);
      newEmail->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
      newEmail->setObjectName(it);
      ui->monthlyEmailLayout->addWidget(newEmail);

      connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadMonthlyEmail);
   }
   m_loadColorScheduleEmail();
}

//Loads all parameters into String List
void Reporter::m_createParamList(QStringList & tmp, qint32 & tmpCount){
   if(!(ui->param1->text().isEmpty())){
      tmp.append(ui->param1->text());
      tmpCount++;
   }
   if(!(ui->param2->text().isEmpty())){
      tmp.append(ui->param2->text());
      tmpCount++;
   }
   if(!(ui->param3->text().isEmpty())){
      tmp.append(ui->param3->text());
      tmpCount++;
   }
   if(!(ui->param4->text().isEmpty())){
      tmp.append(ui->param4->text());
      tmpCount++;
   }
   if(!(ui->param5->text().isEmpty())){
      tmp.append(ui->param5->text());
      tmpCount++;
   }
   if(tmpCount == 0){
      tmp.append("Parameter1");
      tmpCount++;
   }
}
//Clears param text input
void Reporter::m_clearParam(){
   ui->param1->clear();
   ui->param2->clear();
   ui->param3->clear();
   ui->param4->clear();
   ui->param5->clear();
}
//Clears query text input
void Reporter::m_clearQuery(){
   ui->queryEdit->clear();
   ui->queryNameEdit->clear();
   ui->queryParamEdit->clear();
   ui->queryActive->setChecked(false);
}
//Clears schedule text input
void Reporter::m_clearSchedule(){
   m_clearShift();
   m_clearDaily();
   m_clearWeekly();
   m_clearMonthly();
   ui->scheduleName->clear();
}
//Clears shift text input
void Reporter::m_clearShift(){
   ui->shiftActive->setChecked(false);
   ui->shiftAttachCSV->setChecked(false);
   ui->shiftAttachXLS->setChecked(false);
   ui->shiftSubj->clear();
   ui->shiftAttach->clear();
   ui->shiftCSV->clear();
   ui->shiftXLS->clear();
   ui->shiftFrom->clear();
   ui->shiftTo->clear();
   ui->shiftFrom_2->clear();
   ui->shiftEmail->clear();
   ui->shiftMondayActive->setChecked(false);
   ui->shiftTuesdayActive->setChecked(false);
   ui->shiftWednesdayActive->setChecked(false);
   ui->shiftThursdayActive->setChecked(false);
   ui->shiftFridayActive->setChecked(false);
   ui->shiftSaturdayActive->setChecked(false);
   ui->shiftSundayActive->setChecked(false);
}
//Clears daily text input
void Reporter::m_clearDaily(){
   ui->dailyActive->setChecked(false);
   ui->dailyAttachCSV->setChecked(false);
   ui->dailyAttachXLS->setChecked(false);
   ui->dailySubj->clear();
   ui->dailyAttach->clear();
   ui->dailyCSV->clear();
   ui->dailyXLS->clear();
   ui->dailyTime->clear();
   ui->dailyEmail->clear();
   ui->dailyMondayActive->setChecked(false);
   ui->dailyTuesdayActive->setChecked(false);
   ui->dailyWednesdayActive->setChecked(false);
   ui->dailyThursdayActive->setChecked(false);
   ui->dailyFridayActive->setChecked(false);
   ui->dailySaturdayActive->setChecked(false);
   ui->dailySundayActive->setChecked(false);
}
//Clears weekly text input
void Reporter::m_clearWeekly(){
   ui->weeklyActive->setChecked(false);
   ui->weeklyAttachCSV->setChecked(false);
   ui->weeklyAttachXLS->setChecked(false);
   ui->weeklySubj->clear();
   ui->weeklyAttach->clear();
   ui->weeklyCSV->clear();
   ui->weeklyXLS->clear();
   ui->weeklyTime->clear();
   ui->weeklyDays->setCurrentIndex(0);
   ui->weeklyEmail->clear();
}
//Clears monthly text input
void Reporter::m_clearMonthly(){
   ui->monthlyActive->setChecked(false);
   ui->monthlyAttachCSV->setChecked(false);
   ui->monthlyAttachXLS->setChecked(false);
   ui->monthlySubj->clear();
   ui->monthlyAttach->clear();
   ui->monthlyCSV->clear();
   ui->monthlyXLS->clear();
   ui->monthlyTIme->clear();
   ui->monthlyDays->setCurrentIndex(0);
   ui->monthlyEmail->clear();
}
//Deletes selected parameter
void Reporter::on_paramDelete_clicked(){
   if(m_selectedParam == -1){
      QMessageBox::critical(this,QObject::tr("Text error"), "No parameter selected.");
   }else{
      m_deleteParam();
   }
}
//Deletes selected query
void Reporter::on_queryDelete_clicked(){
   if(m_validateQuerySelected()){
      QMessageBox::StandardButton confirmSchedulingDelete;
      confirmSchedulingDelete = QMessageBox::question(this, "Confirm",
                                                      "Are you sure you want to delete selected query?",
                                                      QMessageBox::Yes|QMessageBox::No);

      if(confirmSchedulingDelete == QMessageBox::Yes){
         m_deleteQuery();
      }
   }
}
//Changes query state to active or inactive
void Reporter::on_queryActive_stateChanged(int state){
   m_queryActive = state;
   //   m_saveQuery();
}
//Function to display shift schedule values
void Reporter::m_displayShift(qint32 keyString){
   m_loadActiveShiftDays(keyString);
   ui->shiftActive->setChecked(m_Schedule[keyString]->getShift().getActive());
   ui->shiftAttachCSV->setChecked(m_Schedule[keyString]->getShift().getCsvAttach());
   ui->shiftAttachXLS->setChecked(m_Schedule[keyString]->getShift().getXlsAttach());
   ui->shiftSubj->setText(m_Schedule[keyString]->getShift().getSubjName());
   ui->shiftAttach->setText(m_Schedule[keyString]->getShift().getAttachName());
   ui->shiftCSV->setText(m_Schedule[keyString]->getShift().getCsvTemplatePath());
   ui->shiftXLS->setText(m_Schedule[keyString]->getShift().getXlsTemplatePath());
   ui->shiftEmail->setText(m_Schedule[keyString]->getShift().getEmailTemplatePath());
   ui->shiftFrom->setTime(m_Schedule[keyString]->getShift().getFrom0());
   ui->shiftTo->setTime(m_Schedule[keyString]->getShift().getTo0());
   ui->shiftFrom_2->setTime(m_Schedule[keyString]->getShift().getFrom1());
}
//Function to display daily schedule values
void Reporter::m_displayDay(qint32 keyString){
   m_loadActiveDailyDays(keyString);
   ui->dailyActive->setChecked(m_Schedule[keyString]->getDaily().getActive());
   ui->dailyAttachCSV->setChecked(m_Schedule[keyString]->getDaily().getCsvAttach());
   ui->dailyAttachXLS->setChecked(m_Schedule[keyString]->getDaily().getXlsAttach());
   ui->dailySubj->setText(m_Schedule[keyString]->getDaily().getSubjName());
   ui->dailyAttach->setText(m_Schedule[keyString]->getDaily().getAttachName());
   ui->dailyCSV->setText(m_Schedule[keyString]->getDaily().getCsvTemplatePath());
   ui->dailyXLS->setText(m_Schedule[keyString]->getDaily().getXlsTemplatePath());
   ui->dailyEmail->setText(m_Schedule[keyString]->getDaily().getEmailTemplatePath());
   ui->dailyTime->setTime(m_Schedule[keyString]->getDaily().getTime());
}
//Function to display weekly schedule values
void Reporter::m_displayWeekly(qint32 keyString){
   ui->weeklyActive->setChecked(m_Schedule[keyString]->getWeekly().getActive());
   ui->weeklyAttachCSV->setChecked(m_Schedule[keyString]->getWeekly().getCsvAttach());
   ui->weeklyAttachXLS->setChecked(m_Schedule[keyString]->getWeekly().getXlsAttach());
   ui->weeklySubj->setText(m_Schedule[keyString]->getWeekly().getSubjName());
   ui->weeklyAttach->setText(m_Schedule[keyString]->getWeekly().getAttachName());
   ui->weeklyCSV->setText(m_Schedule[keyString]->getWeekly().getCsvTemplatePath());
   ui->weeklyXLS->setText(m_Schedule[keyString]->getWeekly().getXlsTemplatePath());
   ui->weeklyEmail->setText(m_Schedule[keyString]->getWeekly().getEmailTemplatePath());
   ui->weeklyTime->setTime(m_Schedule[keyString]->getWeekly().getTime());
   ui->weeklyDays->setCurrentIndex(m_Schedule[keyString]->getWeekly().getDay());
}
//Function to display monthly schedule values
void Reporter::m_displayMonthly(qint32 keyString){
   ui->monthlyActive->setChecked(m_Schedule[keyString]->getMonthly().getActive());
   ui->monthlyAttachCSV->setChecked(m_Schedule[keyString]->getMonthly().getCsvAttach());
   ui->monthlyAttachXLS->setChecked(m_Schedule[keyString]->getMonthly().getXlsAttach());
   ui->monthlySubj->setText(m_Schedule[keyString]->getMonthly().getSubjName());
   ui->monthlyAttach->setText(m_Schedule[keyString]->getMonthly().getAttachName());
   ui->monthlyCSV->setText(m_Schedule[keyString]->getMonthly().getCsvTemplatePath());
   ui->monthlyXLS->setText(m_Schedule[keyString]->getMonthly().getXlsTemplatePath());
   ui->monthlyEmail->setText(m_Schedule[keyString]->getMonthly().getEmailTemplatePath());
   ui->monthlyTIme->setTime(m_Schedule[keyString]->getMonthly().getTime());
   ui->monthlyDays->setCurrentIndex(m_Schedule[keyString]->getMonthly().getDay());
}
void Reporter::m_displayCustom(qint32 keyString){
   //work in progress, in future maybe
}
//Function to load active shift days
void Reporter::m_loadActiveShiftDays(qint32 keyString){
   ui->shiftMondayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[0]);
   ui->shiftTuesdayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[1]);
   ui->shiftWednesdayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[2]);
   ui->shiftThursdayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[3]);
   ui->shiftFridayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[4]);
   ui->shiftSaturdayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[5]);
   ui->shiftSundayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[6]);
}
//Function to load active daily days
void Reporter::m_loadActiveDailyDays(qint32 keyString){
   ui->dailyMondayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[0]);
   ui->dailyTuesdayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[1]);
   ui->dailyWednesdayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[2]);
   ui->dailyThursdayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[3]);
   ui->dailyFridayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[4]);
   ui->dailySaturdayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[5]);
   ui->dailySundayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[6]);
}
//Function to edit active shift days
void Reporter::m_editActiveShiftDays(qint32 keyString){
   m_Schedule[keyString]->getShift().setDays(0, ui->shiftMondayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(1, ui->shiftTuesdayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(2, ui->shiftWednesdayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(3, ui->shiftThursdayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(4, ui->shiftFridayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(5, ui->shiftSaturdayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(6, ui->shiftSundayActive->isChecked());
}
//Function to edit active daily days
void Reporter::m_editActiveDailyDays(qint32 keyString){
   m_Schedule[keyString]->getDaily().setDays(0, ui->dailyMondayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(1, ui->dailyTuesdayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(2, ui->dailyWednesdayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(3, ui->dailyThursdayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(4, ui->dailyFridayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(5, ui->dailySaturdayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(6, ui->dailySundayActive->isChecked());
}
//Function to edit shift schedule values
void Reporter::m_editShift(qint32 keyString){
   m_editActiveShiftDays(keyString);
   m_Schedule[keyString]->getShift().setActive(ui->shiftActive->isChecked());
   m_Schedule[keyString]->getShift().setCsvAttach(ui->shiftAttachCSV->isChecked());
   m_Schedule[keyString]->getShift().setXlsAttach(ui->shiftAttachXLS->isChecked());
   m_Schedule[keyString]->getShift().setSubjName(ui->shiftSubj->text());
   m_Schedule[keyString]->getShift().setAttachName(ui->shiftAttach->text());
   m_Schedule[keyString]->getShift().setCsvTemplatePath(ui->shiftCSV->text());
   m_Schedule[keyString]->getShift().setXlsTemplatePath(ui->shiftXLS->text());
   m_Schedule[keyString]->getShift().setFrom0(ui->shiftFrom->time());
   m_Schedule[keyString]->getShift().setTo0(ui->shiftTo->time());
   m_Schedule[keyString]->getShift().setFrom1(ui->shiftFrom_2->time());
   m_Schedule[keyString]->getShift().setTimeTMP(ui->shiftFrom_2->time());
   m_Schedule[keyString]->getShift().setEmailTemplatePath(ui->shiftEmail->text());
}
//Function to edit daily schedule values
void Reporter::m_editDay(qint32 keyString){
   m_editActiveDailyDays(keyString);
   m_Schedule[keyString]->getDaily().setActive(ui->dailyActive->isChecked());
   m_Schedule[keyString]->getDaily().setAttachName(ui->dailyAttach->text());
   m_Schedule[keyString]->getDaily().setSubjName(ui->dailySubj->text());
   m_Schedule[keyString]->getDaily().setXlsTemplatePath(ui->dailyXLS->text());
   m_Schedule[keyString]->getDaily().setCsvTemplatePath(ui->dailyCSV->text());
   m_Schedule[keyString]->getDaily().setTime(ui->dailyTime->time());
   m_Schedule[keyString]->getDaily().setCsvAttach(ui->dailyAttachCSV->isChecked());
   m_Schedule[keyString]->getDaily().setXlsAttach(ui->dailyAttachXLS->isChecked());
   m_Schedule[keyString]->getDaily().setEmailTemplatePath(ui->dailyEmail->text());
}
//Function to edit weekly schedule values
void Reporter::m_editWeekly(qint32 keyString){
   m_Schedule[keyString]->getWeekly().setActive(ui->weeklyActive->isChecked());
   m_Schedule[keyString]->getWeekly().setAttachName(ui->weeklyAttach->text());
   m_Schedule[keyString]->getWeekly().setSubjName(ui->weeklySubj->text());
   m_Schedule[keyString]->getWeekly().setXlsTemplatePath(ui->weeklyXLS->text());
   m_Schedule[keyString]->getWeekly().setCsvTemplatePath(ui->weeklyCSV->text());
   m_Schedule[keyString]->getWeekly().setTime(ui->weeklyTime->time());
   m_Schedule[keyString]->getWeekly().setCsvAttach(ui->weeklyAttachCSV->isChecked());
   m_Schedule[keyString]->getWeekly().setXlsAttach(ui->weeklyAttachXLS->isChecked());
   m_Schedule[keyString]->getWeekly().setDay(ui->weeklyDays->currentIndex());
   m_Schedule[keyString]->getWeekly().setEmailTemplatePath(ui->weeklyEmail->text());
}
//Function to edit monthly schedule values
void Reporter::m_editMonthly(qint32 keyString){
   m_Schedule[keyString]->getMonthly().setActive(ui->monthlyActive->isChecked());
   m_Schedule[keyString]->getMonthly().setAttachName(ui->monthlyAttach->text());
   m_Schedule[keyString]->getMonthly().setSubjName(ui->monthlySubj->text());
   m_Schedule[keyString]->getMonthly().setXlsTemplatePath(ui->monthlyXLS->text());
   m_Schedule[keyString]->getMonthly().setCsvTemplatePath(ui->monthlyCSV->text());
   m_Schedule[keyString]->getMonthly().setTime(ui->monthlyTIme->time());
   m_Schedule[keyString]->getMonthly().setCsvAttach(ui->monthlyAttachCSV->isChecked());
   m_Schedule[keyString]->getMonthly().setXlsAttach(ui->monthlyAttachXLS->isChecked());
   m_Schedule[keyString]->getMonthly().setDay(ui->monthlyDays->currentIndex());
   m_Schedule[keyString]->getMonthly().setEmailTemplatePath(ui->monthlyEmail->text());
}
//Function to edit Custom schedule
void Reporter::m_editCustom(){
   //work in progress, in future maybe
}
//Function that loads schedule values based on which tab is clicked
void Reporter::on_tabWidget_2_tabBarClicked(int index){
   m_saveSchedule();
   switch(index){
      case 0:
         m_displayShift(m_scheduleKey);
         ui->shiftemailAdress->clear();
         break;
      case 1:
         m_displayDay(m_scheduleKey);
         ui->dailyEmailAdress->clear();
         break;
      case 2:
         m_displayWeekly(m_scheduleKey);
         ui->weeklyemailAdress->clear();
         break;
      case 3:
         m_displayMonthly(m_scheduleKey);
         ui->monthlyEmailAdress->clear();
         break;
      case 4:
         m_displayCustom(m_scheduleKey);
         break;
      default:
         break;
   }
}
//Saves selected schedule on mouse click
void Reporter::on_saveScheduling_clicked(){
   m_saveSchedule();
}
void Reporter::on_shiftBrEMAIL_clicked(){
   ui->shiftEmail->setText(QFileDialog::getOpenFileName(this, tr("Select Email Template")));
}
void Reporter::on_shiftBrXLS_clicked(){
   ui->shiftXLS->setText(QFileDialog::getOpenFileName(this, tr("Select XLS Template")));
}
void Reporter::on_shiftBrCSV_clicked(){
   ui->shiftCSV->setText(QFileDialog::getOpenFileName(this, tr("Select CSV Template")));
}
void Reporter::on_dailyBrEMAIL_clicked(){
   ui->dailyEmail->setText(QFileDialog::getOpenFileName(this, tr("Select Email Template")));
}
void Reporter::on_dailyBrXLS_clicked(){
   ui->dailyXLS->setText(QFileDialog::getOpenFileName(this, tr("Select XLS Template")));
}
void Reporter::on_dailyBrCSV_clicked(){
   ui->dailyCSV->setText(QFileDialog::getOpenFileName(this, tr("Select CSV Template")));
}
void Reporter::on_weeklyBrEMAIL_clicked(){
   ui->weeklyEmail->setText(QFileDialog::getOpenFileName(this, tr("Select Email Template")));
}
void Reporter::on_weeklyBrXLS_clicked(){
   ui->weeklyXLS->setText(QFileDialog::getOpenFileName(this, tr("Select XLS Template")));
}
void Reporter::on_weeklyBrCSV_clicked(){
   ui->weeklyCSV->setText(QFileDialog::getOpenFileName(this, tr("Select CSV Template")));
}
void Reporter::on_monthlyBrEMAIL_clicked(){
   ui->monthlyEmail->setText(QFileDialog::getOpenFileName(this, tr("Select Email Template")));
}
void Reporter::on_monthlyBrXLS_clicked(){
   ui->monthlyXLS->setText(QFileDialog::getOpenFileName(this, tr("Select CSV Template")));
}
void Reporter::on_monthlyBrCSV_clicked(){
   ui->monthlyCSV->setText(QFileDialog::getOpenFileName(this, tr("Select XLS Template")));
}

void Reporter::on_toolButton_2_clicked(){
   if(m_nameKey.isEmpty() || m_nameKey.isNull()){
      QMessageBox::warning(this, "Query error.", "No query selected.");
   }else{
      m_testingQueryGen();
      if(m_mainSQL.getStorage().getQueries()[m_nameKey]->getResult().isActive()){
         m_generateXLS();
         m_generateCSV();
      }
   }
}
void Reporter::on_toolButton_3_clicked(){
   if(m_validateQuerySelected()){
      if(!m_mainSQL.getDatabase().getDatabase().open()){
         qWarning(logWarning()) << "Can not run SQL query due to no Database connection.";
         QMessageBox::critical(this, QObject::tr("Database error"),
                               "Not connected to database");
      }else{
         m_generateTemplateXLS();
         m_saveQuery();
      }
   }
}
void Reporter::on_newScheduling_clicked(){
   //   if(m_validateScheduleName(ui->scheduleName->text())){
   //      m_deleteEmails();
   //      m_addSchedule(ui->scheduleName->text());
   //      m_serializeSchedule();
   //   }
   if(m_scheduleCount == 0){
      m_deleteEmails();
      m_addSchedule(ui->scheduleName->text());
      m_serializeSchedule();
   }else{
      m_deleteEmails();
      m_addSchedule("New Schedule");
      m_serializeSchedule();
   }
}
void Reporter::on_deleteScheduling_clicked(){
   QMessageBox::StandardButton confirmSchedulingDelete;
   confirmSchedulingDelete = QMessageBox::question(this, "Confirm",
                                                   "Are you sure you want to delete scheduling named: " + m_Schedule[m_scheduleKey]->getName() + "?",
                                                   QMessageBox::Yes|QMessageBox::No);

   if(confirmSchedulingDelete == QMessageBox::Yes){
      m_deleteSchedule();
      m_serializeSchedule();
   }
}
void Reporter::on_shiftnewEmailAdress_clicked(){
   m_addShiftScheduleEmail(ui->shiftemailAdress->text());
   m_loadColorScheduleEmail();
}
void Reporter::on_dailynewEmailAdress_clicked(){
   m_addDailyScheduleEmail(ui->dailyEmailAdress->text());
   m_loadColorScheduleEmail();
}
void Reporter::on_saveEmailAdress_7_clicked(){
   m_addWeeklyScheduleEmail(ui->weeklyemailAdress->text());
   m_loadColorScheduleEmail();
}
void Reporter::on_monthlynewEmailAdress_clicked(){
   m_addMonthlyScheduleEmail(ui->monthlyEmailAdress->text());
   m_loadColorScheduleEmail();
}
void Reporter::on_shiftdeleteEmailAdress_clicked(){
   m_Schedule[m_scheduleKey]->getShift().getEmailAdresses().remove(m_emailKey);
   tmp = ui->shiftEmailWidget->findChild<QToolButton *>(QString(m_emailKey));
   delete tmp;
   m_emailKey = "";
   ui->shiftemailAdress->clear();
}
void Reporter::on_dailydeleteEmailAdress_clicked(){
   m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses().remove(m_emailKey);
   tmp = ui->dailyEmailWidget->findChild<QToolButton *>(QString(m_emailKey));
   delete tmp;
   m_emailKey = "";
   ui->dailyEmailAdress->clear();
}
void Reporter::on_weeklydeleteEmailAdress_clicked(){
   m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses().remove(m_emailKey);
   tmp = ui->weeklyEmailWidget->findChild<QToolButton *>(QString(m_emailKey));
   delete tmp;
   m_emailKey = "";
   ui->weeklyemailAdress->clear();
}
void Reporter::on_monthlydeleteEmailAdress_clicked(){
   m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses().remove(m_emailKey);
   tmp = ui->monthlyEmailWidget->findChild<QToolButton *>(QString(m_emailKey));
   delete tmp;
   m_emailKey = "";
   ui->monthlyEmailAdress->clear();
}

void Reporter::on_tableNames_clicked(){
   if(m_mainSQL.getDatabase().getDatabase().isOpen()){
      QStringList dbNames;
      TableInfo * infoDisplay;
      infoDisplay = new TableInfo(this);
      QVector<QStringList> dbInfo;
      //      m_saveQuery();
      dbNames = m_mainSQL.getDatabase().getDatabase().tables();

      for(auto & it : dbNames){
         QStringList tmp;
         tmp.append(it);
         tmp += m_getColumnNames(it);
         dbInfo.push_back(tmp);
      }

      infoDisplay->getInfo(dbInfo);
      infoDisplay->show();
   }else{
      QMessageBox::warning(this, "Database error.", "No connection to database.");
   }
}

void Reporter::timerInterval(){
   QQueue<Scheduling*> tmpSch;
   QQueue<SQLquery> tmpQueries;
   QQueue<SQLParameter> tmpParams;

   if(QDate::currentDate() != m_lastDay){
      m_daysSinceCleanUp++;
      m_lastDay = QDate::currentDate();
   }

   if(m_daysSinceCleanUp >= 15){
      if(m_Setup.cleanUp()){
         qInfo(logInfo()) << "Successfuly deleted old logs.";
         m_daysSinceCleanUp = 0;
      }else{
         qWarning(logWarning()) << "Failed to delete old logs, trying again next day.";
         m_daysSinceCleanUp = 14;
      }
   }

   for(auto it : m_Schedule){
      tmpSch.append(it);
   }

   tmpQueries = m_mainSQL.getStorage().getQueueQueries();
   tmpParams = m_Schedule[m_scheduleKey]->getQueueParameters();

   m_Export.handleExport(tmpSch, tmpQueries, tmpParams, m_mainSQL.getDatabase().getDatabase(), m_generatedBy);
}

void Reporter::on_toolButton_4_clicked(){
   QQueue<SQLquery> tmpQueries;
   QQueue<SQLParameter> tmpParams;
   instantSchedule = new CustomScheduling(this);
   instantSchedule->setModal(true);
   tmpQueries = m_mainSQL.getStorage().getQueueQueries();
   //fix this to use custom parameters

   if(instantSchedule->exec()){
      m_Export.customExport(*instantSchedule,
                            tmpQueries,
                            tmpParams,
                            m_mainSQL.getDatabase().getDatabase(),
                            m_CUSTOMINTERVAL,
                            m_generatedBy);
   }
}

void Reporter::on_param1_textEdited(const QString &arg1){
   if(m_Schedule[m_scheduleKey]->getParamCount() != 0){
      m_saveParameter();
   }
   //   m_saveSchedule();
}

void Reporter::on_param2_textEdited(const QString &arg1){
   if(m_Schedule[m_scheduleKey]->getParamCount() != 0){
      m_saveParameter();
   }
   //   m_saveSchedule();
}

void Reporter::on_param3_textEdited(const QString &arg1){
   if(m_Schedule[m_scheduleKey]->getParamCount() != 0){
      m_saveParameter();
   }
   //   m_saveSchedule();
}

void Reporter::on_param4_textEdited(const QString &arg1){
   if(m_Schedule[m_scheduleKey]->getParamCount() != 0){
      m_saveParameter();
   }
   //   m_saveSchedule();
}

void Reporter::on_param5_textEdited(const QString &arg1){
   if(m_Schedule[m_scheduleKey]->getParamCount() != 0){
      m_saveParameter();
   }
   //   m_saveSchedule();
}

void Reporter::on_shiftActive_clicked(){
   m_saveSchedule();
}
void Reporter::on_shiftMondayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_shiftTuesdayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_shiftWednesdayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_shiftThursdayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_shiftFridayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_shiftSaturdayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_shiftSundayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_shiftFrom_userTimeChanged(const QTime &time){
   // CAUSES BUGS
   //   m_saveSchedule();
}
void Reporter::on_shiftTo_userTimeChanged(const QTime &time){
   // CAUSES BUGS
   //   m_saveSchedule();
}
void Reporter::on_shiftFrom_2_userTimeChanged(const QTime &time){
   // CAUSES BUGS
   //   m_saveSchedule();
}

void Reporter::on_shiftAttach_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_shiftSubj_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_shiftEmail_textEdited(const QString &arg1){
   // CAUSES BUGS
   //   m_saveSchedule();
}

void Reporter::on_shiftXLS_textEdited(const QString &arg1){
   m_saveSchedule();
}
void Reporter::on_shiftCSV_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_shiftAttachXLS_clicked(){
   m_saveSchedule();
}

void Reporter::on_shiftAttachCSV_clicked(){
   m_saveSchedule();
}
void Reporter::on_shiftemailAdress_textEdited(const QString &arg1){
   m_saveSchedule();
}
void Reporter::on_dailyActive_clicked(){
   m_saveSchedule();
}
void Reporter::on_dailyTime_userTimeChanged(const QTime &time){
   // CAUSES BUGS
   //   m_saveSchedule();
}

void Reporter::on_dailyEmailAdress_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_dailyMondayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_dailyTuesdayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_dailyWednesdayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_dailyThursdayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_dailyFridayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_dailySaturdayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_dailySundayActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_dailyAttach_textEdited(const QString &arg1){
   m_saveSchedule();
}
void Reporter::on_dailySubj_textEdited(const QString &arg1){
   m_saveSchedule();
}
void Reporter::on_dailyEmail_textEdited(const QString &arg1){
   m_saveSchedule();
}
void Reporter::on_dailyXLS_textEdited(const QString &arg1){
   m_saveSchedule();
}
void Reporter::on_dailyCSV_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_dailyAttachXLS_clicked(){
   m_saveSchedule();
}

void Reporter::on_dailyAttachCSV_clicked(){
   m_saveSchedule();
}

void Reporter::on_weeklyActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_weeklyTime_userTimeChanged(const QTime &time){
   //   m_saveSchedule();
}
void Reporter::on_weeklyDays_currentIndexChanged(int index){
   // CAUSES BUGS
}
void Reporter::on_weeklyAttach_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_weeklySubj_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_weeklyEmail_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_weeklyXLS_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_weeklyCSV_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_weeklyemailAdress_textEdited(const QString &arg1){
   m_saveSchedule();
}
void Reporter::on_weeklyAttachXLS_clicked(){
   m_saveSchedule();
}
void Reporter::on_weeklyAttachCSV_clicked(){
   m_saveSchedule();
}

void Reporter::on_monthlyActive_clicked(){
   m_saveSchedule();
}

void Reporter::on_monthlyTIme_userTimeChanged(const QTime &time){
   // CAUSES BUGS
   //   m_saveSchedule();
}

void Reporter::on_monthlyEmailAdress_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_monthlyAttach_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_monthlySubj_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_monthlyEmail_textEdited(const QString &arg1){
   m_saveSchedule();
}
void Reporter::on_monthlyXLS_textEdited(const QString &arg1){
   m_saveSchedule();
}
void Reporter::on_monthlyCSV_textEdited(const QString &arg1){
   m_saveSchedule();
}

void Reporter::on_monthlyAttachXLS_clicked(){
   m_saveSchedule();
}

void Reporter::onMonthlyattachcsvClicked(){
   m_saveSchedule();
}

void Reporter::on_startTImer_clicked(){
   if(m_Timer->isActive()){
      m_PauseTimer();
      ui->startTImer->setText("Start");
   }else{
      //run interval check the first time the interval is started
      timerInterval();
      m_SetTimer(m_TIMERINTERVAL);
      ui->startTImer->setText("Pause");
   }
}

void Reporter::on_shiftGenerate_clicked(){
   quint32 count = 0;
   ShiftSchedule tmp;
   tmp = m_Schedule[m_scheduleKey]->getShiftCopy();
   QDateTime currentTime = QDateTime::currentDateTime();

   for(auto & it : m_Schedule[m_scheduleKey]->getParameters()){
      tmp = m_Schedule[m_scheduleKey]->getShiftCopy();
      tmp.setDone0(false);
      tmp.setDone1(false);
      tmp.setDone2(false);
      m_Export.m_shiftDayReset(tmp, currentTime);
      tmp.generateShiftData(currentTime);

      m_Export.m_generateShift(tmp,
                               m_mainSQL.getStorage().getQueueQueries(),
                               *it,
                               m_mainSQL.getDatabase().getDatabase(),
                               currentTime,
                               count,
                               m_generatedBy,
                               true);
   }
}

void Reporter::on_dailyGenerate_clicked(){
   quint32 count = 0;
   DailySchedule tmp;
   tmp = m_Schedule[m_scheduleKey]->getDailyCopy();
   QDateTime currentTime = QDateTime::currentDateTime();

   for(auto & it : m_Schedule[m_scheduleKey]->getParameters()){
      qInfo(logInfo()) << "generating daily instant";
      tmp = m_Schedule[m_scheduleKey]->getDailyCopy();
      tmp.setDone(false);
      tmp.generateDailyData(currentTime);

      m_Export.m_generateDaily(tmp,
                               m_mainSQL.getStorage().getQueueQueries(),
                               *it,
                               m_mainSQL.getDatabase().getDatabase(),
                               currentTime,
                               count,
                               m_generatedBy,
                               true);
   }
}

void Reporter::on_weeklyGenerate_clicked(){
   quint32 count = 0;
   WeeklySchedule tmp;
   tmp = m_Schedule[m_scheduleKey]->getWeeklyCopy();
   QDateTime currentTime = QDateTime::currentDateTime();

   for(auto & it : m_Schedule[m_scheduleKey]->getParameters()){
      qInfo(logInfo()) << "generating weekly instant";
      tmp.setDone(false);
      tmp.generateWeeklyData(currentTime);
      m_Export.m_generateWeekly(tmp,
                                m_mainSQL.getStorage().getQueueQueries(),
                                *it,
                                m_mainSQL.getDatabase().getDatabase(),
                                currentTime,
                                count,
                                m_generatedBy,
                                true);
   }
}

void Reporter::on_monthlyGenerate_clicked(){
   quint32 count = 0;
   MonthlySchedule tmp;
   tmp = m_Schedule[m_scheduleKey]->getMonthlyCopy();
   QDateTime currentTime = QDateTime::currentDateTime();

   for(auto & it : m_Schedule[m_scheduleKey]->getParameters()){
      qInfo(logInfo()) << "generating monthly instant";
      tmp.setDone(false);
      tmp.generateMonthlyData(currentTime);
      m_Export.m_generateMonthly(tmp,
                                 m_mainSQL.getStorage().getQueueQueries(),
                                 *it,
                                 m_mainSQL.getDatabase().getDatabase(),
                                 currentTime,
                                 count,
                                 m_generatedBy,
                                 true);
   }
}

void Reporter::on_customParameters_clicked(){
   auto tmpParameters = new customParametersQuery(this,
                                                  m_CustomParameters,
                                                  m_CustomParametersFrom,
                                                  m_CustomParametersTo);
   tmpParameters->setModal(true);

   if(tmpParameters->exec()){
      m_CustomParameters = tmpParameters->m_Parameters;
      m_CustomParametersFrom = tmpParameters->m_From;
      m_CustomParametersTo = tmpParameters->m_To;
      m_generate = true;
   }
   if(m_generate){
      //DEBUG
      for(const auto & it : m_CustomParameters){
         qInfo(logInfo()) << m_CustomParameters.key(it) + " : " + it;
      }
   }
   delete tmpParameters;
}

void Reporter::on_queryNameEdit_textEdited(const QString &arg1){
   //   if(!arg1.isEmpty()){
   //      m_saveQuery();
   //   }
}

void Reporter::on_queryEdit_textChanged(){
}

void Reporter::on_scheduleName_textEdited(const QString &arg1){
   //   if(!arg1.isEmpty()){
   //      m_saveSchedule();
   //   }
}
