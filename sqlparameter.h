#ifndef SQLPARAMETER_H
#define SQLPARAMETER_H

#include <QObject>
#include <QDebug>

class SQLParameter{
   public:
      //Constructor
      explicit SQLParameter(QStringList  parameters,
                            const qint32 & count);
      SQLParameter(qint32 count);
      //Setter for paremeter member
      void editInfo(const QStringList & param,
                    const qint32 & count);
      //Getters
      QStringList getParameters() const;
      QString getName() const;
      qint32 getCount() const;
      void setCount(qint32 count);
      void printParams() const;

   private:
      //parameter
      QStringList m_Parameters;
      //name PROBABLY USELESS?
      QString m_Name;
      qint32 m_paramCount;
};

#endif // SQLPARAMETER_H
