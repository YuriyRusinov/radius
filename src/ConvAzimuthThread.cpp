#include <QTime>
#include <QImage>
#include <QFile>
#include <QMessageBox>
#include <QtDebug>

#include "constants1.h"
#include "fft_c.h"
#include "ConvAzimuthPhys.h"
#include "ConvAzimuthThread.h"

ConvAzimuthThread :: ConvAzimuthThread (ConvAzimuthPhysParameters * cAParams, QObject * parent)
    : QThread (parent),
    convAzParameters (cAParams)
{
}

ConvAzimuthThread :: ~ConvAzimuthThread (void)
{
}

void ConvAzimuthThread :: run (void)
{
    if (!convAzParameters)
        return;

    QTime * fftTime = new QTime;
    fftTime->start();
    QString fileName = convAzParameters->getInputFileName();
    FILE * fid6 = fopen (fileName.toAscii().constData(), "rb");
    if (!fid6)
        return;
    int nd = FFT_Transform::pow2roundup (convAzParameters->getFFTDim());
    int na2 (nd);
    int na_ots = convAzParameters->getShift ();
    qDebug () << __PRETTY_FUNCTION__ << fid6 << na_ots << nd;
    int h1 = fseek (fid6, nd*8*na_ots, SEEK_SET);
    if (h1 < 0)
    {
        fclose (fid6);
        return;
    }
    int read (0);
    int ndDim = convAzParameters->getDimension();
    int ndcentre = convAzParameters->getCenter();
    int ndv = ndcentre-ndDim/2;
    int ndn = ndcentre+ndDim/2;
    int ndrz = ndn-ndv;
    CMatrix rgg1 (0.0, ndrz, na2);
//    complex<double>* rgg1Vec = new complex<double> [ndrz*na2];
    QFile rggMatrC("rgg1.dat");
    rggMatrC.open(QIODevice::WriteOnly);
    QTextStream rggStr(&rggMatrC);

    for (int i0=0; i0<na2; i0++)
    {
        double * stlb = new double [2*nd];
        double * stlb2 = new double [ndrz*2];
        int cr = fread (stlb, sizeof (double)/2, nd*2, fid6);
        if (cr == 0)
        {
//            QMessageBox::warning(this, tr("Read data"), tr("Cannot read convolution data"), QMessageBox::Ok);
            return;
        }
        for (int j=0; j<ndrz*2;j++)
            stlb2[j]=stlb[j+2*ndv];
        for (int j=0; j<ndrz;j++)
        {
            rgg1(j, i0) = complex<double>(stlb2[2*j], stlb2[2*j+1]);
        }
        //qDebug () << __PRETTY_FUNCTION__ << i0 << na2 << cr;
        // << ndv << stlb2[0] << stlb2[2*ndv] << sizeof (quint32);
        delete [] stlb2;
        delete [] stlb;
        read++;
    }
    for (int i=0; i<20; i++)
    {
        for (int j=0; j<20; j++)
            rggStr << real (rgg1(i, j)) << " " << imag (rgg1(i, j)) << "i ";
        rggStr << endl;
    }
    int nas = na2/2;
    qDebug () << __PRETTY_FUNCTION__ << QString ("Matrices were set") << ndrz << nas << na2;

//    CMatrix corf2 (complex<double>(0.0), ndrz, na2);
    CMatrix corf3 (complex<double>(0.0), ndrz, nas);
    QFile corfCont ("ctest.dat");
    corfCont.open (QIODevice::WriteOnly);
    QTextStream stCorf (&corfCont);
    int from_opor (0);
    //complex<double> * corfVec = new complex<double> [ndrz*na2];
    CMatrix corf (complex<double>(0.0), ndrz, na2);
    qDebug () << __PRETTY_FUNCTION__ << nas << na2;
    double dx = convAzParameters->getAzimuthStep();
    double R = convAzParameters->radius ();
    double H = convAzParameters->height ();
    double dnr = convAzParameters->getDistStep ();
    double lamb = convAzParameters->getLambda ();
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<nas; j++)
        {
            double x = (-nas/2+j)*dx;
            double rt = sqrt (R*R+x*x+H*H);
            double rt1 = rt - sqrt (R*R+H*H);
            int N0 = (int)(rt1/dnr);
            double phase = -4*pi*rt/lamb;
            corf3(N0, j) = complex<double>(cos(phase), sin(phase));
        }
        from_opor++;
    }
//    complex<double>* c = corf3.getData();
    complex<double> * corfVec = corf3.getData();
    Q_UNUSED (corfVec);
    for (int i=0; i<10/*ndrz*/; i++)
    {
        for (int j=0; j<nas; j++)
        {
            stCorf << qSetRealNumberPrecision(14) << real(corf3(i, j)) << " " << qSetRealNumberPrecision(14) << imag (corf3(i, j)) << "i ";
        }
        stCorf << endl;
        //qDebug () << __PRETTY_FUNCTION__ << i;
    }

    int cor_func (0);
    for (int i=0; i<nas/2; i++)
    {
        for (int j=0; j<ndrz; j++)
        {
            corf(j, i) = corf3(j, i+nas/2); //corfVec[j+(i+nas/2)*ndrz];//
            corf(j, i+na2-nas/2) = corf3(j, i); // corfVec[j + i*ndrz]; 
        }
        cor_func++;
    }

    stCorf << QString("Matrix for FFT") << endl;

    for (int i=0; i<2; i++)
    {
        for (int j=0; j<na2; j++)
        {
            double x = real (corf(i,j));
            double y = imag (corf(i,j));
            //if ((i==0 && fabs (x + 0.50376075247194) <= 0.1e-5))// || fabs (xd) > 0.1e-15 || fabs (yd) > 0.1e-15)
            //    qDebug() << __PRETTY_FUNCTION__ << "Debug indices" << i << j;
            stCorf << qSetRealNumberPrecision(16) << x << " " << qSetRealNumberPrecision(16) << y << " ";
        }
        stCorf << endl;
    }
    stCorf << endl;

    FFT2_Transform fft2;// = new FFT2_Transform;
    complex<double> * corfw = fft2(corf.getData(), ndrz, na2, FFTW_FORWARD, FFTW_ESTIMATE);
    stCorf << QString("Results of FFT") << endl;
    int numMatr (0);
    CMatrix corf2 (ndrz, na2);
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<na2; j++)
        {
            corf2(i, j) = corfw[i*na2+j];
            if (i<10)
                stCorf << /*qSetFieldWidth(19) <<*/ qSetRealNumberPrecision(16) << real (corfw[i*na2+j])*ndrz*na2 << " " 
                       << /*qSetFieldWidth(19) <<*/ qSetRealNumberPrecision(16) << imag (corfw[i*na2+j])*ndrz*na2 << "i ";
            numMatr++;
        }
        if (i<10)
            stCorf << endl;
    }
    delete [] corfw;
    complex<double> * rggD = fft2(rgg1.getData(), ndrz, na2, FFTW_FORWARD, FFTW_ESTIMATE);
    CMatrix rgg (ndrz, na2);
    int cor_volfr (0);
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<na2; j++)
        {
            rgg (i, j) = rggD[i*na2+j];
            rgg (i, j) *= conj (corf2 (i, j));
            //rggD[j+i*na2] *= conj (corfw[j+i*na2]);
            //rggD[j+i*na2] /= na2*ndrz;
        }
        cor_volfr++;
    }
    delete [] rggD;
    complex<double> * rggBD = fft2(rgg.getData(), ndrz, na2, FFTW_BACKWARD, FFTW_ESTIMATE);
    double maxVal = 0.0;
//    double minVal = 0.0;//sqrt (real(rggBD[0])*real(rggBD[0])+imag(rggBD[0])*imag(rggBD[0]));
    for (int i=0; i<ndrz*nas/2; i++)
    {
        maxVal = qMax (maxVal, sqrt (real(rggBD[i])*real(rggBD[i])+imag(rggBD[i])*imag(rggBD[i])));
    }
    stCorf << tr("Results ") << endl;
    for (int i=0; i<5; i++)
    {
        for (int j=0; j<na2; j++)
        {
            stCorf << qSetRealNumberPrecision(16) << real (rggBD[i*na2+j])*na2*ndrz << " "
                   << qSetRealNumberPrecision(16) << imag (rggBD[i*na2+j])*na2*ndrz << "i ";
        }
        stCorf << endl;
    }
    qDebug () << __PRETTY_FUNCTION__ << tr ("Image was calculated");
    int nCal = convAzParameters->getNCalibration();
    QImage * hIm = new QImage (ndrz, nas/2/nCal, QImage::Format_ARGB32);// QImage::Format_Indexed8);
    //double * imData = new double [ndrz*nas/2/nCal];
    int ii (0);
    quint32 maxvalim = 0;
    QVector<QRgb> colors;
    for (int i=0; i<ndrz*nas/2; i++)//=nCal)
    {
        QColor c;
        complex<double> arg (rggBD[i]);
        double argAbs = sqrt (real(rggBD[i])*real(rggBD[i])+imag(rggBD[i])*imag(rggBD[i])) / maxVal;
        c.setRgbF (argAbs, argAbs, argAbs);
        //qDebug () << __PRETTY_FUNCTION__ << c.rgb();
        colors.append (c.rgb());
    }
    hIm->setColorTable (colors);
    qDebug () << __PRETTY_FUNCTION__ << hIm->size();
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<nas/2/nCal;j++)
        {
            double sum = 0.0;
            for (int iii=0; iii<nCal; iii++)
            {
                complex<double> arg (rggBD[ii]);
                if (!convAzParameters->getLogarithm())
                    sum += convAzParameters->getImScale() * sqrt (real(arg)*real(arg)+imag(arg)*imag(arg))/maxVal+convAzParameters->getImOffset();
                else
                    sum += sqrt (real(arg)*real(arg)+imag(arg)*imag(arg));///maxVal;
                ii++;
            }
            //ii++;
            //imData[ii] = sum/nCal;//sqrt (real(rggBD[ii])*real(rggBD[ii])+imag(rggBD[ii])*imag(rggBD[ii]))/maxVal;
            uint val = convAzParameters->getLogarithm() ? (uint)(256*log(1.+sum/nCal)/log(1.+maxVal)) : (uint)(256*sum/nCal);///512/0.3);
            maxvalim = qMax (maxvalim, val);
            uint ind = colors[(ii-1)/nCal];//colors.indexOf (qRgb (val, val, val));
            hIm->setPixel (i, j, ind);//qGray (val, val, val));//qRgb(255, 255, 255));
        }
    }
    //delete [] imData;
    //bool isLoaded = hIm->loadFromData (imData, ndrz*nas/2);
    qDebug () << __PRETTY_FUNCTION__ << maxvalim << maxVal;//isLoaded;
    QString fileImage = convAzParameters->getConvFileName();
    FILE * fid7 = fopen (fileImage.toAscii().constData(), "wb");
    //hIm->save(fileImage, "PNG");
    emit sendImage (hIm);
    fwrite ("FLT=", sizeof (char), 4, fid7);
    fwrite (&ndrz, sizeof (int), 1, fid7);
    ii = 0;
    for (int i=0; i<ndrz*nas/2/nCal; i++)
    {
        float wSum (0.0);
        for (int iii=0; iii<nCal; iii++)
        {
            float w = sqrt (real(rggBD[ii])*real(rggBD[ii])+imag(rggBD[ii])*imag(rggBD[ii]))/maxVal;//*6.6e11;
            wSum += w;
            ii++;
        }
        wSum /= nCal;
        wSum *= 256;
        fwrite (&wSum, sizeof (float), 1, fid7);
    }
    delete [] rggBD;

    fclose (fid7);
//    QPixmap pIm = QPixmap::fromImage (*hIm);
//    QLabel * lIm = new QLabel ;
//    QWidget * wImage = new QWidget;
//    QHBoxLayout * hImLay = new QHBoxLayout (wImage);
//    lIm->setPixmap (pIm);
//    hImLay->addWidget (lIm);
//    QMdiSubWindow * subIm = m_mdiArea->addSubWindow (wImage);
//    wImage->show ();
//    subIm->setAttribute (Qt::WA_DeleteOnClose);

    fclose (fid6);
    int msecs = fftTime->elapsed ();
    emit sendTime (msecs);
    //FFTTimeWidget * fftWidget = new FFTTimeWidget;
    //fftWidget->setTimeElapsed (msecs);
    //QMdiSubWindow * subFFTW = m_mdiArea->addSubWindow (fftWidget);
    //fftWidget->show();
    //subFFTW->setAttribute (Qt::WA_DeleteOnClose);
    delete fftTime;
}
