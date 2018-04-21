#ifndef SHIFTSCHEDULE_H
#define SHIFTSCHEDULE_H
#include <QMap>
#include <QObject>
#include <QTime>
#include <QDebug>
#include <QDateTime>
#include <sqlparameter.h>

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
      QStringList emailAdresses();
      QTime getFrom0() const;
      QTime getTo0() const;
      QTime getFrom1() const;
      QDateTime getDate0() const;
      QDateTime getDate1() const;
      QDateTime getDate2() const;
      bool getActive() const;
      QVector<bool> getDays() const;
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
      QTime getTimeTMP() const;
      void setTimeTMP(const QTime & timeTMP);
      void setDate2(const QDateTime & Date2);
      void setDate1(const QDateTime & Date1);
      void setDate0(const QDateTime & Date0);
      void fixParameters(SQLParameter & param, QDateTime & currentTime);

      void setEmailTemplatePath(const QString & emailTemplatePath);
      bool generateShiftData(const QDateTime & currentTime);

      bool getDone0() const;
      void setDone0(bool Done0);

      bool getDone1() const;
      void setDone1(bool Done1);

      bool getDone2() const;
      void setDone2(bool Done2);


      QDateTime getDate3() const;
      void setDate3(const QDateTime & Date3);

      QDateTime getDate4() const;
      void setDate4(const QDateTime & Date4);

   private:
      bool m_checkDoneInterval(const QDateTime & currentTime);
      void m_timeInterval0();
      void m_timeInterval1();
      void m_timeInterval2();
      bool m_Active;
      QVector<bool> m_Days;
      bool m_csvAttach;
      bool m_xlsAttach;
      bool m_Done0;
      bool m_Done1;
      bool m_Done2;
      QTime m_time0;
      QTime m_time1;
      QTime m_time2;
      QTime m_timeTMP;
      QDateTime m_Date0;
      QDateTime m_Date1;
      QDateTime m_Date2;
      QDateTime m_Date3;
      QDateTime m_Date4;
      QString m_AttachName;
      QString m_SubjName;
      QString m_emailTemplatePath;
      QString m_xlsTemplatePath;
      QString m_csvTemplatePath;
      QMap<QString, QString> m_emailAdresses;
};

#endif // SHIFTSCHEDULE_H
