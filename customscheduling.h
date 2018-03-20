#ifndef CUSTOMSCHEDULING_H
#define CUSTOMSCHEDULING_H

#include <QDialog>

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
};

#endif // CUSTOMSCHEDULING_H
