#include <QPushButton>
#include <QToolButton>
#include <QValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QCoreApplication>
#include <QThread>
#include <QtDebug>

#include "ConvDistPhys.h"
#include "constants1.h"
#include "convdistancewidget.h"
#include "radapplication.h"
#include "RadSettings.h"
#include "ui_conv_distance_widget.h"

ConvDistanceWidget :: ConvDistanceWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    UI (new Ui::conv_distance_widget)
{
    UI->setupUi (this);
    this->setWindowIcon (QIcon(":/radius/dcconv.png"));
    this->init();

    connect (UI->lELightSpeed, SIGNAL (textChanged (const QString &)), this, SLOT (calcFQuant (const QString&)) );
    connect (UI->lEDistanceStep, SIGNAL (textChanged (const QString &)), this, SLOT (calcFQuant (const QString&)) );
    connect (UI->lEImpulseDuration, SIGNAL (textChanged (const QString &)), this, SLOT (calcNumbImp (const QString&)) );
    connect (UI->tbRggFileName, SIGNAL (clicked()), this, SLOT (loadDataFile()) );
    connect (UI->tbConvDFileName, SIGNAL (clicked()), this, SLOT (setSaveFile()) );
    connect (UI->pbStart, SIGNAL (clicked()), this, SLOT (startConv()) );
    connect (UI->pbClose, SIGNAL (clicked()), this, SLOT (close()) );
}

ConvDistanceWidget :: ~ConvDistanceWidget (void)
{
    delete UI;
}

void ConvDistanceWidget :: loadDataFile (void)
{
    QString fileName = QFileDialog::getOpenFileName (this, tr("Select source file"), QDir::currentPath(), tr("All files (*)"));
    if (fileName.isEmpty())
        return;

    UI->lERggFileName->setText (fileName);
//    bool isStart (!UI->lEConvDistFileName->text().isEmpty());
//    UI->pbStart->setEnabled (isStart);
}

void ConvDistanceWidget :: setSaveFile (void)
{
    QString fileConvName = QFileDialog::getSaveFileName (this, tr("Save convolution by distance data"), QDir::currentPath(), tr("All files (*)"));
    if (fileConvName.isEmpty())
        return;

    UI->lEConvDistFileName->setText (fileConvName);
//    bool isStart (!UI->lERggFileName->text().isEmpty());
//    UI->pbStart->setEnabled (isStart);
}

void ConvDistanceWidget :: startConv (void)
{
    int ndn = UI->lEReadingsNumber->text().toInt();
    int nch = UI->lENumberOfDistanceChannels->text().toInt();
    double cl = UI->lELightSpeed->text().toDouble();
    double bw = UI->lEBandwidth->text().toDouble();
    double dnr = UI->lEDistanceStep->text().toDouble();
    double dimp = UI->lEImpulseDuration->text().toDouble()*0.1e-5;
    QString fRGGName = UI->lERggFileName->text();
    QString fConvName = UI->lEConvDistFileName->text();
    int nCal = UI->lECalibration->text().toInt();
    int nThr = UI->lEFFTThreads->text().toInt();
    QCoreApplication * app = QCoreApplication::instance ();
    if (qobject_cast<RadApplication *>(app))
    {
        RadiusSettings * rSettings = qobject_cast<RadApplication *>(app)->getRadSettings ();
        if (rSettings)
        {
            rSettings->beginGroup ("System settings");
            rSettings->writeSettings ("radius", "RGG file", fRGGName);
            rSettings->writeSettings ("radius", "Convolution file", fConvName);
            rSettings->writeSettings ("radius", "FFT Threads", QString::number (nThr));
            rSettings->endGroup();
        }
    }
    ConvDistPhysParameters * cParams = new ConvDistPhysParameters (ndn, nch, cl, bw, dnr, dimp, fRGGName, fConvName, nCal, nThr);
    qDebug () << __PRETTY_FUNCTION__ << cParams;
    emit setParams (cParams);
}

void ConvDistanceWidget :: init (void)
{
    QString fRGGName = QString ();
    QString fConvName = QString ();
    QCoreApplication * app = QCoreApplication::instance ();
    RadiusSettings * rSettings = 0;//qobject_cast<RadApplication *>(app)->getRadSettings ();
    if (qobject_cast<RadApplication *>(app))
    {
        rSettings = qobject_cast<RadApplication *>(app)->getRadSettings ();
        if (rSettings)
        {
            rSettings->beginGroup ("System settings/radius");
            fRGGName = rSettings->getParam ("RGG file");
            fConvName = rSettings->getParam ("Convolution file");
            rSettings->endGroup();
        }
    }
    UI->lERggFileName->setText (fRGGName);
    UI->lEConvDistFileName->setText (fConvName);
    UI->tbRggFileName->setIcon (QIcon (":/radius/open.png"));
    UI->tbConvDFileName->setIcon (QIcon (":/radius/save.png"));

    QValidator * calVal = new QIntValidator (this);
    UI->lECalibration->setValidator (calVal);
    UI->lECalibration->setText (QString::number (20000));

    QValidator * iReadVal = new QIntValidator (1, 100000, this);
    UI->lEReadingsNumber->setValidator (iReadVal);
    UI->lEReadingsNumber->setText (QString::number (ndn));

    QValidator * iChannelsNubVal = new QIntValidator (this);
    UI->lENumberOfDistanceChannels->setValidator (iChannelsNubVal);
    UI->lENumberOfDistanceChannels->setText (QString::number (na));

    QValidator * dBandVal = new QDoubleValidator (0, 1e10, 6,this);
    UI->lEBandwidth->setValidator (dBandVal);
    UI->lEBandwidth->setText (QString::number (fsp, 'f', 3));

    QValidator * dDnrVal = new QDoubleValidator (0.1e-15, 1e20, 12, this);
    UI->lEDistanceStep->setValidator (dDnrVal);
    UI->lEDistanceStep->setText (QString::number (dnr));

    QValidator * dLightSpVal = new QDoubleValidator (2e8, 3e8, 3, this);
    UI->lELightSpeed->setValidator (dLightSpVal);
    UI->lELightSpeed->setText (QString::number (c, 'f', 3));

    UI->lEQuantizationFrequency->setText (QString::number (fcvant2, 'f', 3));

    QValidator * dImpVal = new QDoubleValidator (this);
    UI->lEImpulseDuration->setValidator (dImpVal);
    UI->lEImpulseDuration->setText (QString::number (dimp*1e6));

    UI->lENumberOfReadingsInImpulse->setText (QString::number (N1));

    QValidator * fftVal = new QIntValidator (1, 100, this);
    UI->lEFFTThreads->setValidator (fftVal);
    int nthreads = 1;
    if (rSettings)
    {
        rSettings->beginGroup ("System settings/radius");
        nthreads = rSettings->getParam ("FFT Threads").toInt();
    }
    else
        nthreads = QThread::idealThreadCount ();
    UI->lEFFTThreads->setText (QString::number (nthreads));
    UI->lEFFTThreads->setToolTip (tr("Number of FFT threads"));

//    UI->pbStart->setEnabled (true);
}

void ConvDistanceWidget :: closeEvent(QCloseEvent *event)
{
    QWidget * pWidget = parentWidget();
    event->accept();
    if (pWidget)
        pWidget->close();
}

void ConvDistanceWidget :: calcFQuant (const QString& text)
{
    double dnrw;
    double clight;
    if (qobject_cast<QLineEdit *>(this->sender()) == UI->lEDistanceStep)
    {
        QString dnrText (text);
        int pos (0);
        if (UI->lEDistanceStep->validator() && UI->lEDistanceStep->validator()->validate(dnrText, pos) == QValidator::Intermediate)
            return;
        bool ok;
        dnrw = text.toDouble (&ok);
        if (!ok)
        {
            QMessageBox::warning (this, tr("Set parameters"), tr ("Incorrect parameters"), QMessageBox::Ok);
            return;
        }
        if (qAbs (dnrw) < 0.1e-15)
            return;
        clight = UI->lELightSpeed->text().toDouble (&ok);
        if (!ok)
        {
            QMessageBox::warning (this, tr("Set parameters"), tr ("Incorrect parameters"), QMessageBox::Ok);
            return;
        }
    }
    else
    {
        bool ok;
        dnrw = UI->lEDistanceStep->text().toDouble (&ok);
        if (!ok || qAbs (dnrw) < 0.1e-15)
        {
            QMessageBox::warning (this, tr("Set parameters"), tr ("Incorrect parameters"), QMessageBox::Ok);
            return;
        }
        clight = text.toDouble (&ok);
        if (!ok)
        {
            QMessageBox::warning (this, tr("Set parameters"), tr ("Incorrect parameters"), QMessageBox::Ok);
            return;
        }
    }

    UI->lEQuantizationFrequency->setText (QString::number (clight/(2*dnrw), 'f', 3));
}

void ConvDistanceWidget :: calcNumbImp (const QString& text)
{
    bool ok;
    double Dimp = text.toDouble (&ok);
    if (!ok)
    {
        QMessageBox::warning (this, tr("Set parameters"), tr ("Incorrect parameters"), QMessageBox::Ok);
        return;
    }
    Dimp *= 0.1e-5;
    double fQuant2 = UI->lEQuantizationFrequency->text().toDouble();
    int NImp = (int)(Dimp*fQuant2)+1;
    UI->lENumberOfReadingsInImpulse->setText (QString::number (NImp));
}
