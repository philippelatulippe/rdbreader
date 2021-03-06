/*
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

#include "common.h"

uint32 Buffer::read(ifstream* idx)
{
	uint32 ret = 0;
	uint8 a = idx->get();
	uint8 b = idx->get();
	uint8 c = idx->get();
	uint8 d = idx->get();
	
	ret+= (a &0xFF);
	ret&=0x000000ff;
	ret+= (b &0xFF)*0x100;
	ret&=0x0000ffff;
	ret+= (c &0xFF)*0x10000;
	ret&=0x00ffffff;
	ret+= (d &0xFF)*0x1000000;
	if(!idx->good())
	{
		printf("Error with files\n");
	}
	return ret;
}

string Buffer::read(ifstream* idx, unsigned int size, bool swap)
{
	string ret = "";
	for(unsigned int i = 0; i< size; i++)
	{
		ret += idx->get();
	}
	if(ret.size() > 0)
	{
		if(swap)
		{
			char x;
			for(unsigned int i = 0; i< size/2; i++)
			{
				x = ret[i];
				ret[i] = ret[size-1-i];
				ret[size-1-i] = x;
			}
		}
	}
	return ret;
}