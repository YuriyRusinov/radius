#ifndef _ConvAzimuthPhysPars_H
#define _ConvAzimuthPhysPars_H

#include <QString>

#include "src_config.h"

class _FFTMATR_EXPORT_ ConvAzimuthPhysParameters
{
public:
    ConvAzimuthPhysParameters (int nfft, int ndc, int ndim, int nshift, double c, double r, double h, double bw, double dnr, double dimp, double aStep, double alamb, const QString& fileName, const QString& fileConvName, int nCal, double sc, double off);
    ConvAzimuthPhysParameters (const ConvAzimuthPhysParameters& CPars);
    ~ConvAzimuthPhysParameters (void);

    int getFFTDim (void) const;
    int getCenter (void) const;
    int getDimension (void) const;
    int getShift (void) const;
    double cLightSpeed (void) const;
    double radius (void) const;
    double height (void) const;
    double getBandWidth (void) const;
    double getDistStep (void) const;
    double getFQuant (void) const;
    double getDimp (void) const;
    int getImpNumb (void) const;
    double getAzimuthStep (void) const;
    double getLambda (void) const;

    const QString& getInputFileName (void) const;
    const QString& getConvFileName (void) const;

    int getNCalibration (void) const;
    double getImScale (void) const;
    double getImOffset (void) const;

private:
    //
    // Variables
    //
    int nFFT;
    int NdCentre;
    int NdDimension;
    int NShift;
    double cSpeedOfLight;
    double R;
    double H;
    double FreqSp;
    double Dnr;
    double FQuant2;
    double Dimp;
    int NImp;
    double AzimuthStep;
    double ALamb;

    QString m_fileName;
    QString m_fileConvName;

    int nCalibration;
    double imScale;
    double imOffset;
};

#endif
