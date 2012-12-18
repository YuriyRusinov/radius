TEMPLATE = app

include(../../radius.conf)

TARGET = test1

DESTDIR = ../../bin

#HEADERS += radmainwindow.h

#FORMS += radius_mainwindow.ui

SOURCES += main.cpp

INCLUDEPATH *= $$FFT_INC_DIR
INCLUDEPATH *= $$BLAS_INC_DIR

LIBS *= -L$$FFT_LIB_DIR -lfftw3 -lm -L$$BLAS_LIB_DIR -lblas

INCLUDEPATH *= ../../src \
               ../../src/gui \
               ../../src/misc

DEPENDPATH *= ../../src \
               ../../src/gui \
               ../../src/misc

LIBS += -L$$DESTDIR -lfft_matrix

target.path = $$RAD_EXEC_DIR
INSTALLS += target
