#include <QTableView>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QGridLayout>
#include <QtDebug>

#include "radTableWidget.h"

radTableWidget :: radTableWidget (const complex<double> * zVec, int n, int m, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tvData (new QTableView (this))
{
    setWindowTitle (tr("Source matrix"));
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (tvData, 0, 0, 1, 1);
    QAbstractItemModel * sMod = initComplexModel (zVec, n, m);
    setModel (sMod);
}

radTableWidget :: radTableWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tvData (new QTableView (this))
{
    setWindowTitle (tr("Source matrix"));
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (tvData, 0, 0, 1, 1);
}

radTableWidget :: ~radTableWidget (void)
{
}

void radTableWidget :: setModel (QAbstractItemModel * mod)
{
    QAbstractItemModel * oldModel = tvData->model ();
    qDebug () << __PRETTY_FUNCTION__ << mod << oldModel;
    tvData->setModel (mod);
    qDebug () << __PRETTY_FUNCTION__ << mod << tr("Model was set");
/*    if (oldModel && oldModel != mod)
        delete oldModel;
*/
}

QAbstractItemModel * radTableWidget :: initComplexModel (const complex<double> * zVec, int n, int m)
{
    if (!zVec || n==0 || m==0)
        return 0;

    QAbstractItemModel * cModel = new QStandardItemModel (n, 2*m);
    for (int i=0; i<m; i++)
    {
        cModel->setHeaderData (2*i, Qt::Horizontal, tr("Real component"), Qt::DisplayRole);
        cModel->setHeaderData (2*i+1, Qt::Horizontal, tr("Image component"), Qt::DisplayRole);
    }
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            double r = real(zVec[j+n*i]);
            double im = imag(zVec[j+n*i]);
            QModelIndex cRInd = cModel->index (i, 2*j);
            cModel->setData (cRInd, QString::number (r, 'f', 16), Qt::DisplayRole);
            QModelIndex cImInd = cModel->index (i, 2*j+1);
            cModel->setData (cImInd, QString::number (im, 'f', 16), Qt::DisplayRole);
        }
    }

    return cModel;
}
