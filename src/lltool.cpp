
#include <iostream>
#include <math.h>

#include <windows.h>
#include <direct.h>

#include "../include/llutils.h"
#include "../include/llcreateworkers.h"
#include "../include/llcommands.h"
#include "../include/llquadlist.h"

//#define USE_CATCH 

void usage(void) {

	_llLogger()->WriteNextLine(LOG_INFO,"Usage: lltool -f \"flags,flags,flags,...\" batchfile");
	_llLogger()->WriteNextLine(LOG_INFO,"       batchfile: the name of the batch script");
	_llLogger()->WriteNextLine(LOG_INFO,"       flags: the flags which are propagated to the batch script");
	_llLogger()->Dump();

}

void DumpExit(void) {		
	_llLogger()->Dump();
	exit(-1);
}


//************************************************************************************

int main(int argc, char **argv) {

	llLogger   *mesg  = _llLogger();
	llUtils    *utils = _llUtils();
	llCommands *batch = new llCommands();

    std::cout << "" << std::endl;
	std::cout << "Written by gruftikus@github" << std::endl;
	std::cout << "V1.00, xx.xx.2013" << std::endl;
    std::cout << "***********************" << std::endl;

    //******************
    //read the arguments
    //******************

	if (argc<2) {
		usage();
		DumpExit();
	}

    char * batchname = argv[argc-1];

	for (int i=1; i<(argc-1); i++) {
		if (strcmp(argv[i],"-f")==0) {
			//flagliste holen
			char *ptr;          
			char *saveptr1 = NULL;
			ptr = strtok_int(argv[i+1], ',', &saveptr1);
			
			while(ptr != NULL) {
				char *my_flag_list=new char[strlen(ptr)+2];
				strcpy_s(my_flag_list,strlen(ptr)+1,ptr);
				ptr = strtok_int(NULL, ',', &saveptr1);
				utils->AddFlag(my_flag_list);

				mesg->WriteNextLine(LOG_INFO,"Flag: %s",my_flag_list);
			}
		}
	}

	mesg->Dump();		
	CreateWorkers(batch);
	mesg->Dump();

	//******************
    //open the batch
	//******************
	if (batchname) {
		if (!batch->Open(batchname,"[lltool]")) DumpExit();
		batch->ReadCache();
		batch->CompileScript();
	} else {
		batch->ReadStdin("[lltool]");
		batch->ReadCache();
		batch->CompileScript();
	}

	//float minab=256;
	_llUtils()->SetValue("_mindistance", "256");
	_llUtils()->SetValue("_cellsize_x", "4096");
	_llUtils()->SetValue("_cellsize_y", "4096");

	_llUtils()->SetValue("_quadsize_x", "4096");
	_llUtils()->SetValue("_quadsize_y", "4096");
	_llUtils()->SetValue("_quad_levels", "2");


	_llUtils()->SetValue("_dds_tool", "s3tc.exe");



	__int64 time_statistics[LLCOM_MAX_WORKERS];
	int time_statistics_cmd[LLCOM_MAX_WORKERS];
	char *time_statistics_cmdname[LLCOM_MAX_WORKERS];
	unsigned int time_statistics_pointer = 0;
	
	//******************
	//batch loop
	//******************


	int com;

	mesg->WriteNextLine(LOG_INFO,"****** Go into batch mode ******");

	while ((com = batch->GetCommand())>-2) {
		//cout << com << endl;
		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;
		BOOL res = GetSystemTimes( &idleTime, &kernelTime, &userTime );

		mesg->Dump();

#ifdef USE_CATCH
		try {
#endif


#if 0
		if (com == COM_SETOPTION) {
			if (batch->quadtreelevels > 1) {
				quadtreelevels = batch->quadtreelevels;
				batch->quadtreelevels = 1;
				mesg->WriteNextLine(LOG_INFO,"SetOption -quadtreelevels=%s", quadtreelevels);
				quads->SubQuadLevels(quadtreelevels - 1);
			}
			if (batch->mindistance>0) {
				mesg->WriteNextLine(LOG_INFO,"SetOption -mindistance=%i", batch->mindistance);
				minab = float(batch->mindistance);
			}
			if (batch->nquadmax>0) {
				mesg->WriteNextLine(LOG_INFO,"SetOption -nquadmax=%i", batch->nquadmax);
				quads->SetMaxPoints(batch->nquadmax);
			}
			opt_size_x = batch->size_x;
			opt_size_y = batch->size_y;
			opt_center = batch->center;
		}



		if ((com == COM_SETPOINTS) || (com == COM_SETPOINTSPERQUAD) || 
			(com == COM_SETMAXPOINTS) || (com == COM_SETMAXPOINTSPERQUAD)) {
				mesg->WriteNextLine(LOG_COMMAND,"%s: -n=%i", batch->CurrentCommand, batch->npoints);
				mesg->Dump();

				if (!heightmap) {
					mesg->WriteNextLine(LOG_FATAL,"No heightmap present.");
					DumpExit();
				}

				if (com == COM_SETPOINTSPERQUAD || com == COM_SETMAXPOINTSPERQUAD) quads->Reset();
setquadsloop:
				int mynum=batch->npoints;
				if (com == COM_SETMAXPOINTS) {
#if 0
					llquad * quad = 
						quads->GetQuad((batch->x00+batch->x11)/2, (batch->y00+batch->y11)/2);
					mynum -= quad->GetPoints();
#endif
					mynum -= gen_npoints;
					mesg->WriteNextLine(LOG_INFO,"Vertices left to be placed: %i",mynum);
				}

				llQuad * my_quad = NULL;
				double mean=0,num=0,num_real=0,empty=0;
				if (com == COM_SETPOINTSPERQUAD || com == COM_SETMAXPOINTSPERQUAD) {

					batch->x00 = float(quads->GetCurrentX())*batch->cellsize_x*batch->quadsize_x;
					batch->x11 = (float(quads->GetCurrentX())+1.f)*batch->cellsize_x*batch->quadsize_x;
					batch->y00 = float(quads->GetCurrentY())*batch->cellsize_y*batch->quadsize_y;
					batch->y11 = (float(quads->GetCurrentY())+1.f)*batch->cellsize_y*batch->quadsize_y;
					batch->quadx = float(quads->GetCurrentX());
					batch->quady = float(quads->GetCurrentY());

					mesg->WriteNextLine(LOG_INFO,"Current quad: x=%.0f,y=%.0f",batch->quadx,batch->quady);

					if (com == COM_SETMAXPOINTSPERQUAD) {
						my_quad = 
							quads->GetQuad((batch->x00+batch->x11)/2, (batch->y00+batch->y11)/2);
						mynum -= my_quad->GetNumPoints();
						mesg->WriteNextLine(LOG_INFO,"Vertices left to be placed: %i",mynum);
					}

					if (batch->x00 < _llMapList()->GetX1()) batch->x00 = _llMapList()->GetX1();
					if (batch->y00 < _llMapList()->GetY1()) batch->y00 = _llMapList()->GetY1();
					if (batch->x11 > _llMapList()->GetX2()) batch->x11 = _llMapList()->GetX2();
					if (batch->y11 > _llMapList()->GetY2()) batch->y11 = _llMapList()->GetY2();
					int mynum2 = int(float(mynum)*((batch->x11-batch->x00)*(batch->y11-batch->y00))/(batch->cellsize_x*batch->quadsize_x*batch->cellsize_y*batch->quadsize_y));
					if (mynum2<mynum)
						mesg->WriteNextLine(LOG_INFO,"Partly filled quad: vertices reduced to: %i",mynum2);
					mynum=mynum2;
				}

				if (alg_list->GetSize() == 0) {
					mesg->WriteNextLine(LOG_ERROR,"%s: no algorithm specified", batch->CurrentCommand);
					goto end;
				}

				heightmap->InitRnd(heightmap->GetRawX(batch->x00), heightmap->GetRawY(batch->y00),
					heightmap->GetRawX(batch->x11), heightmap->GetRawY(batch->y11));

				for (int num_point=0;num_point<mynum;num_point++) {	    
					int maxtry=0,maxtry_total=0;
					mesg->Dump();
					if ((num_point % 1000) == 0 && num_point) 
						mesg->WriteNextLine(LOG_INFO,"[%i]",num_point);
			
loop:	    
					//float x = float((batch->x11 - batch->x00) * float(rand())/float(RAND_MAX)) + batch->x00;
					float x = heightmap->GetCoordRndX();
					//float y = float((batch->y11 - batch->y00) * float(rand())/float(RAND_MAX)) + batch->y00;
					float y = heightmap->GetCoordRndY();

					
					float z=heightmap->GetZ(x,y);

					double ceiling;
					double value;

					alg_list->GetValue(x,y,&value,&ceiling);

					if (empty>1000) {
						mesg->WriteNextLine(LOG_WARNING,"This quad seems to be empty, skipped after %i vertices",num_point);
						goto end;
					} else if (value<0.0000001) { //filter very small
						empty++;
						goto loop; 
					}

//					if (heightmap->IsDefault(x,y)) //NaN
//						goto loop;


					empty=0;
					mean+=value;num++;
					float idealdist = 4096.f- (((4096.f-minab)/float(mean/num)) * float(value));

					if (idealdist < minab) idealdist = minab;

					if (ceiling>(10.f*mean/num)) ceiling=(10.f*mean/num);  //cutoff -> BUGBUG
					double test = double(rand())/double(RAND_MAX) * ceiling;
					if (test>value) { 
						goto loop; 
					}
					float mingrid = points->GetMinDistanceGrid(x,y);
					float maxradius = idealdist + mingrid;
					if (mingrid > minab) {
						float mindist = points->GetMinDistance(x, y, maxradius, my_quad); //time consuming!!!
						if (mindist > minab || mindist < 0) {
						//if (mindist >= 0) {

							test = 2 * float(rand())/float(RAND_MAX) * idealdist;
							if (test> (mindist + mingrid))
								goto loop; 

							maxtry=0;
							//all conditions fulfilled
							points->AddPoint(x,y,z);
							gen_npoints++;
							num_real++;
						} else { //1 -> see below
							if (maxtry<50 && maxtry_total<100*batch->npoints) {
								maxtry++;
								maxtry_total++;
								goto loop;
							} else {
								mesg->WriteNextLine(LOG_WARNING,"Mesh is too dense: quad aborted after %i vertices",num_point);
								goto end;
							}
						}
					} else { //2 -> this was done 2x on purpose to save time
						if (maxtry<50 && maxtry_total<100*batch->npoints) {
							maxtry++;
							maxtry_total++;
							goto loop;
						} else {
							mesg->WriteNextLine(LOG_WARNING,"Mesh is too dense: quad aborted after %i vertices",num_point);
							goto end;
						}
					}
				}
end:
				mesg->Dump();
				if (com == COM_SETPOINTSPERQUAD || com == COM_SETMAXPOINTSPERQUAD) if (quads->GetNextQuad()) goto setquadsloop;
		}
#endif

#ifdef USE_CATCH
		} catch (char * str) {
			if (batch->CurrentCommand)
				mesg->WriteNextLine(LOG_FATAL,"Catched exception [%s] in [%s]", str, batch->CurrentCommand);
			else 
				mesg->WriteNextLine(LOG_FATAL,"Catched exception [%s]", str);
			DumpExit();
		} catch (int str) {
			if (batch->CurrentCommand)
				mesg->WriteNextLine(LOG_FATAL,"Catched exception [%i] in [%s]", str, batch->CurrentCommand);
			else
				mesg->WriteNextLine(LOG_FATAL,"Catched exception [%i]", str);
			DumpExit();
		} catch (...) {
			if (batch->CurrentCommand)
				mesg->WriteNextLine(LOG_FATAL,"Catched exception in [%s]", batch->CurrentCommand);
			else
				mesg->WriteNextLine(LOG_FATAL,"Catched exception");
			DumpExit();
		}
#endif

		mesg->Dump();

		FILETIME userTime_old = userTime;

		res = GetSystemTimes( &idleTime, &kernelTime, &userTime );

		BOOL found = false;
		for (unsigned int ii=0; ii < time_statistics_pointer;ii++) {
			if (com == time_statistics_cmd[ii]) {
				ULARGE_INTEGER u1 = { userTime.dwLowDateTime, userTime.dwHighDateTime }; 
				ULARGE_INTEGER u2 = { userTime_old.dwLowDateTime, userTime_old.dwHighDateTime }; 
				time_statistics[ii] = u1.QuadPart - u2.QuadPart;
				found = true;
			}
		}
		if (!found && batch->CurrentCommand) {
			ULARGE_INTEGER u1 = { userTime.dwLowDateTime, userTime.dwHighDateTime }; 
			ULARGE_INTEGER u2 = { userTime_old.dwLowDateTime, userTime_old.dwHighDateTime }; 
			time_statistics[time_statistics_pointer] = u1.QuadPart - u2.QuadPart;
			time_statistics_cmd[time_statistics_pointer] = com;
			time_statistics_cmdname[time_statistics_pointer] = new char[strlen(batch->CurrentCommand)+1];
			strcpy_s(time_statistics_cmdname[time_statistics_pointer],strlen(batch->CurrentCommand)+1,batch->CurrentCommand);
			time_statistics_pointer++;
		}
	}

	std::cout << "****** Batch loop done ******" << std::endl;

	for (unsigned int ii=0; ii < time_statistics_pointer;ii++) {
		for (unsigned int jj=0; jj < time_statistics_pointer - 1;jj++) {
			if (time_statistics[jj] < time_statistics[jj+1]) {
				__int64 time_statistics_tmp = time_statistics[jj];
				char * time_statistics_cmdname_tmp = time_statistics_cmdname[jj];
				time_statistics[jj] = time_statistics[jj+1];
				time_statistics_cmdname[jj] = time_statistics_cmdname[jj+1];
				time_statistics[jj+1] = time_statistics_tmp;
				time_statistics_cmdname[jj+1] = time_statistics_cmdname_tmp;
			}
		}
	}

	std::cout << "User time per command (sorted):" << std::endl;
	for (unsigned int ii=0; ii < time_statistics_pointer;ii++) {
		if (time_statistics[ii] > 1000000)
			std::cout << time_statistics_cmdname[ii] << ": " << (((double)time_statistics[ii]) /10000000.)<< " s" << std::endl;
	}

	return 0;

}
