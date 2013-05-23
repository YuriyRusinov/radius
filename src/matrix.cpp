#include <stdexcept>

#include "matrix.h"

CMatrix :: CMatrix (int nrows, int ncols) :
    nRows (nrows),
    nCols (ncols),
    matr (new complex<double> [nrows*ncols])
{
}

CMatrix :: CMatrix (double value, int nrows, int ncols) :
    nRows (nrows),
    nCols (ncols),
    matr (new complex<double> [nrows*ncols])
{
    for (int i=0; i<nrows*ncols; i++)
        matr[i] = complex<double>(value, 0.0);
}

CMatrix :: CMatrix (complex<double> cval, int nrows, int ncols) :
    nRows (nrows),
    nCols (ncols),
    matr (new complex<double> [nrows*ncols])
{
    for (int i=0; i<nrows*ncols; i++)
        matr[i] = cval;
}

CMatrix :: ~CMatrix (void)
{
    delete [] matr;
}

complex<double>& CMatrix :: operator() (int i, int j)
{
    if (i<0 || i>=nRows || j<0 || j>=nCols)
        throw std::out_of_range ("Invalid indices");

    return matr[i*nRows+j];
}

const complex<double>& CMatrix :: operator() (int i, int j) const
{
    if (i<0 || i>=nRows || j<0 || j>=nCols)
        throw std::out_of_range ("Invalid indices");

    return matr[i*nRows+j];
}

complex<double> * CMatrix :: getData (void) const
{
    return matr;
}
