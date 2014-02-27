#include "ConvAzimuthPhys.h"

ConvAzimuthPhysParameters :: ConvAzimuthPhysParameters (int nfft, int ndc, int ndim, int nshift, double c, double r, double h, double bw, double dnr, double dimp, double aStep, double alamb, const QString& fileName, const QString& fileConvName, int nCal, double sc, double off)
    : nFFT (nfft),
    NdCentre (ndc),
    NdDimension (ndim),
    NShift (nshift),
    cSpeedOfLight (c),
    R (r),
    H (h),
    FreqSp (bw),
    Dnr (dnr),
    FQuant2 (c/(2*dnr)),
    Dimp (dimp),
    NImp ((int)FQuant2*Dimp+1),
    AzimuthStep (aStep),
    ALamb (alamb),
    m_fileName (fileName),
    m_fileConvName (fileConvName),
    nCalibration (nCal),
    imScale (sc),
    imOffset (off)
{
}

ConvAzimuthPhysParameters :: ConvAzimuthPhysParameters (const ConvAzimuthPhysParameters& CPars)
    : nFFT (CPars.nFFT),
    NdCentre (CPars.NdCentre),
    NdDimension (CPars.NdDimension),
    NShift (CPars.NShift),
    cSpeedOfLight (CPars.cSpeedOfLight),
    R (CPars.R),
    H (CPars.H),
    FreqSp (CPars.FreqSp),
    Dnr (CPars.Dnr),
    FQuant2 (CPars.FQuant2),
    Dimp (CPars.Dimp),
    NImp (CPars.NImp),
    AzimuthStep (CPars.AzimuthStep),
    ALamb (CPars.ALamb),
    m_fileName (CPars.m_fileName),
    m_fileConvName (CPars.m_fileConvName),
    nCalibration (CPars.nCalibration),
    imScale (CPars.imScale),
    imOffset (CPars.imOffset)
{
}

ConvAzimuthPhysParameters :: ~ConvAzimuthPhysParameters (void)
{
}

int ConvAzimuthPhysParameters :: getFFTDim (void) const
{
    return nFFT;
}

int ConvAzimuthPhysParameters :: getCenter (void) const
{
    return NdCentre;
}

int ConvAzimuthPhysParameters :: getDimension (void) const
{
    return NdDimension;
}

int ConvAzimuthPhysParameters :: getShift (void) const
{
    return NShift;
}

double ConvAzimuthPhysParameters :: cLightSpeed (void) const
{
    return cSpeedOfLight;
}

double ConvAzimuthPhysParameters :: getBandWidth (void) const
{
    return FreqSp;
}

double ConvAzimuthPhysParameters :: getDistStep (void) const
{
    return Dnr;
}

double ConvAzimuthPhysParameters :: getFQuant (void) const
{
    return FQuant2;
}

double ConvAzimuthPhysParameters :: getDimp (void) const
{
    return Dimp;
}

int ConvAzimuthPhysParameters :: getImpNumb (void) const
{
    return NImp;
}

double ConvAzimuthPhysParameters :: getAzimuthStep (void) const
{
    return AzimuthStep;
}

double ConvAzimuthPhysParameters :: getLambda (void) const
{
    return ALamb;
}

const QString& ConvAzimuthPhysParameters :: getInputFileName (void) const
{
    return m_fileName;
}

const QString& ConvAzimuthPhysParameters :: getConvFileName (void) const
{
    return m_fileConvName;
}

int ConvAzimuthPhysParameters :: getNCalibration (void) const
{
    return nCalibration;
}

double ConvAzimuthPhysParameters :: radius (void) const
{
    return R;
}

double ConvAzimuthPhysParameters :: height (void) const
{
    return H;
}

double ConvAzimuthPhysParameters :: getImScale (void) const
{
    return imScale;
}

double ConvAzimuthPhysParameters :: getImOffset (void) const
{
    return imOffset;
}
