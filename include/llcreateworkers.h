

#include "../include/llworker.h"
#include "../include/llmap.h"
#include "../include/llcommands.h"

//Workers:
#include "../include/llcreatemap.h"
#include "../include/llselectrec.h"
#include "../include/llexit.h"
#include "../include/llsetflag.h"
#include "../include/llecho.h"

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

//Map-based workers:
#include "../include/llmapworker.h"
#include "../include/lldiamondsquare.h"
#include "../include/llfillcolormap.h"
#include "../include/llfilter.h"
#include "../include/llexportmap.h"
#include "../include/llmakederivatives.h"
#include "../include/llquaditerator.h"
#include "../include/llsetheight.h"
#include "../include/llselectall.h"
#include "../include/lltriangulation.h"

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

//Triangle-modifiers:
#include "../include/lltrimod.h"
#include "../include/llactivatevisiblevertices.h"
#include "../include/llremoveinactivetriangles.h"
#include "../include/llsplitat.h"
#include "../include/llsplitatgrid.h"
#include "../include/llsplitatpolygonborder.h"
#include "../include/llsplitbetween.h"
#include "../include/llstencilpolygon.h"

//Triangle-Exporters:
#include "../include/llexportmeshtoobj.h"
#include "../include/llexportmeshtops.h"

void CreateWorkers(llCommands *_batch) {

	//Workers:
	_batch->RegisterWorker(new llSelectRec());
	_batch->RegisterWorker(new llExit());
	_batch->RegisterWorker(new llSetFlag());
	_batch->RegisterWorker(new llEcho());

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

	_batch->RegisterWorker(new llCreateMap());

	//Map worker:
	_batch->RegisterWorker(new llExportMap());
	_batch->RegisterWorker(new llQuadIterator());
	_batch->RegisterWorker(new llSetHeight());
	_batch->RegisterWorker(new llSelectAll());



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

	_batch->RegisterWorker(new llTriangulation());

	_batch->RegisterWorker(new llSplitAt());
	_batch->RegisterWorker(new llSplitAtGrid());
	_batch->RegisterWorker(new llSplitBetween());
	_batch->RegisterWorker(new llSplitAtPolygonBorder());
	_batch->RegisterWorker(new llStencilPolygon());
	_batch->RegisterWorker(new llRemoveInactiveTriangles());
	_batch->RegisterWorker(new llActivateVisibleVertices());

	_batch->RegisterWorker(new llExportMeshToObj());
	_batch->RegisterWorker(new llExportMeshToPs());

}