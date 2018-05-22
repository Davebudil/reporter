#ifndef MONTHLYSCHEDULE_H
#define MONTHLYSCHEDULE_H
#include <QObject>
#include <QDateEdit>
#include <QMap>
#include <QDebug>
#include <QDateTime>
#include "sqlparameter.h"

class MonthlySchedule
{
   public:
      MonthlySchedule();
      ~MonthlySchedule();

      QString getCsvTemplatePath() const;
      QString getXlsTemplatePath() const;
      QString getEmailTemplatePath() const;
      QMap<QString, QString> & getEmailAdresses();
      QStringList emailAdresses();
      QString getSubjName() const;
      QString getAttachName() const;
      QTime getTime() const;
      qint32 getDay() const;
      bool getActive() const;
      bool getCsvAttach() const;
      bool getXlsAttach() const;
      QStringList prepareSerialization();
      void deserializeList(const QStringList & list);

      void setCsvTemplatePath(const QString & csvTemplatePath);
      void setXlsTemplatePath(const QString & xlsTemplatePath);
      void setSubjName(const QString & SubjName);
      void setAttachName(const QString & AttachName);
      void setTime(const QTime & Time);
      void setDay(const qint32 & Day);
      void setActive(bool Active);
      void setXlsAttach(bool xlsAttach);
      void setCsvAttach(bool csvAttach);
      void setEmailTemplatePath(const QString & emailTemplatePath);
      bool generateMonthlyData(const QDateTime & currentData);
      void fixParameters(QSharedPointer<SQLParameter> & param, QDateTime & currentTime);
      bool getDone() const;
      void setDone(bool Done);

   private:
      bool m_checkDoneInterval(const QDateTime & currentDate);
      void m_monthlyInterval();
      bool m_Active;
      bool m_csvAttach;
      bool m_xlsAttach;
      bool m_Done;
      QDateTime m_lastMonth;
      qint32 m_Day;
      QTime m_Time;
      QString m_AttachName;
      QString m_SubjName;
      QString m_xlsTemplatePath;
      QString m_csvTemplatePath;
      QString m_emailTemplatePath;
      QMap<QString, QString> m_emailAdresses;
};

#endif // MONTHLYSCHEDULE_H
