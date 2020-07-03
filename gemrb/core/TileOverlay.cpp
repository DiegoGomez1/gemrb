/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2003 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 */

#include "TileOverlay.h"

#include "Game.h" // for GetGlobalTint
#include "GlobalTimer.h"
#include "Interface.h"
#include "Video.h"

namespace GemRB {

TileOverlay::TileOverlay(int Width, int Height)
{
	w = Width;
	h = Height;
	count = 0;
	tiles = ( Tile * * ) calloc( w * h, sizeof( Tile * ) );
}

TileOverlay::~TileOverlay(void)
{
	for (int i = 0; i < count; i++) {
		delete( tiles[i] );
	}
	free( tiles );
}

void TileOverlay::AddTile(Tile* tile)
{
	tiles[count++] = tile;
}

void TileOverlay::Draw(const Region& viewport, std::vector< TileOverlay*> &overlays, int flags)
{
	// determine which tiles are visible
	int sx = std::max(viewport.x / 64, 0);
	int sy = std::max(viewport.y / 64, 0);
	int dx = ( std::max(viewport.x, 0) + viewport.w + 63 ) / 64;
	int dy = ( std::max(viewport.y, 0) + viewport.h + 63 ) / 64;

	Game* game = core->GetGame();
	assert(game);
	const Color* tintcol = game->GetGlobalTint();
	if (tintcol) {
		flags |= BLIT_TINTED;
	}

	Video* vid = core->GetVideoDriver();
	for (int y = sy; y < dy && y < h; y++) {
		for (int x = sx; x < dx && x < w; x++) {
			Tile* tile = tiles[( y* w ) + x];

			//draw door tiles if there are any
			Animation* anim = tile->anim[tile->tileIndex];
			if (!anim && tile->tileIndex) {
				anim = tile->anim[0];
			}
			assert(anim);

			// this is the base terrain tile
			vid->BlitTile( anim->NextFrame(), ( x * 64 ) - viewport.x,
						  ( y * 64 ) - viewport.y, NULL, flags, tintcol);

			if (!tile->om || tile->tileIndex) {
				continue;
			}

			int mask = 2;
			for (size_t z = 1;z<overlays.size();z++) {
				TileOverlay * ov = overlays[z];
				if (ov && ov->count > 0) {
					Tile *ovtile = ov->tiles[0]; //allow only 1x1 tiles now
					if (tile->om & mask) {
						//draw overlay tiles, they should be half transparent except for BG1
						uint32_t transFlag = (core->HasFeature(GF_LAYERED_WATER_TILES)) ? BLIT_HALFTRANS : BLIT_NO_FLAGS;
						// this is the water (or whatever)
						vid->BlitTile( ovtile->anim[0]->NextFrame(),
									   ( x * 64 ) - viewport.x,
									   ( y * 64 ) - viewport.y,
									   NULL, flags | transFlag, tintcol);
						
						

						if (core->HasFeature(GF_LAYERED_WATER_TILES)) {
							if (tile->anim[1]) {
								// this is the mask to blend the terrain tile with the water for everything but BG1
								vid->BlitTile(tile->anim[1]->NextFrame(),
											  ( x * 64 ) - viewport.x,
											  ( y * 64 ) - viewport.y,
											  NULL, flags | BLIT_BLENDED, tintcol);
							}
						} else {
							// in BG 1 this is the mask to blend the terrain tile with the water
							vid->BlitTile(tile->anim[0]->NextFrame(),
										  ( x * 64 ) - viewport.x,
										  ( y * 64 ) - viewport.y,
										  NULL, flags | BLIT_BLENDED, tintcol);
						}
					}
				}
				mask<<=1;
			}
		}
	}
}

}
