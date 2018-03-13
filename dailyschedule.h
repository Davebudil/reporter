#ifndef DAILYSCHEDULE_H
#define DAILYSCHEDULE_H
#include <QObject>
#include <QTime>
#include <QMap>
#include <QDebug>
#include <QDateTime>

class DailySchedule{
   public:
      DailySchedule();
      ~DailySchedule();

      QString getCsvTemplatePath() const;
      QString getXlsTemplatePath() const;
      QString getEmailTemplatePath() const;
      QString getSubjName() const;
      QString getAttachName() const;
      QMap<QString, QString> & getEmailAdresses();
      QTime getTime() const;
      bool getActive() const;
      bool * getDays() const;
      bool getXlsAttach() const;
      bool getCsvAttach() const;
      QStringList prepareSerialization();
      void deserializeList(const QStringList & list);

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
      void checkDoneInterval(QDateTime & currentDate);

      bool getDone() const;
      void setDone(bool Done);

   private:
      bool m_Active;
      bool * m_Days;
      bool m_csvAttach;
      bool m_xlsAttach;
      bool m_Done;
      QTime m_Time;
      qint32 m_lastDoneDay;
      QString m_AttachName;
      QString m_SubjName;
      QString m_xlsTemplatePath;
      QString m_csvTemplatePath;
      QString m_emailTemplatePath;
      QMap<QString, QString> m_emailAdresses;
};

#endif // DAILYSCHEDULE_H
