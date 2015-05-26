#ifndef _RHISTWIDGET_H
#define _RHISTWIDGET_H

#include <QWidget>
#include "src_config.h"

class QwtPlot;

class _FFTMATR_EXPORT_ HistWidget : public QWidget
{
public:
    HistWidget (const QImage& im=QImage(), QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~HistWidget (void);

protected:
    //
    // Overriedes
    //
    virtual void showEvent ( QShowEvent * event );
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
private:
    Q_OBJECT
};

#endif
