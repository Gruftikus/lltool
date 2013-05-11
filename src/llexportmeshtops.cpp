#include "../include/llexportmeshtops.h"
#include "../include/llmaplist.h"

//constructor
llExportMeshToPs::llExportMeshToPs() : llTriMod() {
	SetCommandName("ExportMeshToPs");
}

int llExportMeshToPs::Prepare(void) {
	if (!llTriMod::Prepare()) return 0;

	filename = NULL;
	level    = 0;

	return 1;
}

int llExportMeshToPs::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-filename", &filename);
	RegisterValue("-sealevel", &level);
	
	return 1;
}

int llExportMeshToPs::Exec(void) {
	if (!llTriMod::Exec()) return 0;

	if (!Used("-filename"))
	    filename = (char *)"map.ps";

	float x00 = _llUtils()->x00;
	float x11 = _llUtils()->x11;
	float y00 = _llUtils()->y00;
	//float y11 = _llUtils()->y11;


	FILE *f;    
	if (fopen_s(&f, filename, "w")) {
		_llLogger()->WriteNextLine(-LOG_ERROR, "Unable to open ps file \"%s\"\n", filename);
		return 0;
	}

	fprintf(f,"%%! \n");	

	float scale = 500.f / (x11 - x00);
	for (unsigned int j=0; j<(unsigned int)triangles->GetN(); j++) {
	    	//llTriangle * tri = triangles->GetTriangle(j);

		int c1   = triangles->GetPoint1(j);
		int c2   = triangles->GetPoint2(j);
		int c3   = triangles->GetPoint3(j);
		float x1 = (points->GetX(c1)-x00)*scale+75;
		float y1 = (points->GetY(c1)-y00)*scale+200;
		float z1 = points->GetZ(c1);
		float x2 = (points->GetX(c2)-x00)*scale+75;
		float y2 = (points->GetY(c2)-y00)*scale+200;
		float z2 = points->GetZ(c2);
		float x3 = (points->GetX(c3)-x00)*scale+75;
		float y3 = (points->GetY(c3)-y00)*scale+200;
		float z3 = points->GetZ(c3);

		if (z1<=level && z2<=level && z3<=level) {
			fprintf(f,"newpath %f %f moveto %f %f lineto %f %f lineto 0.9 setgray fill\n",x1,y1,x2,y2,x3,y3);
		}

		fprintf(f," 0 setgray \n");

		fprintf(f,"newpath %f %f moveto %f %f lineto 0.1 setlinewidth stroke\n",x1,y1,x2,y2);
		fprintf(f,"newpath %f %f moveto %f %f lineto 0.1 setlinewidth stroke\n",x1,y1,x3,y3);
		fprintf(f,"newpath %f %f moveto %f %f lineto 0.1 setlinewidth stroke\n",x3,y3,x2,y2);

#if 0
		fprintf(f,"/Times-Roman findfont 8 scalefont setfont \n");	
		fprintf(f,"newpath %f %f moveto ",(x1+x2+x3)/3,(y1+y2+y3)/3);
		int q1 = GetTriangleQuality(j,0);
		fprintf(f,"(%i) show\n",q1);

		for (int i=0;i<3;i++) {
			int nei = tri->GetNeighbor(i);
			int neiflag = tri->GetEdgeFlag(i);
			if (nei>=0) {
				c1=GetCommonPoints(j,nei,0);
				c2=GetCommonPoints(j,nei,1);
			} else {
				c1=c2=-1;
			}

			if (c1 != -1 && c2 != -1 && neiflag == IS_OTHER_STRIP) {
				x1 = (points->GetX(c1)-ps_x00)*scale+75;
				y1 = (points->GetY(c1)-ps_y00)*scale+200;
				x2 = (points->GetX(c2)-ps_x00)*scale+75;
				y2 = (points->GetY(c2)-ps_y00)*scale+200;
				fprintf(f,"newpath %f %f moveto %f %f lineto 1.0 setlinewidth stroke\n",x1,y1,x2,y2);
			}

		} 
#endif	
	}
	fclose(f);

	return 1;
}
