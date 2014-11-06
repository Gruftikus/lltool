CXX           = g++
CXXFLAGS      = -rdynamic -c -Wall -O0 -Iexternals/ann/include/
#CXXFLAGS      = -O3 -rdynamic -c -Wall -fexceptions -fPIC 
LD            = g++ -pthread -g

OBJS =  llactivatefractionofvertices.o \
	llactivatevisiblevertices.o \
	lladddiscontinuitygrid.o \
	lladddiscontinuityline.o \
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
	llcopymap.o \
	llcreatemap.o \
	llcreatenormalmap.o \
	llcreatepolygon.o \
	lldiamondsquare.o \
	llecho.o \
	llexit.o \
	llexportmap.o \
	llexportmaptodds.o \
	llexportmeshtoobj.o \
	llexportmeshtops.o \
	llexportmeshtoVRML.o \
	llfillcolormap.o \
	llfilter.o \
	llimportmap.o \
	llimportmapfromdds.o \
	llinactivateallvertices.o \
	lllinelist.o \
	lllogger.o \
	llmakederivatives.o \
	llmap.o \
	llmaplist.o \
	llmapworker.o \
	llmkdir.o \
	llpanorama.o \
	llpointlist.o \
	llpolygonlist.o \
	llquaditerator.o \
	llquadlist.o \
	llreaddatafile.o \
	llreadpolygondatafile.o \
	llremoveinactivetriangles.o \
	llremoveinactivevertices.o \
	llremovetriangulation.o \
	llscalemap.o \
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
	llworker.o \

XOBJST = externals/triangle/triangle.o 

XOBJSR = externals/resampler/resampler.o

XOBJSC = externals/crunch/crnlib/libcrunch.a

XOBJSA = externals/ann/src/ANN.o \
	 externals/ann/src/bd_fix_rad_search.o \
	 externals/ann/src/bd_pr_search.o \
	 externals/ann/src/bd_search.o \
	 externals/ann/src/bd_tree.o \
	 externals/ann/src/brute.o \
	 externals/ann/src/kd_dump.o \
	 externals/ann/src/kd_fix_rad_search.o \
	 externals/ann/src/kd_pr_search.o \
	 externals/ann/src/kd_search.o \
	 externals/ann/src/kd_split.o \
	 externals/ann/src/kd_tree.o \
	 externals/ann/src/kd_util.o \
	 externals/ann/src/perf.o


all: lltool

$(XOBJST): %.o : %.cc %.h
	@echo Compiling $*
	@$(CXX) $(CXXFLAGS) $< -o $@

$(XOBJS2): %.o : %.cpp %.h
	@echo Compiling $*
	@$(CXX) $(CXXFLAGS) $< -o $@

$(XOBJSC): 
	cd externals/crunch/crnlib/; make libcrunch.a

$(XOBJSA): %.o : %.cpp 
	@echo Compiling $*
	@$(CXX) $(CXXFLAGS) $< -o $@

$(OBJS): %.o : src/%.cpp include/%.h
	@echo Compiling $*
	$(CXX) $(CXXFLAGS) $< -o $@

lltool.o : src/lltool.cpp
	@echo Compiling $*
	@$(CXX) $(CXXFLAGS) $< -o $@

lltool:	$(OBJS) $(XOBJST) $(XOBJSR) $(XOBJSC) $(XOBJSA) lltool.o
	$(LD) -g -O0 $(OBJS) lltool.o $(XOBJST) $(XOBJSR) $(XOBJSC) $(XOBJSA) -o lltool

clean:
	rm $(OBJS) $(XOBJST) $(XOBJSR) $(XOBJSC) *.o externals/crunch/crnlib/*.o



