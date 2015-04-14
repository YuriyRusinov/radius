#include <QPixmap>
#include <QPainter>
#include <QtDebug>
#include "golographicDelegate.h"

GolographicDelegate :: GolographicDelegate (QObject * parent)
    : QStyledItemDelegate (parent)
{
}

GolographicDelegate :: ~GolographicDelegate (void)
{
}

void GolographicDelegate :: paint (QPainter * painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    if (index.column () == 2)
    {
        QPixmap pMap = index.data (Qt::DecorationRole).value<QPixmap>();
        QRect r = option.rect;
        QPoint p = QPoint ((r.x()+r.width()/2-pMap.width()/2), (r.y()+r.height()/2-pMap.height()/2) );
        if (option.state & QStyle::State_Selected)
             painter->fillRect(option.rect, option.palette.highlight());
        painter->drawPixmap (p, pMap);
    }
    else
        QStyledItemDelegate::paint (painter, option, index);
}

QSize GolographicDelegate :: sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    if (index.column () == 2)
    {
        QPixmap pMap = index.data (Qt::DecorationRole).value<QPixmap>();
        return pMap.size ();
    }
    else
    {
        QSize w = QStyledItemDelegate::sizeHint (option, index);
        qDebug () << __PRETTY_FUNCTION__ << w;
        return w;
    }
}

QWidget * GolographicDelegate :: createEditor (QWidget * parent,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const
{
    Q_UNUSED (parent);
    Q_UNUSED (option);
    Q_UNUSED (index);
    return 0;
}
