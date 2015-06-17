#ifndef _RadImageEq_H
#define _RadImageEq_H

#include <QObject>
#include <QPixmap>

#include "src_config.h"
#include "radarConvIm.h"

class XFormWidget;
class HistWidget;

class _FFTMATR_EXPORT_ RadiusImageEqualizer : public QObject
{
public:
    XFormWidget * viewGolographicImage (QString fileName, QWidget *parent=0);

private slots:
    void viewHistogram (QPixmap pMap);

signals:
    void histView (HistWidget *);

private:
    //
    // Functions
    //
    friend class RadarImageProc;

    RadiusImageEqualizer (QObject * parent=0);
    virtual ~RadiusImageEqualizer (void);
    RadiusImageEqualizer& operator= (const RadiusImageEqualizer&);
private:
    Q_OBJECT
};

#endif
