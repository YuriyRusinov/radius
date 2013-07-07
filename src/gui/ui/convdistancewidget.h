#ifndef _ConvDistanceWidget_H
#define _ConvDistanceWidget_H

#include <QWidget>

#include "src_config.h"

namespace Ui
{
    class conv_distance_widget;
};

class ConvDistPhysParameters;

class QCloseEvent;

class _FFTMATR_EXPORT_ ConvDistanceWidget : public QWidget
{
public:
    ConvDistanceWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~ConvDistanceWidget (void);

private slots:
    void loadDataFile (void);
    void setSaveFile (void);
    void startConv (void);

    void calcFQuant (const QString& text);
    void calcNumbImp (const QString& text);

protected:
     void closeEvent(QCloseEvent *event);

signals:
     void setParams (ConvDistPhysParameters * CParams);

private:
    //
    // Functions
    //
    void init (void);

private:
    //
    // Variables
    //
    Ui::conv_distance_widget * UI;

private:
    Q_OBJECT
};

#endif
