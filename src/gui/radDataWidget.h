#ifndef _RADDataWidget_H
#define _RADDataWidget_H

#include "src_config.h"

#include <QWidget>

#include <complex>

class QTreeView;
class QAbstractItemModel;

using std::complex;

class  _FFTMATR_EXPORT_ radDataWidget : public QWidget
{
public:
    radDataWidget (const complex<double> * zVec, int n, QWidget * parent=0, Qt::WindowFlags flags=0);
    radDataWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~radDataWidget (void);

    void setModel (QAbstractItemModel * mod);
private:
    //
    // Functions
    //
    QAbstractItemModel * initComplexModel (const complex<double> * zVec, int n);
private:
    //
    // Variables
    //
    QTreeView * tvData;

private:
    Q_OBJECT
};
#endif
