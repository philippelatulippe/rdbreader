/*
 * Copyright 2012 Philippe Latulippe
 * https://github.com/philippelatulippe/rdbreader

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

enum TYPES
{
	SCRIPTGROUP						= 0,
	ANIMSYS							= 1010200,
	STATICAIPOINTS					= 1000046,
	BOUNDEDAREAS					= 1000053,
	PATROLS							= 1000067,
	STATICAIPOINSEXPORT				= 1000068,
	CLIMBINGAREASEXPORT				= 1000070,
	RIVERS							= 1000075,
	CONTENTSPAWNDATAEXPORT			= 1000077,
	ACGSPAWNDATAEXPORT				= 1000078,
	NPCSPAWNDATAEXPORT				= 1000079,
	TREASUREDATAEXPORT				= 1000083,
	SCRYSCRIPTDATAEXPORT			= 1000084,
	SCRYSCRIPTDATAEXPORT2			= 1000085,
	DESTRUCTIBLECLIENTDATAEXPORT	= 1000088,
	RESURRECTIONPOINTSEXPORT		= 1000090,
	SIMPLEDYNELDATAEXPORT			= 1000091,
	NOTEPADDATAEXPORT				= 1000093,
	CLIMBINGDYNELDATAEXPORT			= 1000095,
	OCEANVOLUMESROOTNODEEXPORT		= 1000097,
	VOLUMETRICFOGSROOTNODEEXPORT	= 1000098,
	PATHFINDINGAREASROOTNODEEXPORT	= 1000100,
	DOORDYNELDATAEXPORT				 = 1000101,

	STATICAIPOINTS2 = 1000103,
	PNG = 1010008,
	COLLECTIONLIBRARY = 1010014,
	SCRIPTGROUP2 = 1010028,
	SCRIPTGROUP3 = 1010029,

	TEXTSCRIPT = 1010035,

	PNG2 = 1066603,

	//The secret world
	PHYSX30COLLECTION = 1000007,

	DECALPROJECTORS = 1000519,
	EFFECTPACKAGE = 1000622,
	SOUNDENGINE = 1000623,

	MPEG = 1000635,
	
	FCTX1 = 1010004,
	FCTX2 = 1010006,
	FCTX3 = 1010218,
	FCTX4 = 1066606,
	FCTX5 = 1066610,

	JPEG = 1010013,

	MESHINDEXFILE = 1010030,

	TIFF = 1010042, //The TIFF I checked had an invalid version number?!  Is it not always 42?

	MONSTERDATA = 1010201,
	BCTGROUP = 1010202,
	BCTMESH = 1010203,
	MOVEMENTSET = 1010205,

	PNG3 = 1000636, 
	PNG4 = 1010210,
	PNG5 = 1010211,
	PNG6 = 1010213,

	CARS = 1010216,

	LUAARCHIVE = 1010223,
	
	ROOMANDPORTALSYSTEM = 1010300,
	FORMATION = 1010400,
	CHARACTERCREATOR = 1010700,

	OGG1 = 1020002,
	LIP = 1020003,
	OGG3 = 1020005,

	BOUNDEDAREACOLLECTIONS = 1040010,
	ENVIRONMENTSETTINGS = 1060667,
	SKYDOME = 1060668,
	PLAYFIELDDESCRIPTIONDATA = 1070003
};

struct TypId
{
	uint32 idx;
	uint32 type;

	uint32 unk0;
	uint32 offset;
	uint32 size;
	uint32 unk3;
	uint32 unk4;
	uint32 unk5;
	uint32 unk6;
	uint32 unk7;
	uint32 unk8;
	uint32 unk9;
	uint32 unk10;
};

struct Header
{
	string magic;
	uint32 version;
	uint32 hash1;
	uint32 hash2;
	uint32 hash3;
	uint32 hash4;
	uint32 recordCount;
};
