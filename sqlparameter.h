#ifndef SQLPARAMETER_H
#define SQLPARAMETER_H

#include <QObject>

class SQLParameter{
   public:
      //Constructor
      explicit SQLParameter(const QString & name);
      //Getter for parameter member
      QStringList getParameters();
      //Setter for paremeter member
      void addParameter(const QString & parameter);
      //Getter for name member
      QString getName();

   private:
      //parameter
      QStringList m_Parameters;
      //name
      QString m_Name;
};

#endif // SQLPARAMETER_H
