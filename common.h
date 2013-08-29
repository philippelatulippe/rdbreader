/*
This file is from:

Project Faolan a Simple and Free Server Emulator for Age of Conan.
Copyright (C) 2009, 2010, 2011, 2012 The Project Faolan Team

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
using namespace std;

#include "structs.h"

#define VERSION "v0.2"

namespace Buffer
{
	uint32 read(ifstream* idx);
	string read(ifstream* idx, unsigned int size, bool swap = true);
}
