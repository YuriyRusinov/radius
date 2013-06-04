#ifndef _CALCIM_H
#define _CALCIM_H

#include <complex>

#include "src_config.h"

using std::complex;

class _FFTMATR_EXPORT_ Calc
{
public:
    Calc ();
    virtual ~Calc ();

    virtual double* operator () (const double* st, int nd) = 0;
};

class _FFTMATR_EXPORT_ Calc1 : public Calc
{
public:
    Calc1 ();
    virtual ~Calc1 ();

    virtual double * operator () (const double* st, int nd);
};

class _FFTMATR_EXPORT_ Calc2 : public Calc
{
public:
    Calc2 ();
    virtual ~Calc2 ();

    virtual double * operator () (const double* st, int nd);
};

class _FFTMATR_EXPORT_ CalcOpor1
{
public:
    CalcOpor1 (int _nd);
    virtual ~CalcOpor1 ();

    complex<double>* calc ();
    complex<double>* data () const;

private:
    complex<double>* opor;
    int nd;
};

#endif
