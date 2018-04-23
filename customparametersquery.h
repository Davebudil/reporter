#ifndef CUSTOMPARAMETERSQUERY_H
#define CUSTOMPARAMETERSQUERY_H

#include <QDialog>
#include <QObject>
#include <QMap>
#include <QDateTime>

namespace Ui {
   class customParametersQuery;
}

class customParametersQuery : public QDialog{
      Q_OBJECT

   public:
      explicit customParametersQuery(QWidget *parent = 0,
                                     QMap<QString, QString> parameters = QMap<QString, QString>(),
                                     QDateTime from = QDateTime::currentDateTime(),
                                     QDateTime to = QDateTime::currentDateTime());
      ~customParametersQuery();
      QMap<QString, QString> m_Parameters;
      qint32 m_paramCount;
      QDateTime m_From;
      QDateTime m_To;

   private slots:
      void on_buttonBox_accepted();

   private:
      Ui::customParametersQuery *ui;
};

#endif // CUSTOMPARAMETERSQUERY_H
