#ifndef _RHISTWIDGET_H
#define _RHISTWIDGET_H

#include <QWidget>
#include <qwt_plot.h>
#include "src_config.h"

class QwtPlot;

class _FFTMATR_EXPORT_ HistWidget : public QWidget
{
public:
    HistWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~HistWidget (void);

protected:
    //
    // Overriedes
    //
    virtual void showEvent ( QShowEvent * event );
private:
    //
    // Variables
    //
    QwtPlot plot;
private:
    Q_OBJECT
};

#endif
