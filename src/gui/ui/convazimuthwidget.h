#ifndef _ConvAzimuthWidget_H
#define _ConvAzimuthWidget_H

#include <QWidget>

#include "src_config.h"

namespace Ui
{
    class conv_azimuth_widget;
};

class ConvDistPhysParameters;
class ConvAzimuthPhysParameters;

class QCloseEvent;

class _FFTMATR_EXPORT_ ConvAzimuthWidget : public QWidget
{
public:
    ConvAzimuthWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~ConvAzimuthWidget (void);

private slots:
    void loadDataFile (void);
    void setSaveFile (void);
    void startConv (void);

    void calcFQuant (const QString& text);
    void calcNumbImp (const QString& text);

protected:
     void closeEvent(QCloseEvent *event);

signals:
    void sendParameters (ConvAzimuthPhysParameters * cParams);

private:
    //
    // Functions
    //
    void init (void);

private:
    //
    // Variables
    //
    Ui::conv_azimuth_widget * UI;
private:
    Q_OBJECT
};

#endif
