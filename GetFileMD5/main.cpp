#include "operationdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OperationDialog w;
    w.show();
    return a.exec();
}
