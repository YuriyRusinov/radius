#include <QTreeView>
#include <QAbstractItemModel>
#include <QGridLayout>
#include <QtDebug>

#include "radDataWidget.h"

radDataWidget :: radDataWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tvData (new QTreeView)
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (tvData);
    tvData->setModel (0);
}

radDataWidget :: ~radDataWidget (void)
{
    delete tvData;
}

void radDataWidget :: setModel (QAbstractItemModel * mod)
{
    QAbstractItemModel * oldModel = tvData->model ();
    qDebug () << __PRETTY_FUNCTION__ << oldModel;
    tvData->setModel (mod);
    if (oldModel && oldModel != mod)
        delete oldModel;
}
