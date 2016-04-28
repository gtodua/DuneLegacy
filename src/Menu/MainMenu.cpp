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

#include <Menu/MainMenu.h>

#include <globals.h>

#include <FileClasses/GFXManager.h>
#include <FileClasses/TextManager.h>
#include <FileClasses/music/MusicPlayer.h>

#include <MapEditor/MapEditor.h>

#include <Menu/SinglePlayerMenu.h>
#include <Menu/MultiPlayerMenu.h>
#include <Menu/OptionsMenu.h>
#include <Menu/AboutMenu.h>

MainMenu::MainMenu() : MenuBase()
{
    // set up window
    SDL_Texture *pBackground = pGFXManager->getUIGraphic(UI_MenuBackground);
    setBackground(pBackground, false);
    resize(getTextureSize(pBackground));

    setWindowWidget(&windowWidget);

    // set up pictures in the background
    // set up pictures in the background
    SDL_Texture* pPlanetBackground = pGFXManager->getUIGraphic(UI_PlanetBackground);
    planetPicture.setTexture(pPlanetBackground, false);
    SDL_Rect dest1 = calcAlignedDrawingRect(pPlanetBackground);
    dest1.y = dest1.y - getHeight(pPlanetBackground)/2 + 10;
    windowWidget.addWidget(&planetPicture, dest1);

    SDL_Texture* pDuneLegacy = pGFXManager->getUIGraphic(UI_DuneLegacy);
    duneLegacy.setTexture(pDuneLegacy, false);
    SDL_Rect dest2 = calcAlignedDrawingRect(pDuneLegacy);
    dest2.y = dest2.y + getHeight(pDuneLegacy)/2 + 28;
    windowWidget.addWidget(&duneLegacy, dest2);

    SDL_Texture* pMenuButtonBorder = pGFXManager->getUIGraphic(UI_MenuButtonBorder);
    buttonBorder.setTexture(pMenuButtonBorder, false);
    SDL_Rect dest3 = calcAlignedDrawingRect(pMenuButtonBorder);
    dest3.y = dest3.y + getHeight(pMenuButtonBorder)/2 + 59;
    windowWidget.addWidget(&buttonBorder, dest3);

    // set up menu buttons
    windowWidget.addWidget(&MenuButtons,Point((getRendererWidth() - 160)/2,getRendererHeight()/2 + 64),Point(160,111));

    singlePlayerButton.setText(_("SINGLE PLAYER"));
    singlePlayerButton.setOnClick(std::bind(&MainMenu::onSinglePlayer, this));
    MenuButtons.addWidget(&singlePlayerButton);
    singlePlayerButton.setActive();

    MenuButtons.addWidget(VSpacer::create(3));

    multiPlayerButton.setText(_("MULTIPLAYER"));
    multiPlayerButton.setOnClick(std::bind(&MainMenu::onMultiPlayer, this));
    MenuButtons.addWidget(&multiPlayerButton);

    MenuButtons.addWidget(VSpacer::create(3));

//    MenuButtons.addWidget(VSpacer::create(16));
    mapEditorButton.setText(_("MAP EDITOR"));
    mapEditorButton.setOnClick(std::bind(&MainMenu::onMapEditor, this));
    MenuButtons.addWidget(&mapEditorButton);

    MenuButtons.addWidget(VSpacer::create(3));

    optionsButton.setText(_("OPTIONS"));
    optionsButton.setOnClick(std::bind(&MainMenu::onOptions, this));
    MenuButtons.addWidget(&optionsButton);

    MenuButtons.addWidget(VSpacer::create(3));

    aboutButton.setText(_("ABOUT"));
    aboutButton.setOnClick(std::bind(&MainMenu::onAbout, this));
    MenuButtons.addWidget(&aboutButton);

    MenuButtons.addWidget(VSpacer::create(3));

    quitButton.setText(_("QUIT"));
    quitButton.setOnClick(std::bind(&MainMenu::onQuit, this));
    MenuButtons.addWidget(&quitButton);
}

MainMenu::~MainMenu() {
}

int MainMenu::showMenu()
{
    musicPlayer->changeMusic(MUSIC_MENU);

    return MenuBase::showMenu();
}

void MainMenu::onSinglePlayer() {
    SinglePlayerMenu* pSinglePlayerMenu = new SinglePlayerMenu();
    pSinglePlayerMenu->showMenu();
    delete pSinglePlayerMenu;
}

void MainMenu::onMultiPlayer() {
    MultiPlayerMenu* pMultiPlayerMenu = new MultiPlayerMenu();
    pMultiPlayerMenu->showMenu();
    delete pMultiPlayerMenu;
}

void MainMenu::onMapEditor() {
    MapEditor *pMapEditor = new MapEditor();
    pMapEditor->RunEditor();
    delete pMapEditor;
}


void MainMenu::onOptions() {
    OptionsMenu* pOptionsMenu = new OptionsMenu();
    int ret = pOptionsMenu->showMenu();
    delete pOptionsMenu;

    if(ret == MENU_QUIT_REINITIALIZE) {
        quit(MENU_QUIT_REINITIALIZE);
    }
}

void MainMenu::onAbout() {
    AboutMenu* myAbout = new AboutMenu();
    myAbout->showMenu();
    delete myAbout;
}

void MainMenu::onQuit() {
    quit();
}
