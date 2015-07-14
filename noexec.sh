#! /bin/bash

chmod -R -f +rw,-x \
 *.cpp *.tpp *.h *.for *.cs \
 *.dat *.DAT *.txt *.in *.out \
 *.jpg *.png *.ico *.ttf *.iv \
 .gitignore \
 *.gz *.bz2 *.zip \
 Makefile Makefile.* Doxy* \
 README TODO \
 *.pdf *.doc *.ppt *.xls

chmod -f +rwx *.sh

chmod +x `find . -type d`
 
