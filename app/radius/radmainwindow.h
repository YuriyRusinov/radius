#ifndef _RAD_MAINWINDOW_H
#define _RAD_MAINWINDOW_H

#include <QMainWindow>

#include <complex>
#include <gologramma.h>

class QMdiArea;
class QAction;
class QToolBar;
class QMenu;
class Assistant;
class generatingDataPlus;

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
    void initConvDist (void);
    void initConvAz (void);
    void initPulsarCalc(void);

    void openDataFile (void);
    void saveImageFile (void);
    void fftTest (void);
    void stcFFTTest (void);
    void stc1FFTTest (void);
    void oporTest (void);
    void slotTest1 (void);
    void slotTest2 (void);
    void slotFFT2Test (void);
    void slot3DMod (void);
    void slot3DView (void);
    void slotBinariRLI (void);

    void slotSetings (void);
    void slotGologramCalc (generatingDataPlus gdp);

    void addWidget (QWidget * w);
    void slotHelp (void);
    void viewImages (const VGolographic& r);
    void viewHistogram (QPixmap pMap);

private:
    //
    // Functions
    //
    void init (void);
    void initToolBars (void);

private:
    //
    // Variables
    //
    Ui::Rad_Main_Window * UI;
    QMdiArea * m_mdiArea;
    Assistant * ass;
    QString fileName;
    QString fileConvName;
    QToolBar * tbActions;
    QToolBar * tbCalc;
    QToolBar * tbOthers;

    QAction * actCalc1;
    QAction * actCalc2;

    QMenu * menuFile;
    QAction * actFileMenu;
    QAction * actOpenDataFile;
    QAction * actSaveImageFile;
    QAction * actQuit;
    QMenu * calcMenu;
    QAction * actInitConvDist;
    QAction * actInitConvAz;
    QAction * actCalcPulsar;
    QMenu * RLI3DMenu;
    QAction * act3DMod;
    QAction * actView3DMod;
    QAction * act3DBinary;
    QAction * actCalcMenu;
    QMenu * settingsMenu;
    QAction * actSettings;
    QMenu * menuHelp;
    QAction * actHelp;
    QAction * actHelpRad;
    //complex<double> * stc;// = new complex<double> [nd];
    //complex<double> * stc1;/// = new complex<double> [nd];
    //double * stc2;
    //complex<double> * stc3;
    //complex<double> * stc4;// = new complex<double> [nd];
private:
    Q_OBJECT
};

#endif
