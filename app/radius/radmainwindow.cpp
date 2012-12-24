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
    QFile fData (fileName);
    quint8 * st = new quint8 [nd2];
    complex<long double> * stc = new complex<long double> [nd];
    complex<long double> * stc1 = new complex<long double> [nd];
    complex<long double> * stc4 (0);// = new complex<double> [nd];
    Q_UNUSED (stc4);

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
}
