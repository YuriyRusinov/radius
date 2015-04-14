#ifndef _GolographicDelegate_H
#define _GolographicDelegate_H

#include <QStyledItemDelegate>

#include "src_config.h"

class _FFTMATR_EXPORT_ GolographicDelegate : public QStyledItemDelegate
{
public:
    GolographicDelegate (QObject * parent=0);
    virtual ~GolographicDelegate (void);

    void paint (QPainter * painter, const QStyleOptionViewItem &option,
                const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                    const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

private:
    Q_OBJECT
};

#endif
