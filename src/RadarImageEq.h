#ifndef _RadImageEq_H
#define _RadImageEq_H

#include <QObject>
#include <QPixmap>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "src_config.h"
#include "radarConvIm.h"

class QImage;

//class XFormWidget;
class HistWidget;

using cv::Mat;

class _FFTMATR_EXPORT_ RadiusImageEqualizer : public QObject
{
public:
    QWidget * viewGolographicImage (QString fileName, QWidget *parent=0);

    //
    // Функция рассчитывает гистограмму входного изображения wImage
    // rHist, gHist, bHist -- выходные массивы для цветов red, green, blue размерности nColor соответственно
    // nColor -- число цветов для гистограмм
    //
    void calcHistogram (const QImage& wImage, double * rHist, double * gHist, double * bHist, const int& nColor, const cv::Mat& wMatr);

private slots:
    void viewHistogram (QPixmap pMap, const cv::Mat& wMatr);
    void histogramEq (const QImage& wImage, double wNoiseMin, double wNoiseMax, int vRed, int vBlue);

signals:
    void histView (QWidget *);
    void viewEqImage (QImage);

private:
    //
    // Functions
    //
    friend class RadarImageProc;

    RadiusImageEqualizer (QObject * parent=0);
    virtual ~RadiusImageEqualizer (void);
    RadiusImageEqualizer& operator= (const RadiusImageEqualizer&);

    unsigned int * applyHistogram (const QImage &img, unsigned int *histogram);

private:
    Q_OBJECT
};

#endif
