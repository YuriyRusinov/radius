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
#include <QBuffer>

#include <radDataWidget.h>
#include <constants1.h>

#include <complex>
#include <math.h>
#include <stdio.h>

#include "radmainwindow.h"
#include "ui_radius_mainwindow.h"

using std::complex;

RadMainWindow :: RadMainWindow (QWidget * parent, Qt::WindowFlags flags)
    : QMainWindow (parent, flags),
    UI (new Ui::Rad_Main_Window)
{
    UI->setupUi (this);

    m_mdiArea = new QMdiArea (this);
    setCentralWidget (m_mdiArea);
    init ();
}

RadMainWindow :: ~RadMainWindow (void)
{
}

void RadMainWindow :: openDataFile (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString fileName = QFileDialog::getOpenFileName (this, tr("Select source file"), QDir::currentPath(), tr("All files (*)"));
    if (fileName.isEmpty())
        return;

    radDataWidget * w = new radDataWidget();
    QMdiSubWindow * subW = m_mdiArea->addSubWindow (w);
    w->show ();
    subW->setAttribute (Qt::WA_DeleteOnClose);
    QFile * fData = new QFile (fileName);
    double * st = new double [nd2];
    complex<long double> * stc = new complex<long double> [nd];
    complex<long double> * stc1 = new complex<long double> [nd];
    complex<long double> * stc4 (0);// = new complex<double> [nd];
    Q_UNUSED (stc4);
    for (int i=0; i<nd2; i++)
    {
        st [i] = 0.0;
    }
    for (int i=0; i<nd; i++)
    {
        stc[i] = complex<long double> (0.0, 0.0);
        stc1[i] = complex<long double> (0.0, 0.0);
    }
    int a = 0;
    Q_UNUSED (a);

    qDebug () << __PRETTY_FUNCTION__ << fileName.toUtf8().constData();// << fid5;
    if (!fData->open (QIODevice::ReadOnly))
        return;
    QByteArray dataFromRep;
//    QDataStream stStream (fData);
    for (int i=1; i<=na; i++)
    {
        char * colData = new char [nd2*sizeof (unsigned long)];
        qint64 colLength = fData->readLine (colData, nd2*sizeof (unsigned long));
        if (colLength < 0)
        {
            qDebug () << __PRETTY_FUNCTION__ << QString ("Read error");
            continue;
        }
        QByteArray buff (colData);
        delete [] colData;
        //qDebug () << __PRETTY_FUNCTION__ << i << buff;

        QBuffer lBuf (&buff);
        lBuf.open (QIODevice::ReadOnly);
        QDataStream numStr (&lBuf);
/*        for (int ii=0; ii<nd2; ii++)
        {
            char * sNum = new char [sizeof (unsigned long)];
            int lNum = numStr.readRawData (sNum, sizeof (unsigned long));
            if (lNum < 0)
                break;
            QString strNum (sNum);
            bool ok;
            unsigned long num = strNum.toULong (&ok);
            //qDebug () << __PRETTY_FUNCTION__ << num << ok;
            st[ii] = num;
        }
*/
        quint64 num;
        for (int ii=0; ii<nd2; ii++)
        {
            numStr >> num;
            qDebug () << __PRETTY_FUNCTION__ << num;
            st[ii] = num;
        }

        //st[i-1] = num;
        //fread (st+i-1, sizeof (unsigned long), nd2, fid5);
        for (int ii=1; ii<= nd2; ii++)
            if (st[ii-1] > 128)
                st[ii-1] -= 256;

        for (int ii=1; ii<=128; ii++)
            st[ii-1] = 0.0;

        for (int ii=0; ii<ndn; ii++)
            stc[ii] = complex<long double> (st[2*ii], st[2*ii+1]);
        qDebug () << __PRETTY_FUNCTION__ << i << " " << na;// << *(st+i-1);
    }
    fData->close ();
    delete fData;


}

void RadMainWindow :: init (void)
{
    QMenu * openMenu = new QMenu (tr ("&File"), this);
    UI->menuFile->addMenu (openMenu);

    QAction * actOpenDataFile = openMenu->addAction (tr("Open &source file"));
    connect (actOpenDataFile, SIGNAL (triggered()), this, SLOT (openDataFile()) );

    UI->menuFile->addSeparator ();
    QAction * actExit = UI->menuFile->addAction (tr("E&xit"));
    connect (actExit, SIGNAL (triggered()), this, SLOT (close()) );
}
