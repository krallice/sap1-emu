CC=cc
CFLAGS=-g -ansi -std=gnu99 -Wall

LINKER=cc -o
LFLAGS=-Wall

TARGET=sap1emu

SRCDIR=src
BINDIR=bin
OBJDIR=obj

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	    @$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	        @echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	    @$(CC) $(CFLAGS) -c $< -o $@
	    @echo "Compiled "$<" successfully!"

.PHONY : clean
clean:
	@- $(RM) $(OBJECTS)
	@- $(RM) $(BINDIR)/$(TARGET)
	@echo "Cleanup Complete"
