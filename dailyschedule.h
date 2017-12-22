#ifndef DAILYSCHEDULE_H
#define DAILYSCHEDULE_H
#include <QObject>
#include <QTime>

class DailySchedule{
   public:
      DailySchedule();
      ~DailySchedule();

      QString getCsvTemplatePath() const;
      QString getXlsTemplatePath() const;
      QString getEmailTemplatePath() const;
      QString getSubjName() const;
      QString getAttachName() const;
      QTime getTime() const;
      bool getActive() const;
      bool * getDays() const;
      bool getXlsAttach() const;
      bool getCsvAttach() const;

      void setCsvTemplatePath(const QString & csvTemplatePath);
      void setXlsTemplatePath(const QString & xlsTemplatePath);
      void setEmailTemplatePath(const QString & emailTemplatePath);
      void setSubjName(const QString & SubjName);
      void setAttachName(const QString & AttachName);
      void setTime(const QTime & Time);
      void setActive(bool Active);
      void setDays(const qint32 day, bool value);
      void setXlsAttach(bool xlsAttach);
      void setCsvAttach(bool csvAttach);

   private:
      bool m_Active;
      bool * m_Days;
      bool m_csvAttach;
      bool m_xlsAttach;
      QTime m_Time;
      QString m_AttachName;
      QString m_SubjName;
      QString m_xlsTemplatePath;
      QString m_csvTemplatePath;
      QString m_emailTemplatePath;
};

#endif // DAILYSCHEDULE_H
