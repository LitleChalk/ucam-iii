#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection port;
    Widget w(&port);
    w.setWindowTitle("Крышечка?");
    w.show();
    return QApplication::exec();
}
