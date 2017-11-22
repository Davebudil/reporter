#include "reporter.h"
#include <QApplication>
#include <QDebug>



int main(int argc, char *argv[]){
   QApplication reporterApp(argc, argv);
   Reporter App;
   App.show();
   App.defaultSettings();
   return reporterApp.exec();
}
