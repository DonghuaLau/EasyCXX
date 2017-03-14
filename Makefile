
include makefile.inc

.PHONY: all clean test

ifdef ESOCKET
es="ESOCKET is defined"
else
es="ESOCKET is not defined"
endif

all:
	@cd src; make
	@pwd
	@cd test; make
	@pwd
	@echo "main make. $(ESOCKET)"
	@echo $(es) 

clean:
	@cd src; make clean
	@cd test; make clean
	@echo "main clean"

test:
	@echo $(PWD)
