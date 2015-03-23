#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QtDebug>

#include "radmainwindow.h"
#include "radapplication.h"
#include "RadSettings.h"

int main (int argc, char * argv[])
{
    QApplication * app = new RadApplication (argc, argv);

    QString transl_path = QDir::currentPath();//+QString("/transl");//getWDir();
    //qDebug () << __PRETTY_FUNCTION__ << transl_path;
    QTranslator * radiusTr = new QTranslator (0);
    RadiusSettings * rSettings = qobject_cast<RadApplication *> (app) ? qobject_cast<RadApplication *> (app)->getRadSettings () : 0;
    if (!rSettings)
        return 0;
    QString rAppNameEng = QApplication::applicationName ();
    rSettings->beginGroup (QString("System settings/") + rAppNameEng);

    if ( rSettings->getParam("Translators_path").isEmpty() )
    {
        rSettings->endGroup();
        rSettings->writeSettings (QString("System settings/") + rAppNameEng, 
                                     "Translators_path", 
                                     transl_path);
        rSettings->beginGroup (QString("System settings/") + rAppNameEng);
    }
    
    transl_path = rSettings->getParam("Translators_path");

    rSettings->endGroup();
    bool istr = radiusTr->load("radius_ru", transl_path);//.load (transl_path + QString ("reper_app_ru.qm"), ".");
    if (istr)
        app->installTranslator (radiusTr);
  
    RadMainWindow * rw = new RadMainWindow;

    rw->show ();

    return app->exec ();
}
