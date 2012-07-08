#include <iostream>
#include <fstream>
#include <string>
#include "dirent.h"
#include <iomanip>
#include <cstdint>

using namespace std;

struct FCTX{
	uint8_t		magic[4];
	uint32_t	unknown1;
	uint32_t	unknown2;
	uint16_t	width;
	uint16_t	height;
	uint8_t		mipmaps1;
	uint8_t		mipmaps2;
	uint16_t	unknown3;
	uint8_t		fourcc[4];
//	uint16_t	unknown4;
//	uint16_t	unknown5;

};

int DDSD_CAPS			=	0x1;
int DDSD_HEIGHT			=	0x2;
int DDSD_WIDTH			=	0x4;
int DDSD_PITCH			=	0x8;
int DDSD_PIXELFORMAT	=	0x1000;
int DDSD_MIPMAPCOUNT	=	0x20000;
int DDSD_LINEARSIZE		=	0x80000;
int DDSD_DEPTH			=	0x800000;

int DDPF_ALPHAPIXELS	=	0x1;
int DDPF_ALPHA			=	0x2;
int DDPF_FOURCC			=	0x4;
int DDPF_RGB			=	0x40;
int DDPF_YUV			=	0x200;
int DDPF_LUMINANCE		=	0x20000;

int DDSCAPS_COMPLEX	= 0x8;
int DDSCAPS_MIPMAP	= 0x400000;
int DDSCAPS_TEXTURE	= 0x1000;

struct DDS_PIXELFORMAT {
	uint32_t	size;
	uint32_t	flags;
	uint32_t	fourCC;
	uint32_t	RGBBitCount;
	uint32_t	RBitMask;
	uint32_t	GBitMask;
	uint32_t	BBitMask;
	uint32_t	ABitMask;
};

struct DDS{
	uint32_t		magic;
	uint32_t		size;
	uint32_t		flags;
	uint32_t		height;
	uint32_t		width;
	uint32_t		pitchOrLinearSize;
	uint32_t		depth;
	uint32_t		mipMapCount;
	uint32_t		reserved1[11];
	DDS_PIXELFORMAT ddspf;
	uint32_t		caps;
	uint32_t		caps2;
	uint32_t		caps3;
	uint32_t		caps4;
	uint32_t		reserved2;
};

int BC1_BLOCKSIZE = 8;  //DXT1
int BC2_BLOCKSIZE = 16; //DXT3 (and DXT2)
int BC3_BLOCKSIZE = 16; //DXT5 (and DXT4)
int BC4_BLOCKSIZE = 8; 

#define DXTLENGTH(width, height, block_size) max(1, width / 4) * max(1, height / 4) * block_size
//f(n+1)=f(n)/2
#define MIPMAPSIZE(size, n) size * pow(2.0f,(1-n))

int main(int argc, char* argv[]){
	printf("Converts Funcom texture files to DDS /*TODO:*/and vice versa.\n");
	printf("Usage: %s directory\n",argv[0]);
	printf("New file is output in same directory as <filename>.dds or <filename>.fctx\n");
	printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\n");

	string directory = "C:/temp/rdb_extract/2/fctx/fctx1/";

	DIR *dir;
	struct dirent *file;

	dir = opendir(directory.c_str());

	if (dir == NULL) {
		printf("could not open dir");
		return 2;
	}

	struct DDS dds_header = {0};
	dds_header.magic = 0x20534444;
	dds_header.size = 124; //of header
	dds_header.flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT 
						| DDSD_MIPMAPCOUNT;
	dds_header.ddspf.size=0x20;
	dds_header.ddspf.flags=DDPF_FOURCC;	
	//This is a guess.
	dds_header.ddspf.RGBBitCount = 0x18;
	dds_header.ddspf.RBitMask = 0x00ff0000;
	dds_header.ddspf.GBitMask = 0x0000ff00;
	dds_header.ddspf.BBitMask = 0x000000ff;

	dds_header.caps=DDSCAPS_COMPLEX|DDSCAPS_MIPMAP|DDSCAPS_TEXTURE;

	while ((file = readdir (dir)) != NULL) {
		if(file->d_type == DT_REG){
			ifstream filestream(directory+file->d_name, ios::binary);
			
			if(!filestream){
				cerr << "could not open " << file->d_name << endl;
				continue;
			}

			struct FCTX header; 
			filestream.read(reinterpret_cast<char *>(&header), sizeof(header));

			if(memcmp(header.magic,"FCTX",4)!=0){
				continue;
			}

			cout << header.magic[0]<<header.magic[1]<<header.magic[2]<<header.magic[3]
				 << " " << header.unknown1
				 << " " << header.unknown2
				 << " " << setw(4)<<header.width << "x" << setw(4)<<header.height 
				 << " " << setw(2) << (int)header.mipmaps1<<"x"<<(int)header.mipmaps2 
				 << " " << file->d_name 
				 <<endl;

			if(header.mipmaps1!=header.mipmaps2){
				cerr<<"Strange mipmap count"<<endl;
			}

			if(header.unknown1 != 2){
				cerr<<"Strange version. (Is it really the version?)"<<endl;
			}

			if(header.unknown2 != 1 && header.unknown2 != 6){
				//Always 1, except in fctx1 where it is sometimes 6 and in rare cases 2.
				cerr<<"Strange important number.  Maybe this is a normal map? "<<endl;
			}

			dds_header.height = header.height;
			dds_header.width  = header.width;
			dds_header.mipMapCount = header.mipmaps1;
			memcpy(&dds_header.ddspf.fourCC, &header.fourcc, sizeof(dds_header.ddspf.fourCC));

			string output_name = file->d_name;
			output_name = output_name.substr(0,output_name.rfind('.'));

			ofstream output(directory+output_name+".dds", ios::binary);

			output.write(reinterpret_cast<char *>(&dds_header), sizeof(dds_header));

			bool inverse_sort_mipmaps=false;
			int blocksize = 0;
			if(memcmp(header.fourcc,"DXT1",sizeof(header.fourcc))==0){
				inverse_sort_mipmaps = true;
				blocksize = BC1_BLOCKSIZE;
			}else if(memcmp(header.fourcc,"DXT3",sizeof(header.fourcc))==0){
				inverse_sort_mipmaps = true;
				blocksize = BC2_BLOCKSIZE;
			}else if(memcmp(header.fourcc,"DXT5",sizeof(header.fourcc))==0){
				inverse_sort_mipmaps = true;
				blocksize = BC3_BLOCKSIZE;
			}

			if(inverse_sort_mipmaps){
				int seek=0;
				for(int i=header.mipmaps1; i>1; i--){
					cerr << (MIPMAPSIZE(header.width,i)) << " " << (DXTLENGTH(MIPMAPSIZE(header.width,i),MIPMAPSIZE(header.height,i),blocksize)) << endl;
					seek += DXTLENGTH(MIPMAPSIZE(header.width,i),MIPMAPSIZE(header.height,i),blocksize);
				}
				for(int i=1; i<=header.mipmaps1; i++){
					filestream.seekg(seek,ios::cur);
					int dxtlength = DXTLENGTH(MIPMAPSIZE(header.width,i),MIPMAPSIZE(header.height,i),blocksize);
					
					istreambuf_iterator<char> input_iterator(filestream);
					istreambuf_iterator<char> input_end(filestream);
					cerr << filestream.tellg() << " ";
					//This calls operator++ on the iterator, which in turn advances the streambuf.  Damn.
					//Ok, lets try something
					int pos = filestream.tellg();
					std::advance(input_end, dxtlength);
					filestream.seekg(pos);
					cerr << filestream.tellg() << " "; 
					ostreambuf_iterator<char> output_iterator(output); 
					
					

					copy(input_iterator, input_end, output_iterator);
					
					cerr << filestream.tellg() << endl;

					seek = -dxtlength - DXTLENGTH(MIPMAPSIZE(header.width,i-1),MIPMAPSIZE(header.height,i-1),blocksize);

					output.flush();
				}
			}else{
				//Aaaahhhaha the mipmaps are sorted ascending by size instead of descending.
				//Also, alpha seems to work occasionally(mostly?), even if the mask is not set!
				output << filestream.rdbuf();
			}

			output.close();

			filestream.close();
		}
	}

	closedir (dir);
}