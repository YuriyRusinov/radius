#include <QMessageBox>
#include <QtDebug>

#include "savesettingsform.h"
#include "RadSettings.h"

RadiusSettings :: RadiusSettings (const QString & organization, const QString & application, QObject * parent)
    : QSettings (organization, application, parent)
{
}

QString RadiusSettings :: getParam ( const QString& keyname ) const
{
    QString wstr = this->value (keyname).toString ();
    //qDebug () << "key = " << keyname << " value = " << wstr;
    return wstr;
}

QString RadiusSettings :: getGroupParam ( const QString& group, const QString& keyname )
{
    this->beginGroup (group);
    QString val = this->getParam (keyname);
    this->endGroup ();
    return val;
}

void RadiusSettings :: editSettings ( QWidget * parent)
{
    SaveSettingsForm * setForm = new SaveSettingsForm (this, parent);
    if ( setForm->exec() == QDialog::Accepted )
    {
        
        QMessageBox::information ( 0, tr("Save settings"),
                tr("You have to restart your application in order to changes take effect"),
                QMessageBox::Ok );
    }

    delete setForm;
}

void RadiusSettings :: writeSettings ( const QString & group, const QString & key, const QString & val )
{
    this->beginGroup (group);
    this->setValue (key, val);
    this->endGroup ();
}

void RadiusSettings :: readSettings ( void )
{
    QStringList groups = this->childGroups();
    for (int i=0; i<groups.count(); i++)
    {
        this->beginGroup ( groups[i] );
        if (this->childGroups ().count () > 0)
        {
            qDebug () << __PRETTY_FUNCTION__ << QString ("Sub group");
            readSettings ();
        }
        this->endGroup ();
        QStringList keys = this->childKeys ();
        for (int ii=0; ii<keys.count(); ii++)
            this->setValue ( keys[ii], this->value (keys[ii]));
    }
}
