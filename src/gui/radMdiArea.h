#ifndef _MDI_AREA_H
#define _MDI_AREA_H

#include <QMdiArea>
#include "src_config.h"

class QResizeEvent;

class _FFTMATR_EXPORT_ RadMdiArea : public QMdiArea
{
public:
    RadMdiArea (const QImage& im, QWidget * parent = 0);
    ~RadMdiArea (void);

protected:
    //
    // Overrides
    //
    virtual void resizeEvent (QResizeEvent * resizeEvent);

private:
    //
    // Variables
    //
    QImage bkImage;

private:
    Q_OBJECT
};
#endif
