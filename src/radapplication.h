#ifndef _RadApplication_H
#define _RadApplication_H

#include <QApplication>
#include "src_config.h"

class RadiusSettings;

class _FFTMATR_EXPORT_ RadApplication : public QApplication
{
public:
    RadApplication (int & argc, char ** argv);
    virtual ~RadApplication (void);

    static RadApplication * instance (void) { return selfCore; }

    virtual RadiusSettings * getRadSettings (void);
private:
    //
    // Variables
    //
    static RadApplication * selfCore;
    RadiusSettings * rSettings;
private:
    Q_OBJECT
};

#endif
