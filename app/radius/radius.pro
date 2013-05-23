TEMPLATE = app

include(../../radius.conf)

TARGET = $${EXECUTABLE_PREFIX}_app

DESTDIR = ../../bin

HEADERS += radmainwindow.h

FORMS += radius_mainwindow.ui

SOURCES += radmainwindow.cpp \
           main.cpp

INCLUDEPATH *= $$FFT_INC_DIR
INCLUDEPATH *= $$BLAS_INC_DIR

#DEFINES += _FILE_OFFSET_BITS=64

unix{
    LIBS *= -L$$FFT_LIB_DIR -lfftw3l -lm -L$$BLAS_LIB_DIR -lblas
}
win32{
    LIBS *= -L$$FFT_LIB_DIR -llibfftw3l-3
}

INCLUDEPATH *= ../../src \
               ../../src/gui \
               ../../src/misc

DEPENDPATH *= ../../src \
               ../../src/gui \
               ../../src/misc

LIBS += -L$$DESTDIR -lfft_matrix

target.path = $$RAD_EXEC_DIR
INSTALLS += target
