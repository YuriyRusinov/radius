#include <QTreeView>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QGridLayout>
#include <QtDebug>

#include "radDataWidget.h"

radDataWidget :: radDataWidget (const complex<double> * zVec, int n, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tvData (new QTreeView (this))
{
    setWindowTitle (tr("Source matrix"));
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (tvData, 0, 0, 1, 1);
    QAbstractItemModel * sMod = initComplexModel (zVec, n);
    setModel (sMod);
}

radDataWidget :: radDataWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tvData (new QTreeView (this))
{
    setWindowTitle (tr("Source matrix"));
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (tvData, 0, 0, 1, 1);
}

radDataWidget :: ~radDataWidget (void)
{
}

void radDataWidget :: setModel (QAbstractItemModel * mod)
{
    QAbstractItemModel * oldModel = tvData->model ();
    qDebug () << __PRETTY_FUNCTION__ << mod << oldModel;
    tvData->setModel (mod);
    qDebug () << __PRETTY_FUNCTION__ << mod << tr("Model was set");
/*    if (oldModel && oldModel != mod)
        delete oldModel;
*/
}

QAbstractItemModel * radDataWidget :: initComplexModel (const complex<double> * zVec, int n)
{
    if (!zVec || n==0)
        return 0;

    QAbstractItemModel * cModel = new QStandardItemModel (n, 2);
    cModel->setHeaderData (0, Qt::Horizontal, tr("Real component"), Qt::DisplayRole);
    cModel->setHeaderData (1, Qt::Horizontal, tr("Image component"), Qt::DisplayRole);
    for (int i=0; i<n; i++)
    {
        double r = real(zVec[i]);
        double im = imag(zVec[i]);
        QModelIndex cRInd = cModel->index (i, 0);
        cModel->setData (cRInd, QString::number (r, 'f', 16), Qt::DisplayRole);
        QModelIndex cImInd = cModel->index (i, 1);
        cModel->setData (cImInd, QString::number (im, 'f', 16), Qt::DisplayRole);
    }

    return cModel;
}
