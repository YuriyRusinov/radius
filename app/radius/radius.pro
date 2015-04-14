TEMPLATE = app

include(../../radius.conf)

TARGET = $${EXECUTABLE_PREFIX}_app

DESTDIR = ../../bin

RESOURCES = ../../icons/radius_icons.qrc

HEADERS += radmainwindow.h

FORMS += radius_mainwindow.ui

SOURCES += radmainwindow.cpp \
           main.cpp

INCLUDEPATH *= $$FFT_INC_DIR
INCLUDEPATH *= $$BLAS_INC_DIR

DEFINES += _FILE_OFFSET_BITS=64

unix{
    LIBS *= -L$$FFT_LIB_DIR -lfftw3 -lm -L$$BLAS_LIB_DIR -lblas
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
