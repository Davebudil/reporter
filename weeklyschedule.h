#ifndef WEEKLYSCHEDULE_H
#define WEEKLYSCHEDULE_H
#include <QObject>
#include <QDateTime>
#include <QMap>
#include <QDebug>

class WeeklySchedule
{
   public:
      WeeklySchedule();
      ~WeeklySchedule();

      QString getAttachName() const;
      QString getSubjName() const;
      QString getXlsTemplatePath() const;
      QString getCsvTemplatePath() const;
      QString getEmailTemplatePath() const;
      QMap<QString, QString> & getEmailAdresses();
      QTime getTime() const;
      qint32 getDay() const;
      bool getActive() const;
      bool getCsvAttach() const;
      bool getXlsAttach() const;
      QStringList prepareSerialization();
      void deserializeList(const QStringList & list);

      void setAttachName(const QString & AttachName);
      void setSubjName(const QString & SubjName);
      void setXlsTemplatePath(const QString & xlsTemplatePath);
      void setCsvTemplatePath(const QString & csvTemplatePath);
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
      QMap<QString, QString> m_emailAdresses;
};

#endif // WEEKLYSCHEDULE_H
