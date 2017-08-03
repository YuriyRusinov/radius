#ifndef _GologramWidget_H
#define _GologramWidget_H

#include <QWidget>

#include "src_config.h"

namespace Ui
{
    class gologram_widget;
};

class _FFTMATR_EXPORT_ GologramWidget : public QWidget
{
public:
    GologramWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~GologramWidget (void);

private slots:
    void setCSVFile (void);
    void setImageFile (void);
    void startCalculations (void);

signals:
    void sendParams ();
private:
    Ui::gologram_widget * UI;
    QString csvFile;
    QString outputFile;

private:
    Q_OBJECT
};

#endif
