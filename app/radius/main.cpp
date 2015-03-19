#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QtDebug>

#include "radmainwindow.h"

int main (int argc, char * argv[])
{
    QApplication app (argc, argv);
    QString transl_path = QDir::currentPath()+QString("/transl");//getWDir();
    qDebug () << __PRETTY_FUNCTION__ << transl_path;
    QTranslator * radiusTr = new QTranslator (0);
    bool istr = radiusTr->load("radius_ru", transl_path);//.load (transl_path + QString ("reper_app_ru.qm"), ".");
    if (istr)
        app.installTranslator (radiusTr);
  
    RadMainWindow * rw = new RadMainWindow;

    rw->show ();

    return app.exec ();
}
