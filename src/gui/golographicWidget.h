#ifndef _Golographic_Widget_H
#define _Golographic_Widget_H

#include <QWidget>
#include "src_config.h"

class QTableView;
class QAbstractItemModel;

class _FFTMATR_EXPORT_ GolographicWidget : public QWidget
{
public:
    GolographicWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~GolographicWidget (void);

    void setModel (QAbstractItemModel * sMod);
private:
    //
    // Variables
    //
    QTableView * tableV;

private:
    Q_OBJECT
};

#endif
