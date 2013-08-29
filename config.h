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

string output_dir = "c:/temp/rdb_extract/";
//Input directory is working directory.

//AoC and TSW
bool staticaipoinsexport				= false;
bool climbingareasexport				= false;
bool contentspawndataexport				= false;
bool acgspawndataexport					= false;
bool npcspawndataexport					= false;
bool treasuredataexport					= false;
bool scryscriptdataexport				= false;
bool scryscriptdataexport2				= false;
bool destructibleclientdataexport		= false;
bool resurrectionpointsexport			= false;
bool simpledyneldataexport				= false;
bool notepaddataexport					= false;
bool climbingdyneldataexport			= false;
bool oceanvolumesrootnodeexport			= false;
bool volumetricfogsrootnodeexport		= false;
bool pathfindingareasrootnodeexport		= false;
bool doordyneldataexport				= false;

bool textscript			= false;
bool collectionlibrary	= false;
bool staticapoints2		= false;
bool rivers				= false;
bool patrols			= false;
bool boundedareas		= false;
bool staticaipoints		= false;
bool exportPNG			= false;
bool exportPNG2			= false;
bool scriptgroup		= false;
bool animsys			= false;

//export unknown types to <output_dir>/unknown/<type_id>
bool unknown			= false;

//The Secret World
bool exportPNG3			= false;
bool exportPNG4			= false;
bool exportPNG5			= false;
bool exportPNG6			= false;
bool mpeg				= false; 
bool fctx1				= false;
bool fctx2				= false;
bool fctx3				= false;
bool fctx4				= false;
bool fctx5				= false;
bool jpeg				= false;
bool tiff				= false; 

bool ogg1				= false;
bool lip	= true;
bool ogg3				= false;

bool physx30collection	= false;
bool decalprojectors	= false;
bool effectpackage		= false;
bool soundengine		= false;
bool meshindexfile		= false;
bool monsterdata		= false;
bool bctgroup			= false;
bool bctmesh			= false;
bool movementset		= false;
bool cars				= false;
bool roomandportalsystem		= false;
bool formation					= false;
bool charactercreator			= false;
bool boundedareacollections		= false;
bool environmentsettings		= false;
bool skydome					= false;
bool playfielddescriptiondata	= false;

bool luaarchive				= false;
