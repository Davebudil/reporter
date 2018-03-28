#ifndef TABLEINFO_H
#define TABLEINFO_H

#include <QDialog>
#include <QObject>
#include <QDebug>

namespace Ui {
   class TableInfo;
} // namespace Ui

class TableInfo : public QDialog{
      Q_OBJECT

   public:
      explicit TableInfo(QWidget *parent = nullptr);
      ~TableInfo() override;
      void getInfo(const QVector<QStringList> & dbInfo);

   private:
      Ui::TableInfo *ui;
};

#endif // TABLEINFO_H
