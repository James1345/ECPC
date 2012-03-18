#Directories to use
SRCDIR=src/
HEADDIR=include/
BUILDDIR=build/
LIBDIR=lib/

# Names of source files and output files
SOURCES=$(SRCDIR)win.c
HEADERS=$(HEADDIR)win.h
OBJECTS=$(BUILDDIR)win.o
LIBRARY=$(LIBDIR)libecpc.a

#Compiler settings
CC=gcc
CFLAGS=-Wall -I$(HEADDIR) -lxcb -lxcb-atom

AR=ar
ARFLAGS=rcs

# Debugger options
DEBUG=gdb
DEBUGOBJ=$(BUILDDIR)debug

# The default make action
default : release

# Create the library for release
release : object
	rm -rf $(LIBDIR)
	mkdir $(LIBDIR)
	$(AR) $(ARFLAGS) $(LIBRARY) $(OBJECTS)
	rm -rf $(BUILDDIR)

# Create the .o object file from the sources
object : $(SOURCES) $(HEADERS)
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
	
	# Build command, DNDEBUG turns off debug code, O2 level optimizing
	$(CC) $(CFLAGS) -DNDEBUG -c -O2 $(SOURCES) -o $(OBJECTS) 

# Install the compiled file to the system location
install : release
	cp $(LIBRARY) /usr/lib/
	
# As install but also copy header files
install-devel : install
	cp $(HEADERS) /usr/include

# Compile debug code and run normally
test : debug-build
	./$(DEBUGOBJ)

# Compile the debug code and run it
debug : debug-build
	$(DEBUG) $(DEBUGOBJ)

# Compile the debug code
debug-build : $(SOURCES) $(HEADERS)
	rm -rf $(BUILDDIR)
	mkdir $(BUILDDIR)
	$(CC) $(CFLAGS) -g $(SOURCES) -o $(DEBUGOBJ)

#remove everything that's not sources
clean :
	rm -rf $(BUILDDIR) $(LIBDIR)
