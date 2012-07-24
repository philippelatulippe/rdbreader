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
#include <cstdint>
#include <string>
#include <cstring>

using namespace std;

struct LIP{
	uint8_t		magic[4];
	uint32_t	audio_offset; //+8 bytes
	uint32_t	interesting; //Nah, forget about it.
};

int main(int argc, char* argv[]){
	printf("Extract audio from LIP files, and eventually maybe figure out LIP files themselves.\n");
	//Not really, that's a project for another day.
	printf("Usage: %s directory\n",argv[0]);
	printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\n");

	string directory = "./";

	if(argc == 2){
		directory = argv[1];
	}

	DIR *dir;
	struct dirent *file;

	dir = opendir(directory.c_str());

	while ((file = readdir (dir)) != NULL) {
		if(file->d_type == DT_REG){
			ifstream filestream(directory+file->d_name,ios::binary);

			if(!filestream){
				cerr << "could not open " << file->d_name << endl;
				continue;
			}

			struct LIP header; 

			filestream.read(reinterpret_cast<char *>(&header), 
				sizeof(header));

			if(memcmp(header.magic,"LIP ",sizeof(header.magic))!=0){
				continue;
			}

			filestream.seekg(header.audio_offset+8);

			string output_name = file->d_name;
			output_name = output_name.substr(0,output_name.rfind('.'));

			ofstream output(directory+output_name+".ogg", ios::binary);

			output << filestream.rdbuf();

			output.close();
			filestream.close();
		}
	}
}
