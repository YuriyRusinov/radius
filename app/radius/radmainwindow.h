#ifndef _RAD_MAINWINDOW_H
#define _RAD_MAINWINDOW_H

#include <QMainWindow>

class QMdiArea;

namespace Ui
{
    class Rad_Main_Window; 
};

class RadMainWindow : public QMainWindow
{
public:
    RadMainWindow (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~RadMainWindow (void);

private slots:
    void openDataFile (void);

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
private:
    Q_OBJECT
};

#endif
