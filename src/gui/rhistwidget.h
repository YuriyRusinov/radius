#ifndef _RHISTWIDGET_H
#define _RHISTWIDGET_H

#include <QWidget>
#include "src_config.h"

class QPushButton;
class QToolButton;
class QImage;
class QDoubleSpinBox;
class QSpinBox;

class QwtPlot;
class QwtPlotMarker;

class _FFTMATR_EXPORT_ HistWidget : public QWidget
{
public:
    HistWidget (const QImage& im=QImage(), QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~HistWidget (void);

    void setImage (const QImage& wIm);
    void setHistData (const double * const rHist, const double * const gHist, const double * const bHist, int nColors);

private slots:
    void imageHistEq (void);
    void minNoiseValueChanged (double);
    void maxNoiseValueChanged (double);
    void redColorValueChanged (int);
    void blueColorValueChanged (int);

signals:
    void histEq (const QImage& wImage, double wNoiseMin, double wNoiseMax, int cRed, int cBlue);

private:
    //
    // Functions
    //
    void init (void);
private:
    //
    // Variables
    //
    QImage wImage;
    QwtPlot *m_plot;
    QwtPlotMarker * m_plotMarkerNoiseMin;
    QwtPlotMarker * m_plotMarkerNoiseMax;
    QwtPlotMarker * m_plotMarkerColorRed;
    QwtPlotMarker * m_plotMarkerColorBlue;
    QDoubleSpinBox * wNoiseMinSp;
    QDoubleSpinBox * wNoiseMaxSp;
    QSpinBox * redColorSp;
    QSpinBox * blueColorSp;
    QToolButton * tbFiltering;
    QPushButton * pbEqualize;
    double yMax;
private:
    Q_OBJECT
};

#endif
