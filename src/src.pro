TEMPLATE = lib
TARGET = fft_matrix

CONFIG += qwt
win32{
    CONFIG += QwtDll
}

QT += opengl

DESTDIR = ../bin

include (../radius.conf)

DEFINES += _FFTMATR_DLL_
DEFINES += _FILE_OFFSET_BITS=64

INCLUDEPATH *= $$FFT_INC_DIR
INCLUDEPATH *= $$BLAS_INC_DIR
INCLUDEPATH *= $$QWT_INC_DIR

DEPENDPATH *= $$FFT_INC_DIR
DEPENDPATH *= $$BLAS_INC_DIR
DEPENDPATH *= $$QWT_INC_DIR

unix{
    LIBS *= -L$$FFT_LIB_DIR -lfftw3 -lfftw3_threads -lm -L$$BLAS_LIB_DIR -lblas -L$$QWT_LIB_DIR -lqwt6
}
win32{
    LIBS *= -L$$FFT_LIB_DIR -llibfftw3-3
}

INCLUDEPATH *= . \
               gui \
               gui/ui \
               gui/OpenGl \
               gologram \
               misc

DEPENDPATH *= . \
               gui \
               gui/ui \
               gui/OpenGl \
               gologram \
               misc

HEADERS += fft_c.h \
           matrix.h \
           calcim.h \
           radarConvIm.h \
           ConvDistPhys.h \
           ConvDistThread.h \
           ConvDistColumnThread.h \
           ConvAzimuthPhys.h \
           ConvAzimuthThread.h \
           radapplication.h \
           RadSettings.h \
           assistant.h \
           hoverpoints.h \
           ImageGolographicThread.h \
           ImageGeneratorObject.h \
           rhistogram.h \
           src_config.h

SOURCES += fft_c.cpp \
           matrix.cpp \
           calcim.cpp \
           radarConvIm.cpp \
           ConvDistPhys.cpp \
           ConvDistThread.cpp \
           ConvDistColumnThread.cpp \
           ConvAzimuthPhys.cpp \
           ConvAzimuthThread.cpp \
           radapplication.cpp \
           RadSettings.cpp \
           assistant.cpp \
           hoverpoints.cpp \
           ImageGolographicThread.cpp \
           ImageGeneratorObject.cpp \
           rhistogram.cpp

include (gui/gui.pri)
include (misc/misc.pri)
include (gologram/gologram.pri)

headers.files += \
                 fft_c.h \
                 matrix.h \
                 calcim.h \
                 radarConvIm.h \
                 ConvDistPhys.h \
                 ConvDistThread.h \
                 ConvDistColumnThread.h \
                 ConvAzimuthPhys.h \
                 ConvAzimuthThread.h \
                 radapplication.h \
                 RadSettings.h \
                 assistant.h \
                 hoverpoints.h \
                 ImageGolographicThread.h \
                 ImageGeneratorObject.h \
                 rhistogram.h \
                 src_config.h

headers.path = $$RAD_INCLUDE_DIR
