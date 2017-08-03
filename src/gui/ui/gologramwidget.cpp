#include <QFileDialog>
#include <QStringList>
#include <QtDebug>

#include "gologramwidget.h"
#include "ui_gologram_widget.h"

GologramWidget :: GologramWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    UI (new Ui::gologram_widget)
{
    UI->setupUi (this);

    connect (UI->tbLoad, SIGNAL (clicked()), this, SLOT (setCSVFile()) );
    connect (UI->tbOutputFile, SIGNAL (clicked()), this, SLOT (setImageFile()) );
    connect (UI->pbStart, SIGNAL (clicked()), this, SLOT (startCalculations()) );
    connect (UI->pbClose, SIGNAL (clicked()), this, SLOT (close ()) );
}

GologramWidget :: ~GologramWidget (void)
{
    delete UI;
}

void GologramWidget :: setCSVFile (void)
{
    QString filters;
    filters = tr("CSV files (*.csv);;All files (*)");
    csvFile = QFileDialog::getOpenFileName (this, tr("Select source file"), QDir::currentPath(), filters);
    if (csvFile.isEmpty())
        return;
    UI->lEFileName->setText (csvFile);
}

void GologramWidget :: setImageFile (void)
{
    QString filters;
    filters = tr("Image files (*.xpm *.png *.ico *.jpg *.jpeg *.bmp *.gif *.pbm *.pgm *.xbm);;All files (*)");
    outputFile = QFileDialog::getSaveFileName (this, tr("Save image"), QDir::currentPath(), filters);
    if (outputFile.isEmpty())
        return;
    UI->lEImageFileName->setText (outputFile);
}

void GologramWidget :: startCalculations (void)
{
}
