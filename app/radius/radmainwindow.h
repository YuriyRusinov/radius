#ifndef _RAD_MAINWINDOW_H
#define _RAD_MAINWINDOW_H

#include <QMainWindow>

#include <complex>

class QMdiArea;
class QAction;

namespace Ui
{
    class Rad_Main_Window; 
};

using std::complex;

class RadMainWindow : public QMainWindow
{
public:
    RadMainWindow (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~RadMainWindow (void);

private slots:
    void openDataFile (void);
    void openConvFile (void);
    void fftTest (void);
    void oporTest (void);
    void slotTest1 (void);
    void slotTest2 (void);

private:
    //
    // Functions
    //
    void init (void);

private:
    //
    // Variables
    //
    Ui::Rad_Main_Window * UI;
    QMdiArea * m_mdiArea;
    QString fileName;
    QString fileConvName;

    QAction * actCalc1;
    QAction * actCalc2;

    complex<double> * stc;// = new complex<double> [nd];
    complex<double> * stc1;/// = new complex<double> [nd];
    double * stc2;
    complex<double> * stc3;
    complex<double> * stc4;// = new complex<double> [nd];
private:
    Q_OBJECT
};

#endif
