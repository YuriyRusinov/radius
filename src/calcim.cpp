#include <QtDebug>
#include <fftw3.h>

#include "constants1.h"
#include "fft_c.h"
#include "matrix.h"
#include "calcim.h"

Calc :: Calc ()
{
}

Calc :: ~Calc ()
{
}

Calc1 :: Calc1 () : Calc ()
{
}

Calc1 :: ~Calc1 ()
{
}

double * Calc1 :: operator () (const double* st, int nd2)
{
    double * st1 = new double [nd];
    complex<double> * opor = new complex<double> [nd];
    complex<double> * opor2 = new complex<double> [nd];
    for (int i=0; i<nd; i++)
    {
        st1[i] = 0.0;
        opor[i] = complex<double>(0.0, 0.0);
        opor2[i] = complex<double>(0.0, 0.0);
    }

    for (int n=1; n<= N1; n++)
    {
        double phase = pi*fsp*(n-1)*(n-1)/(N1*fcvant2) - pi*fsp*(n-1)/fcvant2 ; 
        double oc = cos (phase);
        double os = sin (phase);
        opor[n-1] = complex<double>(oc, os);
    }
    int N2 = (N1/2);
    for (int i=1; i<=N2; i++)
    {
        opor2[i] = opor[i+N2];
        opor2[i+nd-N2] = opor[i];
    }

    FFT_Transform fft;
    opor = fft (opor2, N2, nd, FFTW_FORWARD, FFTW_ESTIMATE);
    complex<double> * stc = new complex<double> [nd];
    complex<double> * stc1 = new complex<double> [nd];
    complex<double> * stc4 (0);// = new complex<double> [nd];
/*    for (int i=0; i<nd2; i++)
    {
        st [i] = 0.0;
    }*/
    for (int i=0; i<nd; i++)
    {
        stc[i] = complex<double> (0.0, 0.0);
        stc1[i] = complex<double> (0.0, 0.0);
    }

    for (int i=1; i<=na; i++)
    {
        for (int ii=0; ii<nd2/2; ii++)
            stc[ii] = complex<double> (st[2*ii], st[2*ii+1]);
    }
    stc4 = fft (stc, ndn, 2*nd, FFTW_FORWARD, FFTW_ESTIMATE);
    for (int i=0; i<nd; i++)
    {
        stc1[i] = stc4[i]*opor[i];///(nd*nd);
        stc1[i] = complex<double> (real (stc1[i]) / ((double)nd*nd), imag (stc1[i]) / ((double)nd*nd));
    }

    complex<double> * stc3 = fft (stc1, nd, nd, FFTW_BACKWARD, FFTW_ESTIMATE);
    double * stc2 = new double [2*nd];
    for (int i=0; i<nd; i++)
    {
        stc2[2*i] = real (stc3[i]);
        stc2[2*i+1] = imag (stc3[i]);
    }
    delete [] stc3;
    delete [] stc4;
    delete [] stc1;
    delete [] stc;
    delete [] opor;
    delete [] st1;


    return stc2;
}

Calc2 :: Calc2 ()
{
}

Calc2 :: ~Calc2 ()
{
}

double * Calc2 :: operator () (const double* stlb, int nd2)
{
    CMatrix rgg1 (0.0, ndrz, na);
    CMatrix rgg  (0.0, ndrz, na);
    int read (0);
    for (int i=0; i<nd2; i++)
    {
//        double stlb[2*nd];
        double stlb2[ndrz*2];
//        fread (stlb, 32, nd*2, fid6);
        for (int j=0; j<ndrz*2;j++)
            stlb2[j]=stlb[j+2*ndv];
        for (int j=0; j<ndrz;j++)
            rgg1(j, i) = complex<double>(stlb2[2*j], stlb2[2*j+1]);
        read++;
    }

    CMatrix corf (complex<double>(0.0), ndrz, na);
    CMatrix corf2 (complex<double>(0.0), ndrz, na);
    CMatrix corf3 (complex<double>(0.0), ndrz, nas);
    int from_opor (0);
    for (int i=0; i<ndrz; i++)
    {
        for (int j=0; j<nas; j++)
        {
            double x = (-nas/2+i)*dx;
            double rt = sqrt (R*R+x*x+H*H);
            double rt1 = rt - sqrt (R*R+H*H);
            int N0 = (int)(rt1/dnr);
            double phase = -4*pi*rt/lamb;
            corf3(N0, i) = complex<double>(cos(phase), sin(phase));
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
    complex<double> * corfw = fft2(corf.getData(), ndrz, nas/2, FFTW_FORWARD, FFTW_ESTIMATE);
//    Q_UNUSED (corfw);
    complex<double> * rggD = fft2(rgg1.getData(), ndrz, nas/2, FFTW_FORWARD, FFTW_ESTIMATE);
    int cor_volfr (0);
    for (int i=0; i<na; i++)
    {
        for (int j=0; j<ndrz; j++)
            rgg(j, i) *= conj (corf2(j, i));
        cor_volfr++;
    }
    complex<double> * rggBD = fft2(rgg.getData(), ndrz, nas/2, FFTW_BACKWARD, FFTW_ESTIMATE);

    return 0;
}

CalcOpor1 :: CalcOpor1 (int _nd)
    : opor (new complex<double> [_nd]),
    nd (_nd)
{
}

CalcOpor1 :: ~CalcOpor1 ()
{
    delete [] opor;
}

complex<double>* CalcOpor1 :: calc ()
{
    complex<double> * opor2 = new complex<double> [nd];
    for (int i=0; i<nd; i++)
    {
        opor[i] = complex<double>(0.0, 0.0);
        opor2[i] = complex<double>(0.0, 0.0);
    }
    for (int n=0; n< N1; n++)
    {
        double phase = pi*fsp*n*n/(N1*fcvant2) - pi*fsp*n/fcvant2 ; 
        double oc = cos (phase);
        double os = sin (phase);
        opor[n] = complex<double>(oc, os);
    }
    int N2 = (N1/2);
    for (int i=0; i<N2; i++)
    {
        opor2[i] = opor[i+N2];
        opor2[i+nd-N2] = opor[i];
    }
    FFT_Transform fft;
    opor = fft (opor2, nd, FFT_Transform :: pow2roundup(nd), FFTW_FORWARD, FFTW_ESTIMATE);

    delete [] opor2;
    return opor;
}

complex<double>* CalcOpor1 :: data () const
{
    return opor;
}
