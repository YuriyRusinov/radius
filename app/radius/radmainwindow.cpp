#include <QMenu>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QtDebug>
#include <QMdiArea>
#include <QMdiSubWindow>

#include <radDataWidget.h>
#include "radmainwindow.h"
#include "ui_radius_mainwindow.h"

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

    qDebug () << __PRETTY_FUNCTION__ << fileName;
    radDataWidget * w = new radDataWidget();
    QMdiSubWindow * subW = m_mdiArea->addSubWindow (w);
    w->show ();
    subW->setAttribute (Qt::WA_DeleteOnClose);
    QFile fData (fileName);
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
