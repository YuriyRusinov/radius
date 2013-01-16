#ifndef _CONST1_H
#define _CONST1_H

const int ndn = 11776;
const int nd2 = 2*ndn;
const int nd = 16384;
const int na = 100000;
const double c = 0.3e9;
const double fsp = 0.3e9;
const double dnr = 0.375;
const double fcvant2 = c/(0.2e1*dnr);
const double dimp = 10.0*0.1e-6;
const int N1 = (int)(dimp*fcvant2);
const double pi (3.14159265358979);
const int ndcentre (6178);
const int ndrazmer (256);

const int ndv = ndcentre - ndrazmer/2;
const int ndn2 = ndcentre + ndrazmer/2;

const int ndrz = ndn2 - ndv;

const int na2 (16384);
const int nas (8192);
const int na_ots (3000);

const long double R (5396);
const long double H (3964);

const long double dx (0.058);
const long double lamp (0.03450);

#endif
