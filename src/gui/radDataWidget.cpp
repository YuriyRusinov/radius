#include <QTreeView>
#include <QAbstractItemModel>
#include <QGridLayout>

#include "radDataWidget.h"

radDataWidget :: radDataWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tvData (new QTreeView)
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (tvData);
}

radDataWidget :: ~radDataWidget (void)
{
    delete tvData;
}

void radDataWidget :: setModel (QAbstractItemModel * mod)
{
    QAbstractItemModel * oldModel = tvData->model ();
    tvData->setModel (mod);
    if (oldModel && oldModel != mod)
        delete oldModel;
}
