#ifndef _CONV_SINGLETON_H
#define _CONV_SINGLETON_H

#include "src_config.h"

class _FFTMATR_EXPORT_ ConvSingleton
{
public:
    static ConvSingleton* getConv ();

private:
    //
    // Functions
    //
    ConvSingleton ();
    ConvSingleton (const ConvSingleton& CS);
    virtual ~ConvSingleton ();
    ConvSingleton& operator=(const ConvSingleton&);

    //
    // Variables
    //
    static ConvSingleton *m_ConvSingleton;
};
#endif
