TEMPLATE = app

include(../../radius.conf)

TARGET = $${EXECUTABLE_PREFIX}_app

DESTDIR = ../../bin

CONFIG += qwt

RESOURCES = ../../icons/radius_icons.qrc

include(opencv_test.pri)

CONFIG += link_pkgconfig
PKGCONFIG += gsl
PKGCONFIG += fftw3 fftw3_threads
PKGCONFIG += blas
PKGCONFIG += opencv


TARGET = opencv_test
QMAKE_CXXFLAGS += -std=c++11
DESTDIR = ../../bin

INCLUDEPATH *= $$FFT_INC_DIR
INCLUDEPATH *= $$BLAS_INC_DIR
INCLUDEPATH *= $$QWT_INC_DIR

DEFINES += _FILE_OFFSET_BITS=64

unix{
    LIBS *= -L$$FFT_LIB_DIR -lfftw3 -lm -L$$BLAS_LIB_DIR -L$$QWT_LIB_DIR
}
win32{
    LIBS *= -L$$FFT_LIB_DIR -llibfftw3-3
}

INCLUDEPATH *= ../../src \
               ../../src/gui \
               ../../src/gui/ui \
               ../../src/gui/OpenGl \
               ../../src/gologram \
               ../../src/misc

DEPENDPATH *= ../../src \
               ../../src/gui \
               ../../src/gui/ui \
               ../../src/gui/OpenGl \
               ../../src/gologram \
               ../../src/misc

LIBS += -L$$DESTDIR -lfft_matrix

target.path = $$RAD_EXEC_DIR
INSTALLS += target
