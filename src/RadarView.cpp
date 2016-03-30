/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Dune Legacy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dune Legacy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <RadarView.h>

#include <globals.h>

#include <FileClasses/GFXManager.h>

#include <Game.h>
#include <Map.h>
#include <House.h>
#include <ScreenBorder.h>
#include <Tile.h>
#include <SoundPlayer.h>

#include <globals.h>

#include <misc/draw_util.h>


RadarView::RadarView()
 : RadarViewBase(), currentRadarMode(Mode_RadarOff), animFrame(NUM_STATIC_FRAMES - 1), animCounter(NUM_STATIC_FRAME_TIME)
{
    radarStaticAnimation = pGFXManager->getUIGraphic(UI_RadarAnimation);

	if((radarSurface = SDL_CreateRGBSurface(0, 128, 128, SCREEN_BPP, RMASK, GMASK, BMASK, AMASK)) == NULL) {
		throw std::runtime_error("RadarView::RadarView(): Cannot create new surface!");
	}
	SDL_FillRect(radarSurface, NULL, COLOR_BLACK);

	radarTexture = SDL_CreateTexture(renderer, SCREEN_FORMAT, SDL_TEXTUREACCESS_STREAMING, 128, 128);
}


RadarView::~RadarView()
{
    SDL_FreeSurface(radarSurface);
    SDL_DestroyTexture(radarTexture);
}

int RadarView::getMapSizeX() const {
    return currentGameMap->getSizeX();
}

int RadarView::getMapSizeY() const {
    return currentGameMap->getSizeY();
}


void RadarView::draw(SDL_Surface* screen, Point position)
{
    SDL_Rect radarPosition = { position.x + RADARVIEW_BORDERTHICKNESS, position.y + RADARVIEW_BORDERTHICKNESS, RADARWIDTH, RADARHEIGHT};

    switch(currentRadarMode) {
        case Mode_RadarOff:
        case Mode_RadarOn: {
            int mapSizeX = currentGameMap->getSizeX();
            int mapSizeY = currentGameMap->getSizeY();

            int scale = 1;
            int offsetX = 0;
            int offsetY = 0;

            calculateScaleAndOffsets(mapSizeX, mapSizeY, scale, offsetX, offsetY);

            updateRadarSurface(mapSizeX, mapSizeY, scale, offsetX, offsetY);

            SDL_UpdateTexture(radarTexture, NULL, radarSurface->pixels, radarSurface->pitch);

            SDL_Rect dest = calcDrawingRect(radarTexture, radarPosition.x, radarPosition.y);
            SDL_RenderCopy(renderer, radarTexture, NULL, &dest);

            SDL_Rect radarRect;
            radarRect.x = (screenborder->getLeft() * mapSizeX*scale) / (mapSizeX*TILESIZE) + offsetX;
            radarRect.y = (screenborder->getTop() * mapSizeY*scale) / (mapSizeY*TILESIZE) + offsetY;
            radarRect.w = ((screenborder->getRight() - screenborder->getLeft()) * mapSizeX*scale) / (mapSizeX*TILESIZE);
            radarRect.h = ((screenborder->getBottom() - screenborder->getTop()) * mapSizeY*scale) / (mapSizeY*TILESIZE);

            if(radarRect.x < offsetX) {
                radarRect.w -= radarRect.x;
                radarRect.x = offsetX;
            }

            if(radarRect.y < offsetY) {
                radarRect.h -= radarRect.y;
                radarRect.y = offsetY;
            }

            int offsetFromRightX = 128 - mapSizeX*scale - offsetX;
            if(radarRect.x + radarRect.w > radarPosition.w - offsetFromRightX) {
                radarRect.w  = radarPosition.w - offsetFromRightX - radarRect.x - 1;
            }

            int offsetFromBottomY = 128 - mapSizeY*scale - offsetY;
            if(radarRect.y + radarRect.h > radarPosition.h - offsetFromBottomY) {
                radarRect.h = radarPosition.h - offsetFromBottomY - radarRect.y - 1;
            }

            renderDrawRect( renderer,
                            radarPosition.x + radarRect.x,
                            radarPosition.y + radarRect.y,
                            radarPosition.x + (radarRect.x + radarRect.w),
                            radarPosition.y + (radarRect.y + radarRect.h),
                            COLOR_WHITE);

        } break;

        case Mode_AnimationRadarOff:
        case Mode_AnimationRadarOn: {
            SDL_Rect source = calcSpriteSourceRect(radarStaticAnimation, animFrame, NUM_STATIC_FRAMES);
            SDL_Rect dest = calcSpriteDrawingRect(radarStaticAnimation, radarPosition.x, radarPosition.y, NUM_STATIC_FRAMES);
            SDL_RenderCopy(renderer, radarStaticAnimation, &source, &dest);
        } break;
    }
}

void RadarView::update() {
    if(pLocalHouse->hasRadarOn()) {
        if(currentRadarMode != Mode_RadarOn && currentRadarMode != Mode_AnimationRadarOn && currentRadarMode != Mode_AnimationRadarOff) {
            switchRadarMode(true);
        }
    } else {
        if(currentRadarMode != Mode_RadarOff && currentRadarMode != Mode_AnimationRadarOn && currentRadarMode != Mode_AnimationRadarOff) {
            switchRadarMode(false);
        }
    }

    switch(currentRadarMode) {
        case Mode_RadarOff: {

        } break;

        case Mode_RadarOn: {

        } break;

        case Mode_AnimationRadarOff: {
            if(animFrame >= NUM_STATIC_FRAMES-1) {
                currentRadarMode = Mode_RadarOff;
            } else {
                animCounter--;
                if(animCounter <= 0) {
                    animFrame++;
                    animCounter = NUM_STATIC_FRAME_TIME;
                }
            }
        } break;

        case Mode_AnimationRadarOn: {
            if(animFrame <= 0) {
                currentRadarMode = Mode_RadarOn;
            } else {
                animCounter--;
                if(animCounter <= 0) {
                    animFrame--;
                    animCounter = NUM_STATIC_FRAME_TIME;
                }
            }
        } break;
    }
}

void RadarView::switchRadarMode(bool bOn) {
    soundPlayer->playSound(RadarNoise);

    if(bOn == true) {
        soundPlayer->playVoice(RadarActivated,pLocalHouse->getHouseID());
        currentRadarMode = Mode_AnimationRadarOn;
    } else {
        soundPlayer->playVoice(RadarDeactivated,pLocalHouse->getHouseID());
        currentRadarMode = Mode_AnimationRadarOff;
    }
}

void RadarView::updateRadarSurface(int mapSizeX, int mapSizeY, int scale, int offsetX, int offsetY) {

    // Lock radarSurface for direct access to the pixels
    if(!SDL_MUSTLOCK(radarSurface) || (SDL_LockSurface(radarSurface) == 0)) {
        for(int x = 0; x <  mapSizeX; x++) {
            for(int y = 0; y <  mapSizeY; y++) {

                Tile* pTile = currentGameMap->getTile(x,y);

                /* Selecting the right color is handled in Tile::getRadarColor() */
                Uint32 color = pTile->getRadarColor(pLocalHouse, pLocalHouse->hasRadarOn());
                color = MapRGBA(radarSurface->format, color);

                for(int j = 0; j < scale; j++) {
                    Uint32* p = ((Uint32*) ((Uint8 *) radarSurface->pixels + (offsetY + scale*y + j) * radarSurface->pitch)) + (offsetX + scale*x);

                    for(int i = 0; i < scale; i++, p++) {
                        // Do not use putPixel here to avoid overhead
                        *p = color;
                    }
                }
            }
        }

        if(SDL_MUSTLOCK(radarSurface)) {
            SDL_UnlockSurface(radarSurface);
        }
    }
}
