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
#include <QKeySequence>
#include <QBuffer>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QTreeView>

#include <radDataWidget.h>
#include <constants1.h>

#include <math.h>
#include <stdio.h>
#include <fftw3.h>

#include "fft_c.h"

#include "radmainwindow.h"
#include "ui_radius_mainwindow.h"

RadMainWindow :: RadMainWindow (QWidget * parent, Qt::WindowFlags flags)
    : QMainWindow (parent, flags),
    UI (new Ui::Rad_Main_Window),
    m_mdiArea (new QMdiArea (this)),
    actCalc1 (new QAction (tr("Calculate&1"), this)),
    actCalc2 (new QAction (tr("Calculate&2"), this)),
    stc (new complex<long double> [nd]),
    stc1 (new complex<long double> [nd]),
    stc2 (new long double [2*nd]),
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
    QString fileName = QFileDialog::getOpenFileName (this, tr("Select source file"), QDir::currentPath(), tr("All files (*)"));
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
        stc[i] = complex<long double> (0.0, 0.0);
        stc1[i] = complex<long double> (0.0, 0.0);
    }
    int a = 0;
    Q_UNUSED (a);

    FILE * fid5 = fopen (fileName.toAscii().constData(), "rb");
    qDebug () << __PRETTY_FUNCTION__ << fileName.toAscii().constData() << fid5;
    if (!fData.open (fid5, QIODevice::ReadOnly | QIODevice::Unbuffered))
        return;

    qDebug () << __PRETTY_FUNCTION__ << (int)na;
    QAbstractItemModel * radModel = new QStandardItemModel (nd2, 2, 0);// (nd2, na);

    QFile fContData (QString ("cont_data.dat"));
    fContData.open (QIODevice::WriteOnly);
    QTextStream stCont (&fContData);
    for (int i=1; i<=na; i++)
    {
        int cr = fread (st, sizeof (quint8), nd2, fid5);
        if (cr <= 0)
            return;
        if (i == 1)
            for (int ii=0; ii < nd2; ii++)
                stCont << st[ii] << " \n";
        for (int ii=1; ii<= nd2; ii++)
            if (st[ii-1] > 128)
                st[ii-1] -= 256;

        for (int ii=1; ii<=128; ii++)
            st[ii-1] = 0.0;

        for (int ii=0; ii<ndn; ii++)
        {
            double re = st[2*ii];
            double im = st[2*ii+1];
            stc[ii] = complex<long double> (re, im);//st[2*ii], st[2*ii+1]);

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

    fContData.close();
    w->setModel (radModel);
    fData.close ();
    QMdiSubWindow * subW = m_mdiArea->addSubWindow (w);
    w->show ();
    subW->setAttribute (Qt::WA_DeleteOnClose);
    delete [] st;
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
    qDebug () << __PRETTY_FUNCTION__;
    double * st1 = new double [nd];
    complex<long double> * opor = new complex<long double> [nd];
    complex<long double> * opor2 = new complex<long double> [nd];

    for (int i=0; i<nd; i++)
    {
        st1[i] = 0.0;
        opor[i] = complex<long double>(0.0, 0.0);
        opor2[i] = complex<long double>(0.0, 0.0);
    }

    for (int n=1; n<= N1; n++)
    {
        double phase = pi*fsp*(n-1)*(n-1)/(N1*fcvant2) - pi*fsp*(n-1)/fcvant2 ; 
        double oc = cos (phase);
        double os = sin (phase);
        opor[n-1] = complex<long double>(oc, os);
    }
    int N2 = (N1/2);
    for (int i=1; i<=N2; i++)
    {
        opor2[i] = opor[i+N2];
        opor2[i+nd-N2] = opor[i];
    }

    FFT_Transform fft;// = new FFT_Transform;
    opor = fft (opor2, N2, nd, FFTW_FORWARD, FFTW_ESTIMATE);
    stc4 = fft (stc, ndn, 2*nd, FFTW_FORWARD, FFTW_ESTIMATE);
    for (int i=0; i<nd; i++)
    {
        stc1[i] = stc4[i]*opor[i];///(nd*nd);
        stc1[i] = complex<long double> (real (stc1[i]) / ((long double)nd*nd), imag (stc1[i]) / ((long double)nd*nd));
    }
    delete [] opor;
    delete [] st1;
    stc3 = fft (stc1, nd, nd, FFTW_BACKWARD, FFTW_ESTIMATE);
    for (int i=0; i<nd; i++)
    {
        stc2[2*i] = real (stc3[i]);
        stc2[2*i+1] = imag (stc3[i]);
    }

    QString fileOutName = QFileDialog::getSaveFileName (this, tr("Save 1st data"), QDir::currentPath(), tr("All files (*)"));
    if (fileOutName.isEmpty())
        return;
    FILE * fid6 = fopen (fileOutName.toAscii().constData(), "wb");
    if (!fid6)
        return;
    size_t h = fwrite (stc2, sizeof (long double), 2*nd, fid6);
    qDebug () << __PRETTY_FUNCTION__ << h;
    fclose (fid6);
    actCalc2->setEnabled (true);
}

void RadMainWindow :: slotTest2 (void)
{
    qDebug () << __PRETTY_FUNCTION__;
}
