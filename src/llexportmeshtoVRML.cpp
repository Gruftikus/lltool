#include "../include/llexportmeshtoVRML.h"

#include "../../lltool/include/llmaplist.h"

//constructor
llExportMeshToVRML::llExportMeshToVRML() : llExportMeshToObj() {
	SetCommandName("ExportMeshToVRML");

}

int llExportMeshToVRML::Prepare(void) {
	if (!llExportMeshToObj::Prepare()) return 0;

	return 1;
}

int llExportMeshToVRML::RegisterOptions(void) {
	if (!llExportMeshToObj::RegisterOptions()) return 0;

	return 1;
}


int llExportMeshToVRML::Exec(void) {
	if (!llExportMeshToObj::Exec()) return 0;

	if (!Used("-filename"))
	    filename = (char *)"map.nif";
	if (!MakeSelection()) return 0;

	//look for _install_dir:
	if (_llUtils()->GetValue("_install_dir")) {
		char *filename_tmp = new char[strlen(filename) + strlen(_llUtils()->GetValue("_install_dir")) + 2];
		sprintf_s(filename_tmp, strlen(filename) + strlen(_llUtils()->GetValue("_install_dir")) + 2, "%s\\%s", 
			_llUtils()->GetValue("_install_dir"), filename);
		filename = filename_tmp;
	}

	//Now the VRML-specific part:
	FILE *fptr;

	if (fopen_s(&fptr, filename, "w")) {
		_llLogger()->WriteNextLine(-LOG_ERROR,"Unable to open VRML file '%s'\n", filename);
		return 0;
	}

	fprintf(fptr, "#VRML V2.0 utf8\n");
	fprintf(fptr, "DEF MATERIAL Material {\n");
	fprintf(fptr, "   diffuseColor 1 1 1\n");
	fprintf(fptr, "}\n");
	fprintf(fptr, "DEF ORITGT Transform {\n");
	fprintf(fptr, "	 rotation 0.5774 0.5774 0.5774 -2.094\n");
	fprintf(fptr, "	 scale 1 1 1\n");
	fprintf(fptr, "	 children [\n");
	fprintf(fptr, "	 Shape {\n");
	fprintf(fptr, "		 appearance Appearance {\n");
	fprintf(fptr, "			 material USE MATERIAL\n");

	if (texname) {
		fprintf(fptr, "texture ImageTexture {\n");
		fprintf(fptr, "	 url \"%s\"\n", texname);
		fprintf(fptr, "}\n");
	}

	fprintf(fptr, "geometry IndexedFaceSet {\n");
	fprintf(fptr, "		 creaseAngle 3.1415\n");

	fprintf(fptr, "coord Coordinate {\n");
	fprintf(fptr, "    point [\n");

	//Vertices:
	for (int i=0; i<newpoints->GetN(); i++) {
		fprintf(fptr, "%f %f %f\n", newpoints->GetX(i), newpoints->GetY(i), newpoints->GetZ(i));
	}

	fprintf(fptr, "] \n }\n");

	//Tex-coords:
	if (texname) {
		fprintf(fptr, "texCoord TextureCoordinate  {\n");
		fprintf(fptr, "    point [\n");
		for (int i=0; i<newpoints->GetN(); i++) {
			fprintf(fptr, "%f %f\n", newpoints->GetU(i), newpoints->GetV(i));
		}
		fprintf(fptr, "] \n }\n");
	}

	fprintf(fptr, "coordIndex [ \n");

	for (int i=0; i<newtriangles->GetN(); i++) {
			fprintf(fptr, "%i %i %i -1 \n", newtriangles->GetPoint1(i),
				newtriangles->GetPoint2(i),
				newtriangles->GetPoint3(i));
	}

	fprintf(fptr, "]\n");

	if (texname) {
		fprintf(fptr, "texCoordIndex  [ \n");
		for (int i=0; i<newtriangles->GetN(); i++) {
			fprintf(fptr, "%i %i %i -1 \n", newtriangles->GetPoint1(i),
				newtriangles->GetPoint2(i),
				newtriangles->GetPoint3(i));
		}
		fprintf(fptr, "] \n");
	}



	if (_llUtils()->GetValue("_install_dir")) {
		delete filename;
	}


	fprintf(fptr, " } \n } \n ] \n } \n");


	return 1;
}
