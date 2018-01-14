#include "reporter.h"
#include "ui_reporter.h"

//poznamky:TODO - knihovna na grid -> pocitani a tak, export template -> funkce, xslwrite, vyuzit, upravovani.
//CSV -> priorita, generovani, nemusi byt posta, dobre padatc


// databaze eventu -> ciselny log, verze

//Constructor
Reporter::Reporter(QWidget *parent)
   : QMainWindow(parent),
     ui(new Ui::Reporter),
     m_paramKey(0),
     m_scheduleKey(0),
     m_scheduleCount(0),
     tmp(nullptr),
     m_queryActive(false){
   ui->setupUi(this);
   ui->queryNameEdit->setText("Query Name");
   ui->queryParamEdit->setText("Master name");
   ui->queryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->dbPassword->setEchoMode(QLineEdit::Password);
   ui->dbPassword->setText(m_mainSQL.getPassword());
   QFont newFont("DejaVu Sans Mono", 11, QFont::Normal, true);
   setFont(newFont);
   ui->weeklyDays->setMaximumHeight(100);
   ui->monthlyDays->setMaximumHeight(100);
   m_shwHide = new QHotkey(QKeySequence("ctrl+alt+Q"), true);
   connect(m_shwHide, SIGNAL(activated()), this, SLOT(m_showHide()));
}
//Destructor
Reporter::~Reporter(){
   delete ui;
   delete m_shwHide;
   qDeleteAll(m_Schedule);
}
void Reporter::m_showHide(){
   if(isVisible()){
      hide();
   }else{
      show();
   }
}
//Function to connect to DB triggered by click connect button
void Reporter::on_dbConnect_clicked(){
   if(ui->dbPassword->text().isEmpty()){
      QMessageBox::critical(this, "Password Error", "No password entered.");
   }else{
      m_ConnectDB();
   }
   ui->dbPassword->setText(m_mainSQL.getPassword());
}
//Print query result to the table
void Reporter::m_displaySQLResult(const QString & name){
   m_mainSQL.setQueryModel(name);
   ui->queryTable->setModel(m_mainSQL.getModel());
}
//Function to Generate selected query and print results to table
void Reporter::on_toolButton_clicked(){
   m_testingQueryGen();
}

void Reporter::m_executeQuery(const QString & name){
   m_mainSQL.getStorage().executeQuery(name);
}

//used to generate result from current selected query
void Reporter::m_generateQuery(const QString & name){
   m_mainSQL.getStorage().generateQuery(name,m_mainSQL.getDatabase().getDatabase());
}
//Function to add new query
void Reporter::on_newQuery_clicked(){
   m_addQuery(ui->queryEdit->toPlainText(),
              ui->queryNameEdit->text(),
              ui->queryParamEdit->text(),
              true,
              m_queryActive);
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

   queryText = ui->queryEdit->toPlainText();
   queryName = ui->queryNameEdit->text();
   paramName = ui->queryParamEdit->text();

   if((!m_mainSQL.getStorage().getQueries().contains(paramName)) && (!paramName.isEmpty())){
      QMessageBox::critical(this,QObject::tr("New Query Error"), "No query with specified master param name exists.");
   }else if(m_nameKey.isEmpty()){
      QMessageBox::critical(this,QObject::tr("Query Edit Error"), "No query selected.");
   }else{
      if(m_mainSQL.getStorage().addQuery(queryText,queryName,paramName, m_queryActive, false, false)){
         tmp = ui->scrollAreaWidgetContents->findChild<QToolButton *>(m_nameKey);
         tmp->setObjectName(queryName);
         tmp->setText(queryName);
         m_mainSQL.getStorage().getQueries().remove(m_nameKey);
         m_nameKey = queryName;
      }else{
         m_mainSQL.getStorage().getQueries()[m_nameKey]->setParam(paramName);
         m_mainSQL.getStorage().getQueries()[m_nameKey]->setQuery(queryText);
         m_mainSQL.getStorage().getQueries()[m_nameKey]->setActive(m_queryActive);
         ui->queryEdit->document()->setPlainText(queryText);
         ui->queryParamEdit->setText(paramName);
      }
   }
   m_serializeQueries();
   m_loadMaster();
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
      if(m_mainSQL.getStorage().addQuery(queryText,queryName,param, active, true, mode)){
         QToolButton * newQuery = new QToolButton;
         newQuery->setText(queryName);
         //Ugly design settings ignore please
         newQuery->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                                 "background-color: #f44336; } QToolButton::menu-button "
                                 "{ background: url('./images/downarrowgray.png') "
                                 "center center no-repeat; background-color: #f44336; "
                                 "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                                 "{ background-color: #f44336; } QStatusBar { background-color: "
                                 "#FDD835; } QLabel { color: #f44336; } QToolButton "
                                 "{ color: white; background-color: #f44336; "
                                 "} QToolButton:hover, QToolButton:pressed, "
                                 "QToolButton:checked { background-color: "
                                 "#607D8B; } QToolButton:hover { color: black; } "
                                 "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                                 "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                                 "QToolButton:hover { background-color: white; color: black; border: 1px "
                                 "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                                 "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                                 "transparent; width: 16px; } QToolButton::menu-button:hover { "
                                 "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                                 "QStatusBar::item { color: black; background-color: #f44336; } "
                                 "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
         newQuery->setObjectName(queryName);
         ui->scrollLayout->addWidget(newQuery);
         connect(newQuery, &QToolButton::clicked, this, &Reporter::m_scrollQueryClicked);
         m_clearQuery();
         m_nameKey = "";
         m_serializeQueries();
      }
   }else{
      QMessageBox::critical(0, QObject::tr("Text error"), "No text entered.");
   }
}

void Reporter::m_addParameters(const QStringList & params, const qint32 & count){
   if(m_mainSQL.getStorage().addParam(params,count,m_paramKey)){
      QToolButton * newParameter = new QToolButton;
      newParameter->setText(params.at(0));
      newParameter->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                                  "background-color: #f44336; } QToolButton::menu-button "
                                  "{ background: url('./images/downarrowgray.png') "
                                  "center center no-repeat; background-color: #f44336; "
                                  "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                                  "{ background-color: #f44336; } QStatusBar { background-color: "
                                  "#FDD835; } QLabel { color: #f44336; } QToolButton "
                                  "{ color: white; background-color: #f44336; "
                                  "} QToolButton:hover, QToolButton:pressed, "
                                  "QToolButton:checked { background-color: "
                                  "#607D8B; } QToolButton:hover { color: black; } "
                                  "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                                  "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                                  "QToolButton:hover { background-color: white; color: black; border: 1px "
                                  "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                                  "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                                  "transparent; width: 16px; } QToolButton::menu-button:hover { "
                                  "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                                  "QStatusBar::item { color: black; background-color: #f44336; } "
                                  "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
      newParameter->setObjectName(QString::number(m_paramKey++));
      ui->scrollLayout_2->addWidget(newParameter);
      connect(newParameter, &QToolButton::clicked, this, &Reporter::m_scrollParamClicked);
      m_clearParam();
      m_selectedParam = -1;
      m_serializeParameters();
   }
}


void Reporter::m_addSchedule(const QString & name){
   if(name.isEmpty() || name.isNull()){
      QMessageBox::critical(this, QObject::tr("Text error"), QObject::tr("Enter name for new schedule."));
      return;
   }

   if(!m_noSchedule()){
      Scheduling * tmp = new Scheduling;
      tmp->setName(ui->scheduleName->text());
      m_Schedule.insert(m_scheduleCount, tmp);
      m_scheduleKey = m_scheduleCount++;
   }else{
      m_scheduleKey = 0;
      m_Schedule[m_scheduleKey]->setName(ui->scheduleName->text());
   }

   m_editDay(m_scheduleKey);
   m_editMonthly(m_scheduleKey);
   m_editShift(m_scheduleKey);
   m_editWeekly(m_scheduleKey);

   QToolButton * newSchedule = new QToolButton;
   newSchedule->setText(name);
   newSchedule->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                              "background-color: #f44336; } QToolButton::menu-button "
                              "{ background: url('./images/downarrowgray.png') "
                              "center center no-repeat; background-color: #f44336; "
                              "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                              "{ background-color: #f44336; } QStatusBar { background-color: "
                              "#FDD835; } QLabel { color: #f44336; } QToolButton "
                              "{ color: white; background-color: #f44336; "
                              "} QToolButton:hover, QToolButton:pressed, "
                              "QToolButton:checked { background-color: "
                              "#607D8B; } QToolButton:hover { color: black; } "
                              "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                              "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                              "QToolButton:hover { background-color: white; color: black; border: 1px "
                              "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                              "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                              "transparent; width: 16px; } QToolButton::menu-button:hover { "
                              "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                              "QStatusBar::item { color: black; background-color: #f44336; } "
                              "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
   newSchedule->setObjectName(QString::number(m_scheduleKey));
   ui->scrollLayour_3->addWidget(newSchedule);
   connect(newSchedule, &QToolButton::clicked, this, &Reporter::m_loadSchedule);
}
void Reporter::m_addShiftScheduleEmail(const QString & email){
   if(email.isEmpty() || email.isNull() || !m_validateEmail(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Enter valid email adress."));
      return;
   }

   if(m_Schedule[m_scheduleKey]->getShift().getEmailAdresses().contains(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Email adress already exists."));
      return;
   }

   QToolButton * newEmail = new QToolButton;
   newEmail->setText(email);
   newEmail->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                              "background-color: #f44336; } QToolButton::menu-button "
                              "{ background: url('./images/downarrowgray.png') "
                              "center center no-repeat; background-color: #f44336; "
                              "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                              "{ background-color: #f44336; } QStatusBar { background-color: "
                              "#FDD835; } QLabel { color: #f44336; } QToolButton "
                              "{ color: white; background-color: #f44336; "
                              "} QToolButton:hover, QToolButton:pressed, "
                              "QToolButton:checked { background-color: "
                              "#607D8B; } QToolButton:hover { color: black; } "
                              "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                              "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                              "QToolButton:hover { background-color: white; color: black; border: 1px "
                              "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                              "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                              "transparent; width: 16px; } QToolButton::menu-button:hover { "
                              "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                              "QStatusBar::item { color: black; background-color: #f44336; } "
                              "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
   newEmail->setObjectName(email);
   ui->shiftEmailLayout->addWidget(newEmail);
   connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadShiftEmail);
   m_Schedule[m_scheduleKey]->getShift().getEmailAdresses().insert(email, email);
}
void Reporter::m_addDailyScheduleEmail(const QString & email){
   if(email.isEmpty() || email.isNull() || !m_validateEmail(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Enter valid email adress."));
      return;
   }

   if(m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses().contains(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Email adress already exists."));
      return;
   }

   QToolButton * newEmail = new QToolButton;
   newEmail->setText(email);
   newEmail->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                              "background-color: #f44336; } QToolButton::menu-button "
                              "{ background: url('./images/downarrowgray.png') "
                              "center center no-repeat; background-color: #f44336; "
                              "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                              "{ background-color: #f44336; } QStatusBar { background-color: "
                              "#FDD835; } QLabel { color: #f44336; } QToolButton "
                              "{ color: white; background-color: #f44336; "
                              "} QToolButton:hover, QToolButton:pressed, "
                              "QToolButton:checked { background-color: "
                              "#607D8B; } QToolButton:hover { color: black; } "
                              "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                              "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                              "QToolButton:hover { background-color: white; color: black; border: 1px "
                              "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                              "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                              "transparent; width: 16px; } QToolButton::menu-button:hover { "
                              "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                              "QStatusBar::item { color: black; background-color: #f44336; } "
                              "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
   newEmail->setObjectName(email);
   ui->dailyEmailLayout->addWidget(newEmail);
   connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadDailyEmail);
   m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses().insert(email, email);
}
void Reporter::m_addWeeklyScheduleEmail(const QString & email){
   if(email.isEmpty() || email.isNull() || !m_validateEmail(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Enter valid email adress."));
      return;
   }

   if(m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses().contains(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Email adress already exists."));
      return;
   }

   QToolButton * newEmail = new QToolButton;
   newEmail->setText(email);
   newEmail->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                              "background-color: #f44336; } QToolButton::menu-button "
                              "{ background: url('./images/downarrowgray.png') "
                              "center center no-repeat; background-color: #f44336; "
                              "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                              "{ background-color: #f44336; } QStatusBar { background-color: "
                              "#FDD835; } QLabel { color: #f44336; } QToolButton "
                              "{ color: white; background-color: #f44336; "
                              "} QToolButton:hover, QToolButton:pressed, "
                              "QToolButton:checked { background-color: "
                              "#607D8B; } QToolButton:hover { color: black; } "
                              "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                              "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                              "QToolButton:hover { background-color: white; color: black; border: 1px "
                              "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                              "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                              "transparent; width: 16px; } QToolButton::menu-button:hover { "
                              "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                              "QStatusBar::item { color: black; background-color: #f44336; } "
                              "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
   newEmail->setObjectName(email);
   ui->weeklyEmailLayout->addWidget(newEmail);
   connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadWeeklyEmail);
   m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses().insert(email, email);
}
void Reporter::m_addMonthlyScheduleEmail(const QString & email){
   if(email.isEmpty() || email.isNull() || !m_validateEmail(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Enter valid email adress."));
      return;
   }

   if(m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses().contains(email)){
      QMessageBox::critical(this, QObject::tr("Email error"), QObject::tr("Email adress already exists."));
      return;
   }

   QToolButton * newEmail = new QToolButton;
   newEmail->setText(email);
   newEmail->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                              "background-color: #f44336; } QToolButton::menu-button "
                              "{ background: url('./images/downarrowgray.png') "
                              "center center no-repeat; background-color: #f44336; "
                              "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                              "{ background-color: #f44336; } QStatusBar { background-color: "
                              "#FDD835; } QLabel { color: #f44336; } QToolButton "
                              "{ color: white; background-color: #f44336; "
                              "} QToolButton:hover, QToolButton:pressed, "
                              "QToolButton:checked { background-color: "
                              "#607D8B; } QToolButton:hover { color: black; } "
                              "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                              "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                              "QToolButton:hover { background-color: white; color: black; border: 1px "
                              "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                              "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                              "transparent; width: 16px; } QToolButton::menu-button:hover { "
                              "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                              "QStatusBar::item { color: black; background-color: #f44336; } "
                              "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
   newEmail->setObjectName(email);
   ui->monthlyEmailLayout->addWidget(newEmail);
   connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadMonthlyEmail);
   m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses().insert(email, email);
}

void Reporter::m_scrollParamClicked(){
   QObject * senderObj = sender();
   qint32 paramCount;
   QString senderObjID = senderObj->objectName();

   m_selectedParam = senderObjID.toInt();
   paramCount = m_mainSQL.getStorage().getParameters()[m_selectedParam]->getCount();
   m_clearParam();

   if(paramCount > 0){
      ui->param1->setText(m_mainSQL.getStorage().getParameters()[m_selectedParam]->getParameters().at(0));
   }
   if(paramCount > 1){
      ui->param2->setText(m_mainSQL.getStorage().getParameters()[m_selectedParam]->getParameters().at(1));
   }
   if(paramCount > 2){
      ui->param3->setText(m_mainSQL.getStorage().getParameters()[m_selectedParam]->getParameters().at(2));
   }
   if(paramCount > 3){
      ui->param4->setText(m_mainSQL.getStorage().getParameters()[m_selectedParam]->getParameters().at(3));
   }
   if(paramCount > 4){
      ui->param5->setText(m_mainSQL.getStorage().getParameters()[m_selectedParam]->getParameters().at(4));
   }
}

void Reporter::m_saveParameter(){
   QStringList parameters;
   qint32 tmpCount = 0;

   m_createParamList(parameters,tmpCount);
   if(tmpCount == 0 || m_selectedParam == -1){
      QMessageBox::critical(this,QObject::tr("Text error"), "No parameter selected.");
   }else{
      m_mainSQL.getStorage().getParameters()[m_selectedParam]->editInfo(parameters, tmpCount);
      tmp = ui->scrollAreaWidgetContents_2->findChild<QToolButton *>(QString::number(m_selectedParam));
      tmp->setText(parameters.at(0));
      m_serializeParameters();
   }
}

void Reporter::m_deleteQuery(){
   m_mainSQL.getStorage().getQueries().remove(m_nameKey);
   tmp = ui->scrollAreaWidgetContents->findChild<QToolButton *>(m_nameKey);
   delete tmp;
   m_clearQuery();
   m_serializeQueries();
   m_nameKey = "";
}

void Reporter::m_deleteSchedule(){
   m_deleteEmails();
   m_Schedule.remove(m_scheduleKey);
   tmp = ui->scrollSchedule->findChild<QToolButton *>(QString::number(m_scheduleKey));
   delete tmp;
   m_scheduleCount--;
   //serialize
   m_clearSchedule();
   if(m_noSchedule()){
      Scheduling * tmp = new Scheduling;
      m_scheduleKey = 0;
      m_Schedule[0] = tmp;
      m_scheduleCount++;
   }
   m_Schedule[m_scheduleKey]->getShift().getEmailAdresses().clear();
   m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses().clear();
   m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses().clear();
   m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses().clear();
}

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

void Reporter::m_deleteParam(){
   m_mainSQL.getStorage().getParameters().remove(m_selectedParam);
   tmp = ui->scrollAreaWidgetContents_2->findChild<QToolButton *>(QString::number(m_selectedParam));
   delete tmp;
   m_selectedParam = -1;
   m_clearParam();
   m_serializeParameters();
}

//Sets up default settings of the app
void Reporter::defaultSettings(){
   m_ConnectDB();
   m_Deserialize();
//   m_deserializeSchedule();
   m_loadMaster();
   if(m_noSchedule()){
      Scheduling * tmp = new Scheduling;
      m_Schedule[m_scheduleKey] = tmp;
      m_scheduleCount++;
   }
}
//Load the default query
void Reporter::m_defaultQuery(){

}
//Function to clear current text
void Reporter::on_clearQuery_clicked(){
   ui->queryEdit->clear();
   ui->queryNameEdit->clear();
   ui->queryParamEdit->clear();
   ui->queryActive->setChecked(false);
}
//function to determine which button was clicked and select the stored query based on it
void Reporter::m_scrollQueryClicked(){
   QObject * senderObj = sender();
   QString senderObjname = senderObj->objectName();
   m_nameKey = senderObjname;

   ui->queryEdit->document()->setPlainText(m_mainSQL.getStorage().getQueries()[senderObjname]->getQuery());
   ui->queryNameEdit->setText(m_mainSQL.getStorage().getQueries()[senderObjname]->getName());
   ui->queryParamEdit->setText(m_mainSQL.getStorage().getQueries()[senderObjname]->getParam());
   ui->queryActive->setChecked(m_mainSQL.getStorage().getQueries()[senderObjname]->getActive());
}
//Setup connection to DB
void Reporter::m_ConnectDB(){
   if(!m_mainSQL.getDatabase().createConnection()){
      QMessageBox::warning(this, "Database connection error",
                           m_mainSQL.getDatabase().getDatabase().lastError().text());
   }else{
      QMessageBox::information(this, "Database connection success", "Connecting to database successful.");
   }
}
//Serialization
void Reporter::m_serializeQueries(){
   QStringList tmpSerialize = m_mainSQL.loadList();
   m_Setup.serializeQueries(tmpSerialize);
}
void Reporter::m_serializeParameters(){
   QVector<qint32> tmpCount;
   QStringList tmpSerialize = m_loadParameters(tmpCount);
   m_Setup.serializeParameters(tmpSerialize,tmpCount);
}
void Reporter::m_serializeSchedule(){
   QList<QStringList> scheduleSerialization;
   for(auto & it : m_Schedule){
      scheduleSerialization.append(it->getShift().prepareSerialization());
      scheduleSerialization.append(it->getDaily().prepareSerialization());
      scheduleSerialization.append(it->getWeekly().prepareSerialization());
      scheduleSerialization.append(it->getMonthly().prepareSerialization());
   }
   m_Setup.serializeSchedule(scheduleSerialization);
}
//Deserialization
void Reporter::m_Deserialize(){
   QStringList tmpDeserializeQueries;

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

   m_Setup.deserializeParameters(tmpDeserializeParameters,tmpDeserializeCounts);
   for(auto it : tmpDeserializeCounts){
      qint32 tmpInt = it;
      QStringList tmpList;

      for(qint32 i = tmpInt; i > 0; i--){
         tmpList << tmpDeserializeParameters.at(drivingI);
         drivingI++;
      }
      m_addParameters(tmpList,tmpInt);
   }
}
void Reporter::m_deserializeSchedule(){
   QStringList shift;
   QStringList day;
   QStringList weekly;
   QStringList monthly;

//   m_Setup.deserializeSchedule(shift, day, weekly, monthly);
//   m_Schedule[keyString]->getShift().deserializeList(shift);
//   m_Schedule[keyString]->getDaily().deserializeList(day);
//   m_Schedule[keyString]->getWeekly().deserializeList(weekly);
//   m_Schedule[keyString]->getMonthly().deserializeList(monthly);
}

void Reporter::m_saveSchedule(){
   m_editShift(m_scheduleKey);
   m_editDay(m_scheduleKey);
   m_editWeekly(m_scheduleKey);
   m_editMonthly(m_scheduleKey);
   m_editCustom();
   m_serializeSchedule();
}

void Reporter::m_generateCSV(){
//   //TESTING FOR SHIFT
//   QSqlQuery generateCSV;
//   generateCSV = m_mainSQL.getStorage().getQueries()[m_nameKey]->getResultQuery();
//   m_Export.getCSV().generateFile(m_Schedule[keyString]->getShift().getCsvTemplatePath(),
//                                  m_Schedule[keyString]->getShift().getAttachName(),
//                                  generateCSV);
}

void Reporter::m_generateXLS(){
//   //TESTING FOR SHIFT
//   QList<std::pair<QString, QString>> tmp;
//   QList<QStringList> tmpQueries;
//   m_generateQuery(m_nameKey);
//   m_executeQuery(m_nameKey);
//   tmp.append(std::make_pair("CURRENT_DATE","15.12.2017"));
//   tmp.append(std::make_pair("DateTimeFromTo", "Od datumu A po datum B"));
//   tmp.append(std::make_pair("vygeneroval", "David BUDIL"));
//   tmpQueries.append(m_mainSQL.getStorage().getQueries()[m_nameKey]->queryList());
//   if(!m_mainSQL.getStorage().getQueries()[m_nameKey]->getIsMaster()){
//      m_generateQuery(m_mainSQL.getStorage().getQueries()[m_nameKey]->getParam());
//      m_executeQuery(m_mainSQL.getStorage().getQueries()[m_nameKey]->getParam());
//      tmpQueries.push_front(m_mainSQL.getStorage().getQueries()[m_mainSQL.getStorage().getQueries()[m_nameKey]->getParam()]->queryList());
//   }

//   m_Export.getXLS().generateFile(m_Schedule[keyString]->getShift().getXlsTemplatePath(),
//                                  m_Schedule[keyString]->getShift().getAttachName(),
//                                  tmp,
//                                  tmpQueries);
}

void Reporter::m_generateTemplateXLS(){
   m_testingQueryGen();
   QXlsx::Document xlsx;
   QSqlQuery tmpModel = m_mainSQL.getStorage().getQueries()[m_nameKey]->getResultQuery();
   QSqlRecord rec = tmpModel.record();
   for(qint32 i = 0; i < rec.count(); ++i){
      QString tmp = rec.fieldName(i);
      xlsx.write(1, i + 1, tmp);
   }
   xlsx.saveAs("/home/dave/Documents/sielaff/project/reporter/reporter/templateFieldNamesXLSX.xlsx");
   QDesktopServices::openUrl(QUrl("/home/dave/Documents/sielaff/project/reporter/reporter/templateFieldNamesXLSX.xlsx"));
}

void Reporter::m_testingQueryGen(){
   if(!m_mainSQL.getDatabase().getDatabase().open()){
      QMessageBox::critical(0, QObject::tr("Database error"),
                            "Not connected to database");

   }else{
      m_generateQuery(m_nameKey);
      m_executeQuery(m_nameKey);
      m_displaySQLResult(m_nameKey);
//      m_mainSQL.getStorage().printQueryText();
   }
}
//Adds first schedule item
bool Reporter::m_noSchedule(){
   return (ui->scrollLayour_3->count() == 0 ? true : false);
}

bool Reporter::m_validateEmail(const QString & email){
   QRegularExpression regex("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
   return regex.match(email).hasMatch();
}
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
}

void Reporter::m_loadShiftEmail(){
   QObject * senderObj = sender();
   m_emailKey = senderObj->objectName();
   ui->shiftemailAdress->setText(m_Schedule[m_scheduleKey]->getShift().getEmailAdresses()[m_emailKey]);
}

void Reporter::m_loadDailyEmail(){
   QObject * senderObj = sender();
   m_emailKey = senderObj->objectName();
   ui->shiftemailAdress->setText(m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses()[m_emailKey]);
}

void Reporter::m_loadWeeklyEmail(){
   QObject * senderObj = sender();
   m_emailKey = senderObj->objectName();
   ui->shiftemailAdress->setText(m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses()[m_emailKey]);
}

void Reporter::m_loadMonthlyEmail(){
   QObject * senderObj = sender();
   m_emailKey = senderObj->objectName();
   ui->shiftemailAdress->setText(m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses()[m_emailKey]);
}

void Reporter::on_paramNew_clicked(){
   QStringList tmp;
   qint32 tmpCount = 0;

   m_createParamList(tmp, tmpCount);
   if(tmpCount != 0){
      m_addParameters(tmp, tmpCount);
   }else{
      QMessageBox::warning(this, "Text error", "No parameters entered.");
   }
}

void Reporter::on_paramEdit_clicked(){
   m_saveParameter();
}

void Reporter::m_loadMaster(){
   for(auto & it : m_mainSQL.getStorage().getQueries()){
      if(!it->getIsMaster()){
         m_mainSQL.getStorage().masterQuery(it->getName(),it->getParam());
      }
   }
}

void Reporter::m_loadEmails(){
   for(auto & it : m_Schedule[m_scheduleKey]->getShift().getEmailAdresses()){
      QString email = it;
      QToolButton * newEmail = new QToolButton;
      newEmail->setText(email);
      newEmail->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                                 "background-color: #f44336; } QToolButton::menu-button "
                                 "{ background: url('./images/downarrowgray.png') "
                                 "center center no-repeat; background-color: #f44336; "
                                 "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                                 "{ background-color: #f44336; } QStatusBar { background-color: "
                                 "#FDD835; } QLabel { color: #f44336; } QToolButton "
                                 "{ color: white; background-color: #f44336; "
                                 "} QToolButton:hover, QToolButton:pressed, "
                                 "QToolButton:checked { background-color: "
                                 "#607D8B; } QToolButton:hover { color: black; } "
                                 "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                                 "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                                 "QToolButton:hover { background-color: white; color: black; border: 1px "
                                 "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                                 "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                                 "transparent; width: 16px; } QToolButton::menu-button:hover { "
                                 "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                                 "QStatusBar::item { color: black; background-color: #f44336; } "
                                 "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
      newEmail->setObjectName(email);
      ui->shiftEmailLayout->addWidget(newEmail);
      connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadShiftEmail);
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getDaily().getEmailAdresses()){
      QString email = it;
      QToolButton * newEmail = new QToolButton;
      newEmail->setText(email);
      newEmail->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                                 "background-color: #f44336; } QToolButton::menu-button "
                                 "{ background: url('./images/downarrowgray.png') "
                                 "center center no-repeat; background-color: #f44336; "
                                 "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                                 "{ background-color: #f44336; } QStatusBar { background-color: "
                                 "#FDD835; } QLabel { color: #f44336; } QToolButton "
                                 "{ color: white; background-color: #f44336; "
                                 "} QToolButton:hover, QToolButton:pressed, "
                                 "QToolButton:checked { background-color: "
                                 "#607D8B; } QToolButton:hover { color: black; } "
                                 "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                                 "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                                 "QToolButton:hover { background-color: white; color: black; border: 1px "
                                 "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                                 "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                                 "transparent; width: 16px; } QToolButton::menu-button:hover { "
                                 "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                                 "QStatusBar::item { color: black; background-color: #f44336; } "
                                 "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
      newEmail->setObjectName(email);
      ui->dailyEmailLayout->addWidget(newEmail);
      connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadDailyEmail);
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getWeekly().getEmailAdresses()){
      QString email = it;
      QToolButton * newEmail = new QToolButton;
      newEmail->setText(email);
      newEmail->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                                 "background-color: #f44336; } QToolButton::menu-button "
                                 "{ background: url('./images/downarrowgray.png') "
                                 "center center no-repeat; background-color: #f44336; "
                                 "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                                 "{ background-color: #f44336; } QStatusBar { background-color: "
                                 "#FDD835; } QLabel { color: #f44336; } QToolButton "
                                 "{ color: white; background-color: #f44336; "
                                 "} QToolButton:hover, QToolButton:pressed, "
                                 "QToolButton:checked { background-color: "
                                 "#607D8B; } QToolButton:hover { color: black; } "
                                 "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                                 "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                                 "QToolButton:hover { background-color: white; color: black; border: 1px "
                                 "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                                 "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                                 "transparent; width: 16px; } QToolButton::menu-button:hover { "
                                 "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                                 "QStatusBar::item { color: black; background-color: #f44336; } "
                                 "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
      newEmail->setObjectName(email);
      ui->weeklyEmailLayout->addWidget(newEmail);
      connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadWeeklyEmail);
   }
   for(auto & it : m_Schedule[m_scheduleKey]->getMonthly().getEmailAdresses()){
      QString email = it;
      QToolButton * newEmail = new QToolButton;
      newEmail->setText(email);
      newEmail->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                                 "background-color: #f44336; } QToolButton::menu-button "
                                 "{ background: url('./images/downarrowgray.png') "
                                 "center center no-repeat; background-color: #f44336; "
                                 "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                                 "{ background-color: #f44336; } QStatusBar { background-color: "
                                 "#FDD835; } QLabel { color: #f44336; } QToolButton "
                                 "{ color: white; background-color: #f44336; "
                                 "} QToolButton:hover, QToolButton:pressed, "
                                 "QToolButton:checked { background-color: "
                                 "#607D8B; } QToolButton:hover { color: black; } "
                                 "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                                 "} QToolButton { border: 1px solid #f44336; margin: 1px; } "
                                 "QToolButton:hover { background-color: white; color: black; border: 1px "
                                 "solid #f44336; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } "
                                 "QToolButton::menu-button { border-left: 1px solid #f44336; background: "
                                 "transparent; width: 16px; } QToolButton::menu-button:hover { "
                                 "border-left: 1px solid #FDD835; background: transparent; width: 16px; } "
                                 "QStatusBar::item { color: black; background-color: #f44336; } "
                                 "QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
      newEmail->setObjectName(email);
      ui->monthlyEmailLayout->addWidget(newEmail);
      connect(newEmail, &QToolButton::clicked, this, &Reporter::m_loadMonthlyEmail);
   }
}

QStringList Reporter::m_loadParameters(QVector<qint32> & count){
   QStringList tmp;
   for(auto it : m_mainSQL.getStorage().getParameters()){
      count.append(it->getCount());
      tmp+=it->getParameters();
   }
   return tmp;
}

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

void Reporter::m_clearSchedule(){
   m_clearShift();
   m_clearDaily();
   m_clearWeekly();
   m_clearMonthly();
   ui->scheduleName->clear();
}

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
   ui->shiftTo_2->clear();
   ui->shiftEmail->clear();
   ui->shiftMondayActive->setChecked(false);
   ui->shiftTuesdayActive->setChecked(false);
   ui->shiftWednesdayActive->setChecked(false);
   ui->shiftThursdayActive->setChecked(false);
   ui->shiftFridayActive->setChecked(false);
   ui->shiftSaturdayActive->setChecked(false);
   ui->shiftSundayActive->setChecked(false);
}

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

void Reporter::on_paramDelete_clicked(){
   m_deleteParam();
}

void Reporter::on_queryDelete_clicked(){
   m_deleteQuery();
}

void Reporter::on_queryActive_stateChanged(int state){
   m_queryActive = state;
}
void Reporter::m_displayShift(qint32 keyString){
   m_loadActiveShiftDays(keyString);
   ui->shiftActive->setChecked(m_Schedule[keyString]->getShift().getActive());
   ui->shiftAttachCSV->setChecked(m_Schedule[keyString]->getShift().getCsvAttach());
   ui->shiftAttachXLS->setChecked(m_Schedule[keyString]->getShift().getXlsAttach());
   ui->shiftSubj->setText(m_Schedule[keyString]->getShift().getSubjName());
   ui->shiftAttach->setText(m_Schedule[keyString]->getShift().getAttachName());
   ui->shiftCSV->setText(m_Schedule[keyString]->getShift().getCsvTemplatePath());
   ui->shiftXLS->setText(m_Schedule[keyString]->getShift().getXlsTemplatePath());
   ui->shiftFrom->setTime(m_Schedule[keyString]->getShift().getFrom0());
   ui->shiftTo->setTime(m_Schedule[keyString]->getShift().getTo0());
   ui->shiftFrom_2->setTime(m_Schedule[keyString]->getShift().getFrom1());
   ui->shiftTo_2->setTime(m_Schedule[keyString]->getShift().getTo1());
   ui->shiftEmail->setText(m_Schedule[keyString]->getShift().getEmailTemplatePath());
}
void Reporter::m_displayDay(qint32 keyString){
   m_loadActiveDailyDays(keyString);
   ui->dailyActive->setChecked(m_Schedule[keyString]->getDaily().getActive());
   ui->dailyAttachCSV->setChecked(m_Schedule[keyString]->getDaily().getCsvAttach());
   ui->dailyAttachXLS->setChecked(m_Schedule[keyString]->getDaily().getXlsAttach());
   ui->dailySubj->setText(m_Schedule[keyString]->getDaily().getSubjName());
   ui->dailyAttach->setText(m_Schedule[keyString]->getDaily().getAttachName());
   ui->dailyCSV->setText(m_Schedule[keyString]->getDaily().getCsvTemplatePath());
   ui->dailyXLS->setText(m_Schedule[keyString]->getDaily().getXlsTemplatePath());
   ui->dailyTime->setTime(m_Schedule[keyString]->getDaily().getTime());
   ui->dailyEmail->setText(m_Schedule[keyString]->getDaily().getEmailTemplatePath());
}
void Reporter::m_displayWeekly(qint32 keyString){
   ui->weeklyActive->setChecked(m_Schedule[keyString]->getWeekly().getActive());
   ui->weeklyAttachCSV->setChecked(m_Schedule[keyString]->getWeekly().getCsvAttach());
   ui->weeklyAttachXLS->setChecked(m_Schedule[keyString]->getWeekly().getXlsAttach());
   ui->weeklySubj->setText(m_Schedule[keyString]->getWeekly().getSubjName());
   ui->weeklyAttach->setText(m_Schedule[keyString]->getWeekly().getAttachName());
   ui->weeklyCSV->setText(m_Schedule[keyString]->getWeekly().getCsvTemplatePath());
   ui->weeklyXLS->setText(m_Schedule[keyString]->getWeekly().getXlsTemplatePath());
   ui->weeklyTime->setTime(m_Schedule[keyString]->getWeekly().getTime());
   ui->weeklyDays->setCurrentIndex(m_Schedule[keyString]->getWeekly().getDay());
   ui->weeklyEmail->setText(m_Schedule[keyString]->getWeekly().getEmailTemplatePath());
}
void Reporter::m_displayMonthly(qint32 keyString){
   ui->monthlyActive->setChecked(m_Schedule[keyString]->getMonthly().getActive());
   ui->monthlyAttachCSV->setChecked(m_Schedule[keyString]->getMonthly().getCsvAttach());
   ui->monthlyAttachXLS->setChecked(m_Schedule[keyString]->getMonthly().getXlsAttach());
   ui->monthlySubj->setText(m_Schedule[keyString]->getMonthly().getSubjName());
   ui->monthlyAttach->setText(m_Schedule[keyString]->getMonthly().getAttachName());
   ui->monthlyCSV->setText(m_Schedule[keyString]->getMonthly().getCsvTemplatePath());
   ui->monthlyXLS->setText(m_Schedule[keyString]->getMonthly().getXlsTemplatePath());
   ui->monthlyTIme->setTime(m_Schedule[keyString]->getMonthly().getTime());
   ui->monthlyDays->setCurrentIndex(m_Schedule[keyString]->getMonthly().getDay());
   ui->monthlyEmail->setText(m_Schedule[keyString]->getMonthly().getEmailTemplatePath());
}
void Reporter::m_displayCustom(qint32 keyString){
   //work in progress, in future maybe
}
void Reporter::m_loadActiveShiftDays(qint32 keyString){
   ui->shiftMondayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[0]);
   ui->shiftTuesdayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[1]);
   ui->shiftWednesdayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[2]);
   ui->shiftThursdayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[3]);
   ui->shiftFridayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[4]);
   ui->shiftSaturdayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[5]);
   ui->shiftSundayActive->setChecked(m_Schedule[keyString]->getShift().getDays()[6]);
}
void Reporter::m_loadActiveDailyDays(qint32 keyString){
   ui->dailyMondayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[0]);
   ui->dailyTuesdayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[1]);
   ui->dailyWednesdayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[2]);
   ui->dailyThursdayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[3]);
   ui->dailyFridayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[4]);
   ui->dailySaturdayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[5]);
   ui->dailySundayActive->setChecked(m_Schedule[keyString]->getDaily().getDays()[6]);
}
void Reporter::m_editActiveShiftDays(qint32 keyString){
   m_Schedule[keyString]->getShift().setDays(0, ui->shiftMondayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(1, ui->shiftTuesdayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(2, ui->shiftWednesdayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(3, ui->shiftThursdayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(4, ui->shiftFridayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(5, ui->shiftSaturdayActive->isChecked());
   m_Schedule[keyString]->getShift().setDays(6, ui->shiftSundayActive->isChecked());
}
void Reporter::m_editActiveDailyDays(qint32 keyString){
   m_Schedule[keyString]->getDaily().setDays(0, ui->shiftMondayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(1, ui->shiftTuesdayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(2, ui->shiftWednesdayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(3, ui->shiftThursdayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(4, ui->shiftFridayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(5, ui->shiftSaturdayActive->isChecked());
   m_Schedule[keyString]->getDaily().setDays(6, ui->shiftSundayActive->isChecked());
}
void Reporter::m_editShift(qint32 keyString){
   m_editActiveShiftDays(keyString);
   m_Schedule[keyString]->getShift().setActive(ui->shiftActive->isChecked());
   m_Schedule[keyString]->getShift().setAttachName(ui->shiftAttach->text());
   m_Schedule[keyString]->getShift().setSubjName(ui->shiftSubj->text());
   m_Schedule[keyString]->getShift().setXlsTemplatePath(ui->shiftXLS->text());
   m_Schedule[keyString]->getShift().setCsvTemplatePath(ui->shiftCSV->text());
   m_Schedule[keyString]->getShift().setFrom0(ui->shiftFrom->time());
   m_Schedule[keyString]->getShift().setTo0(ui->shiftTo->time());
   m_Schedule[keyString]->getShift().setFrom1(ui->shiftFrom_2->time());
   m_Schedule[keyString]->getShift().setTo1(ui->shiftTo_2->time());
   m_Schedule[keyString]->getShift().setCsvAttach(ui->shiftAttachCSV->isChecked());
   m_Schedule[keyString]->getShift().setXlsAttach(ui->shiftAttachXLS->isChecked());
   m_Schedule[keyString]->getShift().setEmailTemplatePath(ui->shiftEmail->text());
}
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

void Reporter::m_editCustom(){
   //work in progress, in future maybe
}
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
   m_generateXLS();
   m_generateCSV();
}

void Reporter::on_toolButton_3_clicked(){
   m_generateTemplateXLS();
}
void Reporter::on_newScheduling_clicked(){
   m_deleteEmails();
   m_addSchedule(ui->scheduleName->text());
   m_serializeSchedule();
}
void Reporter::on_deleteScheduling_clicked(){
   m_deleteSchedule();
   m_serializeSchedule();
}
void Reporter::on_shiftnewEmailAdress_clicked(){
   m_addShiftScheduleEmail(ui->shiftemailAdress->text());
}
void Reporter::on_dailynewEmailAdress_clicked(){
   m_addDailyScheduleEmail(ui->dailyEmailAdress->text());
}
void Reporter::on_saveEmailAdress_7_clicked(){
   m_addWeeklyScheduleEmail(ui->weeklyemailAdress->text());
}
void Reporter::on_monthlynewEmailAdress_clicked(){
   m_addMonthlyScheduleEmail(ui->monthlyEmailAdress->text());
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
