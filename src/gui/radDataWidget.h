#ifndef _RADDataWidget_H
#define _RADDataWidget_H

#include "src_config.h"

#include <QWidget>

class QTreeView;
class QAbstractItemModel;

class  _FFTMATR_EXPORT_ radDataWidget : public QWidget
{
public:
    radDataWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~radDataWidget (void);

    void setModel (QAbstractItemModel * mod);
private:
    //
    // Variables
    //
    QTreeView * tvData;

private:
    Q_OBJECT
};
#endif
