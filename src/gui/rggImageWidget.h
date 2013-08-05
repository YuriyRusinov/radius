#ifndef _RGGImageWidget_H
#define _RGGImageWidget_H

#include "src_config.h"

#include <QWidget>
#include <QImage>

class QLabel;
class QScrollArea;

class _FFTMATR_EXPORT_ rggImageWidget : public QWidget
{
public:
    rggImageWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~rggImageWidget (void);

    void setImage (const QImage& image);

private:
    //
    // Variables
    //
    QImage im;
    QLabel * lImage;
    QScrollArea * lImScroll;

private:
    Q_OBJECT
};

#endif
