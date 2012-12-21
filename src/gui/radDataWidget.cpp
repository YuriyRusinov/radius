#include <QTreeView>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QGridLayout>
#include <QtDebug>

#include "radDataWidget.h"

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
