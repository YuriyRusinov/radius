#include <QFile>
#include <QTime>
#include <QtDebug>

#include <complex>
#include <math.h>
#include <stdio.h>
#include <fftw3.h>

#include "matrix.h"
#include "fft_c.h"
#include "calcim.h"
#include "ConvDistPhys.h"
#include "ConvDistColumnThread.h"

ConvDistColumnThread :: ConvDistColumnThread (ConvDistPhysParameters * cParams, FILE * fidIn, FILE * fidOut, int iCol, QObject * parent)
    : QThread (parent),
    convParameters (cParams),
    fInput (fidIn),
    fOutput (fidOut),
    iColumn (iCol)
{
}

ConvDistColumnThread :: ~ConvDistColumnThread (void)
{
}

void ConvDistColumnThread :: run (void)
{
    QTime * fftTime = new QTime;
    if (!fInput || !fOutput || iColumn < 0)
        exit (-1);
    int ndn = convParameters->getNRChannels();
    int nd = FFT_Transform::pow2roundup (ndn);
    int nd2 = 2*ndn;
    CalcOpor1 * cop = new CalcOpor1 (nd);
    complex<double> * opor = cop->calc();// new complex<double> [nd];
    delete cop;
    int N1 = convParameters->getImpNumb();
    qDebug () << __PRETTY_FUNCTION__ << iColumn << N1 << opor << cop->data();
    if (iColumn >= 8400)
        qDebug () << __PRETTY_FUNCTION__;
//        for (int i=0; i<nd; i++)
//            qDebug () << __PRETTY_FUNCTION__ << real (opor[i]) << real (*(cop->data ()+i)) << imag (opor[i]) << imag (*(cop->data ()+i));
    
//    QFile fContData (QString ("stc4_%1.dat").arg (iColumn));
//    fContData.open (QIODevice::WriteOnly);
//    QTextStream stCont (&fContData);
    quint8 * st = new quint8 [nd2];
    for (int i=0; i<nd2; i++)
    {
        st [i] = 0;
    }
    complex<double> * stc = new complex<double>[nd];
    complex<double> * stc1 = new complex<double>[nd];
    for (int i=0; i<nd; i++)
    {
        stc[i] = complex<double> (0.0, 0.0);
        stc1[i] = complex<double> (0.0, 0.0);
    }
    int cr = fread (st, sizeof (quint8), nd2, fInput);
    if (cr <= 0)
        exit (-2);
    int i0 = iColumn;
    Q_UNUSED (i0);
    for (int ii=0; ii< nd2; ii++)
    {
/*        if (i0<1)
        {
            QModelIndex wcIndex = radModel->index (nr, 0);
            radModel->setData (wcIndex, QString::number (st[ii]), Qt::DisplayRole);
            nr++;
        }
*/
    }
#if 0

    for (int ii=0; ii<128; ii++)
        st[ii] = 0.0;

    QFile fContStData (QString ("stc_%1.dat").arg (iColumn));
    fContStData.open (QIODevice::WriteOnly);
    QTextStream stContSt (&fContStData);
    for (int ii=0; ii<ndn; ii++)
    {
        double re = st[2*ii+1];
        double im = st[2*ii];
        if (re > 128)
            re -= 256;
        if (im > 128)
            im -= 256;
        if (i0==0)
            stContSt << re << " " << im << endl;
        stc[ii] = complex<double> (re, im);//st[2*ii], st[2*ii+1]);
    }
    complex<double> * stc4 = 0;//new complex<double> [nd];
    FFT_Transform fft;// = new FFT_Transform;
    stc4 = fft (stc, nd, nd, FFTW_FORWARD, FFTW_ESTIMATE);
    delete [] stc;
    qDebug () << __PRETTY_FUNCTION__ << tr ("FFT of data, elapsed time %1").arg (fftTime->elapsed ()/1.0e3);
    for (int ii=0; ii<nd; ii++)
    {
        double re = real (stc4[ii])*nd;
        double im = imag (stc4[ii])*nd;
        stc4[ii] = complex<double> (re, im);
        stCont << re << (im >= 0 ? "+" : " ") << im << "i" << endl;
    }

    for (int ii=0; ii<nd; ii++)
    {
        double rstc4 = real (stc4[ii])/nd;
        double imstc4 = imag (stc4[ii])/nd;
        double ropor = real (opor[ii]);
        double imopor = imag (opor[ii]);
        complex<double> res = complex<double> ((rstc4 * ropor - imstc4*imopor), (rstc4*imopor+imstc4*ropor));
        stc1[ii] = res;//stc4[i]*opor[i];///(nd*nd);
    }
    delete [] stc4;

    complex<double> * xfft = 0;//new complex<double> [nd];
    //qDebug () << __PRETTY_FUNCTION__ << QString ("Reverse fft");
    xfft = fft (stc1, nd, nd, FFTW_BACKWARD, FFTW_ESTIMATE );//| FFTW_MEASURE);
    //delete [] xConv;
    double * stc2 = new double [2*nd];
    //double * stc2abs = new double [nd];
    double maxval = 0.0;
    for (int ii=0; ii<nd; ii++)
    {
        int ind = ii;//(ii==0 ? nd-1 : ii-1);
        stc2[2*ii] = real (xfft[ind])/nd;//stc3[i]);
        stc2[2*ii+1] = imag (xfft[ind])/nd;
        double vmod = sqrt (stc2[2*ii]*stc2[2*ii] + stc2[2*ii+1]*stc2[2*ii+1]);
        maxval = qMax (maxval, vmod);
    }
    if (fOutput)
    {
        //qDebug () << __PRETTY_FUNCTION__ << QString ("Write data");
        size_t h = fwrite (stc2, sizeof (double)/2, 2*nd, fOutput);
        int ier = ferror (fOutput);
        //qDebug () << __PRETTY_FUNCTION__ << QString ("Data were written %1 bytes, error indicator =%2 ").arg (h).arg(ier);
        if (h ==0 || ier)
        {
            qDebug () << __PRETTY_FUNCTION__ << tr ("Write error, code=%1").arg (ier);
            return;
        }
    }
    delete [] xfft;

    delete [] stc2;
#endif
    delete [] stc1;
    delete [] stc;
    delete [] st;
    delete opor;
    delete fftTime;
}
