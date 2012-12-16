#include "fft_c.h"

FFT_Transform :: FFT_Transform (void)
{
}

FFT_Transform :: ~FFT_Transform (void)
{
}

complex<long double> * FFT_Transform :: operator () (complex<long double> * src, int nsrc, int n2, int sign, unsigned flags)
{
    complex<long double> * res = new complex<long double> [n2];
    complex<long double> * in = new complex<long double> [n2];

    if (nsrc > n2)
    {
        delete [] res;
        delete [] in;
        return 0;
    }
    for (int i=0; i<nsrc; i++)
        in[i] = src[i];

    for (int i=nsrc; i<n2; i++)
        in[i] = complex<long double> (0.0, 0.0);

    fftwl_plan p;
    //fftwl_complex * out = (fftwl_complex*) fftwl_malloc (sizeof(fftwl_complex) * n2);
    p = fftwl_plan_dft_1d (n2, reinterpret_cast<fftwl_complex*>(in), reinterpret_cast<fftwl_complex*>(res), sign, flags);
    fftwl_execute (p);
    fftwl_destroy_plan (p);
    return res;
}

FFT2_Transform :: FFT2_Transform (void)
{
}

FFT2_Transform :: ~FFT2_Transform (void)
{
}

complex<long double> * FFT2_Transform :: operator () (complex<long double> * src, int nr, int nc, int sign, unsigned flags)
{
    complex<long double> * res = new complex<long double> [nr*nc];

    complex<long double> * in = new complex<long double> [nr*nc];

    for (int i=0; i<nr*nc; i++)
        in[i] = src[i];


    fftwl_plan p;
    //fftwl_complex * out = (fftwl_complex*) fftwl_malloc (sizeof(fftwl_complex) * n2);
    p = fftwl_plan_dft_2d (nr, nc, reinterpret_cast<fftwl_complex*>(in), reinterpret_cast<fftwl_complex*>(res), sign, flags);
    fftwl_execute (p);
    fftwl_destroy_plan (p);
    return res;
}
