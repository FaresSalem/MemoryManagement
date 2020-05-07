#include "memorymangement.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MemoryMangement w;
    w.show();
    return a.exec();
}
