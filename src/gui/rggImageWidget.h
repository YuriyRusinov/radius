#ifndef _RGGImageWidget_H
#define _RGGImageWidget_H

#include "src_config.h"

#include <QWidget>
#include <QImage>

class QLabel;
class QScrollArea;
class QMenu;
class QContextMenuEvent;

class _FFTMATR_EXPORT_ rggImageWidget : public QWidget
{
public:
    rggImageWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~rggImageWidget (void);

    void setImage (const QImage& image);

protected:
    //
    // Overrides functions
    //
    void contextMenuEvent ( QContextMenuEvent * event );

private slots:
    void saveImage (void);

private:
    //
    // Variables
    //
    QImage im;
    QLabel * lImage;
    QScrollArea * lImScroll;
    QMenu * pMenu;
    QAction * actSaveImage;

private:
    Q_OBJECT
};

#endif
