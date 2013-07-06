#include <QPushButton>
#include <QToolButton>
#include <QValidator>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtDebug>
#include "constants1.h"
#include "convdistancewidget.h"
#include "ui_conv_distance_widget.h"

ConvDistanceWidget :: ConvDistanceWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    UI (new Ui::conv_distance_widget)
{
    UI->setupUi (this);
    this->init();

    connect (UI->lELightSpeed, SIGNAL (textChanged (const QString &)), this, SLOT (calcFQuant (const QString&)) );
    connect (UI->lEDistanceStep, SIGNAL (textChanged (const QString &)), this, SLOT (calcFQuant (const QString&)) );
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
}

void ConvDistanceWidget :: setSaveFile (void)
{
}

void ConvDistanceWidget :: startConv (void)
{
}

void ConvDistanceWidget :: init (void)
{
    QValidator * calVal = new QIntValidator (this);
    UI->lECalibration->setValidator (calVal);
    UI->lECalibration->setText (QString::number (20000));

    QValidator * iReadVal = new QIntValidator (this);
    UI->lEReadingsNumber->setValidator (iReadVal);
    UI->lEReadingsNumber->setText (QString::number (ndn));

    QValidator * iChannelsNubVal = new QIntValidator (this);
    UI->lENumberOfDistanceChannels->setValidator (iChannelsNubVal);
    UI->lENumberOfDistanceChannels->setText (QString::number (na));

    QValidator * dBandVal = new QDoubleValidator (this);
    UI->lEBandwidth->setValidator (dBandVal);
    UI->lEBandwidth->setText (QString::number (fsp));

    QValidator * dDnrVal = new QDoubleValidator (0.1e-15, 1e20, 12, this);
    UI->lEDistanceStep->setValidator (dDnrVal);
    UI->lEDistanceStep->setText (QString::number (dnr));

    QValidator * dLightSpVal = new QDoubleValidator (this);
    UI->lELightSpeed->setValidator (dLightSpVal);
    UI->lELightSpeed->setText (QString::number (c));

    UI->lEQuantizationFrequency->setText (QString::number (fcvant2));

    QValidator * dImpVal = new QDoubleValidator (this);
    UI->lEImpulseDuration->setValidator (dImpVal);
    UI->lEImpulseDuration->setText (QString::number (dimp*1e6));

    UI->lENumberOfReadingsInImpulse->setText (QString::number (N1));

    UI->pbStart->setEnabled (false);
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

    UI->lEQuantizationFrequency->setText (QString::number (clight/(2*dnrw)));
}
