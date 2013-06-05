#include <QValidator>
#include <QDoubleValidator>

#include "ffttimewidget.h"
#include "ui_fft_time_widget.h"

FFTTimeWidget :: FFTTimeWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    UI (new Ui::fft_time_widget)
{
    UI->setupUi (this);
    QValidator * val = new QDoubleValidator (0, 10000000, 8, this);
    UI->lEElapsed->setValidator (val);
}

FFTTimeWidget :: ~FFTTimeWidget (void)
{
    delete UI;
}

void FFTTimeWidget :: setTimeElapsed (int mseconds)
{
    double secEl = mseconds/0.1e4;
    UI->lEElapsed->setText (QString::number (secEl, 'f', 8));
}

