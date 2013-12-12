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
bool staticaipoinsexport                        = true;
bool climbingareasexport                        = true;
bool contentspawndataexport                     = true;
bool acgspawndataexport                         = true;
bool npcspawndataexport                         = true;
bool treasuredataexport                         = true;
bool scryscriptdataexport                       = true;
bool scryscriptdataexport2                      = true;
bool destructibleclientdataexport               = true;
bool resurrectionpointsexport                   = true;
bool simpledyneldataexport                      = true;
bool notepaddataexport                          = true;
bool climbingdyneldataexport                    = true;
bool oceanvolumesrootnodeexport                 = true;
bool volumetricfogsrootnodeexport               = true;
bool pathfindingareasrootnodeexport             = true;
bool doordyneldataexport                        = true;

bool textscript                        = true;
bool collectionlibrary                 = true;
bool staticapoints2                    = true;
bool rivers                            = true;
bool patrols                           = true;
bool boundedareas                      = true;
bool staticaipoints                    = true;
bool exportPNG                         = true;
bool exportPNG2                        = true;
bool scriptgroup                       = true;
bool animsys                           = true;

//export unknown types to <output_dir>/unknown/<type_id>
bool unknown                           = true;

//The Secret World
bool exportPNG3                        = true;
bool exportPNG4                        = true;
bool exportPNG5                        = true;
bool exportPNG6                        = true;
bool mpeg                              = true; 
bool fctx1                             = true;
bool fctx2                             = true;
bool fctx3                             = true;
bool fctx4                             = true;
bool fctx5                             = true;
bool jpeg                              = true;
bool tiff                              = true; 

bool ogg1                              = true;
bool lip                               = true;
bool ogg3                              = true;

bool physx30collection                 = true;
bool decalprojectors                   = true;
bool effectpackage                     = true;
bool soundengine                       = true;
bool meshindexfile                     = true;
bool monsterdata                       = true;
bool bctgroup                          = true;
bool bctmesh                           = true;
bool movementset                       = true;
bool cars                              = true;
bool roomandportalsystem               = true;
bool formation                         = true;
bool charactercreator                  = true;
bool boundedareacollections            = true;
bool environmentsettings               = true;
bool skydome                           = true;
bool playfielddescriptiondata          = true;

bool luaarchive                        = true;
