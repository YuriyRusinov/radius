#ifndef _MDI_AREA_H
#define _MDI_AREA_H

#include <QMdiArea>
#include "src_config.h"

class QResizeEvent;
class QPaintEvent;

class _FFTMATR_EXPORT_ RadMdiArea : public QMdiArea
{
public:
    RadMdiArea (const QImage& im, QString title, QWidget * parent = 0);
    ~RadMdiArea (void);

protected:
    //
    // Overrides
    //
    virtual void resizeEvent (QResizeEvent * resizeEvent);
    virtual void paintEvent (QPaintEvent * pEvent);

private:
    //
    // Variables
    //
    QImage bkImage;
    QString rTitle;

private:
    Q_OBJECT
};
#endif
