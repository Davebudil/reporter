#include "sqlparameter.h"

SQLParameter::SQLParameter(const QString & name)
   : m_Name(name){
}


QStringList SQLParameter::getParameters(){
   return m_Parameters;
}

void SQLParameter::addParameter(const QString & parameter){
   m_Parameters << parameter;
}

QString SQLParameter::getName(){
   return m_Name;
}

