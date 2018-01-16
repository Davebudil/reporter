#ifndef SHIFTSCHEDULE_H
#define SHIFTSCHEDULE_H
#include <QMap>
#include <QObject>
#include <QTime>
#include <QDebug>

class ShiftSchedule{
   public:
      ShiftSchedule();
      ~ShiftSchedule();
      QString getAttachName() const;
      QString getCsvTemplatePath() const;
      QString getXlsTemplatePath() const;
      QString getSubjName() const;
      QString getEmailTemplatePath() const;
      QMap<QString, QString> & getEmailAdresses();
      QTime getFrom0() const;
      QTime getTo0() const;
      QTime getFrom1() const;
      QTime getTo1() const;
      bool getActive() const;
      bool * getDays() const;
      bool getCsvAttach() const;
      bool getXlsAttach() const;
      QStringList prepareSerialization();
      void deserializeList(const QStringList & list);

      void setAttachName(const QString & AttachName);
      void setActive(bool Active);
      void setXlsTemplatePath(const QString & xlsTemplatePath);
      void setCsvTemplatePath(const QString & csvTemplatePath);
      void setDays(const qint32 day, bool value);
      void setXlsAttach(bool xlsAttach);
      void setCsvAttach(bool csvAttach);
      void setSubjName(const QString & SubjName);
      void setFrom0(const QTime & From0);
      void setTo0(const QTime & To0);
      void setFrom1(const QTime & From1);
      void setTo1(const QTime & To1);
      void setEmailTemplatePath(const QString & emailTemplatePath);

   private:
      bool m_Active;
      bool * m_Days;
      bool m_csvAttach;
      bool m_xlsAttach;
      QTime m_From0;
      QTime m_To0;
      QTime m_From1;
      QTime m_To1;
      QString m_AttachName;
      QString m_SubjName;
      QString m_emailTemplatePath;
      QString m_xlsTemplatePath;
      QString m_csvTemplatePath;
      QMap<QString, QString> m_emailAdresses;
};

#endif // SHIFTSCHEDULE_H
