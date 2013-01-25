CXX           = g++
CXXFLAGS      = -g -O0 -rdynamic -c -Wall -fexceptions -fPIC 
#CXXFLAGS      = -O3 -rdynamic -c -Wall -fexceptions -fPIC 
LD            = g++

OBJS =  llactivatevisiblevertices.o \
	lladdvertextopolygon.o \
	llalg.o \
	llalgconst.o \
	llalgfirst.o \
	llalglist.o \
	llalgpeakfinder.o \
	llalgradial.o \
	llalgsecond.o \
	llalgslope.o \
	llalgstripe.o \
	llcommands.o \
	llcreatemap.o \
	llcreatenormalmap.o \
	llcreatepolygon.o \
	llmap.o

$(OBJS): %.o : src/%.cpp include/%.h
	 @echo Compiling $*
	 @$(CXX) $(CXXFLAGS) $< -o $@

lltool:	$(OBJS) 
	$(LD) -g -O0 $(OBJS) $(XOBJS) -L -o lltool


