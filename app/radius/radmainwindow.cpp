#include <QMenu>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QtDebug>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QIODevice>
#include <QDataStream>
#include <QTextStream>
#include <QKeySequence>
#include <QBuffer>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QPixmap>
#include <QLabel>
#include <QWidget>
#include <QVector>
#include <QHBoxLayout>
#include <QColor>

#include <radDataWidget.h>
#include <rggImageWidget.h>
#include <constants1.h>

#include <complex>
#include <math.h>
#include <stdio.h>
#include <fftw3.h>

#include "matrix.h"
#include "fft_c.h"
#include "calcim.h"

#include "radmainwindow.h"
#include "ui_radius_mainwindow.h"

using std::complex;

RadMainWindow :: RadMainWindow (QWidget * parent, Qt::WindowFlags flags)
    : QMainWindow (parent, flags),
    UI (new Ui::Rad_Main_Window),
    m_mdiArea (new QMdiArea (this)),
    actCalc1 (new QAction (tr("Calculate&1"), this)),
    actCalc2 (new QAction (tr("Calculate&2"), this))
//    stc (new complex<double> [nd]),
//    stc1 (new complex<double> [nd]),
//    stc2 (new double [2*nd]),
//    stc3 (0),
//    stc4 (0)
{
    UI->setupUi (this);

    setCentralWidget (m_mdiArea);
    init ();
}

RadMainWindow :: ~RadMainWindow (void)
{
/*    delete [] stc2;
    delete [] stc1;
    delete [] stc;
    if (stc3)
        delete [] stc3;
    if (stc4)
        delete [] stc4;
*/
}

void RadMainWindow :: openDataFile (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    fileName = QFileDialog::getOpenFileName (this, tr("Select source file"), QDir::currentPath(), tr("All files (*)"));
    if (fileName.isEmpty())
        return;

    actCalc1->setEnabled (true);
}

void RadMainWindow :: init (void)
{
    QMenu * openMenu = new QMenu (tr ("&File"), this);
    UI->menuFile->addMenu (openMenu);

    QAction * actOpenDataFile = openMenu->addAction (tr("Open &source file"));
    QKeySequence keyOpen (tr("Ctrl+O", "File|Open"));
    actOpenDataFile->setShortcut (keyOpen);
    connect (actOpenDataFile, SIGNAL (triggered()), this, SLOT (openDataFile()) );

    QAction * actOpenConvFile = openMenu->addAction (tr("Open &convolution file"));
    connect (actOpenConvFile, SIGNAL (triggered()), this, SLOT (openConvFile()) );

    UI->menuFile->addSeparator ();
    QAction * actFFTTest = openMenu->addAction (tr("Test &FFT"));
    connect (actFFTTest, SIGNAL (triggered()), this, SLOT (fftTest()) );

    UI->menuFile->addSeparator ();
    QAction * actQuit = UI->menuFile->addAction (tr("&Quit"));
    QKeySequence keyQuit (tr("Ctrl+Q", "File|Quit"));
    actQuit->setShortcut (keyQuit);
    connect (actQuit, SIGNAL (triggered()), this, SLOT (close()) );

    QMenu * calcMenu = new QMenu (tr ("&Calculate"), this);
    UI->menuBar->addMenu (calcMenu);

    calcMenu->addAction (actCalc1);
    connect (actCalc1, SIGNAL (triggered()), this, SLOT (slotTest1()) );
    actCalc1->setEnabled (false);

    calcMenu->addAction (actCalc2);
    connect (actCalc2, SIGNAL (triggered()), this, SLOT (slotTest2()) );
    actCalc2->setEnabled (false);

    QAction * actOporTest = calcMenu->addAction (tr("Test FFT for opor"));
    connect (actOporTest, SIGNAL (triggered()), this, SLOT (oporTest()) );

    QAction * actStcTest = calcMenu->addAction (tr("Test FFT for stc"));
    connect (actStcTest, SIGNAL (triggered()), this, SLOT (stcFFTTest()) );

    QAction * actStc1Test = calcMenu->addAction (tr("Test FFT for stc1"));
    connect (actStc1Test, SIGNAL (triggered()), this, SLOT (stc1FFTTest()) );
}

void RadMainWindow :: slotTest1 (void)
{
    if (fileName.isEmpty())
        return;

    CalcOpor1 * cop = new CalcOpor1 (nd);
    complex<double> * opor = cop->calc();//new complex<double> [nd];
    qDebug () << __PRETTY_FUNCTION__ << N1;
    QFile fContData (QString ("stc4.dat"));
    fContData.open (QIODevice::WriteOnly);
    QTextStream stCont (&fContData);
    FFT_Transform fft;// = new FFT_Transform;
    radDataWidget * wOpFFT = new radDataWidget (opor, N1);
    QMdiSubWindow * subWopFFT = m_mdiArea->addSubWindow (wOpFFT);
    wOpFFT->show ();
    subWopFFT->setAttribute (Qt::WA_DeleteOnClose);
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
        return;

    fileConvName = QFileDialog::getSaveFileName (this, tr("Save 1st data"), QDir::currentPath(), tr("All files (*)"));

    FILE * fid6 = fileConvName.isEmpty() ? 0 : fopen (fileConvName.toAscii().constData(), "w+");

    qDebug () << __PRETTY_FUNCTION__ << (int)na;
    QAbstractItemModel * radModel = new QStandardItemModel (nd2, 1, 0);// (nd2, na);

    int nr (0);
    QFile fContStData (QString ("stc.dat"));
    fContStData.open (QIODevice::WriteOnly);
    QTextStream stContSt (&fContStData);
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
        if (i0 >= 5779)
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
        if (i0 >= 5779)
            qDebug () << __PRETTY_FUNCTION__ << QString ("Reverse fft");
        xfft = fft (stc1, nd, nd, FFTW_BACKWARD, FFTW_ESTIMATE );//| FFTW_MEASURE);
        //delete [] xConv;
        double * stc2 = new double [2*nd];
        for (int ii=0; ii<nd; ii++)
        {
            int ind = ii;//(ii==0 ? nd-1 : ii-1);
            stc2[2*ii] = real (xfft[ind])/nd;//stc3[i]);
            stc2[2*ii+1] = imag (xfft[ind])/nd;
/*            stc2Op << stc2[2*i] << " " << stc2[2*i+1] << endl;
            stc2abs[i] = sqrt (stc2[2*i]*stc2[2*i] + stc2[2*i+1]*stc2[2*i+1]);
            double gray (stc2abs[i]);
            uint val = (uint)(256*gray);

            QRgb v = qRgb (val, val, val);
            Q_UNUSED (v);
            ////qDebug ()  << __PRETTY_FUNCTION__ << v;
            convImage->setPixel (i, i0/5, v);//qRgb(val, val, val));

    ////        qDebug () << __PRETTY_FUNCTION__ << i << (double)stc2abs[i];
*/
        }
        if (i0==0)
        {
            radDataWidget * wStc2 = new radDataWidget();
            wStc2->setWindowTitle (tr("Stc2 after FFT"));
            QAbstractItemModel * radCModel = new QStandardItemModel (nd, 2, 0);// (nd2, na);
            radCModel->setHeaderData (0, Qt::Horizontal, QString("Real"), Qt::DisplayRole);
            radCModel->setHeaderData (1, Qt::Horizontal, QString("Image"), Qt::DisplayRole);
            radCModel->setHeaderData (2, Qt::Horizontal, QString("Module"), Qt::DisplayRole);
            for (int ii=0; ii<nd; ii++)
            {
                QModelIndex wIndex = radCModel->index (ii, 0);
                radCModel->setData (wIndex, (double)(stc2[2*ii]), Qt::DisplayRole);
                wIndex = radCModel->index (ii, 1);
                radCModel->setData (wIndex, (double)(stc2[2*ii+1]), Qt::DisplayRole);
//                wIndex = radCModel->index (ii, 2);
//                radCModel->setData (wIndex, (double)stc2abs[ii], Qt::DisplayRole);
            }
            wStc2->setModel (radCModel);
            QMdiSubWindow * subWStc2 = m_mdiArea->addSubWindow (wStc2);
            wStc2->show ();
            subWStc2->setAttribute (Qt::WA_DeleteOnClose);
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

        delete [] stc2;
    }
    qDebug () << __PRETTY_FUNCTION__ << QString ("Data were read and processed");
    fContData.close();
//    double * stc2abs = new double [nd];
    QSize imSize (nd, na/5);
    QImage * convImage = new QImage (imSize, QImage::Format_RGB32);//QImage::Format_Mono);
    if (!convImage || convImage->size().isNull())
    {
        if (convImage)
            delete convImage;
        return;
    }
    convImage->fill (0);
    qDebug () << __PRETTY_FUNCTION__ << convImage->size () << imSize;
/*    QFile fileStc2 ("stc2.dat");
    fileStc2.open (QIODevice::WriteOnly);
    QTextStream stc2Op (&fileStc2);
    for (int i0=0; i0<na; i0++)
    {
        for (int i=0; i<nd; i++)
        {
            int ind = (i==0 ? nd-1 : i-1);
            stc2[2*i] = real (xfft[ind])/nd;//stc3[i]);
            stc2[2*i+1] = imag (xfft[ind])/nd;
            stc2Op << stc2[2*i] << " " << stc2[2*i+1] << endl;
            stc2abs[i] = sqrt (stc2[2*i]*stc2[2*i] + stc2[2*i+1]*stc2[2*i+1]);
            double gray (stc2abs[i]);
            uint val = (uint)(256*gray);

            QRgb v = qRgb (val, val, val);
            Q_UNUSED (v);
            ////qDebug ()  << __PRETTY_FUNCTION__ << v;
            convImage->setPixel (i, i0/5, v);//qRgb(val, val, val));

    ////        qDebug () << __PRETTY_FUNCTION__ << i << (double)stc2abs[i];
        }
        delete [] xfft;

        if (i0==0)
        {
            radDataWidget * wStc2 = new radDataWidget();
            wStc2->setWindowTitle (tr("Stc2 after FFT"));
            QAbstractItemModel * radCModel = new QStandardItemModel (nd, 3, 0);// (nd2, na);
            radCModel->setHeaderData (0, Qt::Horizontal, QString("Real"), Qt::DisplayRole);
            radCModel->setHeaderData (1, Qt::Horizontal, QString("Image"), Qt::DisplayRole);
            radCModel->setHeaderData (2, Qt::Horizontal, QString("Module"), Qt::DisplayRole);
            for (int i=0; i<nd; i++)
            {
                QModelIndex wIndex = radCModel->index (i, 0);
                radCModel->setData (wIndex, (double)(stc2[2*i]), Qt::DisplayRole);
                wIndex = radCModel->index (i, 1);
                radCModel->setData (wIndex, (double)(stc2[2*i+1]), Qt::DisplayRole);
                wIndex = radCModel->index (i, 2);
                radCModel->setData (wIndex, (double)stc2abs[i], Qt::DisplayRole);
            }
            wStc2->setModel (radCModel);
            QMdiSubWindow * subWStc2 = m_mdiArea->addSubWindow (wStc2);
            wStc2->show ();
            subWStc2->setAttribute (Qt::WA_DeleteOnClose);
        }

    //    delete [] stc3;
    ////        qDebug () << __PRETTY_FUNCTION__ << h;
    }
*/
    QString fileImageName = QString ("rgg.png");
    convImage->save (fileImageName, "PNG");

    rggImageWidget * imW = new rggImageWidget;
    imW->setImage (*convImage);
    QMdiSubWindow * subImW = m_mdiArea->addSubWindow (imW);
    imW->show ();
    subImW->setAttribute (Qt::WA_DeleteOnClose);

//    delete [] stc4;
//    delete [] opor;
//    delete [] opor2;
    delete [] stc1;
//    delete [] stc;
    delete [] st;
    delete cop;
    if (fid6)
        fclose (fid6);
    actCalc2->setEnabled (true);
}

void RadMainWindow :: openConvFile (void)
{
    actCalc2->setEnabled (true);
    slotTest2 ();
}

void RadMainWindow :: slotTest2 (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString fileName = QFileDialog::getOpenFileName (this, tr("Open convolution data"), QDir::currentPath(), tr("All files (*)"));
    if (fileName.isEmpty())
        return;

    FILE * fid6 = fopen (fileName.toAscii().constData(), "rb");
    if (!fid6)
        return;
    qDebug () << __PRETTY_FUNCTION__ << fid6 << na_ots;
    int h1 = fseek (fid6, nd*8*na_ots, SEEK_SET);
    if (h1 < 0)
    {
        fclose (fid6);
        return;
    }
    int read (0);
    CMatrix rgg1 (0.0, ndrz, na2);
    complex<double>* rgg1Vec = new complex<double> [ndrz*na2];
    QFile rggMatrC("rgg1.dat");
    rggMatrC.open(QIODevice::WriteOnly);
    QTextStream rggStr(&rggMatrC);

    QFile stlb2C("stlb2.dat");
    stlb2C.open(QIODevice::WriteOnly);
    QTextStream stlb2Str (&stlb2C);
    for (int i0=0; i0<1; i0++)
    {
        double stlb[2*nd];
        double stlb2[ndrz*2];
        int cr = fread (stlb, sizeof (double)/2, nd*2, fid6);
        for (int j=0; j<ndrz*2;j++)
            stlb2[j]=stlb[j+2*ndv];
        for (int j=0; j<ndrz;j++)
        {
            rgg1(j, i0) = complex<double>(stlb2[2*j], stlb2[2*j+1]);
            rgg1Vec[j*ndrz+i0] = complex<double>(stlb2[2*j], stlb2[2*j+1]);
        }
        if (i0==0)
        {
            for (int j=0; j<ndrz*2;j++)
                stlb2Str << stlb2[j] << endl;
            qDebug () << __PRETTY_FUNCTION__ << i0 << na2 << cr;// << ndv << stlb2[0] << stlb2[2*ndv] << sizeof (quint32);
        }
        read++;
    }
    for (int i=0; i<10; i++)
    {
        for (int j=0; j<10; j++)
            rggStr << real (rgg1(i, j)) << " " << imag (rgg1(i, j)) << "i ";
        rggStr << endl;
    }
//    for (int i=0; i<ndrz*na2; i++)
//        rgg1Vec[i] = rgg1.getData()[i];
/*    for (int i=0; i<ndrz*nas/2; i++)
    {
        double xr = real (rgg1Vec[i]);//.getData()[i]);
        double yr = imag (rgg1Vec[i]);//.getData()[i]);
        qDebug () << __PRETTY_FUNCTION__ << (double)xr << (double)yr;
    }*/
    CMatrix rgg  (0.0, ndrz, na2);
    qDebug () << __PRETTY_FUNCTION__ << QString ("Matrices were set") << ndrz << nas << na2;

//    CMatrix corf2 (complex<double>(0.0), ndrz, na2);
    CMatrix corf3 (complex<double>(0.0), ndrz, nas);
    QFile corfCont ("ctest.dat");
    corfCont.open (QIODevice::WriteOnly);
    QTextStream stCorf (&corfCont);
    int from_opor (0);
    complex<double> * corfVec = new complex<double> [ndrz*na2];
    CMatrix corf (complex<double>(0.0), ndrz, na2);
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<nas; j++)
        {
            double x = (-nas/2+j)*dx;
            double rt = sqrt (R*R+x*x+H*H);
            double rt1 = rt - sqrt (R*R+H*H);
            int N0 = (int)(rt1/dnr);
            double phase = -4*pi*rt/lamp;
            corf3(N0, i) = complex<double>(cos(phase), sin(phase));
            //stCorf << (double)real (corf3(N0, i)) << " " << (double)imag (corf3(N0, i)) << " " << N0 << " " << i << endl;
            //qDebug () << __PRETTY_FUNCTION__ << x << phase << (double)real (corf3(N0, i)) << (double)imag (corf3(N0, i)) << N0 << i;
        }
        from_opor++;
    }
//    complex<double>* c = corf3.getData();
    int num(0);
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<nas; j++)
        {
            corfVec[num] = corf3(i, j);
            if (real(corfVec[num]) > 0.1e-15 || imag (corfVec[num]) > 0.1e-15)
                qDebug () << __PRETTY_FUNCTION__ << real(corfVec[num]) << imag (corfVec[num]);
            num++;
        }
    }

    int cor_func (0);
    for (int i=0; i<nas/2; i++)
    {
        for (int j=0; j<ndrz; j++)
        {
            corf(j, i) = corfVec[j*ndrz+i+nas/2];//corf3(j, i+nas/2);
            corf(j, i+na2-nas/2) = corfVec[j*ndrz+i];// corf3(j, i);
//            if (sqrt (real (corf3(j, i+nas/2))*real (corf3(j, i+nas/2)) + imag (corf3(j, i+nas/2))*imag (corf3(j, i+nas/2))) > 0.1e-15 )
//            {
//                qDebug () << __PRETTY_FUNCTION__ << i << " " << j << real (corf(j, i+na2-nas/2)) << imag (corf(j, i+na2-nas/2)) << real (corf3(j, i)) << imag (corf3(j, i)) << real (corf.getData()[i*ndrz+j]) << imag (corf.getData()[i*ndrz+j]);
//            }
        }
        cor_func++;
    }

    stCorf << QString("Row matrix") << endl;
    for (int i=0; i < ndrz*na2; i++)
    {
        double xr = real (corfVec[i]);
        double yr = imag (corfVec[i]);
        stCorf << xr << " " << yr << endl;
    }
    stCorf << QString("Matrix for FFT") << endl;

    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<nas; j++)
        {
            double x = real (corf(i,j));
            double y = imag (corf(i,j));
            stCorf << x << " " << y << " ";
        }
        stCorf << endl;
    }
    stCorf << endl;

    FFT2_Transform fft2;// = new FFT2_Transform;
    complex<double> * corfw = fft2(corf.getData(), ndrz, nas/2, FFTW_FORWARD, FFTW_ESTIMATE);
    stCorf << QString("Results of FFT") << endl;
    for (int i=0; i<ndrz*nas/2; i++)
    {
        stCorf << qSetFieldWidth(4) << qSetRealNumberPrecision(2) << real (corfw[i]) << " " 
               << qSetFieldWidth(4) << qSetRealNumberPrecision(2) << imag (corfw[i]);
        stCorf << endl;
    }
/*    for (int i=0; i<100; i++)
    {
        double xr = real (rgg1Vec[i]);//.getData()[i]);
        double yr = imag (rgg1Vec[i]);//.getData()[i]);
        qDebug () << __PRETTY_FUNCTION__ << (double)xr << (double)yr;
    }*/
    complex<double> * rggD = fft2(rgg1Vec, ndrz, nas/2, FFTW_FORWARD, FFTW_ESTIMATE);
//    Q_UNUSED (rggD);
    int cor_volfr (0);
    for (int i=0; i<na2; i++)
    {
        for (int j=0; j<ndrz; j++)
        {
            rggD[j*ndrz+i] *= conj (corfw[j*ndrz+i]);
            rggD[j*ndrz+i] /= na2*ndrz;
        }
        cor_volfr++;
    }
/*    for (int i=0; i<100; i++)
    {
        double xr = real (rggD[i]);//.getData()[i]);
        double yr = imag (rggD[i]);//.getData()[i]);
        qDebug () << __PRETTY_FUNCTION__ << (double)xr << (double)yr;
    }*/
    complex<double> * rggBD = fft2(rggD/*rgg.getData()*/, ndrz, nas/2, FFTW_BACKWARD, FFTW_ESTIMATE);
    double maxVal = 0.0;
//    double minVal = 0.0;//sqrt (real(rggBD[0])*real(rggBD[0])+imag(rggBD[0])*imag(rggBD[0]));
    for (int i=0; i<ndrz*nas/2; i++)
    {
        rggBD[i] /= (ndrz*nas)/2.0;
        rggBD[i] /= (ndrz*nas)/2.0;
        maxVal = qMax (maxVal, sqrt (real(rggBD[i])*real(rggBD[i])+imag(rggBD[i])*imag(rggBD[i])));
//        minVal = qMin (minVal, sqrt (real(rggBD[i])*real(rggBD[i])+imag(rggBD[i])*imag(rggBD[i])));
    }
    for (int i=0; i<100; i++)
    {
        double xr = real (rggBD[i]);//.getData()[i]);
        double yr = imag (rggBD[i]);//.getData()[i]);
        qDebug () << __PRETTY_FUNCTION__ << (double)xr << (double)yr;
    }
    qDebug () << __PRETTY_FUNCTION__ << tr ("Image was calculated");
    QImage * hIm = new QImage (ndrz, nas/2, QImage::Format_ARGB32);
//    qDebug () << __PRETTY_FUNCTION__ << hIm->colorCount ();
    uchar * imData = new uchar [ndrz*nas/2];
    int ii (0);
    quint32 maxvalim = 0;
    QVector<QRgb> colors;
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<nas/2;j++)
        {
            imData[ii] = sqrt (real(rggBD[ii])*real(rggBD[ii])+imag(rggBD[ii])*imag(rggBD[ii]))/maxVal*8000;
            uint val = (uint)(256*imData[ii]/512/0.3);
            maxvalim = qMax (maxvalim, val);
            QRgb v = qRgb (val, val, val);
            if (!colors.contains(v))
                colors.append (v);
            QColor vCol (v);
            //int pIndex = hIm->pixelIndex (i, j);
            hIm->setPixel (i, j, v);//qRgb(255, 255, 255));
            if (val > 0)
                qDebug () << __PRETTY_FUNCTION__ << i<< j << ii << imData[ii] << val << vCol ;
            ii++;
        }
    }
    //bool isLoaded = hIm->loadFromData (imData, ndrz*nas/2);
    qDebug () << __PRETTY_FUNCTION__ << maxvalim;//isLoaded;
    hIm->save("rgg2a.png", "PNG");
    QPixmap pIm = QPixmap::fromImage (*hIm);
    QLabel * lIm = new QLabel ;
    QWidget * wImage = new QWidget;
    QHBoxLayout * hImLay = new QHBoxLayout (wImage);
    lIm->setPixmap (pIm);
    hImLay->addWidget (lIm);
    m_mdiArea->addSubWindow (wImage);

    delete [] corfVec;

    fclose (fid6);
}

void RadMainWindow :: fftTest (void)
{
    QString fName = QFileDialog::getOpenFileName (this, tr("Open File"), QDir::currentPath(), tr("All files (*.*)"));
    if (fName.isEmpty())
        return;

    QVector<double> v001;
    QFile f001 (fName);
    f001.open (QIODevice::ReadOnly);
    QString rfftStr;

    QTextStream fstr (&f001);
    rfftStr = fstr.readLine();// >> rfftStr;
    //qDebug () << __PRETTY_FUNCTION__ << rfftStr;
    QStringList fftStrList = rfftStr.split(QString("\t"));
    //fstr >> v001;
    int n = fftStrList.size();
    qDebug () << __PRETTY_FUNCTION__ << n;
    QFile fftc ("001c.dat");
    fftc.open (QIODevice::WriteOnly);
    QTextStream fftcStr (&fftc);
    for (int i=0; i<n; i++)
    {
        bool ok;
        double x = fftStrList[i].toDouble (&ok);
        if (ok)
        {
            v001.append (x);
            fftcStr << x << endl;
        }
    }

//    for (int i=0; i<v001.size(); i++)
//        qDebug () << __PRETTY_FUNCTION__ << v001[i];

    int n2 = FFT_Transform::pow2roundup(v001.size());//1024;
    complex<double> * x = new complex<double>[n2];
    //double * x = new double [n2];
    for (int i=0; i<n2; i++)
        x[i] = complex<double>(0.0, 0.0);

    for (int i=0; i<n; i++)
        x[i] = complex<double>(v001[i], 0.0);// v001[i];
    FFT_Transform fft;// = new FFT_Transform;
    complex<double> * xfft = new complex<double> [n2];
    xfft = fft (x, n, n2, FFTW_FORWARD, FFTW_ESTIMATE );//| FFTW_MEASURE);
    QString fSaveName = QFileDialog::getSaveFileName (this, tr("Save File"), QDir::currentPath(), tr("All files (*.*)"));
    if (fSaveName.isEmpty())
    {
        delete [] xfft;
        delete [] x;
        return;
    }
    QFile fSave(fSaveName);
    fSave.open (QIODevice::WriteOnly);
    QTextStream fftRes (&fSave);
    complex<double> * xfftrev = new complex<double> [n2];
    xfftrev = fft (xfft, n, n2, FFTW_BACKWARD, FFTW_ESTIMATE );//| FFTW_MEASURE);

    for (int i=0; i<n2; i++)
    {
        double r = real (xfft[i])*n2;
        double im = imag (xfft[i])*n2;
        fftRes << qSetFieldWidth(4) << qSetRealNumberPrecision(2) << real (x[i]) << " "
               << qSetFieldWidth(4) << qSetRealNumberPrecision(2) << imag (x[i]) << "i "
               << qSetFieldWidth(20) << qSetRealNumberPrecision(16) << r << " "
               << qSetFieldWidth(20) << qSetRealNumberPrecision(16) << im << "i "
               << qSetFieldWidth(4) << qSetRealNumberPrecision(2) << real (xfftrev[i]) << " "
               << qSetFieldWidth(4) << qSetRealNumberPrecision(2) << imag (xfftrev[i]) << "i"
               << endl;
    }
    delete [] xfft;
    delete [] x;
}

void RadMainWindow :: oporTest (void)
{
    complex<double> * opor = new complex<double> [nd];
    complex<double> * opor2 = new complex<double> [nd];

    for (int i=0; i<nd; i++)
    {
        opor[i] = complex<double>(0.0, 0.0);
        opor2[i] = complex<double>(0.0, 0.0);
    }

    for (int n=0; n< N1; n++)
    {
        double phase = pi*fsp*n*n/(N1*fcvant2) - pi*fsp*n/fcvant2 ; 
        double oc = cos (phase);
        double os = sin (phase);
        opor[n] = complex<double>(oc, os);
    }
    int N2 = (N1/2);
    for (int i=0; i<N2; i++)
    {
        opor2[i] = opor[i+N2];
        opor2[i+nd-N2] = opor[i];
    }
    radDataWidget * wOp = new radDataWidget (opor, N1);
    QMdiSubWindow * subWop = m_mdiArea->addSubWindow (wOp);
    wOp->setWindowTitle (tr("Opor before FFT"));
    wOp->show ();
    subWop->setAttribute (Qt::WA_DeleteOnClose);
    FFT_Transform fft;
    int nfft = nd;//4*FFT_Transform :: pow2roundup(N1);
    opor = fft (opor2, nfft, nfft, FFTW_FORWARD, FFTW_ESTIMATE);
    complex<double> * oporFFT = new complex<double> [nd];
    for (int i=0; i<nd; i++)
    {
        double re = real(opor[i])*nfft;
        double im = imag (opor[i])*nfft;
        oporFFT[i] = complex<double>(re, im);
    }

    radDataWidget * wOpFFT = new radDataWidget (oporFFT, N1);
    wOpFFT->setWindowTitle (tr("Opor after FFT"));
    QMdiSubWindow * subWopFFT = m_mdiArea->addSubWindow (wOpFFT);
    wOpFFT->show ();
    subWopFFT->setAttribute (Qt::WA_DeleteOnClose);
    delete [] opor2;
    delete [] opor;
    delete [] oporFFT;
}

void RadMainWindow :: stc1FFTTest (void)
{
    QString fName = QFileDialog::getOpenFileName (this, tr("Open File"), QDir::currentPath(), tr("All files (*.*)"));
    if (fName.isEmpty())
        return;

    QVector<double> v001;
    QFile f001 (fName);
    f001.open (QIODevice::ReadOnly);
    QString rfftStr;

    QTextStream fstr (&f001);
    rfftStr = fstr.readLine();// >> rfftStr;
    //qDebug () << __PRETTY_FUNCTION__ << rfftStr;
    QStringList fftStrList = rfftStr.split(QString("\t"));
    //fstr >> v001;
    int n = fftStrList.size();
    qDebug () << __PRETTY_FUNCTION__ << n;
    QFile fftc ("stc1c.dat");
    fftc.open (QIODevice::WriteOnly);
    QTextStream fftcStr (&fftc);
    for (int i=0; i<n; i++)
    {
        bool ok;
        Q_UNUSED (ok);
        //QStringList fftstr = fftStrList[i].split(QString(" "));
        QByteArray fftBuf (fftStrList[i].mid(0, fftStrList[i].count()-1).toUtf8());
        if (i<=10)
            qDebug () << __PRETTY_FUNCTION__ << fftBuf;
        QTextStream strBuf (fftBuf);
        double x, y;// = fftstr[0].toDouble (&ok);
        QString sp;
        strBuf >> x >> sp >> y ;
///        strBuf >> y;
        if (sp.contains("-"))
            y *= -1.0;
        if (i<=10)
            qDebug () << __PRETTY_FUNCTION__ << x << sp << y;
        fftcStr << x << " " << y << endl;
        v001.append (x);
        v001.append (y);
    }

//    for (int i=0; i<v001.size(); i++)
//        qDebug () << __PRETTY_FUNCTION__ << v001[i];

    int n2 = FFT_Transform::pow2roundup(v001.size()/2);//1024;
    complex<double> * x = new complex<double>[n2];
    //double * x = new double [n2];
    for (int i=0; i<n2; i++)
        x[i] = complex<double>(0.0, 0.0);

    for (int i=0; i<n; i++)
    {
        x[i] = complex<double>(v001[2*i], v001[2*i+1]);
        fftcStr << real (x[i]) << " " << imag (x[i]) << endl;
    }
    FFT_Transform fft;// = new FFT_Transform;
    complex<double> * xfft = new complex<double> [n2];
    xfft = fft (x, n2, n2, FFTW_BACKWARD, FFTW_ESTIMATE );//| FFTW_MEASURE);
    QString fSaveName = QFileDialog::getSaveFileName (this, tr("Save File"), QDir::currentPath(), tr("All files (*.*)"));
    if (fSaveName.isEmpty())
    {
        delete [] xfft;
        delete [] x;
        return;
    }
    QFile fSave(fSaveName);
    fSave.open (QIODevice::WriteOnly);
    QTextStream fftRes (&fSave);
    complex<double> * xfftrev = new complex<double> [n2];
    xfftrev = fft (xfft, n2, n2, FFTW_FORWARD, FFTW_ESTIMATE );//| FFTW_MEASURE);
    fftRes.setRealNumberNotation (QTextStream::FixedNotation);

    for (int i=0; i<n2; i++)
    {
        double r = real (xfft[i])/n2;
        double im = imag (xfft[i])/n2;
        fftRes << qSetFieldWidth(10) << qSetRealNumberPrecision(16) << real (x[i]) << " "
               << qSetFieldWidth(10) << qSetRealNumberPrecision(16) << imag (x[i]) << "i ";
        scientific (fftRes);
        fftRes
               << qSetFieldWidth(10) << qSetRealNumberPrecision(16) << r << " "
               << qSetFieldWidth(10) << qSetRealNumberPrecision(16) << im << "i ";
        fixed (fftRes);
        fftRes
               << qSetFieldWidth(10) << qSetRealNumberPrecision(16) << real (xfftrev[i]) << " "
               << qSetFieldWidth(10) << qSetRealNumberPrecision(16) << imag (xfftrev[i]) << "i"
               << endl;
    }
    delete [] xfft;
    delete [] x;
}

void RadMainWindow :: stcFFTTest (void)
{
    if (fileName.isEmpty())
        return;

    radDataWidget * w = new radDataWidget();
    QFile fData (fileName);
    quint8 * st = new quint8 [nd2];
    for (int i=0; i<nd2; i++)
    {
        st [i] = 0;
    }
    complex<double>* stc = new complex<double> [nd];
    complex<double>* stc1 = new complex<double> [nd];
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
        return;

    qDebug () << __PRETTY_FUNCTION__ << (int)na;
    QAbstractItemModel * radModel = new QStandardItemModel (nd2, 1, 0);// (nd2, na);

    int nr (0);
    QFile fContStData (QString ("stc.dat"));
    fContStData.open (QIODevice::WriteOnly);
    QTextStream stContSt (&fContStData);
/*    for (int i=0; i<na; i++)
    {
*/
    int cr = fread (st, sizeof (quint8), nd2, fid5);
    if (cr <= 0)
        return;
    for (int ii=0; ii< nd2; ii++)
    {
        //if (i<1)
        //{
        //}
        //if (st[ii] > 128)
        //    st[ii] -= 256;
        QModelIndex wcIndex = radModel->index (nr, 0);
        radModel->setData (wcIndex, QString::number (st[ii]), Qt::DisplayRole);
        nr++;
    }
/*
    }
*/

    for (int ii=0; ii<128; ii++)
        st[ii] = 0.0;

    for (int ii=1; ii<=ndn; ii++)
    {
        double re = st[2*ii-1];
        double im = st[2*ii-2];
        if (re > 128)
            re -= 256;
        if (im > 128)
            im -= 256;
        stContSt << re << " " << im << endl;
        stc[ii-1] = complex<double> (re, im);//st[2*ii], st[2*ii+1]);
    }
    for (int i=ndn; i<nd; i++)
        stc[i] = complex<double>(0.0, 0.0);
    radDataWidget * wStc = new radDataWidget(stc, nd);
    QMdiSubWindow * subWStc = m_mdiArea->addSubWindow (wStc);
    wStc->show();
    subWStc->setAttribute (Qt::WA_DeleteOnClose);
    w->setModel (radModel);
    fData.close ();
    QMdiSubWindow * subW = m_mdiArea->addSubWindow (w);
    w->show ();
    subW->setAttribute (Qt::WA_DeleteOnClose);

    FFT_Transform fft;// = new FFT_Transform;
    complex<double>* stc4 = new complex<double> [nd];
    stc4 = fft (stc, nd, nd, FFTW_FORWARD, FFTW_ESTIMATE);
    for (int i=0; i<nd; i++)
    {
        stc4[i] *= nd;
    }
    radDataWidget * wStcFFT = new radDataWidget(stc4, nd);
    QMdiSubWindow * subWStcFFT = m_mdiArea->addSubWindow (wStcFFT);
    wStcFFT->show ();
    subWStcFFT->setAttribute (Qt::WA_DeleteOnClose);
    delete [] stc4;
}
