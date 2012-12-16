#ifndef _MATRIX_H
#define _MATRIX_H

#include <complex>
#include <exception>

#include "src_config.h"

using std::complex;
using std::exception;

class _FFTMATR_EXPORT_ CMatrix
{
public:
    CMatrix (int nrows=0, int ncols=0);
    CMatrix (double value, int nrows, int ncols);
    CMatrix (complex<long double> cval, int nrows, int ncols);
    ~CMatrix (void);

    complex<long double>& operator() (int i, int j);
    const complex<long double>& operator() (int i, int j) const;
    complex<long double> * getData (void) const;
private:
    int nRows;
    int nCols;
    complex<long double> * matr;
};

#endif
