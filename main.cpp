#include "reporter.h"
#include <QApplication>
#include <QDebug>



qint32 main(qint32 argc, char *argv[]){
   QApplication reporterApp(argc, argv);
   Reporter App;
   App.show();
   App.defaultSettings();
   return reporterApp.exec();
}
