#ifndef MONTHLYSCHEDULE_H
#define MONTHLYSCHEDULE_H
#include <QObject>
#include <QDateEdit>

class MonthlySchedule
{
   public:
      MonthlySchedule();
      ~MonthlySchedule();

      QString getCsvTemplatePath() const;
      QString getXlsTemplatePath() const;
      QString getEmailTemplatePath() const;
      QString getSubjName() const;
      QString getAttachName() const;
      QTime getTime() const;
      qint32 getDay() const;
      bool getActive() const;
      bool getCsvAttach() const;
      bool getXlsAttach() const;
      QStringList prepareSerialization();

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

   private:
      bool m_Active;
      bool m_csvAttach;
      bool m_xlsAttach;
      qint32 m_Day;
      QTime m_Time;
      QString m_AttachName;
      QString m_SubjName;
      QString m_xlsTemplatePath;
      QString m_csvTemplatePath;
      QString m_emailTemplatePath;
};

#endif // MONTHLYSCHEDULE_H
