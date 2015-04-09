/*
 * Copyright 2012 Philippe Latulippe
 * https://github.com/philippelatulippe/rdbreader

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <fstream>
#ifdef _WIN32
#include "dirent.h"
#else
#include <dirent.h>
#endif
#include <iomanip>
#include <cstdint>
#include <cstdio> 

#include <string>
#include <cstring>

#include <math.h>
#include <algorithm>

using namespace std;

struct FCTX{
	uint8_t		magic[4];
	uint32_t	unknown1; //Perhaps an image type (e.g. normal map)
	uint32_t	unknown2; //Most likely the version
	uint16_t	width;
	uint16_t	height;
	uint8_t		mipmaps1;
	uint8_t		mipmaps2;
	uint16_t	unknown3; //always zero
	uint8_t		fourcc[4];

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

#define DXTLENGTH(width, height, block_size) max(1.0f, width / 4) * max(1.0f, height / 4) * block_size
//f(n+1)=f(n)/2
#define MIPMAPSIZE(size, n) size * pow(2.0f,(1.0f-n))

int main(int argc, char* argv[]){
	printf("Converts Funcom texture files to DDS\n");
	printf("Usage: %s directory\n",argv[0]);
	printf("New file is output in same directory as <filename>.dds or <filename>.fctx\n");
	printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\n");

	string directory = "./";

	if(argc == 2){
		directory = argv[1];
	}

	char buf[BUFSIZ];
    size_t size;

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
			//ifstream filestream(directory+file->d_name, ios::binary);
			FILE* filestream = fopen((directory+file->d_name).c_str(), "rb");
			
			if(!filestream){
				cerr << "could not open " << file->d_name << endl;
				continue;
			}

			struct FCTX header; 
			fread(&header, 1, sizeof(header), filestream);

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

			if(header.unknown3 != 0){
				cerr<<"Strangeness in the header."<<endl;
			}

			dds_header.height = header.height;
			dds_header.width  = header.width;
			dds_header.mipMapCount = header.mipmaps1;
			memcpy(&dds_header.ddspf.fourCC,
				&header.fourcc,
				sizeof(dds_header.ddspf.fourCC));


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
			}else if(memcmp(header.fourcc,"MIXD",sizeof(header.fourcc))==0){
				//This doesn't quite work.  Most of the images are corrupted,
				//all the images appear in greyscale, the images with recogni-
				//sable textures have a block offset.
				//TODO:
				//1. check the block misalignment.
				//     strange: the smaller mipmaps are working. (but broken 
				//     in the same fashion)
				//2. Play around with the colour masks in the DDS header. (but
				//     remember they might be greyscale textures!  But 
				//     probably not considering the dark area.)
				//3. Perhaps the unknown byte in the FCTX header correlates 
				//     with the completely corrupt images.
				//4. Maybe I'm using the wrong blocksize?
				inverse_sort_mipmaps = true;
				memcpy(&dds_header.ddspf.fourCC,"ATI1",sizeof(dds_header.ddspf.fourCC));
				blocksize = BC4_BLOCKSIZE;
			}


			string output_name = file->d_name;
			output_name = output_name.substr(0,output_name.rfind('.'));

			ofstream output(directory+output_name+".dds", ios::binary);

			output.write(reinterpret_cast<char *>(&dds_header), 
				sizeof(dds_header));

			if(inverse_sort_mipmaps){
				int seekto=0;
				for(int i=header.mipmaps1; i>1; i--){
					seekto += DXTLENGTH(MIPMAPSIZE(header.width,i),MIPMAPSIZE(header.height,i),blocksize);
				}

				for(int i=1; i<=header.mipmaps1; i++){
					fseek(filestream, seekto, SEEK_CUR);
					unsigned int dxtlength = DXTLENGTH(	MIPMAPSIZE(header.width,i),MIPMAPSIZE(header.height,i),blocksize);
					
					size_t readsize = BUFSIZ;
					unsigned int bytesread=0;
					if(dxtlength<readsize){
						readsize = dxtlength;
					}
					while((size = fread(buf, 1, readsize, filestream))){
						bytesread += size;
						//mixing C's files API with C++ streams :-\ .
						output.write(buf,size);

						if(dxtlength < (bytesread+BUFSIZ)){
							readsize = dxtlength - bytesread;
						}
					}
					
					seekto = -dxtlength - (int)(DXTLENGTH(MIPMAPSIZE(header.width,i-1),MIPMAPSIZE(header.height,i-1),blocksize));
					
					output.flush();
				}
			}else{
				size_t size;
				while((size = fread(buf, 1, BUFSIZ, filestream))){
					output.write(buf,size);
				}
			}

			output.close();

			fclose(filestream);
		}
	}

	closedir (dir);
}
