# Compiler and ROOT flags
CXX      = g++
CXXFLAGS = -O2 -Wall -fPIC -std=c++17 $(shell root-config --cflags)
LDFLAGS  = $(shell root-config --libs)

INCDIR   = inc
SRCDIR   = src
DICTDIR  = dict
BUILDDIR = build

TARGET_LIB = libModularEarth.so

HEADERS := $(filter-out inc/LinkDef.h, $(wildcard inc/*.h))
SOURCES = $(wildcard $(SRCDIR)/*.C)
OBJS    = $(patsubst $(SRCDIR)/%.C,$(BUILDDIR)/%.o,$(SOURCES))

DICT_SRC = $(DICTDIR)/ModularEarthDict.cxx
DICT_OBJ = $(BUILDDIR)/ModularEarthDict.o

# ------------------------

.PHONY: all clean run

all: $(TARGET_LIB)

# Make dirs if needed
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(DICTDIR):
	mkdir -p $(DICTDIR)

# Build ROOT dictionary
$(DICT_SRC): $(HEADERS) | $(DICTDIR)
	rootcling -f $@ -I$(INCDIR) $(HEADERS) $(INCDIR)/LinkDef.h

$(DICT_OBJ): $(DICT_SRC) | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I. -c $< -o $@

# Compile src objects
$(BUILDDIR)/%.o: $(SRCDIR)/%.C | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Build shared lib
$(TARGET_LIB): $(OBJS) $(DICT_OBJ)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

# ------------------------------------------------------------------
# Tutorials
# ------------------------------------------------------------------
TUTDIR     = tutorials
TUTSOURCES := $(wildcard $(TUTDIR)/*.cpp) $(wildcard $(TUTDIR)/*.C)

# Tutorial "names" without path/extension, e.g. tutorials/SubLayerTutorial.cpp -> SubLayerTutorial
TUTNAMES := $(sort $(basename $(notdir $(TUTSOURCES))))

.PHONY: run tut tutorials $(TUTNAMES)

# Show available tutorials
tutorials:
	@echo "Available tutorials:"
	@$(foreach t,$(TUTNAMES),echo "  make $(t)";)
	@echo "Or: make tut NAME=<one of the above>"

# Generic runner: make tut NAME=SubLayerTutorial
tut: $(TARGET_LIB)
	@if [ -z "$(NAME)" ]; then \
		echo "Usage: make tut NAME=<tutorial>"; \
		echo "Try: make tutorials"; \
		exit 1; \
	fi
	@TFILE="$(firstword $(wildcard $(TUTDIR)/$(NAME).cpp) $(wildcard $(TUTDIR)/$(NAME).C))"; \
	if [ -z "$$TFILE" ]; then \
		echo "Tutorial not found: $(NAME)"; \
		echo "Try: make tutorials"; \
		exit 1; \
	fi; \
	echo "Running $$TFILE"; \
	root -l -e 'gSystem->Load("./$(TARGET_LIB)");' $$TFILE

# One target per tutorial: make SubLayerTutorial
$(TUTNAMES): $(TARGET_LIB)
	@TFILE="$(firstword $(wildcard $(TUTDIR)/$@.cpp) $(wildcard $(TUTDIR)/$@.C))"; \
	echo "Running $$TFILE"; \
	root -l -e 'gSystem->Load("./$(TARGET_LIB)");' $$TFILE

# Optional default "run" target (pick one tutorial as default)
run: $(TARGET_LIB)
	@echo "Default run target: make tut NAME=SubLayerTutorial (or choose another)"
	@$(MAKE) tut NAME=SubLayerTutorial


clean:
	rm -rf $(BUILDDIR) $(DICTDIR) *.so *.pcm
