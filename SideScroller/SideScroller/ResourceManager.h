#ifndef RESMANG_H
#define RESMANG_H

#include <unordered_map>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "Camera.h"
#include "ItemContainer.h"

class Camera;
struct ItemStack;

static enum COLOR
{
	WHITE,
	BLACK,
	GREY,
	LIGHT_GREY,
	DARK_GREY
};

static const std::unordered_map<COLOR, Uint32> HEX_COLORS =
{
	{ WHITE, 0xFFFFFFFF },
	{ BLACK, 0xFF000000},
	{ GREY, 0xFF555555 },
	{ LIGHT_GREY, 0xFF777777 },
	{ DARK_GREY, 0xFF313131 }
};

static const std::unordered_map<COLOR, SDL_Color> SDL_COLORS =
{
	{ WHITE, { 255, 255, 255, 255 } },
	{ BLACK, { 0, 0, 0, 255 } },
	{ GREY, { 150, 150, 150, 255 } },
	{ LIGHT_GREY, { 215, 215, 215, 255 } },
	{ DARK_GREY, { 100, 100, 100, 255 } }
};

typedef enum TTF_ID
{
	font_ALPHABETA = 1
};

typedef enum FontID
{
	font_ALPHABETA_20_W,
	font_ALPHABETA_20_BL,
	font_ALPHABETA_20_LG,
	font_ALPHABETA_20_DG,
	font_ALPHABETA_22_BL,
	font_ALPHABETA_24_W,
	font_ALPHABETA_24_BL,
	font_ALPHABETA_40_W,
	font_ALPHABETA_40_LG,
	font_ALPHABETA_40_G,
	font_ALPHABETA_48_W,
	font_ALPHABETA_48_LG,
	font_ALPHABETA_48_G
};

typedef enum TILE_SET
{
	DIRT_TILESET,
	ROCK_TILESET,
	GRASS_TILESET
};

static const std::unordered_map<int, TILE_SET> TILE_SET_BY_INT =
{
	{ 1, DIRT_TILESET},
	{ 2, ROCK_TILESET},
	{ 3, GRASS_TILESET}
};

static const std::unordered_map<TILE_SET, const char*> TILE_SET_DIRS
{
	{ DIRT_TILESET, "E:\\Programming\\Projects\\C++\\Game\\Resources\\DirtTileMap.png" },
	{ ROCK_TILESET, "E:\\Programming\\Projects\\C++\\Game\\Resources\\RockTileMap.png" },
	{ GRASS_TILESET, "E:\\Programming\\Projects\\C++\\Game\\Resources\\GrassTileMap.png" }
};

typedef enum TILE_SITUATION //S=SameType, D=Different Type, E=Empty
{
	TS_S_T = 1 << 1,
	TS_S_L = 1 << 2,
	TS_S_R = 1 << 3,
	TS_S_B = 1 << 4,
	TS_D_T = 1 << 5,
	TS_D_L = 1 << 6,
	TS_D_R = 1 << 7,
	TS_D_B = 1 << 8,
	TS_E_T = 1 << 9,
	TS_E_L = 1 << 10,
	TS_E_R = 1 << 11,
	TS_E_B = 1 << 12,
};

class ResourceManager
{
private:
	SDL_Window* _win;
	SDL_Renderer* _renderer;
	int windowSize[2];

	unsigned int colorkey;

	SDL_Surface* loadResource(std::string path);

	//Tile Map
	SDL_Texture* texturePages[5];
	std::unordered_map<TILE_SET, std::unordered_map<uint64_t, std::pair<SDL_Rect**, int>>*> loadedTileSetMap;
	bool loadTileVarations(std::unordered_map<uint64_t, std::pair<SDL_Rect**, int>>* TileSetMap_entry, uint64_t tileSituation, int pageIndex, SDL_Surface* pageSurface, SDL_Rect* pageLoc, int pageWidth, SDL_Surface* src_res, int src_x, int src_y, int x_inc, int y_inc);

	//Fonts
	std::unordered_map<FontID, SDL_Rect*> fontCharRect_map;
	std::unordered_map<FontID, SDL_Texture*> fontTexture_map;
	void createFont(FontID font_id, TTF_ID ttf_id, int size, SDL_Color color);

	//Item
	const int ICON_SIZE = 34;
	SDL_Rect* icon_map;
	SDL_Texture* icons;
	void loadIcons();

	SDL_Texture* menuTemplateTexture;
	SDL_Texture* background;

	bool initialized;
public:

	ResourceManager(SDL_Renderer* renderer, SDL_Window* window);
	~ResourceManager();

	SDL_Renderer* getRenderer();

	SDL_Surface* CreateSurface(int width, int height);
	SDL_Surface* CreateColorSurface(int width, int height, uint32_t color);
	SDL_Texture* CreateColorTexture(int width, int height, uint32_t color);

	//Load Texture Group
	bool loadTileSet(TILE_SET ts);
	bool loadDefaultTextures();

	//Tile Functions
	bool updateTileTexture(int tileSet, int tileVariation, uint64_t tile_situation, int &texturePageDst, SDL_Rect &textureLocDst);

	//Text Funtions
	void queryText(FontID font_id, char* text, int* dstW, int* dstH);

	//Draw Functions
	void drawTile(Camera* viewPort, int x, int y, int texturePage, SDL_Rect* textureLocation);
	void drawText(FontID font_id, char* text, int x, int y);
	void drawItem(int id, int x, int y);
	void drawItem(Item& item, int x, int y);
	void drawItemStack(ItemStack* itemStack, int x, int y);
	void drawMenuPanel(int x, int y, int w, int h, int variation);
	void drawMenu(int x, int y, int w, int h, bool titlebar, int barh);
	void drawEquipmentSlot(int x, int y, int variation);
	void drawItemContainerSlot(int x, int y, ItemStack* itemStack);
	void drawBackground();
	void drawEscapeButton(int x, int y, int state); // 1 - inactive, 2 - hover, 3 - pressed

	void init();
};

#endif