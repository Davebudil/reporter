#include "sqlparameter.h"

SQLParameter::SQLParameter(const QStringList & parameters, const qint32 & count)
   : m_Parameters(parameters),
     m_paramCount(count){
}

void SQLParameter::editInfo(const QStringList & param, const qint32 & count){
   m_Parameters = param;
   m_paramCount = count;
}


QStringList SQLParameter::getParameters(){
   return m_Parameters;
}

QString SQLParameter::getName(){
   return m_Name;
}

qint32 SQLParameter::getCount(){
   return m_paramCount;
}

void SQLParameter::printParams(){
   for(auto it : m_Parameters){
      QString tmp = it;
      qDebug() << tmp;
   }
}

