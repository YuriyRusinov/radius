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
#include "ConvDistColumnThread.h"

ConvDistThread :: ConvDistThread (ConvDistPhysParameters * cParams, QObject * parent)
    : QThread (parent),
    convParameters (cParams),
    stc2MatrAbs (0)
{
}

ConvDistThread :: ~ConvDistThread (void)
{
//    if (stc2MatrAbs)
//        delete [] stc2MatrAbs;
}

void ConvDistThread :: run (void)
{
    if (!convParameters)
        return;
    QTime * fftTime = new QTime;
    fftTime->start();
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

    FILE * fid6 = 0;
    if (!fileConvName.isEmpty())
        fid6 = fopen (fileConvName.toAscii().constData(), "w");

    int na = convParameters->getChannelsNumb ();
    qDebug () << __PRETTY_FUNCTION__ << (int)na;
    QAbstractItemModel * radModel = new QStandardItemModel (nd2, 1, 0);// (nd2, na);
    Q_UNUSED (radModel);

    int nr (0);
    Q_UNUSED (nr);
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
    double maxval = 0.0;
//    stc2MatrAbs = new double (na*nd);
    qDebug () << __PRETTY_FUNCTION__ << fftTime->elapsed ();
    int nThr = convParameters->getNumThreads();
    fftw_init_threads ();
    fftw_plan_with_nthreads (nThr);
    for (int i0=0; i0<na; i0++)
    {
//        qDebug () << __PRETTY_FUNCTION__ << QString("Read new data");
        QList<QThread *> thrList;
        for (int ii=0; ii<nThr; ii++)
        {
            ConvDistColumnThread * thrCol = new ConvDistColumnThread (convParameters, fid5, fid6, i0/nThr+ii, opor, nd, 0);
            connect (thrCol, SIGNAL (terminated()), this, SLOT (columnTerminated()) );
            connect (thrCol, SIGNAL (finished()), this, SLOT (columnFinished()) );
            thrList.append (thrCol);
//            thrCol->start();
//            thrCol->wait();
        }
        for (int ii=0; ii<nThr; ii++)
        {
            QThread * thr = thrList[ii];
            thr->start ();
        }
        for (int ii=0; ii<nThr; ii++)
        {
            QThread * thr = thrList[ii];
            thr->wait ();
        }
//        thrCol->wait();

/*        int cr = fread (st, sizeof (quint8), nd2, fid5);
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
        stc4 = fft (stc, nd, nd, FFTW_FORWARD, FFTW_ESTIMATE);
        //qDebug () << __PRETTY_FUNCTION__ << tr ("FFT of data, elapsed time %1").arg (fftTime->elapsed ()/1.0e3);
        for (int ii=0; ii<nd; ii++)
        {
            double re = real (stc4[ii])*nd;
            double im = imag (stc4[ii])*nd;
            stc4[ii] = complex<double> (re, im);
            if (i0==0)
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
        for (int ii=0; ii<nd; ii++)
        {
            int ind = ii;//(ii==0 ? nd-1 : ii-1);
            stc2[2*ii] = real (xfft[ind])/nd;//stc3[i]);
            stc2[2*ii+1] = imag (xfft[ind])/nd;
            double vmod = sqrt (stc2[2*ii]*stc2[2*ii] + stc2[2*ii+1]*stc2[2*ii+1]);
            maxval = qMax (maxval, vmod);
        }
        if (fid6)
        {
            //qDebug () << __PRETTY_FUNCTION__ << QString ("Write data");
            size_t h = fwrite (stc2, sizeof (double)/2, 2*nd, fid6);
            int ier = ferror (fid6);
            //qDebug () << __PRETTY_FUNCTION__ << QString ("Data were written %1 bytes, error indicator =%2 ").arg (h).arg(ier);
            if (h ==0 || ier)
            {
                qDebug () << __PRETTY_FUNCTION__ << tr ("Write error, code=%1").arg (ier);
                return;
            }
        }
        delete [] xfft;

        //delete [] stc2abs;
        delete [] stc2;
*/
    }
    delete cop;
    if (fid6)
        fclose (fid6);
    mFile.unlock();
    fid6 = fileConvName.isEmpty() ? 0 : fopen (fileConvName.toAscii().constData(), "r+");
    qDebug () << __PRETTY_FUNCTION__ << maxval;
    if (fid6)
    {
        double * vals = new double [nd];
        int * nums = new int [nd];
        for (int i=0; i<nd; i++)
        {
            vals[i] = 0.0;
            nums[i] = 0;
        }

        for (int i0 = 0; i0 < FFT_Transform::pow2roundup(na/50); i0++)
        {
            for (int i=0; i<50; i++)
            {
                double * stc2c = new double [2*nd];
                size_t h = fread (stc2c, sizeof (double)/2, 2*nd, fid6);
                //qDebug () << __PRETTY_FUNCTION__ << h;
                if (h > 0)
                {
                    for (int ii=0; ii<nd; ii++)
                    {
                        vals [ii] += sqrt (stc2c[2*ii]*stc2c[2*ii] + stc2c[2*ii+1]*stc2c[2*ii+1]);
                        nums[ii]++;
                    }
                }
                delete [] stc2c;
            }
            for (int ii=0; ii<nd; ii++)
            {
                double vmod = vals[ii]/nums[ii];//sqrt (stc2c[2*ii]*stc2c[2*ii] + stc2c[2*ii+1]*stc2c[2*ii+1]);
                
                double gray = (vmod/maxval)*convParameters->getNCalibration();
                //vals [(i0+1)%50+ii] = gray;
                //if ((i0+1)/50*50 == i0+1)
                //{
                //    double gray_ave (0.0);
                //    for (int iii=0; iii<50; iii++)
                //        gray_ave += vals [iii];
                uint val = (uint)(256*(gray));

                QRgb v = qRgb (val, val, val);
                convImage->setPixel (ii, i0, v);//qRgb(val, val, val));
                //}
            }
        }
        delete [] vals;
    }
    fftw_cleanup_threads ();
    //delete [] vals;
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
    int msecs = fftTime->elapsed ();
    emit sendTime (msecs);
    delete fftTime;

    delete [] stc1;
    delete [] st;
//    exec();
}

void ConvDistThread :: columnTerminated (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QThread * thr = qobject_cast<QThread *>(this->sender());
    thr->deleteLater();
}

void ConvDistThread :: columnFinished (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QThread * thr = qobject_cast<QThread *>(this->sender());
    thr->deleteLater();
}
