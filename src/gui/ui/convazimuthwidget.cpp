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
#include <QtDebug>

#include "constants1.h"
#include "ConvAzimuthPhys.h"
#include "convazimuthwidget.h"
#include "radapplication.h"
#include "RadSettings.h"
#include "ui_conv_azimuth_widget.h"

ConvAzimuthWidget :: ConvAzimuthWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    UI (new Ui::conv_azimuth_widget)
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

ConvAzimuthWidget :: ~ConvAzimuthWidget (void)
{
    delete UI;
}

void ConvAzimuthWidget :: loadDataFile (void)
{
    QString fileName = QFileDialog::getOpenFileName (this, tr("Select source file"), QDir::currentPath(), tr("All files (*)"));
    if (fileName.isEmpty())
        return;

    UI->lERggFileName->setText (fileName);
    UI->pbStart->setEnabled (true);
}

void ConvAzimuthWidget :: setSaveFile (void)
{
    QString fileConvName = QFileDialog::getSaveFileName (this, tr("Save convolution image by azimuth data"), QDir::currentPath(), tr("All files (*)"));
    if (fileConvName.isEmpty())
        return;

    UI->lEConvDistFileName->setText (fileConvName);
}

void ConvAzimuthWidget :: startConv (void)
{
    int nfft = UI->lEFFT->text().toInt();
    int ndc = UI->lEImageCenter->text().toInt();
    int ndim = UI->lEImageSize->text().toInt();
    int nshift = UI->lEShift->text().toInt();
    double c = UI->lELightSpeed->text().toDouble();
    double r = UI->lEHorizontalComponent->text().toDouble();
    double h = UI->lEVerticalComponent->text().toDouble();
    double bw = UI->lEBandwidth->text().toDouble();
    double dnr = UI->lEDistanceStep->text().toDouble();
    double dimp = UI->lEImpulseDuration->text().toDouble();
    double aStep = UI->lEAzimuthStep->text().toDouble();
    double alamb = UI->lEAlamb->text().toDouble();
    int nCal = UI->lECalibration->text().toInt();
    QString fName = UI->lERggFileName->text();
    QString fConvName = UI->lEConvDistFileName->text();
    double sc = UI->lEScaleFactor->text().toDouble();
    double off = UI->lEOffset->text().toDouble();
    Qt::CheckState chLog = UI->chLogarithm->checkState ();
    bool isLog = (chLog == Qt::Checked);
    QCoreApplication * app = QCoreApplication::instance ();
    if (qobject_cast<RadApplication *>(app))
    {
        RadiusSettings * rSettings = qobject_cast<RadApplication *>(app)->getRadSettings ();
        if (rSettings)
        {
            rSettings->beginGroup ("System settings");
            rSettings->writeSettings ("radius", "Convolution file", fName);
            rSettings->writeSettings ("radius", "Image file", fConvName);
            rSettings->endGroup();
        }
    }
    ConvAzimuthPhysParameters * cParams = new ConvAzimuthPhysParameters (nfft, ndc, ndim, nshift, c, r, h, bw, dnr, dimp, aStep, alamb, fName, fConvName, nCal, sc, off, isLog);
    emit sendParameters (cParams);
}

void ConvAzimuthWidget :: calcFQuant (const QString& text)
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

void ConvAzimuthWidget :: calcNumbImp (const QString& text)
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

void ConvAzimuthWidget :: init (void)
{
    QString fConvName = QString ();
    QString fImageName =  QString ();
    QCoreApplication * app = QCoreApplication::instance ();
    if (qobject_cast<RadApplication *>(app))
    {
        RadiusSettings * rSettings = qobject_cast<RadApplication *>(app)->getRadSettings ();
        if (rSettings)
        {
            rSettings->beginGroup ("System settings/radius");
            fConvName = rSettings->getParam ("Convolution file");
            fImageName = rSettings->getParam ("Image file");
            rSettings->endGroup();
        }
    }
    UI->lERggFileName->setText (fConvName);
    UI->lEConvDistFileName->setText (fImageName);
    UI->tbRggFileName->setIcon (QIcon (":/radius/open.png"));
    UI->tbConvDFileName->setIcon (QIcon (":/radius/save.png"));

    QValidator * calVal = new QIntValidator (this);
    UI->lECalibration->setValidator (calVal);
    UI->lECalibration->setText (QString::number (1));

    QValidator * imCenterVal = new QIntValidator (1, na2, this);
    UI->lEImageCenter->setValidator (imCenterVal);
    UI->lEImageCenter->setText (QString::number (ndcentre));

    QValidator * imDimVal = new QIntValidator (1, na2, this);
    UI->lEImageSize->setValidator (imDimVal);
    UI->lEImageSize->setText (QString::number (ndrazmer));

    QValidator * nFFTVal = new QIntValidator (1, 2*na2, this);
    UI->lEFFT->setValidator (nFFTVal);
    UI->lEFFT->setText (QString::number (na2));

    QValidator * shiftVal = new QIntValidator (0, 2*na2, this);
    UI->lEShift->setValidator (shiftVal);
    UI->lEShift->setText (QString::number (na_ots));
    QValidator * dBandVal = new QDoubleValidator (0, 1e10, 6,this);
    UI->lEBandwidth->setValidator (dBandVal);
    UI->lEBandwidth->setText (QString::number (fsp, 'f', 3));

    QValidator * dDnrVal = new QDoubleValidator (0.1e-15, 1e20, 12, this);
    UI->lEDistanceStep->setValidator (dDnrVal);
    UI->lEDistanceStep->setText (QString::number (dnr));

    QValidator * dLightSpVal = new QDoubleValidator (2e8, 3e8, 3, this);
    UI->lELightSpeed->setValidator (dLightSpVal);
    UI->lELightSpeed->setText (QString::number (c, 'f', 3));

    QValidator * dRVal = new QDoubleValidator (0, 1e4, 3, this);
    UI->lEHorizontalComponent->setValidator (dRVal);
    UI->lEHorizontalComponent->setText (QString::number (R, 'f', 3));

    QValidator * dHVal = new QDoubleValidator (0, 1e4, 3, this);
    UI->lEVerticalComponent->setValidator (dHVal);
    UI->lEVerticalComponent->setText (QString::number (H, 'f', 3));

    UI->lEQuantizationFrequency->setText (QString::number (fcvant2, 'f', 3));

    QValidator * dImpVal = new QDoubleValidator (this);
    UI->lEImpulseDuration->setValidator (dImpVal);
    UI->lEImpulseDuration->setText (QString::number (dimp*1e6));

    UI->lENumberOfReadingsInImpulse->setText (QString::number (N1));

    QValidator * dAzStepVal = new QDoubleValidator (0.0, 1.0, 6, this);
    UI->lEAzimuthStep->setValidator (dAzStepVal);
    UI->lEAzimuthStep->setText (QString::number (dx, 'f', 4));

    QValidator * dLambVal = new QDoubleValidator (0.0, 1.0, 6, this);
    UI->lEAlamb->setValidator (dLambVal);
    UI->lEAlamb->setText (QString::number (lamb, 'f', 5));

    QValidator * dScaleVal = new QDoubleValidator (this);
    UI->lEScaleFactor->setValidator (dScaleVal);
    UI->lEScaleFactor->setText (QString::number (1.0, 'f', 5));

    QValidator * dOffsetVal = new QDoubleValidator (this);
    UI->lEOffset->setValidator (dOffsetVal);
    UI->lEOffset->setText (QString::number (0.0, 'f', 5));

    UI->pbStart->setEnabled (!fConvName.isEmpty());
}

void ConvAzimuthWidget :: closeEvent(QCloseEvent *event)
{
    QWidget * pWidget = parentWidget();
    event->accept();
    if (pWidget)
        pWidget->close();
}


