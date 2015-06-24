#ifndef _RadImageEq_H
#define _RadImageEq_H

#include <QObject>
#include <QPixmap>

#include "src_config.h"
#include "radarConvIm.h"

class QImage;

class XFormWidget;
class HistWidget;

class _FFTMATR_EXPORT_ RadiusImageEqualizer : public QObject
{
public:
    XFormWidget * viewGolographicImage (QString fileName, QWidget *parent=0);

    //
    // ������� ������������ ����������� �������� ����������� wImage
    // rHist, gHist, bHist -- �������� ������� ��� ������ red, green, blue ����������� nColor ��������������
    // nColor -- ����� ������ ��� ����������
    //
    void calcHist (const QImage& wImage, double * rHist, double * gHist, double * bHist, const int& nColor);

private slots:
    void viewHistogram (QPixmap pMap);
    void histogramEq (const QImage& wImage);

signals:
    void histView (QWidget *);

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