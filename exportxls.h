#ifndef EXPORTXLS_H
#define EXPORTXLS_H
#include <QObject>
#include <QPair>
#include <QTimer>
#include <QDataStream>
#include <QTextStream>
#include <QProcess>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QThread>
#include <QDir>

class ExportXLS : public QObject{
      Q_OBJECT
   public:
      explicit ExportXLS(QObject *parent = 0);
      ~ExportXLS() override;

      bool generateFile(const QString & templatePath,
                        const QString & genPath,
                        const QList<std::pair<QString, QString>> constants,
                        const QList<QStringList> queries);
      void readResult();
      bool editIni();
      QProcess * generateXLS;
   signals:

   public slots:
      void sdLaunchError(QProcess::ProcessError error);
      void processFinished();
};

#endif // EXPORTXLS_H
