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

#include <radDataWidget.h>
#include <rggImageWidget.h>
#include <constants1.h>

#include <complex>
#include <math.h>
#include <stdio.h>
#include <fftw3.h>

#include "matrix.h"
#include "fft_c.h"

#include "radmainwindow.h"
#include "ui_radius_mainwindow.h"

using std::complex;

RadMainWindow :: RadMainWindow (QWidget * parent, Qt::WindowFlags flags)
    : QMainWindow (parent, flags),
    UI (new Ui::Rad_Main_Window),
    m_mdiArea (new QMdiArea (this)),
    actCalc1 (new QAction (tr("Calculate&1"), this)),
    actCalc2 (new QAction (tr("Calculate&2"), this)),
    stc (new complex<double> [nd]),
    stc1 (new complex<double> [nd]),
    stc2 (new double [2*nd]),
    stc3 (0),
    stc4 (0)
{
    UI->setupUi (this);

    setCentralWidget (m_mdiArea);
    init ();
}

RadMainWindow :: ~RadMainWindow (void)
{
    delete [] stc2;
    delete [] stc1;
    delete [] stc;
    if (stc3)
        delete [] stc3;
    if (stc4)
        delete [] stc4;
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
}

void RadMainWindow :: slotTest1 (void)
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
    QAbstractItemModel * radModel = new QStandardItemModel (nd2, 2, 0);// (nd2, na);

    for (int i=1; i<=na; i++)
    {
        int cr = fread (st, sizeof (quint8), nd2, fid5);
        if (cr <= 0)
            return;
        for (int ii=1; ii<= nd2; ii++)
            if (st[ii-1] > 128)
                st[ii-1] -= 256;

        for (int ii=1; ii<=128; ii++)
            st[ii-1] = 0.0;

        for (int ii=0; ii<ndn; ii++)
        {
            double re = st[2*ii];
            double im = st[2*ii+1];
            stc[ii] = complex<double> (re, im);//st[2*ii], st[2*ii+1]);

            if (i <= 2)
            {
                QModelIndex stReInd = radModel->index (2*ii, i-1);
                radModel->setData (stReInd, QString::number (re), Qt::DisplayRole);
                QModelIndex stImInd = radModel->index (2*ii+1, i-1);
                radModel->setData (stImInd, QString::number (im), Qt::DisplayRole);
            }
        }
//        qDebug () << __PRETTY_FUNCTION__ << i << " " << na;// << *(st+i-1);
    }

    QFile contOp ("opor.dat");
    contOp.open (QIODevice::WriteOnly);

    w->setModel (radModel);
    fData.close ();
    QMdiSubWindow * subW = m_mdiArea->addSubWindow (w);
    w->show ();
    subW->setAttribute (Qt::WA_DeleteOnClose);
    delete [] st;
    qDebug () << __PRETTY_FUNCTION__;
    double * st1 = new double [nd];
    complex<double> * opor = new complex<double> [nd];
    complex<double> * opor2 = new complex<double> [nd];

    for (int i=0; i<nd; i++)
    {
        st1[i] = 0.0;
        opor[i] = complex<double>(0.0, 0.0);
        opor2[i] = complex<double>(0.0, 0.0);
    }

    for (int n=0; n< N1; n++)
    {
        double phase = pi*fsp*n*n/(N1*fcvant2) - pi*fsp*n/fcvant2 ; 
        double oc = cosl (phase);
        double os = sinl (phase);
        opor[n] = complex<double>(oc, os);
    }
    QTextStream stOp (&contOp);
    for (int i=0; i<N1; i++)
    {
        double re = real (opor[i]);
        double im = imag (opor[i]);
        stOp << re << " " << im << "i" << endl;
    }
    contOp.close ();
    int N2 = (N1/2);
    for (int i=0; i<N2; i++)
    {
        opor2[i] = opor[i+N2];
        opor2[i+nd-N2] = opor[i];
    }
    QFile fContData (QString ("stc4.dat"));
    fContData.open (QIODevice::WriteOnly);
    QTextStream stCont (&fContData);
    for (int i=0; i<nd; i++)
    {
        double r = real (opor2[i]);
        double im = imag (opor2[i]);
        Q_UNUSED (r);
        Q_UNUSED (im);
        //stCont << r << (im >=0 ? "+ " : " ") << im << "i" << endl;
    }
    fileConvName = QFileDialog::getSaveFileName (this, tr("Save 1st data"), QDir::currentPath(), tr("All files (*)"));

    FILE * fid6 = fileConvName.isEmpty() ? 0 : fopen (fileConvName.toAscii().constData(), "wb");

    FFT_Transform fft;// = new FFT_Transform;
    opor = fft (opor2, N2, nd, FFTW_FORWARD, FFTW_ESTIMATE);
    stc4 = fft (stc, ndn, nd, FFTW_FORWARD, FFTW_ESTIMATE);
    for (int i=0; i<nd; i++)
    {
        double r = real (stc[i]);
        double im = imag (stc[i]);
        stCont << r << (im >= 0 ? "+" : " ") << im << "i" << endl;
    }
    fContData.close();
    double * stc2abs = new double [nd];
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
    for (int i0=0; i0<na; i0++)
    {
        for (int i=0; i<nd; i++)
        {
            stc1[i] = stc4[i]*opor[i];///(nd*nd);
            stc1[i] = complex<double> (real (stc1[i]) / ((double)nd*nd), imag (stc1[i]) / ((double)nd*nd));
        }
        stc3 = fft (stc1, nd, nd, FFTW_BACKWARD, FFTW_ESTIMATE);
        qDebug () << __PRETTY_FUNCTION__ << stc3 << i0+1 << " of " << na;
        for (int i=0; i<nd; i++)
        {
            stc2[2*i] = real (stc3[i]);
            stc2[2*i+1] = imag (stc3[i]);
            stc2abs[i] = sqrt (stc2[2*i]*stc2[2*i] + stc2[2*i+1]*stc2[2*i+1]);
            double gray (stc2abs[i]);
            uint val = (uint)(256*gray);

            QRgb v = qRgb (val, val, val);
            //qDebug ()  << __PRETTY_FUNCTION__ << v;
            convImage->setPixel (i, i0/5, v);//qRgb(val, val, val));

    //        qDebug () << __PRETTY_FUNCTION__ << i << (double)stc2abs[i];
        }

        if (fid6)
        {
            size_t h = fwrite (stc2, sizeof (double), 2*nd, fid6);
            Q_UNUSED (h);
        }
        if (i0==0)
        {
            radDataWidget * w = new radDataWidget();
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
            w->setModel (radCModel);
            QMdiSubWindow * subW = m_mdiArea->addSubWindow (w);
            w->show ();
            subW->setAttribute (Qt::WA_DeleteOnClose);
        }

        delete [] stc3;
//        qDebug () << __PRETTY_FUNCTION__ << h;
    }
    QString fileImageName = QString ("rgg.png");
    convImage->save (fileImageName, "PNG");

    rggImageWidget * imW = new rggImageWidget;
    imW->setImage (*convImage);
    QMdiSubWindow * subImW = m_mdiArea->addSubWindow (imW);
    imW->show ();
    subImW->setAttribute (Qt::WA_DeleteOnClose);

    delete convImage;
    delete [] stc2abs;
    delete [] st1;
    delete [] opor;
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
    QString fileName = QFileDialog::getOpenFileName (this, tr("Save 1st data"), QDir::currentPath(), tr("All files (*)"));
    if (fileName.isEmpty())
        return;

    FILE * fid6 = fopen (fileName.toAscii().constData(), "rb");
    if (!fid6)
        return;
    qDebug () << __PRETTY_FUNCTION__ << fid6;
    int h1 = fseek (fid6, nd*8*na_ots, SEEK_SET);
    if (h1 < 0)
    {
        fclose (fid6);
        return;
    }
    int read (0);
    CMatrix rgg1 (0.0, ndrz, na2);
    for (int i=0; i<na2; i++)
    {
        double stlb[2*nd];
        double stlb2[ndrz*2];
        fread (stlb, 32, nd*2, fid6);
        for (int j=0; j<ndrz*2;j++)
            stlb2[j]=stlb[j+2*ndv];
        for (int j=0; j<ndrz;j++)
            rgg1(j, i) = complex<double>(stlb2[2*j], stlb2[2*j+1]);
        //qDebug () << __PRETTY_FUNCTION__ << i << na2;
        read++;
    }
    CMatrix rgg  (0.0, ndrz, na2);
    qDebug () << __PRETTY_FUNCTION__ << QString ("Matrices were set");

    CMatrix corf (complex<double>(0.0), ndrz, na2);
    CMatrix corf2 (complex<double>(0.0), ndrz, na2);
    CMatrix corf3 (complex<double>(0.0), ndrz, nas);
    int from_opor (0);
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<nas; j++)
        {
            double x = (-nas/2+i)*dx;
            double rt = sqrt (R*R+x*x+H*H);
            double rt1 = rt - sqrt (R*R+H*H);
            int N0 = (int)(rt1/dnr);
            double phase = -4*pi*rt/lamp;
            corf3(N0, i) = complex<double>(cos(phase), sin(phase));
            qDebug () << __PRETTY_FUNCTION__ << x << phase << (double)real (corf3(N0, i)) << (double)imag (corf3(N0, i));
        }
        from_opor++;
    }

    int cor_func (0);
    for (int i=0; i<nas/2; i++)
    {
        for (int j=0; j<ndrz; j++)
        {
            corf(j, i) = corf3(j, i+nas/2);
            corf(j, i+na2-nas/2) = corf3 (j, i);
        }
        cor_func++;
    }

    FFT2_Transform fft2;// = new FFT2_Transform;
    complex<double> * corfw = fft2(corf.getData(), ndrz, nas/2, FFTW_FORWARD, FFTW_ESTIMATE);
    Q_UNUSED (corfw);
/*    for (int i=0; i<ndrz*nas/2; i++)
    {
        double xr = real (corf3.getData()[i]);
        double yr = imag (corf3.getData()[i]);
        qDebug () << __PRETTY_FUNCTION__ << (double)xr << (double)yr;
    }*/
    complex<double> * rggD = fft2(rgg1.getData(), ndrz, nas/2, FFTW_FORWARD, FFTW_ESTIMATE);
    Q_UNUSED (rggD);
    int cor_volfr (0);
    for (int i=0; i<na2; i++)
    {
        for (int j=0; j<ndrz; j++)
            rgg(j, i) *= conj (corf2(j, i));
        cor_volfr++;
    }
    complex<double> * rggBD = fft2(rgg.getData(), ndrz, nas/2, FFTW_BACKWARD, FFTW_ESTIMATE);
    qDebug () << __PRETTY_FUNCTION__ << tr ("Image was calculated");
    QImage * hIm = new QImage (ndrz, nas/2, QImage::Format_ARGB32);
    uchar * imData = new uchar [ndrz*nas/2];
    int ii (0);
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<nas/2;j++)
        {
            imData[ii] = abs (rggBD[ii])*8000;
            uint val = (uint)(256*imData[ii]);
            QRgb v = qRgb (val, val, val);
            hIm->setPixel (i, j, v);
            qDebug () << __PRETTY_FUNCTION__ << ii << imData[ii];
            ii++;
        }
    }
    //bool isLoaded = hIm->loadFromData (imData, ndrz*nas/2);
    //qDebug () << __PRETTY_FUNCTION__ << isLoaded;
    QPixmap pIm = QPixmap::fromImage (*hIm);
    QLabel * lIm = new QLabel ;
    QWidget * wImage = new QWidget;
    QHBoxLayout * hImLay = new QHBoxLayout (wImage);
    lIm->setPixmap (pIm);
    hImLay->addWidget (lIm);
    m_mdiArea->addSubWindow (wImage);

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
    //complex<double> * x = new complex<double>[n2];
    double * x = new double [n2];
    for (int i=0; i<n2; i++)
        x[i] = 0.0;//complex<double>(0.0, 0.0);

    for (int i=0; i<n; i++)
        x[i] = v001[i];//complex<double>(v001[i], 0.0);// v001[i];
    FFT_RealTransform fft;// = new FFT_Transform;
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
    for (int i=0; i<n2; i++)
    {
        double r = real (xfft[i])*n2;
        double im = imag (xfft[i])*n2;
        fftRes << qSetFieldWidth(20) << qSetRealNumberPrecision(16) << r << " "
               << qSetFieldWidth(20) << qSetRealNumberPrecision(16) << im << "i" << endl;
    }

    delete [] xfft;
    delete [] x;
}
