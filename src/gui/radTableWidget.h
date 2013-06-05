#ifndef _RADTableWidget_H
#define _RADTableWidget_H

#include "src_config.h"

#include <QWidget>

#include <complex>

class QTableView;
class QAbstractItemModel;

using std::complex;

class  _FFTMATR_EXPORT_ radTableWidget : public QWidget
{
public:
    radTableWidget (const complex<double> * zVec, int n, int m, QWidget * parent=0, Qt::WindowFlags flags=0);
    radTableWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~radTableWidget (void);

    void setModel (QAbstractItemModel * mod);
private:
    //
    // Functions
    //
    QAbstractItemModel * initComplexModel (const complex<double> * zVec, int n, int m);
private:
    //
    // Variables
    //
    QTableView * tvData;

private:
    Q_OBJECT
};
#endif
