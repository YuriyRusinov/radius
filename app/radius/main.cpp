#include <QApplication>

#include "radmainwindow.h"

int main (int argc, char * argv[])
{
    QApplication app (argc, argv);

    RadMainWindow * rw = new RadMainWindow;

    rw->show ();

    return app.exec ();
}
