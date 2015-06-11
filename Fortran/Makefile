
all: uarea.exe qkpfl.exe

uarea.exe: uarea.for itm.for
	gfortran -o uarea.exe uarea.for itm.for

qkpfl.exe: qkpfl.for itm.for
	gfortran -o qkpfl.exe qkpfl.for itm.for

check:
	./qkpfl < qkpfldat.txt > qkpfldat.out
	dos2unix qkpfldat.out
