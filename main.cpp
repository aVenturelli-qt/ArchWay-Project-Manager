#include "archway.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ArchWay w;
    w.show();
    return a.exec();
}
