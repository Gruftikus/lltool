

#include "../include/llworker.h"
#include "../include/llmap.h"
#include "../include/llcommands.h"

//Workers:
#include "../include/llcreatemap.h"
#include "../include/llselectrec.h"
#include "../include/llexit.h"
#include "../include/lllogfile.h"
#include "../include/llsetflag.h"
#include "../include/llecho.h"
#include "../include/llsetpath.h"
#include "../include/llmkdir.h"
#ifdef _MSC_VER
#include "../include/llfileiterator.h"
#include "../include/llmatchregex.h"
#include "../include/llcopyfile.h"
#endif

//Alg-based:
#include "../include/llalg.h"
#include "../include/llalglist.h"
#include "../include/llalgconst.h"
#include "../include/llalgfirst.h"
#include "../include/llalgpeakfinder.h"
#include "../include/llalgradial.h"
#include "../include/llalgsecond.h"
#include "../include/llalgslope.h"
#include "../include/llalgstripe.h"
#ifdef _MSC_VER
#include "../include/llalgequation.h"
#endif

//Map-based workers:
#include "../include/llmapworker.h"
#include "../include/llcreatenormalmap.h"
#include "../include/lldiamondsquare.h"
#include "../include/llfillcolormap.h"
#include "../include/llfilter.h"
#include "../include/llexportmap.h"
#include "../include/llexportmaptodds.h"
#include "../include/llimportmap.h"
#include "../include/llimportmapfromdds.h"
#include "../include/llmakederivatives.h"
#include "../include/llquaditerator.h"
#include "../include/llcelliterator.h"
#include "../include/llsetheight.h"
#include "../include/llgetheight.h"
#include "../include/llselectall.h"
#include "../include/lltriangulation.h"
#include "../include/llcopymap.h"
#include "../include/lldeletemap.h"
#include "../include/llscalemap.h"

//Vertex-setters:
#include "../include/llset.h"
#include "../include/llsetalgvertices.h"
#include "../include/lladdvertextopolygon.h"
#include "../include/llcreatepolygon.h"
#include "../include/llpanorama.h"
#include "../include/llreaddatafile.h"
#include "../include/llreadpolygondatafile.h"
#include "../include/llsetatgridline.h"
#include "../include/llsetcontour.h"
#include "../include/llsetgrid.h"
#include "../include/llsetgridborder.h"
#include "../include/llsetvertex.h"
#include "../include/lladddiscontinuityline.h"
#include "../include/lladddiscontinuitygrid.h"
#include "../include/llremoveinactivevertices.h"
#include "../include/llactivatefractionofvertices.h"


//Triangle-modifiers:
#include "../include/lltrimod.h"
#include "../include/llinactivateallvertices.h"
#include "../include/llactivatevisiblevertices.h"
#include "../include/llremoveinactivetriangles.h"
#include "../include/llsplitat.h"
#include "../include/llsplitatz.h"
#include "../include/llsplitatgrid.h"
#include "../include/llsplitatpolygonborder.h"
#include "../include/llsplitbetween.h"
#include "../include/llstencilpolygon.h"
#include "../include/llremovetriangulation.h"
#include "../include/llsorttriangles.h"

//Triangle-Exporters:
#include "../include/llexportmeshtoobj.h"
#include "../include/llexportmeshtoVRML.h"
#include "../include/llexportmeshtops.h"

void CreateWorkers(llCommands *_batch) {

	//Workers:
	_batch->RegisterWorker(new llSelectRec());
	_batch->RegisterWorker(new llExit());
	_batch->RegisterWorker(new llLogFile());
	_batch->RegisterWorker(new llSetFlag());
	_batch->RegisterWorker(new llEcho());
	_batch->RegisterWorker(new llSetPath());
	_batch->RegisterWorker(new llMkDir());
#ifdef _MSC_VER	
	_batch->RegisterWorker(new llMatchRegEx());
	_batch->RegisterWorker(new llFileIterator());
	_batch->RegisterWorker(new llCopyFile());
#endif

	//Alg-based:
	_llAlgList()->AddAlgCollection("_heightmap_alg", new llAlgCollection());
	_batch->RegisterWorker(new llCreateAlgCollection());

	_batch->RegisterWorker(new llAlgConst     ("_heightmap_alg", NULL));
	_batch->RegisterWorker(new llAlgFirst     ("_heightmap_alg", "_heightmap_filtered"));
	_batch->RegisterWorker(new llAlgSecond    ("_heightmap_alg", "_heightmap_filtered"));
	_batch->RegisterWorker(new llAlgRadial    ("_heightmap_alg", NULL));
	_batch->RegisterWorker(new llAlgSlope     ("_heightmap_alg", "_heightmap"));
	_batch->RegisterWorker(new llAlgStripe    ("_heightmap_alg", "_heightmap"));
	_batch->RegisterWorker(new llAlgPeakFinder("_heightmap_alg", "_heightmap"));
#ifdef _MSC_VER
	_batch->RegisterWorker(new llAlgEquation  ("_heightmap_alg", NULL));
#endif

	_batch->RegisterWorker(new llCreateMap());

	//Map worker:
	_batch->RegisterWorker(new llCreateNormalMap());
	_batch->RegisterWorker(new llExportMap());
	_batch->RegisterWorker(new llExportMapToDDS());
	_batch->RegisterWorker(new llImportMap());
	_batch->RegisterWorker(new llImportMapFromDDS());
	_batch->RegisterWorker(new llQuadIterator());
	_batch->RegisterWorker(new llCellIterator());
	_batch->RegisterWorker(new llSetHeight());
	_batch->RegisterWorker(new llGetHeight());
	_batch->RegisterWorker(new llSelectAll());
	_batch->RegisterWorker(new llCopyMap());
	_batch->RegisterWorker(new llDeleteMap());
	_batch->RegisterWorker(new llScaleMap());

	_batch->RegisterWorker(new llFillColorMap());

	_batch->RegisterWorker(new llDiamondSquare());
	_batch->RegisterWorker(new llFilter());
	_batch->RegisterWorker(new llMakeDerivatives());

	_batch->RegisterWorker(new llSetVertex());
	_batch->RegisterWorker(new llSetAlgVertices());
	_batch->RegisterWorker(new llReadDataFile());
	_batch->RegisterWorker(new llReadPolygonDataFile());
	_batch->RegisterWorker(new llSetGrid());
	_batch->RegisterWorker(new llSetGridBorder());
	_batch->RegisterWorker(new llSetAtGridLine());
	_batch->RegisterWorker(new llSetContour());
	_batch->RegisterWorker(new llPanorama());
	_batch->RegisterWorker(new llCreatePolygon());
	_batch->RegisterWorker(new llAddVertexToPolygon());
	_batch->RegisterWorker(new llRemoveInactiveVertices());
	_batch->RegisterWorker(new llActivateFractionOfVertices());
	
	_batch->RegisterWorker(new llAddDiscontinuityLine());
	_batch->RegisterWorker(new llAddDiscontinuityGrid());

	_batch->RegisterWorker(new llTriangulation());

	_batch->RegisterWorker(new llSplitAt());
	_batch->RegisterWorker(new llSplitAtZ());
	_batch->RegisterWorker(new llSplitAtGrid());
	_batch->RegisterWorker(new llSplitBetween());
	_batch->RegisterWorker(new llSplitAtPolygonBorder());
	_batch->RegisterWorker(new llStencilPolygon());
	_batch->RegisterWorker(new llInactivateAllVertices());
	_batch->RegisterWorker(new llRemoveInactiveTriangles());
	_batch->RegisterWorker(new llActivateVisibleVertices());
	_batch->RegisterWorker(new llRemoveTriangulation());
	_batch->RegisterWorker(new llSortTriangles());

	_batch->RegisterWorker(new llExportMeshToObj());
	_batch->RegisterWorker(new llExportMeshToVRML());
	_batch->RegisterWorker(new llExportMeshToPs());

}
