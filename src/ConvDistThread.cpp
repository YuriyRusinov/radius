#include <QColor>
#include <QTime>
#include <QMessageBox>
#include <QFile>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QImage>
#include <QtDebug>

#include <complex>
#include <math.h>
#include <stdio.h>
#include <fftw3.h>

#include "matrix.h"
#include "fft_c.h"
#include "calcim.h"
#include "ConvDistPhys.h"
#include "ConvDistThread.h"

ConvDistThread :: ConvDistThread (ConvDistPhysParameters * cParams, QObject * parent)
    : QThread (parent),
    convParameters (cParams),
    stc2MatrAbs (0)
{
}

ConvDistThread :: ~ConvDistThread (void)
{
    if (stc2MatrAbs)
        delete [] stc2MatrAbs;
}

void ConvDistThread :: run (void)
{
    if (!convParameters)
        return;
    mFile.lock ();
    int ndn = convParameters->getNRChannels();
    int nd = FFT_Transform::pow2roundup (ndn);
    int nd2 = 2*ndn;
    CalcOpor1 * cop = new CalcOpor1 (nd);
    complex<double> * opor = cop->calc();//new complex<double> [nd];
    int N1 = convParameters->getImpNumb();
    qDebug () << __PRETTY_FUNCTION__ << N1;
    QFile fContData (QString ("stc4.dat"));
    fContData.open (QIODevice::WriteOnly);
    QTextStream stCont (&fContData);
    FFT_Transform fft;// = new FFT_Transform;
    emit sendData (opor, N1);
    //radDataWidget * wOpFFT = new radDataWidget (opor, N1);
    //emit sendWidget (wOpFFT, this);
/*    QMdiSubWindow * subWopFFT = m_mdiArea->addSubWindow (wOpFFT);
    wOpFFT->show ();
    subWopFFT->setAttribute (Qt::WA_DeleteOnClose);
*/
    QFile contOp ("opor_after_FFT.dat");
    contOp.open (QIODevice::WriteOnly);
    QTextStream stOp (&contOp);
    for (int i=0; i<N1; i++)
    {
        double re = real (opor[i])*nd;
        double im = imag (opor[i])*nd;
        stOp << re << " " << im << "i" << endl;
    }
    contOp.close ();

    QString fileName = convParameters->getInputFileName();
    QFile fData (fileName);
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
    int a = 0;
    Q_UNUSED (a);

    FILE * fid5 = fopen (fileName.toAscii().constData(), "rb");
    qDebug () << __PRETTY_FUNCTION__ << fileName.toAscii().constData() << fid5;
    if (!fData.open (fid5, QIODevice::ReadOnly | QIODevice::Unbuffered))
    {
        mFile.unlock();
        return;
    }

    QString fileConvName = convParameters->getConvFileName();//QFileDialog::getSaveFileName (this, tr("Save 1st data"), QDir::currentPath(), tr("All files (*)"));

    FILE * fid6 = fileConvName.isEmpty() ? 0 : fopen (fileConvName.toAscii().constData(), "w+");

    int na = convParameters->getChannelsNumb ();
    qDebug () << __PRETTY_FUNCTION__ << (int)na;
    QAbstractItemModel * radModel = new QStandardItemModel (nd2, 1, 0);// (nd2, na);

    int nr (0);
    QFile fContStData (QString ("stc.dat"));
    fContStData.open (QIODevice::WriteOnly);
    QTextStream stContSt (&fContStData);
    QSize imSize (nd, FFT_Transform::pow2roundup(na/50));
    QImage * convImage = new QImage (imSize, QImage::Format_RGB32);//QImage::Format_Mono);
    if (!convImage || convImage->size().isNull())
    {
        if (convImage)
            delete convImage;
        mFile.unlock();
        return;
    }
    convImage->fill (0);
    double * vals = new double [50*nd];
    double maxval = 0.0;
    stc2MatrAbs = new double (na*nd);
    for (int i0=0; i0<na; i0++)
    {
        qDebug () << __PRETTY_FUNCTION__ << QString("Read new data");
        int cr = fread (st, sizeof (quint8), nd2, fid5);
        if (cr <= 0)
            return;
        for (int ii=0; ii< nd2; ii++)
        {
            if (i0<1)
            {
                QModelIndex wcIndex = radModel->index (nr, 0);
                radModel->setData (wcIndex, QString::number (st[ii]), Qt::DisplayRole);
                nr++;
            }
        }

        for (int ii=0; ii<128; ii++)
            st[ii] = 0.0;

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
        qDebug () << __PRETTY_FUNCTION__ << QString ("Forward fft");
        stc4 = fft (stc, nd, nd, FFTW_FORWARD, FFTW_ESTIMATE);
        qDebug () << __PRETTY_FUNCTION__ << i0 << na;
        for (int ii=0; ii<nd; ii++)
        {
            double re = real (stc4[ii])*nd;
            double im = imag (stc4[ii])*nd;
            stc4[ii] = complex<double> (re, im);
            if (i0==0)
                stCont << re << (im >= 0 ? "+" : " ") << im << "i" << endl;
        }
        int n2 = FFT_Transform::pow2roundup(nd);//1024;
        qDebug () << __PRETTY_FUNCTION__ << n2;//QString ("Reverse fft");
        //complex<double> * xConv = new complex<double>[n2];
        for (int ii=0; ii<nd; ii++)
        {
            double rstc4 = real (stc4[ii])/nd;
            double imstc4 = imag (stc4[ii])/nd;
            double ropor = real (opor[ii]);
            double imopor = imag (opor[ii]);
            complex<double> res = complex<double> ((rstc4 * ropor - imstc4*imopor), (rstc4*imopor+imstc4*ropor));
            stc1[ii] = res;//stc4[i]*opor[i];///(nd*nd);
            //qDebug () << __PRETTY_FUNCTION__ << ii << nd;
            //if (i<10)
            //    qDebug () << __PRETTY_FUNCTION__ << real (res) << imag(res) << real (xConv[i]) << imag (xConv[i]);
        }
        delete [] stc4;

        complex<double> * xfft = 0;//new complex<double> [nd];
        qDebug () << __PRETTY_FUNCTION__ << QString ("Reverse fft");
        xfft = fft (stc1, nd, nd, FFTW_BACKWARD, FFTW_ESTIMATE );//| FFTW_MEASURE);
        //delete [] xConv;
        double * stc2 = new double [2*nd];
        double * stc2abs = new double [nd];
        for (int ii=0; ii<nd; ii++)
        {
            int ind = ii;//(ii==0 ? nd-1 : ii-1);
            //complex<double> zfft = complex<double>(real (xfft[ind])/nd, imag (xfft[ind])/nd);
            //QMutex matrMutex;
            //matrMutex.lock();
            //stc2MatrAbs[i0*na+ii] = sqrt (real (zfft*conj (zfft)));
            //matrMutex.unlock();
            //qDebug () << __PRETTY_FUNCTION__ << ii << nd;
            stc2[2*ii] = real (xfft[ind])/nd;//stc3[i]);
            stc2[2*ii+1] = imag (xfft[ind])/nd;
//            stc2Op << stc2[2*i] << " " << stc2[2*i+1] << endl;
            stc2abs[ii] = sqrt (stc2[2*ii]*stc2[2*ii] + stc2[2*ii+1]*stc2[2*ii+1]);
            if (i0==0)
                maxval = qMax (maxval, stc2abs[ii]);
            double gray (i0==0 ? 0.0 : (stc2abs[ii]/maxval)*convParameters->getNCalibration());
            vals [(i0+1)%50+ii] = gray;
            if ((i0+1)/50*50 == i0+1)
            {
                double gray_ave (0.0);
                for (int iii=0; iii<50; iii++)
                    gray_ave += vals [iii];
                uint val = (uint)(256*(gray_ave/5.0e1));

                QRgb v = qRgb (val, val, val);
                ////qDebug ()  << __PRETTY_FUNCTION__ << v;
                convImage->setPixel (ii, i0/50, v);//qRgb(val, val, val));
            }

            //qDebug () << __PRETTY_FUNCTION__ << ii << (double)stc2abs[ii];

        }
        if (fid6)
        {
            qDebug () << __PRETTY_FUNCTION__ << QString ("Write data");
            size_t h = fwrite (stc2, sizeof (double)/2, 2*nd, fid6);
            int ier = ferror (fid6);
            qDebug () << __PRETTY_FUNCTION__ << QString ("Data were written %1 bytes, error indicator =%2 ").arg (h).arg(ier);
            if (ier)
                qDebug () << __PRETTY_FUNCTION__ << tr ("Write error");
        }
        delete [] xfft;

        delete [] stc2abs;
        delete [] stc2;
    }
    delete [] vals;
    qDebug () << __PRETTY_FUNCTION__ << QString ("Data were read and processed");
    fContData.close();
//    double * stc2abs = new double [nd];
    qDebug () << __PRETTY_FUNCTION__ << convImage->size () << imSize;
    QString fileImageName = QString ("rgg.png");
    convImage->save (fileImageName, "PNG");
    //rggImageWidget * imW = new rggImageWidget;
    //imW->setImage (*convImage);
    //emit sendWidget (imW, this);
    emit sendImage (convImage);

    delete [] stc1;
    delete [] st;
    delete cop;
    if (fid6)
        fclose (fid6);
    mFile.unlock();
//    exec();
}