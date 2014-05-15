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
    CMatrix (complex<double> cval, int nrows, int ncols);
    CMatrix (complex<double> * mData, int nrows, int ncols);
    CMatrix (const CMatrix& CM);
    ~CMatrix (void) throw();

    int rowCount (void) const;
    int columnCount (void) const;

    complex<double>& operator() (int i, int j) throw();
    const complex<double>& operator() (int i, int j) const throw();
    complex<double> * getData (void) const;

    void setData (int i, int j, complex<double> cval) throw();
    void setData (complex<double> * mData) throw();

    CMatrix& operator+= (const CMatrix& CM);
    CMatrix& operator-= (const CMatrix& CM);

    CMatrix& operator*= (double alamb);
    CMatrix& operator/= (double alamb);
private:
    int nRows;
    int nCols;
    complex<double> * matr;
};

#endif
