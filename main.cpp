#include "reporter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Reporter w;
    w.show();

    return a.exec();
}
