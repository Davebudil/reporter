#ifndef CUSTOMSCHEDULING_H
#define CUSTOMSCHEDULING_H

#include <QDialog>
#include <QDateTime>
#include <QFileDialog>
#include <QWidget>

namespace Ui {
   class CustomScheduling;
}

class CustomScheduling : public QDialog
{
      Q_OBJECT

   public:
      explicit CustomScheduling(QWidget *parent = nullptr);
      ~CustomScheduling() override;
      QDateTime m_From;
      QDateTime m_To;
      QTime m_Shift1;
      QTime m_Shift2;
      QTime m_Shift3;
      QTime m_Daily1;
      QTime m_Weekly1;
      QTime m_Monthly1;
      bool m_Shift;
      bool m_Daily;
      bool m_Weekly;
      bool m_Monthly;
      bool m_shiftDays[7];
      bool m_dailyDays[7];
      bool m_useParameters;
      bool m_shiftXLS;
      bool m_shiftCSV;
      bool m_dailyXLS;
      bool m_dailyCSV;
      bool m_weeklyXLS;
      bool m_weeklyCSV;
      bool m_monthlyXLS;
      bool m_monthlyCSV;
      qint32 m_weeklyDay;
      qint32 m_monthlyDay;
      QString m_ShiftAttachName;
      QString m_DailyAttachName;
      QString m_WeeklyAttachName;
      QString m_MonthlyAttachName;
      QString m_ShiftSubjectName;
      QString m_DailySubjectName;
      QString m_WeeklySubjectName;
      QString m_MonthlySubjectName;
      QString m_emailTemplatePath;
      QString m_ShiftxlsTemplatePath;
      QString m_DailyxlsTemplatePath;
      QString m_WeeklyxlsTemplatePath;
      QString m_MonthlyxlsTemplatePath;
      QString m_Email;

   private slots:
      void on_toolButton_clicked();

      void on_buttonBox_accepted();

      void on_toolButton_3_clicked();

      void on_toolButton_4_clicked();

      void on_toolButton_5_clicked();

      void on_email_textEdited(const QString &arg1);

   private:
      bool m_validateEmail(const QString & email);
      Ui::CustomScheduling *ui;
      void m_loadShift();
      void m_loadDaily();
      void m_loadWeekly();
      void m_loadMonthly();
};

#endif // CUSTOMSCHEDULING_H
