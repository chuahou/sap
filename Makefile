CXX?=g++
CXXFLAGS?=-g
CC?=gcc
CCFLAGS?=-g

export CXXFLAGS:=$(CXXFLAGS) -Wall -O3
export CXX
export CCFLAGS:=$(CCFLAGS) -Wall -O3
export CC
LDFLAGS=-g
AR=ar
ARFLAGS=rsv

SRCDIR:=src
HEADERDIR:=include
OBJDIR:=obj
OUTDIR:=lib

INCLUDES=$(HEADERDIR:%=-I%)

DIRS:=$(SRCDIR) $(OBJDIR) $(OUTDIR)

SRC:=$(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*.c)
HEADERS:=$(wildcard $(HEADERDIR)/*.h) $(wilcard $(HEADERDIR)/*.hpp)
OBJ:=$(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o) $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OUT:=$(OUTDIR)/libsap.a

.PHONY: build setup clean

build: setup $(OUT)

$(OUT): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

setup: $(DIRS)

$(DIRS):
	mkdir -p $@

clean:
	rm $(OUTDIR)/* $(OBJDIR)/* || true

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@
