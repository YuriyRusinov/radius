#ifndef _ConvDistPhysPars_H
#define _ConvDistPhysPars_H

#include <QString>

#include "src_config.h"

class _FFTMATR_EXPORT_ ConvDistPhysParameters
{
public:
    ConvDistPhysParameters (int ndn, int nch, double c, double bw, double dnr, double dimp, const QString& fileName, const QString& fileConvName, int nCal);
    ConvDistPhysParameters (const ConvDistPhysParameters& CPars);
    ~ConvDistPhysParameters (void);

    int getNRChannels (void) const;
    int getChannelsNumb (void) const;
    double cLightSpeed (void) const;
    double getBandWidth (void) const;
    double getDistStep (void) const;
    double getFQuant (void) const;
    double getDImp (void) const;
    int getImpNumb (void) const;

    const QString& getInputFileName (void) const;
    const QString& getConvFileName (void) const;

    int getNCalibration (void) const;
private:
    //
    // Variables
    //
    int Ndn;
    int Nd2;
    int NChannels;
    double cSpeedOfLight;
    double FreqSp;
    double Dnr;
    double FQuant2;
    double Dimp;
    int NImp;

    QString fileName;
    QString fileConvName;

    int nCalibration;
};

#endif
