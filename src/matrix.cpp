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

CMatrix :: CMatrix (complex<double> * mData, int nrows, int ncols) :
    nRows (nrows),
    nCols (ncols),
    matr (mData)
{
}

CMatrix :: CMatrix (const CMatrix& CM) :
    nRows (CM.nRows),
    nCols (CM.nCols),
    matr (new complex<double> [CM.nRows*CM.nCols])
{
    for (int i=0; i<nRows*nCols; i++)
        matr[i] = CM.matr[i];
}

CMatrix :: ~CMatrix (void) throw()
{
    delete [] matr;
}

complex<double>& CMatrix :: operator() (int i, int j) throw()
{
    if (i<0 || i>=nRows || j<0 || j>=nCols)
    {
        throw std::out_of_range ("Invalid indices");
    }

    return matr[i*nCols+j];
}

const complex<double>& CMatrix :: operator() (int i, int j) const throw()
{
    if (i<0 || i>=nRows || j<0 || j>=nCols)
        throw std::out_of_range ("Invalid indices");

    return matr[i*nCols+j];
}

complex<double> * CMatrix :: getData (void) const
{
    return matr;
}

void CMatrix :: setData (int i, int j, complex<double> cval) throw()
{
    if (i<0 || i>=nRows || j<0 || j>=nCols)
        throw std::out_of_range ("Invalid indices");
    matr[i*nCols+j] = cval;
}

void CMatrix :: setData (complex<double> * mData) throw()
{
    matr = mData;
}

int CMatrix :: rowCount (void) const
{
    return nRows;
}

int CMatrix :: columnCount (void) const
{
    return nCols;
}

CMatrix& CMatrix :: operator+= (const CMatrix& CM)
{
    if (nRows != CM.nRows || nCols != CM.nCols)
        throw std::logic_error ("Invalid sizes");

    for (int i=0; i<nRows*nCols; i++)
        matr[i] += CM.matr[i];

    return *this;
}

CMatrix& CMatrix :: operator-= (const CMatrix& CM)
{
    if (nRows != CM.nRows || nCols != CM.nCols)
        throw std::logic_error ("Invalid sizes");

    for (int i=0; i<nRows*nCols; i++)
        matr[i] -= CM.matr[i];

    return *this;
}

CMatrix& CMatrix :: operator*= (double alamb)
{
    for (int i=0; i<nRows*nCols; i++)
        matr[i] *= alamb;

    return *this;
}

CMatrix& CMatrix :: operator/= (double alamb)
{
    for (int i=0; i<nRows*nCols; i++)
        matr[i] /= alamb;

    return *this;
}
