#===============================================================================
# User Options
#===============================================================================

COMPILER    = ibm
MPI         = no
OPENMP      = yes
OPTIMIZE    = yes
DEBUG       = no
PROFILE     = no
PAPI        = yes
BENCHMARK   = no

#===============================================================================
# Program name & source code list
#===============================================================================

program = SimpleMOC

source = \
main.c \
comms.c \
source.c \
test.c \
init.c \
io.c \
tracks.c \
utils.c \
solver.c \
papi.c

obj = $(source:.c=.o)

#===============================================================================
# Sets Flags
#===============================================================================

# Regular gcc Compiler
ifeq ($(COMPILER),gnu)
  CC = gcc
endif

# intel Compiler
ifeq ($(COMPILER),intel)
  CC = icc
endif

# IBM XL Compiler
ifeq ($(COMPILER),ibm)
  CC = mpixlc_r
endif

# BG/Q gcc Cross-Compiler
ifeq ($(MACHINE),bluegene)
  CC = mpicc
endif

# Standard Flags
# CFLAGS := -std=gnu99

# Linker Flags
LDFLAGS = -lm

# Debug Flags
ifeq ($(DEBUG),yes)
  CFLAGS += -g
endif

# Profiling Flags
ifeq ($(PROFILE),yes)
  CFLAGS += -pg
endif

# Optimization Flags
ifeq ($(OPTIMIZE),yes)
ifeq ($(COMPILER),gnu)
  CFLAGS += -Ofast -ffast-math -ftree-vectorize -msse2
endif
ifeq ($(COMPILER),intel)
  CFLAGS += -O3 -xhost -ansi-alias -no-prec-div -DINTEL
endif
ifeq ($(COMPILER),ibm)
  CFLAGS += -O5 -qhot -qsimd=auto -qalias=ansi:allptrs -qarch=qp -DIBM
endif
endif

ifeq ($(COMPILER),ibm)
  CFLAGS += -qreport -qsimd=noauto
endif

# PAPI source (you may need to provide -I and -L pointing
# to PAPI depending on your installation
ifeq ($(PAPI),yes)
  CFLAGS += -DPAPI
  LDFLAGS += -lpapi
  LDFLAGS += -L/soft/perftools/papi-5.3.0-20140528/lib/
  OPENMP = yes
endif

# MPI
ifeq ($(MPI),yes)
  CC = mpixcl_r
  CFLAGS += -DMPI
endif

# OpenMP
ifeq ($(OPENMP),yes)
ifeq ($(COMPILER), gnu)
	CFLAGS += -fopenmp
endif
ifeq ($(COMPILER), intel)
	CFLAGS += -openmp
endif
ifeq ($(COMPILER), ibm)
	CFLAGS += -qsmp
endif
   	CFLAGS += -DOPENMP
endif


#===============================================================================
# Targets to Build
#===============================================================================

$(program): $(obj) SimpleMOC_header.h
	$(CC) $(CFLAGS) $(obj) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(program) $(obj)

edit:
	vim -p $(source) SimpleMOC_header.h

run:
	./$(program)
