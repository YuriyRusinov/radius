#include "ConvDistPhys.h"

ConvDistPhysParameters :: ConvDistPhysParameters (int ndn, int nch, double c, double bw, double dnr, double dimp, const QString& cfileName, const QString& fConvName, int nCal)
    : Ndn (ndn),
    Nd2 (2*ndn),
    NChannels (nch),
    cSpeedOfLight (c),
    FreqSp (bw),
    Dnr (dnr),
    FQuant2 (c/(2*dnr)),
    Dimp (dimp),
    NImp ((int)FQuant2*Dimp+1),
    fileName (cfileName),
    fileConvName (fConvName),
    nCalibration (nCal)
{
}

ConvDistPhysParameters :: ConvDistPhysParameters (const ConvDistPhysParameters& CPars)
    : Ndn (CPars.Ndn),
    Nd2 (CPars.Nd2),
    NChannels (CPars.NChannels),
    cSpeedOfLight (CPars.cSpeedOfLight),
    FreqSp (CPars.FreqSp),
    Dnr (CPars.Dnr),
    FQuant2 (CPars.FQuant2),
    Dimp (CPars.Dimp),
    NImp (CPars.NImp),
    fileName (CPars.fileName),
    fileConvName (CPars.fileConvName),
    nCalibration (CPars.nCalibration)
{
}

ConvDistPhysParameters :: ~ConvDistPhysParameters (void)
{
}

int ConvDistPhysParameters :: getNRChannels (void) const
{
    return Ndn;
}

int ConvDistPhysParameters :: getChannelsNumb (void) const
{
    return NChannels;
}

double ConvDistPhysParameters :: cLightSpeed (void) const
{
    return cSpeedOfLight;
}

double ConvDistPhysParameters :: getBandWidth (void) const
{
    return FreqSp;
}

double ConvDistPhysParameters :: getDistStep (void) const
{
    return Dnr;
}

double ConvDistPhysParameters :: getFQuant (void) const
{
    return FQuant2;
}

double ConvDistPhysParameters :: getDImp (void) const
{
    return Dimp;
}

int ConvDistPhysParameters :: getImpNumb (void) const
{
    return NImp;
}

const QString& ConvDistPhysParameters :: getInputFileName (void) const
{
    return fileName;
}

const QString& ConvDistPhysParameters :: getConvFileName (void) const
{
    return fileConvName;
}


int ConvDistPhysParameters :: getNCalibration (void) const
{
    return nCalibration;
}
