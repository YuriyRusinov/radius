#include <iostream>
#include <complex>
#include <math.h>
#include <stdio.h>
#include <fftw3.h>

#include "fft_c.h"
#include "matrix.h"

using std::cout;
using std::endl;
using std::complex;

int main (int argc, char* argv[])
{
    int nd (16384);
    int ndcentre (6178);
    int ndrazmer (256);

    int ndv = ndcentre - ndrazmer/2;
    int ndn = ndcentre + ndrazmer/2;

    int ndrz = ndn - ndv;

    int na (16384);
    int nas (8192);
    int na_ots (3000);

    const long double c (3e8);
    long double r (5396);
    long double h (3964);

    long double fsp (3e8);
    long double dnr (0.375);

    long double fcvant2 (c/(2*dnr));

    long double dimp (1.0e-5);
    int n1 = (int) (dimp*fcvant2);

    long double dx (0.058);
    long double lamp (0.03450);
    const long double pi (3.141592653589793238462643383279);

    FILE * fid6 = fopen ("./test_data/conv_dal1.rgg", "rb");
    FILE * fid7 = fopen ("./test_data/image.rgg", "w+");

    int h1 = fseek (fid6, nd*8*na_ots, SEEK_SET);
    CMatrix rgg1 (0.0, ndrz, na);
    CMatrix rgg  (0.0, ndrz, na);
    int read (0);
    for (int i=0; i<na; i++)
    {
        long double stlb[2*nd];
        long double stlb2[ndrz*2];
        fread (stlb, 32, nd*2, fid6);
        for (int j=0; j<ndrz*2;j++)
            stlb2[j]=stlb[j+2*ndv];
        for (int j=0; j<ndrz;j++)
            rgg1(j, i) = complex<long double>(stlb2[2*j], stlb2[2*j+1]);
        read++;
    }

    CMatrix corf (complex<long double>(0.0), ndrz, na);
    CMatrix corf2 (complex<long double>(0.0), ndrz, na);
    CMatrix corf3 (complex<long double>(0.0), ndrz, nas);
    int from_opor (0);
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<nas; j++)
        {
            double x = (-nas/2+i)*dx;
            double rt = sqrt (r*r+x*x+h*h);
            double rt1 = rt - sqrt (r*r+h*h);
            int N0 = (int)(rt1/dnr);
            double phase = -4*pi*rt/lamp;
            corf3(N0, i) = complex<long double>(cos(phase), sin(phase));
        }
        from_opor++;
    }

    int cor_func (0);
    for (int i=0; i<nas/2; i++)
    {
        for (int j=0; j<ndrz; j++)
        {
            corf(j, i) = corf3(j, i+nas/2);
            corf(j, i+na-nas/2) = corf3 (j, i);
        }
        cor_func++;
    }

    FFT2_Transform fft2;// = new FFT2_Transform;
    complex<long double> * corfw = fft2(corf.getData(), ndrz, nas/2, FFTW_FORWARD, FFTW_ESTIMATE);
    complex<long double> * rggD = fft2(rgg1.getData(), ndrz, nas/2, FFTW_FORWARD, FFTW_ESTIMATE);
    int cor_volfr (0);
    for (int i=0; i<na; i++)
    {
        for (int j=0; j<ndrz; j++)
            rgg(j, i) *= conj (corf2(j, i));
        cor_volfr++;
    }
    complex<long double> * rggBD = fft2(rgg.getData(), ndrz, nas/2, FFTW_BACKWARD, FFTW_ESTIMATE);

    fclose (fid7);
    fclose (fid6);
    return 0;
}
