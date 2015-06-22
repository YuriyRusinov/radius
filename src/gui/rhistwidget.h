#ifndef _RHISTWIDGET_H
#define _RHISTWIDGET_H

#include <QWidget>
#include "src_config.h"

class QPushButton;
class QwtPlot;
class QImage;

class _FFTMATR_EXPORT_ HistWidget : public QWidget
{
public:
    HistWidget (const QImage& im=QImage(), QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~HistWidget (void);

    void setImage (const QImage& wIm);
    void setHistData (const double * const rHist, const double * const gHist, const double * const bHist, int nColors);

private slots:
    void imageHistEq (void);

signals:
    void histEq (const QImage& wImage);

private:
    //
    // Variables
    //
    QImage wImage;
    QwtPlot *m_plot;
    QPushButton * pbEqualize;
private:
    Q_OBJECT
};

#endif
