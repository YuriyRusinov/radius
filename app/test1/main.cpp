#define _FILE_OFFSET_BITS  64

#include <iostream>
#include <complex>
#include <math.h>
#include <stdio.h>
#include <fftw3.h>

#include "fft_c.h"

using std::cout;
using std::endl;
using std::complex;

int main (int argc, char* argv[])
{
    int ndn = 11776;
    int nd2 = 2*ndn;
    int nd = 16384;
    int na = 100000;
    double c = 0.3e9;
    double fsp = 0.3e9;
    double dnr = 0.375;
    double fcvant2 = c/(0.2e1*dnr);
    double dimp = 10.0*0.1e-6;
    int N1 = (int)dimp*fcvant2;
    const double pi (3.14159265358979);

    double * st1 = new double [nd];
    complex<long double> * opor = new complex<long double> [nd];
    complex<long double> * opor2 = new complex<long double> [nd];
    for (int i=0; i<nd; i++)
    {
        st1[i] = 0.0;
        opor[i] = complex<long double>(0.0, 0.0);
        opor2[i] = complex<long double>(0.0, 0.0);
    }

    for (int n=1; n<= N1; n++)
    {
        double phase = pi*fsp*(n-1)*(n-1)/(N1*fcvant2) - pi*fsp*(n-1)/fcvant2 ; 
        double oc = cos (phase);
        double os = sin (phase);
        opor[n-1] = complex<long double>(oc, os);
    }
    int N2 = (N1/2);
    for (int i=1; i<=N2; i++)
    {
        opor2[i] = opor[i+N2];
        opor2[i+nd-N2] = opor[i];
    }

    FFT_Transform fft;// = new FFT_Transform;
    opor = fft (opor2, N2, nd, FFTW_FORWARD, FFTW_ESTIMATE);
/*    fftwl_complex *in, *out;
    fftwl_plan p;
    in = (fftwl_complex*) fftwl_malloc (sizeof(fftwl_complex) * nd);
    for (int i=0; i<N2; i++)
    {
        in[i][0] = real (opor2[i-1]);
        in[i][1] = imag (opor2[i-1]);
    }
    for (int i=N2; i<nd; i++)
    {
        in[i][0] = 0.0;
        in[i][1] = 0.0;
    }
    out = (fftwl_complex*) fftwl_malloc (sizeof(fftwl_complex) * nd);
    p = fftwl_plan_dft_1d(nd, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftwl_execute (p);
    for (int i=0; i<nd; i++)
    {
        opor[i] = complex<double>(out[i][0]/nd, out[i][1]/nd);
    }
    fftwl_destroy_plan (p);

    fftwl_free (in);
    fftwl_free (out);
*/
    double * st = new double [nd2];
    complex<long double> * stc = new complex<long double> [nd];
    complex<long double> * stc1 = new complex<long double> [nd];
    complex<long double> * stc4 (0);// = new complex<double> [nd];
    for (int i=0; i<nd2; i++)
    {
        st [i] = 0.0;
    }
    for (int i=0; i<nd; i++)
    {
        stc[i] = complex<long double> (0.0, 0.0);
        stc1[i] = complex<long double> (0.0, 0.0);
    }

    FILE * fid5 = fopen ("./test_data/20\ 07\ 10\ \ 23.02.49.rgg", "rb");
    if (!fid5)
    {
        cout << "Cannot open data file " << errno << endl;
        return 1;
    }

    FILE * fid6 = fopen ("./test_data/conv_dal1.rgg", "w+");
    if (!fid6)
        return 1;

    int a = 0;
    for (int i=1; i<=na; i++)
    {
        fread (st+i-1, sizeof (unsigned long), nd2, fid5);
        for (int ii=1; ii<= nd2; ii++)
            if (st[ii-1] > 128)
                st[ii-1] -= 256;

        for (int ii=1; ii<=128; ii++)
            st[ii-1] = 0.0;

        for (int ii=0; ii<ndn; ii++)
        {
            stc[ii] = complex<long double> (st[2*ii], st[2*ii+1]);
            //cout << st[2*ii] << " " << st[2*ii+1] << " ";
        }
        //cout << endl;
        cout << i << " " << na << endl;
    }

    cout << "Data were read" << endl;
/*    fftwl_complex *in1, *out1;
    in1 = (fftwl_complex*) fftwl_malloc (sizeof(fftwl_complex) * 2*nd);
    out1 = (fftwl_complex*) fftwl_malloc (sizeof(fftwl_complex) * nd*2);
    for (int i=0; i<ndn; i++)
    {
        in1 [i][0] = real (stc[i]);
        in1 [i][1] = imag (stc[i]);
    }
    for (int i=ndn; i<2*nd; i++)
    {
        in1 [i][0] = 0.0;
        in1 [i][1] = 0.0;
    }
    cout << "FFTW data were set" << endl;
    fftwl_plan p1 = fftwl_plan_dft_1d(2*nd, in1, out1, FFTW_FORWARD, FFTW_ESTIMATE);
    cout << "FFTW plan was set" << endl;
    fftwl_execute (p1);
*/
    stc4 = fft (stc, ndn, 2*nd, FFTW_FORWARD, FFTW_ESTIMATE);
    for (int i=0; i<nd; i++)
    {
        stc1[i] = stc4[i]*opor[i];///(nd*nd);
        stc1[i] = complex<long double> (real (stc1[i]) / ((long double)nd*nd), imag (stc1[i]) / ((long double)nd*nd));
    }

    complex<long double> * stc3 = fft (stc1, nd, nd, FFTW_BACKWARD, FFTW_ESTIMATE);
    long double * stc2 = new long double [2*nd];
    for (int i=0; i<nd; i++)
    {
        stc2[2*i] = real (stc3[i]);
        stc2[2*i+1] = imag (stc3[i]);
    }
    size_t h = fwrite (stc2, sizeof (long double), 2*nd, fid6);
    cout << h << endl;
    a++;

    fclose (fid6);
    fclose (fid5);
    cout << "Test of FFTW" << endl;

    delete [] stc2;
    delete [] stc3;
    delete [] stc4;
    delete [] stc1;
    delete [] stc;
    delete [] st;
    delete [] opor;
    delete [] st1;

    return 0;
}
