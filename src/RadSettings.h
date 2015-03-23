#ifndef _RadSettings_H
#define _RadSettings_h

#include <QSettings>

#include "src_config.h"

class _FFTMATR_EXPORT_ RadiusSettings : public QSettings
{
public:
    QString getParam ( const QString& keyname ) const;
    QString getGroupParam ( const QString& group, const QString& keyname );

public slots:
    void editSettings ( QWidget * parent = NULL );
    void writeSettings ( const QString & group, const QString & key, const QString & val );
    void readSettings ( void );

private:
    //
    // Functions
    //
    friend class RadApplication;
    RadiusSettings (const QString & organization, const QString & application, QObject * parent = NULL);
private:
    Q_OBJECT
    
};

#endif
