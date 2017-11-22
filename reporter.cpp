#include "reporter.h"
#include "ui_reporter.h"

//Constructor
Reporter::Reporter(QWidget *parent)
   : QMainWindow(parent),
     ui(new Ui::Reporter){
   ui->setupUi(this);
   ui->queryNameEdit->setText("Query Name");
   ui->queryParamEdit->setText("Master name");
   ui->queryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->dbPassword->setEchoMode(QLineEdit::Password);
   ui->dbPassword->setText(m_mainSQL.getPassword());
   QFont newFont("DejaVu Sans Mono", 11, QFont::Normal, true);
   setFont(newFont);
}
//Destructor
Reporter::~Reporter(){
   delete ui;
   delete tmp;
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
   if(!m_mainSQL.getDatabase().getDatabase().open()){
      QMessageBox::critical(0, QObject::tr("Database error"),
                            "Not connected to database");

   }else{
      m_generateQuery(m_nameKey);
      m_displaySQLResult(m_nameKey);
   }
}
//used to generate result from current selected query
void Reporter::m_generateQuery(const QString & name){
   m_mainSQL.getStorage().generateQuery(name,m_mainSQL.getDatabase().getDatabase());
}
//Function to add new query
void Reporter::on_newQuery_clicked(){
   m_addQuery(ui->queryEdit->toPlainText(),
              ui->queryNameEdit->text(),
              ui->queryParamEdit->text());
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
   if(m_nameKey.isEmpty()){
      QMessageBox::critical(this,QObject::tr("Text error"), "No query selected.");
      return;
   }else{
      if(m_mainSQL.getStorage().addQuery(queryText,queryName,paramName, false)){
         tmp = ui->scrollAreaWidgetContents->findChild<QToolButton *>(m_nameKey);
         tmp->setObjectName(queryName);
         tmp->setText(queryName);
         m_mainSQL.getStorage().getQueries().remove(m_nameKey);
         m_nameKey = queryName;
         m_Serialize();
      }else{
         m_mainSQL.getStorage().getQueries()[m_nameKey]->setParam(paramName);
         m_mainSQL.getStorage().getQueries()[m_nameKey]->setQuery(queryText);
         ui->queryEdit->document()->setPlainText(queryText);
         ui->queryParamEdit->setText(paramName);
         m_Serialize();
      }
   }
}
//function to add query
void Reporter::m_addQuery(const QString & queryText,
                          const QString & queryName,
                          const QString & paramName){
   QString param;
   if(paramName.isEmpty() || paramName.isNull()){
      param = "0";
   }else{
      param = paramName;
   }
   if(!queryText.isEmpty() && !queryName.isEmpty()){
      if(m_mainSQL.getStorage().addQuery(queryText,queryName,param, true)){
         QToolButton * newQuery = new QToolButton;
         newQuery->setText(queryName);
         //Ugly design settings ignore please
         newQuery->setStyleSheet("QToolButton:hover, QToolButton:pressed { "
                                 "background-color: #607D8B; } QToolButton::menu-button "
                                 "{ background: url('./images/downarrowgray.png') "
                                 "center center no-repeat; background-color: #FDD835; "
                                 "} QToolButton::menu-button:hover, QToolButton::menu-button:pressed "
                                 "{ background-color: #FDD835; } QStatusBar { background-color: "
                                 "#FDD835; } QLabel { color: #FDD835; } QToolButton "
                                 "{ color: black; background-color: #FDD835; "
                                 "} QToolButton:hover, QToolButton:pressed, "
                                 "QToolButton:checked { background-color: "
                                 "#607D8B; } QToolButton:hover { color: black; } "
                                 "QToolButton:checked, QToolButton:pressed { color: #FFFFFF; "
                                 "} QToolButton { border: 1px solid #FDD835; margin: 1px; } "
                                 "QToolButton:hover { background-color: #607D8B; border: 1px "
                                 "solid #607D8B; } QToolButton[popupMode=\"1\"] { padding-right: 20px; } QToolButton::menu-button { border-left: 1px solid #FDD835; background: transparent; width: 16px; } QToolButton::menu-button:hover { border-left: 1px solid #FDD835; background: transparent; width: 16px; } QStatusBar::item { color: black; background-color: #FDD835; } QAbstractScrollArea { /* Borders around the code editor and debug window */ border: 0; }");
         newQuery->setObjectName(queryName);
         ui->scrollLayout->addWidget(newQuery);
         connect(newQuery, &QToolButton::clicked, this, &Reporter::m_scrollQueryClicked);
         ui->queryEdit->clear();
         ui->queryNameEdit->clear();
         ui->queryParamEdit->clear();
         m_nameKey = "";
         m_Serialize();
      }
   }else{
      QMessageBox::critical(0, QObject::tr("Text error"), "No text entered.");
   }
}
//Sets up default settings of the app
void Reporter::defaultSettings(){
//   m_ConnectDB();
   m_Deserialize();
}
//Load the default query
void Reporter::m_defaultQuery(){

}
//Function to clear current text
void Reporter::on_clearQuery_clicked(){
   ui->queryEdit->clear();
   ui->queryNameEdit->clear();
   ui->queryParamEdit->clear();
}
//function to determine which button was clicked and select the stored query based on it
void Reporter::m_scrollQueryClicked(){
   QObject * senderObj = sender();
   QString senderObjname = senderObj->objectName();
   m_nameKey = senderObjname;
   ui->queryEdit->document()->setPlainText(m_mainSQL.getStorage().getQueries()[senderObjname]->getQuery());
   ui->queryNameEdit->setText(m_mainSQL.getStorage().getQueries()[senderObjname]->getName());
   ui->queryParamEdit->setText(m_mainSQL.getStorage().getQueries()[senderObjname]->getParam());
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
void Reporter::m_Serialize(){
   QStringList tmpSerialize = m_mainSQL.loadList();
   m_Setup.serialize(tmpSerialize);
}
//Deserialization
void Reporter::m_Deserialize(){
   QStringList tmpDeserialize;
   m_Setup.deserialize(tmpDeserialize);
   for(int i = 0; i + 2 < tmpDeserialize.size(); i+=3){
      m_addQuery(tmpDeserialize.at(i),tmpDeserialize.at(i+1),tmpDeserialize.at(i+2));
   }
}










