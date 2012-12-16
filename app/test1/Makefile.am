CC = g++
CFLAGS = -g -fPIC -Wall
TMPDIR = .tmp
SRCDIR = ../../src
INCPATH = @FFTW_CFLAGS@ @QTCORE_CFLAGS@ @QTGUI_CFLAGS@ @QTOPENGL_CFLAGS@ -I$(SRCDIR)
LD = g++
DESTDIR = ../../bin
LDFLAGS = @FFTW_LIBS@ @QTCORE_LIBS@ @QTGUI_LIBS@ @QTOPENGL_LIBS@ -L$(DESTDIR) -lfft_test
RM = rm
MKDIR = mkdir -p

all: $(DESTDIR)/test1

$(DESTDIR)/test1: $(TMPDIR)/main.o 
	$(MKDIR) $(DESTDIR)
	$(LD) $(LDFLAGS) -o $@ $^

$(TMPDIR)/main.o: main.cpp 
	$(MKDIR) $(TMPDIR)
	$(CC) -c $(CFLAGS) $(INCPATH) $< -o $@

$(TMPDIR)/moc_%.o: $(TMPDIR)/moc_%.cpp
	$(CC) -c $(CFLAGS) $(INCPATH) $< -o $@

$(TMPDIR)/moc_%.cpp: %.h
	$(QTMOC) $(INCPATH) $< -o $@

$(TMPDIR)/ui_%.h: %.ui
	$(QTUIC) $< -o $@

$(TMPDIR)/qrc_%.o: $(TMPDIR)/qrc_%.cpp
	$(CC) -c $(CFLAGS) $(INCPATH) $< -o $@

$(TMPDIR)/qrc_%.cpp: icons/%.qrc
	$(QTRCC) -o $@ $<

clean:
	$(RM) -f $(DESTDIR)/libfft_test.so $(TMPDIR)/*.o $(TMPDIR)/moc_*.cpp $(TMPDIR)/ui_*.h $(TMPDIR)/qrc_*.cpp

install:

uninstall:


