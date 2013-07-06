#include "ConvolSingleton.h"

ConvSingleton* ConvSingleton::m_ConvSingleton = 0;

ConvSingleton* ConvSingleton::getConv ()
{
    if (!m_ConvSingleton)
        m_ConvSingleton = new ConvSingleton;

    return m_ConvSingleton;
}

ConvSingleton::ConvSingleton ()
{
}

ConvSingleton::ConvSingleton (const ConvSingleton&)
{
}

ConvSingleton::~ConvSingleton ()
{
    if (m_ConvSingleton)
        delete m_ConvSingleton;
    m_ConvSingleton = 0;
}
