#include "..\include\lltriangulation.h"
#include "..\include\llmaplist.h"

#define REAL double
#include "..\externals\triangle\triangle.h"

//constructor
llTriangulation::llTriangulation() : llMapWorker() {

	SetCommandName("MakeTriangulation");

}

int llTriangulation::RegisterOptions(void) {
	if (!llMapWorker::RegisterOptions()) return 0;
	return 1;
}

int llTriangulation::Init(void) {
	if (!llMapWorker::Init()) return 0;

	if (!points) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map [%s] has no point list", command_name, mapname);
		return 0;
	}

	if (!triangles) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "%s: map [%s] has no triangle list", command_name, mapname);
		return 0;
	}

	if (triangles->GetN()) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"%s: Triangulation already done.", command_name);
		return 0;
	}

	int good_flag=1;

	struct triangulateio in, mid, vorout;
	in.numberofpoints = points->GetN();
	in.numberofpointattributes = 0;
	in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
	in.pointattributelist = (REAL *) malloc(in.numberofpoints *
		in.numberofpointattributes *
		sizeof(REAL));
	in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));
	in.numberofsegments = 0;
	in.numberofholes = 0;
	in.numberofregions = 0;
	in.regionlist = (REAL *) malloc(in.numberofregions * 4 * sizeof(REAL));

	int rnum=0;
	for (int i = 0; i < points->GetN(); i++) {
		in.pointlist[rnum*2]     = double(points->GetX(i));
		in.pointlist[rnum*2+1]   = double(points->GetY(i));
		in.pointmarkerlist[rnum] = i;
		rnum++;
	}
	in.numberofpoints = rnum;

	_llLogger()->WriteNextLine(LOG_INFO,"Number of vertices forwarded to Delaunay algorithm: %i",rnum);

	mid.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
	/* Not needed if -N switch used or number of point attributes is zero: */
	mid.pointattributelist = (REAL *) NULL;
	mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
	mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
	/* Not needed if -E switch used or number of triangle attributes is zero: */
	mid.triangleattributelist = (REAL *) NULL;
	mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
	/* Needed only if segments are output (-p or -c) and -P not used: */
	mid.segmentlist = (int *) NULL;
	/* Needed only if segments are output (-p or -c) and -P and -B not used: */
	mid.segmentmarkerlist = (int *) NULL;
	mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
	mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */

	vorout.pointlist = (REAL *) NULL;        /* Needed only if -v switch used. */
	/* Needed only if -v switch used and number of attributes is not zero: */
	vorout.pointattributelist = (REAL *) NULL;
	vorout.edgelist = (int *) NULL;          /* Needed only if -v switch used. */
	vorout.normlist = (REAL *) NULL;         /* Needed only if -v switch used. */

	/* Triangulate the points.  Switches are chosen to read and write a  */
	/*   PSLG (p), preserve the convex hull (c), number everything from  */
	/*   zero (z), assign a regional attribute to each element (A), and  */
	/*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
	/*   neighbor list (n).   */

	_llLogger()->WriteNextLine(-LOG_INFO,"Call Triangle algorithm");
	_llLogger()->WriteNextLine(-LOG_INFO,"(written by by J. R. Shewchuk, see README)");

	//triangulate("pczAevn", &in, &mid, &vorout);
	triangulate("-YY -D -z -A -v -S0", &in, &mid, &vorout);

	int ntri = mid.numberoftriangles;

	_llLogger()->WriteNextLine(-LOG_INFO, "Number of triangles in entire worldspace: %i", ntri);

	for (int i = 0; i < ntri; i++) {
		int new_num1 = mid.trianglelist[i * 3];
		int new_num2 = mid.trianglelist[i * 3 + 1];
		int new_num3 = mid.trianglelist[i * 3 + 2];

		if (triangles->AddTriangle(new_num1, new_num2, new_num3) == -1) 
			good_flag = 0;
	}

	if (!good_flag) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"Triangulation (partly) failed.");
		return 0;
	}

	return 1;
}
