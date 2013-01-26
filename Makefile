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
	lldiamondsquare.o \
	llecho.o \
	llexit.o \
	llexportmap.o \
	llexportmeshtoobj.o \
	llexportmeshtops.o \
	llfillcolormap.o \
	llfilter.o \
	llinactivateallvertices.o \
	lllogger.o \
	llmakederivatives.o \
	llmap.o \
	llmaplist.o \
	llmapworker.o \
	llpanorama.o \
	llpointlist.o \
	llpolygonlist.o \
	llquaditerator.o \
	llquadlist.o \
	llreaddatafile.o \
	llreadpolygondatafile.o \
	llremoveinactivetriangles.o \
	llselectall.o \
	llselectrec.o \
	llsetalgvertices.o \
	llsetatgridline.o \
	llsetcontour.o \
	llsetflag.o \
	llsetgridborder.o \
	llsetgrid.o \
	llset.o \
	llsetheight.o \
	llsetpath.o \
	llsetvertex.o \
	llsplitatgrid.o \
	llsplitat.o \
	llsplitatpolygonborder.o \
	llsplitbetween.o \
	llstencilpolygon.o \
	lltrianglelist.o \
	lltriangulation.o \
	lltrimod.o \
	llutils.o \
	llworker.o 

XOBJS = externals/triangle/triangle.o

$(XOBJS): %.o : %.cc %.h
	@echo Compiling $*
	@$(CXX) $(CXXFLAGS) $< -o $@

$(OBJS): %.o : src/%.cpp include/%.h
	@echo Compiling $*
	$(CXX) $(CXXFLAGS) $< -o $@

lltool.o : src/lltool.cpp
	@echo Compiling $*
	@$(CXX) $(CXXFLAGS) $< -o $@

lltool:	$(OBJS) $(XOBJS) lltool.o
	$(LD) -g -O0 $(OBJS) lltool.o $(XOBJS) -o lltool

clean:
	rm *.o



