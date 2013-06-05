#ifndef _FFTTimeWidget_H
#define _FFTTimeWidget_H

#include <QWidget>

#include "src_config.h"

namespace Ui
{
    class fft_time_widget;
};

class _FFTMATR_EXPORT_ FFTTimeWidget : public QWidget
{
public:
    FFTTimeWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~FFTTimeWidget (void);

    void setTimeElapsed (int mseconds);

private:
    Ui::fft_time_widget * UI;
};

#endif
