OUTPUTDIR = "bin"
INCLUDEDIR = "include"
SRCDIR = "src"

build:
	mkdir -p bin
	g++ $(SRCDIR)/iosifbin.cpp -o $(OUTPUTDIR)/iosifbin -I $(INCLUDEDIR) -pie -fstack-protector-all

install: build
	cp $(OUTPUTDIR)/iosifbin /usr/bin/iosifbin
