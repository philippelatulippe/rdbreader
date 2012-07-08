/*
 * Things to do:
 1000010 seems to have filenames:
struct filename_list_header {
	uint32_t type;
	uint32_t unknown;
	uint32_t unknown;
	uint32_t type_count;
}

struct filename_list_type{
	uint32_t type;
	uint32_t name_count;
}

struc filename_list_name{
	uint32_t id;
	uint32_t length;
	char* name;
}

  This will have to be a two-pass process then.
  Of course, they're probably all bunched up in the same RDB file,
  but I'd rather stick with using the index.
  Unless there's only one file?  *TODO*: check this.

  *
  * LMXB files are binary XML files.  With a string pool at the bottom.
  * Looking at that pool, these are the same types of documents as the 
  * plain-text XML ones.  Why have both?  Are they the same?
  *
  * A preliminary search reveals that 358180 (xml) is identical to the 
  * start of 376465 (bxml) except that the BXML file is much longer.
  *
  * The bxml files have what seem to be pointers to the string pools.
  * But their LSB is off!
  *
 */

#include <errno.h>
#include "common.h"
#include "config.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#define _mkdir(a) mkdir(a, S_IRWXU | S_IRWXG | S_IRWXO)
#endif

using namespace std;

#define RDBDATA_FILECOUNT 30

int main(int argc, char *argv[])
{
	printf("Reader for Funcom RDB %s <https://github.com/philippelatulippe/rdbreader>\n", VERSION);
	printf("Options are set at compile time, see config.h.\n");
	printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\n");
	
	int mkdirreturn = _mkdir(output_dir.c_str());
	if(mkdirreturn < 0 && errno != EEXIST){
		printf("Could not create output directory\n");
		return 2;
	}

	ifstream idx("./RDB/le.idx",ios::binary);

	if(!idx){
		printf("Can't open idx file.  It should be at ./RDB/le.idx\n");
		return 3;
	}
	
	vector<TypId> DS;
	Header Hdr;
	Hdr.version = 0;
	Hdr.hash1 = 0;
	Hdr.hash2 = 0;
	Hdr.hash3 = 0;
	Hdr.hash4 = 0;
	Hdr.recordCount = 0;
	uint32 pointer = 0;
		
	Hdr.magic = Buffer::read(&idx, 4);
	printf("Get Header: %s\n", Hdr.magic.c_str());
	Hdr.version = Buffer::read(&idx);
	printf("Version: %i\n", Hdr.version);
		

	if(!(Hdr.version == 5 || Hdr.version == 7)){
		printf("RDBReader does not support this version of the format.\n");	
		printf("It supports version 5 (Age of Conan) and version 7 (The Secret World).\n");	
		return 4;
	}

	Hdr.hash1 = Buffer::read(&idx);
	Hdr.hash2 = Buffer::read(&idx);
	Hdr.hash3 = Buffer::read(&idx);
	Hdr.hash4 = Buffer::read(&idx);
	Hdr.recordCount = Buffer::read(&idx);

	printf("%i records found\n", Hdr.recordCount);
	if(Hdr.recordCount <= 0){
		printf("Got wrong recordCounter\n");
		return 4;
	}


	for(unsigned int i = 0; i < Hdr.recordCount; i++)
	{
		TypId tmp;
		tmp.type = Buffer::read(&idx);
		tmp.idx = Buffer::read(&idx);
		DS.push_back(tmp);
	}

	for(unsigned int i = 0; i < Hdr.recordCount; i++)
	{
		DS[i].unk0 = Buffer::read(&idx);
		DS[i].offset = Buffer::read(&idx);
		DS[i].size = Buffer::read(&idx); 
		DS[i].unk3 = Buffer::read(&idx);
		DS[i].unk4 = Buffer::read(&idx);
		DS[i].unk5 = Buffer::read(&idx);
		DS[i].unk6 = Buffer::read(&idx);
		if(Hdr.version == 5){
			DS[i].unk7 = Buffer::read(&idx);
			DS[i].unk8 = Buffer::read(&idx);
			DS[i].unk9 = Buffer::read(&idx);
			DS[i].unk10 = Buffer::read(&idx);
		}


		/*
		if(!((DS[i].unk0 & 0xff) >=0 && (DS[i].unk0 & 0xff) < RDBDATA_FILECOUNT)){
			printf("%u - %u - %u\n%u - %u - %u - %u - %u - %u - %u - %u - %u - %u - %u\n", i, DS[i].idx, DS[i].type,
				DS[i].unk0, DS[i].offset, DS[i].size, DS[i].unk3, DS[i].unk4, DS[i].unk5, DS[i].unk6, DS[i].unk7, DS[i].unk8, DS[i].unk9, DS[i].unk10);
			;
		}
		*/
	}

	for(unsigned int i = 0; i < Hdr.recordCount; i++)
	{
		char* rdbnr = new char[3];
		sprintf(rdbnr, "%02i", (DS[i].unk0 & 0xff));
		if(!((DS[i].unk0 & 0xff) >=0 && (DS[i].unk0 & 0xff) < RDBDATA_FILECOUNT)){
			//This record does not seem to point to an RDB file.
			continue;
		}
		
		//types that are prefixed by unknown bytes
		int skipWords = 0;
		if( DS[i].type == STATICAIPOINSEXPORT
		 || DS[i].type == CLIMBINGAREASEXPORT
		 || DS[i].type == CONTENTSPAWNDATAEXPORT
		 || DS[i].type == ACGSPAWNDATAEXPORT
		 || DS[i].type == NPCSPAWNDATAEXPORT
		 || DS[i].type == TREASUREDATAEXPORT
		 || DS[i].type == SCRYSCRIPTDATAEXPORT
		 || DS[i].type == SCRYSCRIPTDATAEXPORT2	
		 || DS[i].type == DESTRUCTIBLECLIENTDATAEXPORT
		 || DS[i].type == RESURRECTIONPOINTSEXPORT
		 || DS[i].type == SIMPLEDYNELDATAEXPORT
		 || DS[i].type == NOTEPADDATAEXPORT
		 || DS[i].type == CLIMBINGDYNELDATAEXPORT		
		 || DS[i].type == OCEANVOLUMESROOTNODEEXPORT
		 || DS[i].type == VOLUMETRICFOGSROOTNODEEXPORT
		 || DS[i].type == PATHFINDINGAREASROOTNODEEXPORT
		 || DS[i].type == DOORDYNELDATAEXPORT
		 || DS[i].type == TEXTSCRIPT
		 || DS[i].type == SCRIPTGROUP
		 || DS[i].type == SCRIPTGROUP2
		 || DS[i].type == SCRIPTGROUP3
		 || DS[i].type == COLLECTIONLIBRARY
		 || DS[i].type == RIVERS
		 || DS[i].type == PATROLS
		 || DS[i].type == BOUNDEDAREAS
		 || DS[i].type == STATICAIPOINTS
		 || DS[i].type == STATICAIPOINTS2
		 || DS[i].type == PNG
		 || DS[i].type == PNG2
		 || DS[i].type == PNG3
		 || DS[i].type == PNG4
		 || DS[i].type == PNG5
		 || DS[i].type == PNG6
		 || DS[i].type == ANIMSYS
		 || DS[i].type == MPEG
		 || DS[i].type == FCTX2
		 || DS[i].type == DECALPROJECTORS
		 || DS[i].type == EFFECTPACKAGE
		 || DS[i].type == SOUNDENGINE
		 || DS[i].type == MESHINDEXFILE
		 || DS[i].type == MONSTERDATA
		 || DS[i].type == BCTGROUP
		 || DS[i].type == BCTMESH
		 || DS[i].type == MOVEMENTSET
		 || DS[i].type == CARS
		 || DS[i].type == ROOMANDPORTALSYSTEM
		 || DS[i].type == FORMATION
		 || DS[i].type == CHARACTERCREATOR
		 || DS[i].type == BOUNDEDAREACOLLECTIONS
		 || DS[i].type == ENVIRONMENTSETTINGS
		 || DS[i].type == SKYDOME
		 || DS[i].type == PLAYFIELDDESCRIPTIONDATA
/*		 || DS[i].type == 
		 || DS[i].type == 
		 || */
		){
			skipWords=3;
		}

		//Set directory
		string directory1 = "unknown/";
		string directory2 = "unknown/";
		string fileExt = ".dat";
		bool typeUnknown = true;
		switch(DS[i].type){
		case STATICAIPOINSEXPORT:
			directory1 = "xml/"; 
			directory2 = "staticaipoinsexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case CLIMBINGAREASEXPORT:
			directory1 = "xml/"; 
			directory2 = "climbingareasexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case CONTENTSPAWNDATAEXPORT:
			directory1 = "xml/"; 
			directory2 = "contentspawndataexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case ACGSPAWNDATAEXPORT:
			directory1 = "xml/"; 
			directory2 = "acgspawndataexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case NPCSPAWNDATAEXPORT:
			directory1 = "xml/"; 
			directory2 = "npcspawndataexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case TREASUREDATAEXPORT:
			directory1 = "xml/"; 
			directory2 = "treasuredataexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case SCRYSCRIPTDATAEXPORT:
			directory1 = "xml/"; 
			directory2 = "scryscriptdataexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case SCRYSCRIPTDATAEXPORT2:
			directory1 = "xml/"; 
			directory2 = "scryscriptdataexport2/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case DESTRUCTIBLECLIENTDATAEXPORT:
			directory1 = "xml/"; 
			directory2 = "destructibleclientdataexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case RESURRECTIONPOINTSEXPORT:
			directory1 = "xml/"; 
			directory2 = "resurrectionpointsexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case SIMPLEDYNELDATAEXPORT:
			directory1 = "xml/"; 
			directory2 = "simpledyneldataexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case NOTEPADDATAEXPORT:
			directory1 = "xml/"; 
			directory2 = "notepaddataexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case CLIMBINGDYNELDATAEXPORT:
			directory1 = "xml/"; 
			directory2 = "climbingdyneldataexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case OCEANVOLUMESROOTNODEEXPORT:
			directory1 = "xml/"; 
			directory2 = "oceanvolumesrootnodeexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case VOLUMETRICFOGSROOTNODEEXPORT:
			directory1 = "xml/"; 
			directory2 = "volumetricfogsrootnodeexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case PATHFINDINGAREASROOTNODEEXPORT:
			directory1 = "xml/"; 
			directory2 = "pathfindingareasrootnodeexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case DOORDYNELDATAEXPORT:
			directory1 = "xml/"; 
			directory2 = "doordyneldataexport/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case TEXTSCRIPT:
			directory1 = "xml/"; 
			directory2 = "textscript/";
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case SCRIPTGROUP3:
			directory1 = "xml/";
			directory2 = "scriptgroup3/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case SCRIPTGROUP2:
			directory1 = "xml/";
			directory2 = "scriptgroup2/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case COLLECTIONLIBRARY:
			directory1 = "xml/";
			directory2 = "collectionlibrary/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case STATICAIPOINTS2:
			directory1 = "xml/";
			directory2 = "staticapoints2/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case RIVERS:
			directory1 = "xml/";
			directory2 = "rivers/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case PATROLS:
			directory1 = "xml/";
			directory2 = "patrols/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case BOUNDEDAREAS:
			directory1 = "xml/";
			directory2 = "boundedareas/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case STATICAIPOINTS:
			directory1 = "xml/";
			directory2 = "staticaipoints/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case PNG:
			directory1 = "png/";
			directory2 = "png/"; 
			fileExt = ".png";
			typeUnknown = false;
			break;
		case PNG2:
			directory1 = "png/";
			directory2 = "png2/"; 
			fileExt = ".png";
			typeUnknown = false;
			break;
		case PNG3:
			directory1 = "png/";
			directory2 = "png3/"; 
			fileExt = ".png";
			typeUnknown = false;
			break;
		case PNG4:
			directory1 = "png/";
			directory2 = "png4/"; 
			fileExt = ".png";
			typeUnknown = false;
			break;
		case PNG5:
			directory1 = "png/";
			directory2 = "png5/"; 
			fileExt = ".png";
			typeUnknown = false;
			break;
		case PNG6:
			directory1 = "png/";
			directory2 = "png6/"; 
			fileExt = ".png";
			typeUnknown = false;
			break;
		case SCRIPTGROUP:
			directory1 = "xml/";
			directory2 = "scriptgroup/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case ANIMSYS:
			directory1 = "xml/";
			directory2 = "animsys/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case PHYSX30COLLECTION:
			directory1 = "xml/";
			directory2 = "physx30collection/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case OGG1:
			directory1 = "ogg/";
			directory2 = "sfx/";
			fileExt = ".ogg";
			typeUnknown = false;
			break;
		case OGG3:
			directory1 = "ogg/";
			directory2 = "music/"; 
			fileExt = ".ogg";
			typeUnknown = false;
			break;
		case MPEG:
			//Strange files that contain a strange MPEG stream.
			directory1 = "mpeg/";
			directory2 = "mpeg/"; 
			fileExt = ".mpeg";
			typeUnknown = false;
			break;
		case JPEG:
			directory1 = "jpeg/";
			directory2 = "jpeg/"; 
			fileExt = ".jpg";
			typeUnknown = false;
			break;
		case FCTX1:
			//These seem to encapsulate strange DXT-compressed textures.
			//A few of them are actually TGAs, some are FCTX with an
			//unknown compression format.  What is the pixel format?
			//Some are DDS with the 3 word special header.
			directory1 = "fctx/";
			directory2 = "fctx1/"; 
			fileExt = ".fctx";
			typeUnknown = false;
			break;
		case FCTX2:
			directory1 = "fctx/";
			directory2 = "fctx2/"; 
			fileExt = ".fctx";
			typeUnknown = false;
			break;
		case FCTX3:
			directory1 = "fctx/";
			directory2 = "fctx3/"; 
			fileExt = ".fctx";
			typeUnknown = false;
			break;
		case FCTX4:
			directory1 = "fctx/";
			directory2 = "fctx4/"; 
			fileExt = ".fctx";
			typeUnknown = false;
			break;
		case FCTX5:
			directory1 = "fctx/";
			directory2 = "fctx5/"; 
			fileExt = ".fctx";
			typeUnknown = false;
			break;
		case DECALPROJECTORS:
			directory1 = "xml/";
			directory2 = "decalprojectors/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case EFFECTPACKAGE:
			directory1 = "xml/";
			directory2 = "effectpackage/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case SOUNDENGINE:
			directory1 = "xml/";
			directory2 = "soundengine/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case MESHINDEXFILE:
			directory1 = "xml/";
			directory2 = "meshindexfile/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case TIFF:
			directory1 = "tiff/";
			directory2 = "tiff/"; 
			fileExt = ".tiff";
			typeUnknown = false;
			break;
		case MONSTERDATA:
			directory1 = "xml/";
			directory2 = "monsterdata/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case BCTGROUP:
			directory1 = "xml/";
			directory2 = "bctgroup/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case BCTMESH:
			directory1 = "xml/";
			directory2 = "bctmesh/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case MOVEMENTSET:
			directory1 = "xml/";
			directory2 = "movementset/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case CARS:
			directory1 = "xml/";
			directory2 = "cars/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case LUAARCHIVE:
			directory1 = "archive/";
			directory2 = "lua/"; 
			fileExt = ".dat";
			typeUnknown = false;
			break;
		case ROOMANDPORTALSYSTEM:
			directory1 = "xml/";
			directory2 = "roomandportalsystem/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case FORMATION:
			directory1 = "xml/";
			directory2 = "formation/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case CHARACTERCREATOR:
			directory1 = "xml/";
			directory2 = "charactercreator/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case BOUNDEDAREACOLLECTIONS:
			directory1 = "xml/";
			directory2 = "boundedareacollections/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case ENVIRONMENTSETTINGS:
			directory1 = "xml/";
			directory2 = "environmentsettings/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case SKYDOME:
			directory1 = "xml/";
			directory2 = "skydome/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
		case LIPSYNC_AND_VOICE:
			//Familiar: http://falloutmods.wikia.com/wiki/LIP_File_Format
			directory1 = "ogg/";
			directory2 = "lip/"; 
			fileExt = ".lip";
			typeUnknown = false;
			break;
		case PLAYFIELDDESCRIPTIONDATA:
			directory1 = "xml/";
			directory2 = "playfielddescriptiondata/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;
/*		case :
			directory1 = "xml/";
			directory2 = "/"; 
			fileExt = ".xml";
			typeUnknown = false;
			break;*/
		}

		//Check for types that were disabled in config.h
		bool skipType = false;
		if( (DS[i].type == STATICAIPOINSEXPORT && !staticaipoinsexport)
		 || (DS[i].type == CLIMBINGAREASEXPORT && !climbingareasexport)
		 || (DS[i].type == CONTENTSPAWNDATAEXPORT && !contentspawndataexport)
		 || (DS[i].type == ACGSPAWNDATAEXPORT && !acgspawndataexport)
		 || (DS[i].type == NPCSPAWNDATAEXPORT && !npcspawndataexport)
		 || (DS[i].type == TREASUREDATAEXPORT && !treasuredataexport)
		 || (DS[i].type == SCRYSCRIPTDATAEXPORT && !scryscriptdataexport)
		 || (DS[i].type == SCRYSCRIPTDATAEXPORT2 && !scryscriptdataexport2)
		 || (DS[i].type == DESTRUCTIBLECLIENTDATAEXPORT && !destructibleclientdataexport)
		 || (DS[i].type == RESURRECTIONPOINTSEXPORT && !resurrectionpointsexport)
		 || (DS[i].type == SIMPLEDYNELDATAEXPORT && !simpledyneldataexport)
		 || (DS[i].type == NOTEPADDATAEXPORT && !notepaddataexport)
		 || (DS[i].type == CLIMBINGDYNELDATAEXPORT && !climbingdyneldataexport)
		 || (DS[i].type == OCEANVOLUMESROOTNODEEXPORT && !oceanvolumesrootnodeexport)
		 || (DS[i].type == VOLUMETRICFOGSROOTNODEEXPORT && !volumetricfogsrootnodeexport)
		 || (DS[i].type == PATHFINDINGAREASROOTNODEEXPORT && !pathfindingareasrootnodeexport)
		 || (DS[i].type == DOORDYNELDATAEXPORT && !doordyneldataexport)
		 || (DS[i].type == TEXTSCRIPT && !textscript)
		 || (DS[i].type == SCRIPTGROUP3 && !scriptgroup)
		 || (DS[i].type == SCRIPTGROUP2 && !scriptgroup)
		 || (DS[i].type == COLLECTIONLIBRARY && !collectionlibrary)
		 || (DS[i].type == STATICAIPOINTS2 && !staticapoints2)
		 || (DS[i].type == RIVERS && !rivers)
		 || (DS[i].type == PATROLS && !patrols)
		 || (DS[i].type == BOUNDEDAREAS && !boundedareas)
		 || (DS[i].type == STATICAIPOINTS && !staticaipoints)
		 || (DS[i].type == PNG && !exportPNG)
		 || (DS[i].type == PNG2 && !exportPNG2)
		 || (DS[i].type == PNG3 && !exportPNG3)
		 || (DS[i].type == PNG4 && !exportPNG4)
		 || (DS[i].type == PNG5 && !exportPNG5)
		 || (DS[i].type == PNG6 && !exportPNG6)
		 || (DS[i].type == SCRIPTGROUP && !scriptgroup)
		 || (DS[i].type == ANIMSYS  && !animsys)
		 || (DS[i].type == PHYSX30COLLECTION  && !physx30collection)
		 || (DS[i].type == OGG1  && !ogg1)
		 || (DS[i].type == OGG3  && !ogg3)
		 || (DS[i].type == MPEG  && !mpeg)
		 || (DS[i].type == JPEG  && !jpeg)
		 || (DS[i].type == FCTX1  && !fctx1)
		 || (DS[i].type == FCTX2  && !fctx2)
		 || (DS[i].type == FCTX3  && !fctx3)
		 || (DS[i].type == FCTX4  && !fctx4)
		 || (DS[i].type == FCTX5  && !fctx5)
		 || (DS[i].type == DECALPROJECTORS && !decalprojectors)
		 || (DS[i].type == EFFECTPACKAGE && !effectpackage)
		 || (DS[i].type == SOUNDENGINE && !soundengine)
		 || (DS[i].type == MESHINDEXFILE && !meshindexfile)
		 || (DS[i].type == TIFF && !tiff)
		 || (DS[i].type == MONSTERDATA && !monsterdata)
		 || (DS[i].type == BCTGROUP && !bctgroup)
		 || (DS[i].type == BCTMESH && !bctmesh)
		 || (DS[i].type == MOVEMENTSET && !movementset)
		 || (DS[i].type == CARS && !cars)
		 || (DS[i].type == LUAARCHIVE && !luaarchive)
		 || (DS[i].type == ROOMANDPORTALSYSTEM && !roomandportalsystem)
		 || (DS[i].type == FORMATION && !formation)
		 || (DS[i].type == CHARACTERCREATOR && !charactercreator)
		 || (DS[i].type == BOUNDEDAREACOLLECTIONS && !boundedareacollections)
		 || (DS[i].type == ENVIRONMENTSETTINGS && !environmentsettings)
		 || (DS[i].type == SKYDOME && !skydome)
		 || (DS[i].type == LIPSYNC_AND_VOICE && !lipsync_and_voice)
		 || (DS[i].type == PLAYFIELDDESCRIPTIONDATA && !playfielddescriptiondata)
		 || (typeUnknown == true && !unknown)
		){
			skipType = true;
		}

		if(!skipType){
			string rdbfile = "./RDB/";
			rdbfile += rdbnr;
			rdbfile+=".rdbdata";
			ifstream rdbf(rdbfile.c_str(), ios::binary);
			if(rdbf){
				string file = output_dir+directory1;
				_mkdir(file.c_str());
				
				if(typeUnknown){
					char* typedir = new char[7];
					sprintf(typedir, "%07i", DS[i].type);
					file += typedir;
				}else{
					file += directory2;
				}
				int mkdirvalue = _mkdir(file.c_str());
				/*
				//Only extract one file of each type
				if(mkdirvalue != 0){
					rdbf.close();
					continue;
				}
				*/

				rdbf.seekg(DS[i].offset);

				if(DS[i].type == FCTX1){
					const int magicSize = 4;
					unsigned char magic[magicSize];
					rdbf.read(reinterpret_cast<char *>(&magic),magicSize);
					rdbf.seekg(-magicSize,ios_base::cur);
					//Assume 12-byte funcom header is present
					rdbf.seekg(12,ios_base::cur);
					unsigned char magic2[magicSize];
					rdbf.read(reinterpret_cast<char *>(&magic2),magicSize);
					rdbf.seekg(-12-magicSize,ios_base::cur);

					if(memcmp(magic,"FCTX",4) == 0){
						//already set
					}else if(memcmp(magic,"DDS ",4)==0){
						fileExt = ".dds";
					}else if(memcmp(magic2,"DDS ",4)==0){
						fileExt = ".dds";
						skipWords = 3;
					}else if(magic[0]==0x00 && magic[1]<10 && magic[2]<12){
						fileExt = ".tga";
					}else{
						fileExt = ".dat";
					}
				}

				file += "/RDB";
				file += rdbnr;
				file +="_";
				char* fnr = new char[10];
				sprintf(fnr, "%010i", i);
				file += fnr;
				file += fileExt;
				
				if(DS[i].idx == 207762 || DS[i].idx == 206540){
					cout << "BREAK OUT!" << endl;
				}

				for(int sk=0; sk<skipWords; sk++){
					Buffer::read(&rdbf);
				}
				
				ofstream outstream(file.c_str() ,ios::binary);
				if(outstream){
					//Is there a better way that uses C++ streams?
					for(unsigned l = 0; l < (DS[i].size - skipWords*4); l++){
						outstream << (unsigned char)rdbf.get();
					}
					outstream.close();
				}
			}
			rdbf.close();
		}


	}
}