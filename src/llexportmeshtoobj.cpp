#include "../include/llexportmeshtoobj.h"
#include "../include/llmaplist.h"

//constructor
llExportMeshToObj::llExportMeshToObj() : llTriMod() {
	SetCommandName("ExportMeshToObj");

	newpoints    = NULL;
	newtriangles = NULL;
}

int llExportMeshToObj::Prepare(void) {
	if (!llTriMod::Prepare()) return 0;

	filename        = NULL;
	texname         = NULL;
	mtlname         = NULL;
	createpedestals = 0;
	trans_x = trans_y = trans_z = 0;

	return 1;
}

int llExportMeshToObj::RegisterOptions(void) {
	if (!llTriMod::RegisterOptions()) return 0;

	RegisterValue("-filename",        &filename);
	RegisterValue("-mtlfilename",     &mtlname);
	RegisterValue("-texname",         &texname);
	RegisterValue("-transx",          &trans_x);
	RegisterValue("-transy",          &trans_y);
	RegisterValue("-transz",          &trans_z);
	RegisterFlag ("-createpedestals", &createpedestals);
	
	return 1;
}

int llExportMeshToObj::MakeSelection() {
	float lowestz = 0; //required for pedestals

	float x00 = _llUtils()->x00;
	float x11 = _llUtils()->x11;
	float y00 = _llUtils()->y00;
	float y11 = _llUtils()->y11;

	//make sure that the selection has a boundary:
	triangles->DivideAt(true,  x00, map);    
    triangles->DivideAt(true,  x11, map);  
    triangles->DivideAt(false, y00, map);    
    triangles->DivideAt(false, y11, map);  

	points->ClearSecondaryList();

	//copy pointlist, make the matching old->new:
	if (newpoints) delete newpoints;
	newpoints = new llPointList(points->GetN(), NULL);
	
	newpoints->SetTexAnchor(x00, y00, x11, y11);
	newpoints->ClearSecondaryList();

	for (int i=0; i<points->GetN(); i++) {
		if (points->GetX(i) >= x00 && points->GetX(i)<= x11 &&
			points->GetY(i) >= y00 && points->GetY(i)<= y11) {

				float z = map->GetZ(points->GetX(i), points->GetY(i));
				int newp = newpoints->AddPoint(points->GetX(i), points->GetY(i), z);
				if (z < lowestz) 
					lowestz = z;
				points->SetSecondary(i, newp);
		}
	}

	if (!newpoints->GetN()) {
		_llLogger()->WriteNextLine(LOG_WARNING, "The mesh %s is empty and was therefore not written", filename);
	    delete newpoints;
	    return 0;
	}

	//Make temporary triangle list
	if (newtriangles) delete newtriangles;
	newtriangles = new llTriangleList(newpoints->GetN(), newpoints);

	if (lowestz > 0) 
		lowestz = 0;
	else 
		lowestz -= 100;

	for (int i=0; i<triangles->GetN(); i++) {

		int old1 = triangles->GetPoint1(i);
		int old2 = triangles->GetPoint2(i);
		int old3 = triangles->GetPoint3(i);
		int new1 = points->GetSecondary(old1);
		int new2 = points->GetSecondary(old2);
		int new3 = points->GetSecondary(old3);

		if (new1 > -1 && new2 > -1 && new3 > -1 && triangles->GetTriangle(i)->write_flag) {
			newtriangles->AddTriangle(new1, new2, new3);

			//add optional pedestals
			if (createpedestals) {
				int p1=-1, p2=-1, opt=-1;
				if (fabs(newpoints->GetX(new1)-x00)<1.f && fabs(newpoints->GetX(new2)-x00)<1.f) {p1=new1; p2=new2; opt=0;}
				if (fabs(newpoints->GetX(new1)-x00)<1.f && fabs(newpoints->GetX(new3)-x00)<1.f) {p1=new1; p2=new3; opt=0;}
				if (fabs(newpoints->GetX(new2)-x00)<1.f && fabs(newpoints->GetX(new3)-x00)<1.f) {p1=new2; p2=new3; opt=0;}
				if (fabs(newpoints->GetX(new1)-x11)<1.f && fabs(newpoints->GetX(new2)-x11)<1.f) {p1=new1; p2=new2; opt=1;}
				if (fabs(newpoints->GetX(new1)-x11)<1.f && fabs(newpoints->GetX(new3)-x11)<1.f) {p1=new1; p2=new3; opt=1;}
				if (fabs(newpoints->GetX(new2)-x11)<1.f && fabs(newpoints->GetX(new3)-x11)<1.f) {p1=new2; p2=new3; opt=1;}

				if (p1 > -1) {
					int p3 = newpoints->GetPoint(newpoints->GetX(p1), newpoints->GetY(p1), lowestz);
					int p4 = newpoints->GetPoint(newpoints->GetX(p2), newpoints->GetY(p2), lowestz);
					if (p3 < 0) {
						p3 = newpoints->AddPoint(newpoints->GetX(p1), newpoints->GetY(p1), lowestz);
					}
					if (p4 < 0) {
						p4 = newpoints->AddPoint(newpoints->GetX(p2), newpoints->GetY(p2), lowestz);
					}
					if (opt==0 && newpoints->GetY(p1)>newpoints->GetY(p2)) {
						newtriangles->AddTriangle(p1, p3, p4);
						newtriangles->AddTriangle(p2, p1, p4);
					}
					if (opt==0 && newpoints->GetY(p1)<newpoints->GetY(p2)) {
						newtriangles->AddTriangle(p1, p4, p3);
						newtriangles->AddTriangle(p1, p2, p4);
					}
					if (opt==1 && newpoints->GetY(p1)>newpoints->GetY(p2)) {
						newtriangles->AddTriangle(p1, p4, p3);
						newtriangles->AddTriangle(p1, p2, p4);
					}
					if (opt==1 && newpoints->GetY(p1)<newpoints->GetY(p2)) {
						newtriangles->AddTriangle(p1, p3, p4);
						newtriangles->AddTriangle(p2, p1, p4);
					}
				}

				if (fabs(newpoints->GetY(new1)-y00)<1.f && fabs(newpoints->GetY(new2)-y00)<1.f) {p1=new1; p2=new2; opt=2;}
				if (fabs(newpoints->GetY(new1)-y00)<1.f && fabs(newpoints->GetY(new3)-y00)<1.f) {p1=new1; p2=new3; opt=2;}
				if (fabs(newpoints->GetY(new2)-y00)<1.f && fabs(newpoints->GetY(new3)-y00)<1.f) {p1=new2; p2=new3; opt=2;}
				if (fabs(newpoints->GetY(new1)-y11)<1.f && fabs(newpoints->GetY(new2)-y11)<1.f) {p1=new1; p2=new2; opt=3;}
				if (fabs(newpoints->GetY(new1)-y11)<1.f && fabs(newpoints->GetY(new3)-y11)<1.f) {p1=new1; p2=new3; opt=3;}
				if (fabs(newpoints->GetY(new2)-y11)<1.f && fabs(newpoints->GetY(new3)-y11)<1.f) {p1=new2; p2=new3; opt=3;}

				if (p1 >- 1) {
					int p3 = newpoints->GetPoint(newpoints->GetX(p1), newpoints->GetY(p1), lowestz);
					int p4 = newpoints->GetPoint(newpoints->GetX(p2), newpoints->GetY(p2), lowestz);
					if (p3 < 0) {
						p3 = newpoints->AddPoint(newpoints->GetX(p1), newpoints->GetY(p1), lowestz);
					}
					if (p4 < 0) {
						p4 = newpoints->AddPoint(newpoints->GetX(p2), newpoints->GetY(p2), lowestz);
					}
					
					if (opt==2 && newpoints->GetX(p1)<newpoints->GetX(p2)) {
						newtriangles->AddTriangle(p1, p3, p4);
						newtriangles->AddTriangle(p2, p1, p4);
					}
					if (opt==2 && newpoints->GetX(p1)>newpoints->GetX(p2)) {
						newtriangles->AddTriangle(p1, p4, p3);
						newtriangles->AddTriangle(p1, p2, p4);
					}
					if (opt==3 && newpoints->GetX(p1)<newpoints->GetX(p2)) {
						newtriangles->AddTriangle(p1, p4, p3);
						newtriangles->AddTriangle(p1, p2, p4);
					}
					if (opt==3 && newpoints->GetX(p1)>newpoints->GetX(p2)) {
						newtriangles->AddTriangle(p1, p3, p4);
						newtriangles->AddTriangle(p2, p1, p4);
					}
				}
			}
		}
	}

	newpoints->Resize();
	newpoints->Translation(trans_x, trans_y, trans_z);

	return 1;
}

int llExportMeshToObj::Exec(void) {
	if (!llTriMod::Exec()) return 0;

	if (!Used("-filename"))
	    filename = (char *)"map.obj";
	if (!Used("-mtlfilename"))
		mtlname = (char *)"map.mtl";

	if (!MakeSelection()) return 0;

	//look for _install_dir:
	if (_llUtils()->GetValue("_install_dir")) {
		char *filename_tmp = new char[strlen(filename) + strlen(_llUtils()->GetValue("_install_dir")) + 2];
		sprintf_s(filename_tmp, strlen(filename) + strlen(_llUtils()->GetValue("_install_dir")) + 2, "%s\\%s", 
			_llUtils()->GetValue("_install_dir"), filename);
		filename = filename_tmp;
	}

	//Now the obj-specific part:
	FILE *fptr;

	//mtl file:
	if (texname) {
		if (fopen_s(&fptr, mtlname,"w")) {
			_llLogger()->WriteNextLine(-LOG_ERROR,"Unable to open MTL file '%s'\n", mtlname);
			return 0;
		}
		fprintf(fptr, "newmtl material0\n");
		fprintf(fptr, "Ka 1.000000 1.000000 1.000000\n");
		fprintf(fptr, "Kd 1.000000 1.000000 1.000000\n");
		fprintf(fptr, "Ks 0.000000 0.000000 0.000000\n");
		fprintf(fptr, "Tr 1.000000\n");
		fprintf(fptr, "illum 1\n");
		fprintf(fptr, "Ns 0.000000\n");
		fprintf(fptr, "map_Kd %s\n", texname);
		fclose(fptr);
		fptr = NULL;
	}

	//obj-file:
	if (fopen_s(&fptr, filename,"w")) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"Unable to open OBJ file '%s'\n", filename);
		return 0;
	}

	if (texname) {
		fprintf(fptr, "mtllib %s\n", mtlname);
	}

	//Vertices:
	for (int i=0; i<newpoints->GetN(); i++) {
		fprintf(fptr, "v %f %f %f\n", newpoints->GetX(i), newpoints->GetY(i), newpoints->GetZ(i));
	}

	fprintf(fptr, "\n");

	//Normals:
	for (int i=0; i<newpoints->GetN(); i++) {
		fprintf(fptr, "vn 0.0 0.0 1.0\n");
	}

	fprintf(fptr, "\n");

	//Tex-coords:
	if (texname) {
		for (int i=0; i<newpoints->GetN(); i++) {
			fprintf(fptr, "vt %f %f\n", newpoints->GetU(i), newpoints->GetV(i));
		}
	}

	fprintf(fptr, "\n");

	//Faces:
	if (texname) {
		for (int i=0; i<newtriangles->GetN(); i++) {
			fprintf(fptr, "f %i/%i/%i %i/%i/%i %i/%i/%i \n", newtriangles->GetPoint1(i)+1, newtriangles->GetPoint1(i)+1, newtriangles->GetPoint1(i)+1,
				newtriangles->GetPoint2(i)+1, newtriangles->GetPoint2(i)+1, newtriangles->GetPoint2(i)+1, 
				newtriangles->GetPoint3(i)+1, newtriangles->GetPoint3(i)+1, newtriangles->GetPoint3(i)+1);
		}
	} else {
		for (int i=0; i<newtriangles->GetN(); i++) {
			fprintf(fptr, "f %i/%i/ %i/%i/ %i/%i/ \n", newtriangles->GetPoint1(i)+1, newtriangles->GetPoint1(i)+1,
				newtriangles->GetPoint2(i)+1, newtriangles->GetPoint2(i)+1, 
				newtriangles->GetPoint3(i)+1, newtriangles->GetPoint3(i)+1);
		}
	}

	fclose(fptr);

	if (_llUtils()->GetValue("_install_dir")) {
		delete filename;
	}

	return 1;
}
