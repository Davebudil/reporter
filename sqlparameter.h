#ifndef SQLPARAMETER_H
#define SQLPARAMETER_H

#include <QObject>
#include <QDebug>

class SQLParameter{
   public:
      //Constructor
      explicit SQLParameter(const QStringList & parameters, const qint32 & count);
      //Getter for parameter member
      QStringList getParameters();
      //Setter for paremeter member
      void editInfo(const QStringList & param, const qint32 & count);
      //Getters
      QString getName();
      qint32 getCount();
      void printParams();

   private:
      //parameter
      QStringList m_Parameters;
      //name PROBABLY USELESS?
      QString m_Name;
      qint32 m_paramCount;
};

#endif // SQLPARAMETER_H
