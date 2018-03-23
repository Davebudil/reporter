#ifndef CUSTOMSCHEDULING_H
#define CUSTOMSCHEDULING_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
   class CustomScheduling;
}

class CustomScheduling : public QDialog
{
      Q_OBJECT

   public:
      explicit CustomScheduling(QWidget *parent = 0);
      ~CustomScheduling();

   private:
      Ui::CustomScheduling *ui;
      QDateTime m_From;
      QDateTime m_To;
      bool m_Shift;
      bool m_Daily;
      bool m_Weekly;
      bool m_Monthly;
      bool m_shiftDays[7];
      bool m_dailyDays[7];
      quint32 m_weeklyDay;
      quint32 m_monthlyDay;
      QString m_attachName;
      QString m_subjectName;
      QString m_emailTemplatePath;
      QString m_xlsTemplatePath;
      QStringList m_Emails;
};

#endif // CUSTOMSCHEDULING_H
