#include <utility>
#include "sqlparameter.h"

SQLParameter::SQLParameter(QStringList  parameters, const qint32 & count)
   : m_Parameters(std::move(parameters)),
     m_paramCount(count){
}

SQLParameter::SQLParameter(qint32 count) : m_paramCount(count){

}

void SQLParameter::editInfo(const QStringList & param, const qint32 & count){
   m_Parameters = param;
   m_paramCount = count;
}


QStringList SQLParameter::getParameters() const{
   return m_Parameters;
}

QString SQLParameter::getName() const{
   return m_Name;
}

qint32 SQLParameter::getCount() const{
   return m_paramCount;
}

void SQLParameter::setCount(qint32 count){
   m_paramCount = count;
}

void SQLParameter::printParams() const{
   for(auto & it : m_Parameters){
      const QString& tmp = it;
      qDebug() << tmp;
   }
}

