#include <QAbstractItemModel>
#include <QTableView>
#include <QGridLayout>
#include <QHeaderView>

#include "golographicDelegate.h"
#include "golographicWidget.h"

GolographicWidget :: GolographicWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tableV (new QTableView)
{
    QGridLayout * gLay = new QGridLayout (this);
    QHeaderView * hv = tableV->horizontalHeader ();
    hv->setStretchLastSection (true);
    gLay->addWidget (tableV);
    QAbstractItemDelegate * gDeleg = new GolographicDelegate (this);
    tableV->setItemDelegate (gDeleg);
}

GolographicWidget :: ~GolographicWidget (void)
{
    tableV->setParent (0);
    delete tableV;
}

void GolographicWidget :: setModel (QAbstractItemModel * sMod)
{
    QAbstractItemModel * oldMod = tableV->model ();
    tableV->setModel (sMod);
    if (oldMod && oldMod != sMod)
        delete oldMod;
}
