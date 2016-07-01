################################################################################
# -*-Makefile-*-
#

# change these to set the proper directories where each files shoould be
SRCDIR          = src
BIN_SRCDIR      = examples
UTIL_SRCDIR     = utils

SOURCES        := $(wildcard $(SRCDIR)/**/*.c $(SRCDIR)/*.c)
BIN_SOURCES    := $(wildcard $(BIN_SRCDIR)/**/*.c $(BIN_SRCDIR)/*.c)
UTIL_SOURCES   := $(wildcard $(UTIL_SRCDIR)/**/*.c $(UTIL_SRCDIR)/*.c)

VPATH           = $(SRCDIR) $(BIN_SRCDIR) $(UTIL_SRCDIR)

OBJDIR          = build
BINDIR          = bin
DEPDIR          = dep

OBJECTS        := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJECTS        := $(OBJECTS) $(UTIL_SOURCES:$(UTIL_SRCDIR)/%.c=$(OBJDIR)/%.o)

BIN_OBJECTS    := $(BIN_SOURCES:$(BIN_SRCDIR)/%.c=$(OBJDIR)/%.o)

# using the objects
DEPS           := $(OBJECTS:$(OBJDIR)/%.o=$(DEPDIR)/%.d)

BINARIES       := $(BIN_SOURCES:$(BIN_SRCDIR)/%.c=$(BINDIR)/%)

# some commands
RM          = rm -f
RMDIR       = rm -fd
MKDIR_P     = mkdir -p

# compiler flags
CFLAGS      = -g


# pull in dependency info for *existing* .o files
#-include $(OBJECTS:$(OBJDIR)/%.o=$(DEPDIR)/%.d)


# The Target Build
all: $(BINARIES)


$(OBJDIR):
	$(MKDIR_P) $(OBJDIR)


$(BINDIR):
	$(MKDIR_P) $(BINDIR)


${DEPDIR}:
	$(MKDIR_P) $(DEPDIR)


# directory will only be created if it does not exist
$(BINARIES): $(BIN_OBJECTS) $(OBJECTS) | $(BINDIR)
$(BINARIES): $(BINDIR)/%: %.c
	@echo "Compiling programs..."
	@echo "Compiling programs..." $(BIN_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(OBJDIR)/$*.o


# directory will only be created if it does not exist
$(OBJECTS): $(DEPS) | $(OBJDIR)
$(OBJECTS): $(OBJDIR)/%.o: %.c
	@echo "Compiling object files..."
	$(CC) -c $(CFLAGS) $< -o $@


# directory will only be created if it does not exist
$(BIN_OBJECTS): $(OBJECTS) | $(OBJDIR)
$(BIN_OBJECTS): $(OBJDIR)/%.o: %.c
	@echo "Compiling object files..."
	$(CC) -c $(CFLAGS) $< -o $@


# directory will only be created if it does not exist
$(DEPS): | $(DEPDIR)
$(DEPS): $(DEPDIR)/%.d: %.c
	@echo "Compiling dependency files..."
	$(CC) -MM -MF $@ $(CFLAGS) $<


.PHONEY: clean
clean: clean-dep clean-bin clean-build
	@echo "Cleanup complete!"


.PHONEY: clean-build
clean-build:
	$(RM) $(OBJDIR)/*
	$(RMDIR) $(OBJDIR)
	@echo "Object files removed!"


.PHONEY: clean-bin
clean-bin:
	$(RM)  $(BINDIR)/*
	$(RMDIR) $(BINDIR)
	@echo "Executables removed!"

.PHONEY: clean-dep
clean-dep:
	$(RM)  $(DEPDIR)/*
	$(RMDIR) $(DEPDIR)
	@echo "Dependency files removed!"
