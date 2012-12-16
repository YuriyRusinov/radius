TEMPLATE = lib
TARGET = fft_matrix

CONFIG += qt

DESTDIR = ../bin

include (../radius.conf)

DEFINES += _FFTMATR_DLL_

INCLUDEPATH *= $$FFT_INC_DIR
INCLUDEPATH *= $$BLAS_INC_DIR

DEPENDSPATH *= $$FFT_INC_DIR
DEPENDSPATH *= $$BLAS_INC_DIR

LIBS *= -L$$FFT_LIB_DIR -lfftw3l -lm -L$$BLAS_LIB_DIR -lblas

INCLUDEPATH *= gui \
               misc

DEPENDSPATH *= gui \
               misc

HEADERS += fft_c.h \
           matrix.h \
           calcim.h

SOURCES += fft_c.cpp \
           matrix.cpp \
           calcim.cpp

include (gui/gui.pri)
include (misc/misc.pri)

headers.files += \
                 fft_c.h \
                 matrix.h \
                 calcim.h

headers.path = $$RAD_INCLUDE_DIR
