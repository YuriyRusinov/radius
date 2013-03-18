#ifndef _FFTW_C_H
#define _FFTW_C_H

#include <complex>
#include <math.h>
#include <fftw3.h>

#include "src_config.h"

using std::complex;

class _FFTMATR_EXPORT_ FFT_Transform 
{
public:
    FFT_Transform (void);
    virtual ~FFT_Transform (void);
    virtual complex<long double> * operator () (complex<long double> * src, int nsrc, int n2, int sign, unsigned flags);
};

class _FFTMATR_EXPORT_ FFT2_Transform
{
public:
    FFT2_Transform (void);
    virtual ~FFT2_Transform (void);
    virtual complex<long double> * operator () (complex<long double> * src, int nr, int nc, int sign, unsigned flags);

};

#endif
