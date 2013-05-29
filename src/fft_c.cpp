#include <QtDebug>

#include "fft_c.h"

FFT_Transform :: FFT_Transform (void)
{
}

FFT_Transform :: ~FFT_Transform (void)
{
}

complex<double> * FFT_Transform :: operator () (complex<double> * src, int nsrc, int n2, int sign, unsigned flags)
{
    fftw_complex * in;
    fftw_complex * out;
    fftw_plan p;
    if (n2 <=0 || nsrc > n2)
        return 0;
    complex<double> * res = new complex<double> [n2];
    in = (fftw_complex*) fftw_malloc (sizeof(fftw_complex) * n2);

    for (int i=0; i<nsrc; i++)
    {
        in[i][0] = real (src[i]);
        in[i][1] = imag (src[i]);
    }

    for (int i=nsrc; i<n2; i++)
    {
        in[i][0] = 0.0;
        in[i][1] = 0.0;
    }

    out = (fftw_complex*) fftw_malloc (sizeof(fftw_complex) * n2);
    p = fftw_plan_dft_1d (n2, in, out, sign, flags);//fftw_plan_dft_r2c_1d(n2, in, out, flags);
    fftw_execute (p);
    for (int i=0; i<n2; i++)
    {
        if (sign == FFTW_FORWARD)
            res[i] = complex<double> (out[i][0]/n2, out[i][1]/n2);
        else
            res[i] = complex<double> (out[i][0], out[i][1]);
        //qDebug () << __PRETTY_FUNCTION__ << (double)real(res[i]) << (double)imag(res[i]);
    }
    fftw_destroy_plan (p);
    fftw_free(out);
    fftw_free(in);
    //delete [] in;
    return res;
}

int FFT_Transform :: pow2roundup (int n)
{
    int x(n);
    if (x < 0)
        return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}

FFT_RealTransform :: FFT_RealTransform (void)
{
}

FFT_RealTransform :: ~FFT_RealTransform (void)
{
}

complex<double> * FFT_RealTransform :: operator () (double * src, int nsrc, int n2, int sign, unsigned flags)
{
    double * in;
    fftw_complex * out;
    fftw_plan p;
    if (n2 <=0 || nsrc > n2)
        return 0;
    complex<double> * res = new complex<double> [n2];
    //complex<double> * in = new complex<double> [n2];
    in = (double*) fftw_malloc (sizeof(double) * n2);
    //(fftw_complex*) fftw_malloc (sizeof(fftw_complex) * n2);
    //new double [n2];//(double*) fftw_malloc (sizeof(double) * n2);

    for (int i=0; i<nsrc; i++)
    {
        in[i] = src[i];
//        in[i][1] = 0.0;
    }

    for (int i=nsrc; i<n2; i++)
    {
        in[i] = 0.0;
//        in[i][1] = 0.0;
    }

    out = (fftw_complex*) fftw_malloc (sizeof(fftw_complex) * n2);
    //if (sign == FFTW_FORWARD)
    p = fftw_plan_dft_r2c_1d(n2, in, out, flags);
/*    else
    {
        fftw_free(out);
        fftw_free(in);
        delete [] res;
        return 0;
    }
*/
    fftw_execute (p);
//    qDebug () << __PRETTY_FUNCTION__ << out[0][0];
    for (int i=0; i<=n2/2; i++)
    {
        res[i] = complex<double> (out[i][0]/n2, out[i][1]/n2);
        if (i>0)
            res[n2-i] = complex<double> (out[i][0]/n2, -out[i][1]/n2);
        //qDebug () << __PRETTY_FUNCTION__ << (double)real(res[i]) << (double)imag(res[i]);
    }
    fftw_destroy_plan (p);
    fftw_free(out);
    fftw_free(in);
    //delete [] in;
    return res;
}

FFT2_Transform :: FFT2_Transform (void)
{
}

FFT2_Transform :: ~FFT2_Transform (void)
{
}

complex<double> * FFT2_Transform :: operator () (complex<double> * src, int nr, int nc, int sign, unsigned flags)
{
    complex<double> * res = new complex<double> [nr*nc];

    complex<double> * in = new complex<double> [nr*nc];

    for (int i=0; i<nr*nc; i++)
        in[i] = src[i];


    fftw_plan p;
    //fftw_complex * out = (fftw_complex*) fftw_malloc (sizeof(fftw_complex) * n2);
    p = fftw_plan_dft_2d (nr, nc, reinterpret_cast<fftw_complex*>(in), reinterpret_cast<fftw_complex*>(res), sign, flags);
    fftw_execute (p);
    fftw_destroy_plan (p);
    return res;
}
