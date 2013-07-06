TEMPLATE = lib
TARGET = fft_matrix

CONFIG += qt

DESTDIR = ../bin

include (../radius.conf)

DEFINES += _FFTMATR_DLL_
DEFINES += _FILE_OFFSET_BITS=64

INCLUDEPATH *= $$FFT_INC_DIR
INCLUDEPATH *= $$BLAS_INC_DIR

DEPENDPATH *= $$FFT_INC_DIR
DEPENDPATH *= $$BLAS_INC_DIR

unix{
    LIBS *= -L$$FFT_LIB_DIR -lfftw3 -lm -L$$BLAS_LIB_DIR -lblas
}
win32{
    LIBS *= -L$$FFT_LIB_DIR -llibfftw3-3
}

INCLUDEPATH *= . \
               gui \
               misc

DEPENDPATH *= . \
               gui \
               misc

HEADERS += fft_c.h \
           matrix.h \
           calcim.h \
           ConvolSingleton.h \
           radar_conv_image.h \
           src_config.h

SOURCES += fft_c.cpp \
           matrix.cpp \
           calcim.cpp \
           ConvolSingleton.cpp \
           radar_conv_image.cpp

include (gui/gui.pri)
include (misc/misc.pri)

headers.files += \
                 fft_c.h \
                 matrix.h \
                 calcim.h \
                 ConvolSingleton.h \
                 src_config.h

headers.path = $$RAD_INCLUDE_DIR
