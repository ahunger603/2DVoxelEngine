#include "ResourceManager.h"

ResourceManager::ResourceManager(SDL_Renderer* renderer, SDL_Window* window)
{
	_win = window;
	_renderer = renderer;

	colorkey = NULL;

	SDL_GetWindowSize(_win, &windowSize[0], &windowSize[1]);

	menuTemplateTexture = nullptr;
	for (int i = 0; i < 5; i++)
	{
		texturePages[i] = nullptr;
	}

	initialized = false;
}

ResourceManager::~ResourceManager()
{
	//Delete Tile Set Map
	for (auto it = loadedTileSetMap.begin(); it != loadedTileSetMap.end(); it++)
	{
		std::unordered_map<uint64_t, std::pair<SDL_Rect**, int>>* loadedTileSet = (it->second);
		for (auto itt = loadedTileSet->begin(); itt != loadedTileSet->end(); itt++)
		{
			SDL_Rect** variations = (itt->second.first);
			for (int i = 0; i < 3; i++)
				delete variations[i];

			delete[] variations;

			loadedTileSet->erase(itt++);
		}

		loadedTileSetMap.erase(it++);

		delete loadedTileSet;
	}

	//Destroy Textures
	for (int i = 0; i < 5; i++)
		SDL_DestroyTexture(texturePages[i]);

	//Destory Item Textures
	delete[] icon_map;
	SDL_DestroyTexture(icons);

	//Delete Font Rects
	for (auto it = fontCharRect_map.begin(); it != fontCharRect_map.end(); it++)
		delete (it->second);

	//Delete Font Textures
	for (auto it = fontTexture_map.begin(); it != fontTexture_map.end(); it++)
		SDL_DestroyTexture(it->second);

	//Destory Other Textures
	SDL_DestroyTexture(menuTemplateTexture);
	SDL_DestroyTexture(background);
}

SDL_Surface* ResourceManager::loadResource(std::string path)
{
	SDL_Surface* optimizedSurface = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(_win)->format, NULL);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

bool ResourceManager::loadTileVarations(std::unordered_map<uint64_t, std::pair<SDL_Rect**, int>>* TileSetMap_entry, uint64_t tileSituation, int pageIndex, SDL_Surface* pageSurface, SDL_Rect* pageLoc, int pageWidth, SDL_Surface* src_res, int src_x, int src_y, int x_inc, int y_inc)
{
	SDL_Rect** src_clip_rects = new SDL_Rect*[3];
	for (int i = 0; i < 3; i++)
	{
		if ((pageLoc->x + pageLoc->w) > pageWidth)
		{
			pageLoc->y += pageLoc->h;
			pageLoc->x = 0;
		}

		const SDL_Rect srcLoc = { src_x + (i * x_inc), src_y + (i * y_inc), 24, 24 };
		SDL_BlitSurface(src_res, &srcLoc, pageSurface, pageLoc);

		src_clip_rects[i] = new SDL_Rect(*pageLoc);
		pageLoc->x += 24;
	}
	std::pair<SDL_Rect**, int> pageLocationSet(src_clip_rects, pageIndex);
	std::pair<uint64_t, std::pair<SDL_Rect**, int>> tile_entry(tileSituation, pageLocationSet);

	TileSetMap_entry->insert(tile_entry);

	return true;
}

void ResourceManager::loadIcons()
{
	SDL_Surface* iconSurface = loadResource("E:\\Programming\\Projects\\C++\\Game\\Resources\\IconMap.png");
	SDL_SetColorKey(iconSurface, SDL_TRUE, ((unsigned int*)iconSurface->pixels)[0]);
	icons = SDL_CreateTextureFromSurface(_renderer, iconSurface);

	int width = (iconSurface->w) / ICON_SIZE;
	int height = (iconSurface->h) / ICON_SIZE;

	icon_map = new SDL_Rect[width*height];

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			SDL_Rect srcRect = {x * 34, y * 34, 34, 34};
			icon_map[(y * width) + x] = srcRect;
		}
	}

	SDL_FreeSurface(iconSurface);
}

SDL_Renderer* ResourceManager::getRenderer()
{
	return _renderer;
}

SDL_Surface* ResourceManager::CreateSurface(int width, int height)
{
	uint32_t rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
	if (surface == NULL)
	{
		(void)fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}

	return surface;
}

SDL_Surface* ResourceManager::CreateColorSurface(int width, int height, uint32_t color)
{
	SDL_Surface* s = CreateSurface(width, height);
	SDL_FillRect(s, NULL, color);

	return s;
}

SDL_Texture* ResourceManager::CreateColorTexture(int width, int height, uint32_t color)
{
	SDL_Surface* s = CreateColorSurface(width, height, color);
	SDL_Texture* t = SDL_CreateTextureFromSurface(_renderer, s);
	SDL_FreeSurface(s);

	return t;
}

bool ResourceManager::loadTileSet(TILE_SET ts)
{
	//Find Open Texure Page
	int index = -1;
	for (int i = 0; i < 5 && index == -1; i++)
	{
		if (texturePages[i] == nullptr)
			index = i;
	}

	if (index != -1)
	{
		SDL_Rect pageLoc = { 0, 0, 24, 24};
		SDL_Surface* pageSurface = CreateSurface(512, 512);

		SDL_Surface* src_res = loadResource(TILE_SET_DIRS.at(ts));

		//Loaded Tile Set Map Entry
		std::unordered_map<uint64_t, std::pair<SDL_Rect**, int>>* TileSetMap_entry = new std::unordered_map<uint64_t, std::pair<SDL_Rect**, int>>;

		//- E -
		//E C E
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_L | TS_E_R | TS_E_B), index, pageSurface, &pageLoc, 512, src_res, 216, 72, 24, 0);

		//- S -
		//E C S
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_S_T | TS_S_B | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 0, 0, 0, 24);

		//- E -
		//S C S
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_S_L | TS_S_B | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 24, 0, 24, 0);

		//- S -
		//S C E
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_R | TS_S_T | TS_S_B | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 96, 0, 0, 24);

		//- S -
		//E C E
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_R | TS_E_L | TS_S_T | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 120, 0, 0, 24);

		//- E -
		//E C E
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_L | TS_E_R | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 144, 0, 24, 0);

		//- E -
		//E C S
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_L | TS_E_B | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 216, 0, 0, 24);

		//- E -
		//S C E
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_B | TS_E_R | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 288, 0, 0, 24);

		//- S -
		//S C S
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_S_T | TS_S_B | TS_S_R | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 24, 24, 24, 0);

		//- S -
		//S C S
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_S_T | TS_S_R | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 24, 48, 24, 0);

		//- E -
		//E C S
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_L | TS_S_B | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 0, 72, 48, 0);

		//- E -
		//S C E
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_R | TS_S_B | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 24, 72, 48, 0);

		//- S -
		//E C S
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_E_L | TS_S_T | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 0, 96, 48, 0);

		//- S -
		//S C E
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_E_R | TS_S_T | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 24, 96, 48, 0);

		//- S -
		//E C E
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_E_L | TS_E_R | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 144, 72, 24, 0);

		//- E -
		//S C S
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_B | TS_S_L | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 144, 96, 24, 0);

		//Different--------------------------------------------------------------

		//- D -
		//D C D
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_D_T | TS_D_L | TS_D_R | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 312, 0, 0, 24);

		//- D -
		//E C S
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_D_T | TS_S_R | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 288, 72, 0, 24);

		//- D -
		//S C E
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_R | TS_D_T | TS_S_L | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 312, 72, 0, 24);

		//- D -
		//D C S
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_D_T | TS_D_L | TS_S_R | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 0, 120, 0, 48);

		//- D -
		//S C D
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_D_T | TS_D_R | TS_S_L | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 24, 120, 0, 48);

		//- S -
		//D C S
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_D_B | TS_D_L | TS_S_R | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 0, 144, 0, 48);

		//- S -
		//S C D
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_D_B | TS_D_R | TS_S_L | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 24, 144, 0, 48);

		//- S -
		//E C S
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_D_B | TS_S_R | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 48, 120, 0, 24);

		//- S -
		//S C E
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_R | TS_D_B | TS_S_L | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 72, 120, 0, 24);

		//- D -
		//E C S
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_D_T | TS_S_R | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 48, 192, 0, 24);

		//- D -
		//S C E
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_R | TS_D_T | TS_S_L | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 72, 192, 0, 24);

		//- E -
		//E C E
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_E_T | TS_E_R | TS_D_B), index, pageSurface, &pageLoc, 512, src_res, 96, 120, 0, 24);

		//- D -
		//E C E
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_E_B | TS_E_R | TS_D_T), index, pageSurface, &pageLoc, 512, src_res, 96, 192, 0, 24);

		//- S -
		//E C E
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_E_R | TS_D_B | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 120, 120, 0, 24);

		//- D -
		//E C E
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_E_R | TS_D_T | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 120, 192, 0, 24);

		//- S -
		//S C S
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_D_B | TS_S_L | TS_S_R | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 144, 120, 24, 0);

		//- D -
		//S C S
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_D_T | TS_S_L | TS_S_R | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 144, 144, 24, 0);

		//- S -
		//D C D
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_D_L | TS_D_R | TS_S_T | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 144, 168, 24, 0);

		//- D -
		//S C D
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_D_R | TS_D_T | TS_D_B | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 216, 96, 0, 24);

		//- S -
		//D C D
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_D_R | TS_D_L | TS_D_B | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 240, 96, 0, 24);

		//- D -
		//D C S
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_D_L | TS_D_T | TS_D_B | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 264, 96, 0, 24);

		//- S -
		//S C D
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_D_R | TS_S_T | TS_S_B | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 288, 144, 0, 24);

		//- S -
		//D C S
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_D_L | TS_S_T | TS_S_B | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 312, 144, 0, 24);

		//- D -
		//S C S
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_D_T | TS_D_B | TS_S_L | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 216, 168, 24, 0);

		//- E -
		//D C S
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_D_L | TS_E_T | TS_S_R | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 144, 192, 24, 0);

		//- S -
		//S C D
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_D_R | TS_E_B | TS_S_L | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 144, 216, 24, 0);

		//- S -
		//D C S
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_D_L | TS_E_B | TS_S_R | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 216, 192, 24, 0);

		//- E -
		//S C D
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_D_R | TS_E_T | TS_S_L | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 216, 216, 24, 0);

		//- E -
		//D C S
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_D_L | TS_E_T | TS_E_B | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 0, 264, 24, 0);

		//- E -
		//S C D
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_D_R | TS_E_T | TS_E_B | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 72, 264, 24, 0);

		//- D -
		//D C D
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_D_T | TS_D_B | TS_D_R | TS_D_L), index, pageSurface, &pageLoc, 512, src_res, 144, 240, 24, 0);

		//- E -
		//D C D
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_D_L | TS_D_R | TS_E_B | TS_E_T), index, pageSurface, &pageLoc, 512, src_res, 216, 240, 24, 0);

		//- E -
		//D C E
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_D_L | TS_E_T | TS_E_B | TS_E_R), index, pageSurface, &pageLoc, 512, src_res, 288, 216, 0, 24);

		//- E -
		//E C D
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_D_R | TS_E_T | TS_E_B | TS_E_L), index, pageSurface, &pageLoc, 512, src_res, 312, 216, 0, 24);

		//- E -
		//S C S
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_D_B | TS_S_L | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 144, 24, 24, 0);

		//- D -
		//S C S
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_D_T | TS_S_L | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 144, 48, 24, 0);

		//- E -
		//S C E
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_R | TS_D_B | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 240, 0, 0, 24);

		//- D -
		//E C S
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_E_L | TS_D_T | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 264, 0, 0, 24);

		//- S -
		//D C E
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_E_R | TS_D_L | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 144, 264, 24, 0);

		//- E -
		//E C D
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_L | TS_D_R | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 216, 264, 24, 0);

		//- E -
		//D C E
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_R | TS_D_L | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 0, 288, 24, 0);

		//- S -
		//E C D
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_E_L | TS_D_R | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 72, 288, 24, 0);

		//- D -
		//S C E
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_E_R | TS_D_T | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 0, 312, 24, 0);

		//- E -
		//E C S
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_E_L | TS_D_B | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 72, 312, 24, 0);

		//- E -
		//D C S
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_D_L | TS_D_B | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 336, 24, 0, 48);

		//- D -
		//D C S
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_D_L | TS_D_T | TS_S_R), index, pageSurface, &pageLoc, 512, src_res, 336, 0, 0, 48);

		//- D -
		//S C D
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_B | TS_D_R | TS_D_T | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 360, 0, 0, 48);

		//- E -
		//S C D
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_T | TS_D_R | TS_D_B | TS_S_L), index, pageSurface, &pageLoc, 512, src_res, 360, 24, 0, 48);

		//- D -
		//D C E
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_R | TS_D_T | TS_D_L | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 336, 144, 0, 48);

		//- D -
		//E C D
		//- S -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_D_R | TS_D_T | TS_S_B), index, pageSurface, &pageLoc, 512, src_res, 360, 144, 0, 48);

		//- S -
		//D C E
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_R | TS_D_B | TS_D_L | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 336, 168, 0, 48);

		//- S -
		//E C D
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_D_B | TS_D_R | TS_S_T), index, pageSurface, &pageLoc, 512, src_res, 360, 216, 0, 48);

		//- D -
		//E C D
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_E_B | TS_D_T | TS_D_R), index, pageSurface, &pageLoc, 512, src_res, 168, 288, 48, 0);

		//- D -
		//D C E
		//- E -
		loadTileVarations(TileSetMap_entry, (TS_E_R | TS_E_B | TS_D_T | TS_D_L), index, pageSurface, &pageLoc, 512, src_res, 144, 288, 48, 0);

		//- E -
		//D C E
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_R | TS_E_T | TS_D_B | TS_D_L), index, pageSurface, &pageLoc, 512, src_res, 144, 312, 48, 0);

		//- E -
		//E C D
		//- D -
		loadTileVarations(TileSetMap_entry, (TS_E_L | TS_E_T | TS_D_B | TS_D_R), index, pageSurface, &pageLoc, 512, src_res, 168, 312, 48, 0);


		std::pair<TILE_SET, std::unordered_map<uint64_t, std::pair<SDL_Rect**, int>>*> loadedTileSetPair(ts, TileSetMap_entry);
		loadedTileSetMap.insert(loadedTileSetPair);

		if (colorkey == NULL)
			colorkey = ((unsigned int*)pageSurface->pixels)[0];

		SDL_SetColorKey(pageSurface, SDL_TRUE, colorkey);
		texturePages[index] = SDL_CreateTextureFromSurface(_renderer, pageSurface);

		SDL_FreeSurface(src_res);
		return true;
	}
	else
		return false;
}

bool ResourceManager::loadDefaultTextures()
{
	try
	{
		//Load Fonts
		createFont(font_ALPHABETA_20_W, font_ALPHABETA, 20, SDL_COLORS.at(WHITE));
		createFont(font_ALPHABETA_20_BL, font_ALPHABETA, 20, SDL_COLORS.at(BLACK));
		createFont(font_ALPHABETA_20_LG, font_ALPHABETA, 20, SDL_COLORS.at(LIGHT_GREY));
		createFont(font_ALPHABETA_20_DG, font_ALPHABETA, 20, SDL_COLORS.at(DARK_GREY));
		createFont(font_ALPHABETA_22_BL, font_ALPHABETA, 22, SDL_COLORS.at(BLACK));
		createFont(font_ALPHABETA_24_W, font_ALPHABETA, 24, SDL_COLORS.at(WHITE));
		createFont(font_ALPHABETA_24_BL, font_ALPHABETA, 24, SDL_COLORS.at(BLACK));
		createFont(font_ALPHABETA_40_W, font_ALPHABETA, 40, SDL_COLORS.at(WHITE));
		createFont(font_ALPHABETA_40_LG, font_ALPHABETA, 40, SDL_COLORS.at(LIGHT_GREY));
		createFont(font_ALPHABETA_40_G, font_ALPHABETA, 40, SDL_COLORS.at(GREY));
		createFont(font_ALPHABETA_48_W, font_ALPHABETA, 48, SDL_COLORS.at(WHITE));
		createFont(font_ALPHABETA_48_LG, font_ALPHABETA, 48, SDL_COLORS.at(LIGHT_GREY));
		createFont(font_ALPHABETA_48_G, font_ALPHABETA, 48, SDL_COLORS.at(GREY));

		//Load Tilesets
		loadTileSet(DIRT_TILESET);
		loadTileSet(ROCK_TILESET);
		loadTileSet(GRASS_TILESET);

		//Load Icons
		loadIcons();

		//Load UI
		SDL_Surface* menuTemplateSurface = loadResource("E:\\Programming\\Projects\\C++\\Game\\Resources\\UITemplate.png");
		SDL_SetColorKey(menuTemplateSurface, SDL_TRUE, ((unsigned int*)menuTemplateSurface->pixels)[8]);
		menuTemplateTexture = SDL_CreateTextureFromSurface(_renderer, menuTemplateSurface);

		SDL_Surface* backgroundSurface = loadResource("E:\\Programming\\Projects\\C++\\Game\\Resources\\Background2.png");
		background = SDL_CreateTextureFromSurface(_renderer, backgroundSurface);

		SDL_FreeSurface(menuTemplateSurface);
		SDL_FreeSurface(backgroundSurface);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

//Draw Functions
bool ResourceManager::updateTileTexture(int tileSet, int tileVariation, uint64_t tile_situation, int &texturePageDst, SDL_Rect &textureLocDst)
{
	try
	{
		TILE_SET set = TILE_SET_BY_INT.at(tileSet);
		std::unordered_map<uint64_t, std::pair<SDL_Rect**, int>>* setInfo = loadedTileSetMap.at(set);

		bool found = false;
		std::pair<SDL_Rect**, int> srcInfo;
		for (auto it = setInfo->begin(); (it != setInfo->end() && !found); it++)
		{
			if (((it->first) & tile_situation) == it->first)
			{
				found = true;
				srcInfo = it->second;
			}
		}

		if (found)
		{
			texturePageDst = srcInfo.second;
			textureLocDst = (*srcInfo.first[tileVariation]);
			return true;
		}
		else
			return false;
	}
	catch (...){ return false; }

	return false;
}

//Text Functions-------------------------------------------------------------------------------------
void ResourceManager::createFont(FontID font_id, TTF_ID ttf_id, int size, SDL_Color color)
{
	TTF_Font* font = TTF_OpenFont("E:\\Programming\\Projects\\C++\\Game\\Resources\\alphbeta.ttf", size);
			
	SDL_Surface* fontSurface = CreateSurface(512, 512);

	int index = 0;
	SDL_Rect dstRect = { 0, 0, 0, 0 };
	SDL_Rect* rectArr = new SDL_Rect[10 + (26 * 2)];

	//Numbers
	char character[2];
	character[1] = '\0';
	for (int i = 0; i < 10; i++)
	{
		character[0] = (char)(i + 48);
		SDL_Surface* charSurf = TTF_RenderText_Solid(font, character, color);

		int w = charSurf->w;
		int h = charSurf->h;
		if (dstRect.x + w > 512)
		{
			dstRect.x = 0;
			dstRect.y += h;
		}

		SDL_BlitSurface(charSurf, NULL, fontSurface, &dstRect);

		rectArr[index] = {dstRect.x, dstRect.y, w, h};
		index++;
		dstRect.x += w;

		SDL_FreeSurface(charSurf);
	}

	//Text
	for (int case_ = 0; case_ < 2; case_++)
	{
		for (int i = 0; i < 26; i++)
		{
			character[0] = (char)(i + 65 + (32 * case_));
			SDL_Surface* charSurf = TTF_RenderText_Solid(font, character, color);

			int w = charSurf->w;
			int h = charSurf->h;
			if (dstRect.x + w > 512)
			{
				dstRect.x = 0;
				dstRect.y += h;
			}

			SDL_BlitSurface(charSurf, NULL, fontSurface, &dstRect);

			rectArr[index] = { dstRect.x, dstRect.y, w, h };
			index++;
			dstRect.x += w;

			SDL_FreeSurface(charSurf);
		}
	}

	SDL_Surface* optimizedSurface = CreateSurface(512, dstRect.y + dstRect.h);
	SDL_Rect srcRect = { 0, 0, 512, dstRect.y + dstRect.h };
	SDL_BlitSurface(fontSurface, &srcRect, optimizedSurface, NULL);

	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(_renderer, optimizedSurface);

	SDL_FreeSurface(fontSurface);
	SDL_FreeSurface(optimizedSurface);

	std::pair<FontID, SDL_Rect*> fontCharRects(font_id, rectArr);
	std::pair<FontID, SDL_Texture*> fontTextures(font_id, fontTexture);

	fontCharRect_map.insert(fontCharRects);
	fontTexture_map.insert(fontTextures);
}

void ResourceManager::queryText(FontID font_id, char* text, int* dstW, int* dstH)
{
	try
	{
		int len = strlen(text);
		SDL_Rect* charRects = fontCharRect_map.at(font_id);
		SDL_Texture* charText = fontTexture_map.at(font_id);

		char character = text[0];
		SDL_Rect dstRect = { 0, 0, 0, 0 };
		SDL_Rect srcRect = { 0, 0, 0, 0 };
		for (int i = 0; i < len; i++)
		{
			character = text[i];
			bool set = false;
			if (character >= 48 && character <= 57)
			{
				srcRect = charRects[character - 48];
				set = true;
			}
			if (character >= 65 && character <= 90)
			{
				srcRect = charRects[character - 65 + 10];
				set = true;
			}
			if (character >= 97 && character <= 122)
			{
				srcRect = charRects[character - 97 + 10 + 26];
				set = true;
			}

			if (set)
			{
				dstRect.w = srcRect.w;
				dstRect.h = srcRect.h;
				dstRect.x += srcRect.w;
			}
		}

		*dstW = dstRect.x + dstRect.w;
		*dstH = dstRect.h;
	}
	catch (...)
	{
		std::cout << "Error: Font Does Not Exist!" << std::endl;
	}
}


//Draw Functions-------------------------------------------------------------------------------------
void ResourceManager::drawTile(Camera* viewPort, int x, int y, int texturePage, SDL_Rect* textureLocation)
{
	try
	{
		int tile_xy[2] = { x, y };
		tile_xy[0] = viewPort->getDrawX(tile_xy[0]);
		tile_xy[1] = viewPort->getDrawY(tile_xy[1]);

		SDL_Rect dstRect = { tile_xy[0], tile_xy[1], 24, 24 };
		SDL_RenderCopy(_renderer, texturePages[texturePage], textureLocation, &dstRect);
	}
	catch (...)
	{
		std::cout << "Error: Tile Draw Call Failure!" << std::endl;
	}
}

 void ResourceManager::drawText(FontID font_id, char* text, int x, int y)
{
	try
	{
		int len = strlen(text);
		SDL_Rect* charRects = fontCharRect_map.at(font_id);
		SDL_Texture* charText = fontTexture_map.at(font_id);

		char character = text[0];
		SDL_Rect dstRect = { x, y, 0, 0 };
		SDL_Rect srcRect = { 0, 0, 0, 0 };
		for (int i = 0; i < len; i++)
		{
			character = text[i];
			bool set = false;
			if (character >= 48 && character <= 57)
			{
				srcRect = charRects[character - 48];
				set = true;
			}
			if (character >= 65 && character <= 90)
			{
				srcRect = charRects[character - 65 + 10];
				set = true;
			}
			if (character >= 97 && character <= 122)
			{
				srcRect = charRects[character - 97 + 10 + 26];
				set = true;
			}
			if (character == ' ')
				dstRect.x += dstRect.w;

			if (set)
			{
				dstRect.w = srcRect.w;
				dstRect.h = srcRect.h;
				SDL_RenderCopy(_renderer, charText, &srcRect, &dstRect);

				dstRect.x += srcRect.w;
			}
		}
	}
	catch (...)
	{
		std::cout << "Error: Font Does Not Exist!" << std::endl;
	}
}

void ResourceManager::drawItem(int id, int x, int y)
{
	SDL_Rect srcRect = icon_map[id];
	SDL_Rect dstRect = {x, y, ICON_SIZE, ICON_SIZE};
	SDL_RenderCopy(_renderer, icons, &srcRect, &dstRect);
}

void ResourceManager::drawItem(Item& item, int x, int y)
{
	try
	{
		int iconIndex = ItemIconMap.at((PrimaryTags)item._primaryID).at(item._secondaryID);
		drawItem(iconIndex, x, y);
	}
	catch (...) {}
}

void ResourceManager::drawItemStack(ItemStack* itemStack, int x, int y)
{
	if (itemStack != nullptr)
	{
		drawItem(itemStack->_item, x , y);

		if (itemStack->_size != 0)
		{
			char buff[5];
			_itoa_s(itemStack->_size, buff, 5, 10);

			drawText(font_ALPHABETA_22_BL, buff, x + 1 - ((45 - ICON_SIZE) / 2), y - 1 - ((45 - ICON_SIZE) / 2));
			drawText(font_ALPHABETA_22_BL, buff, x - 1 - ((45 - ICON_SIZE) / 2), y - 1 - ((45 - ICON_SIZE) / 2));
			drawText(font_ALPHABETA_20_W, buff, x - ((45 - ICON_SIZE) / 2), y - ((45 - ICON_SIZE) / 2));
		}
	}
}

void ResourceManager::drawMenuPanel(int x, int y, int w, int h, int variation)
{
	int scrXY[2] = { 0, 0 };
	switch (variation)
	{
	case 1:
		scrXY[0] = 0;
		scrXY[1] = 0;
		break;
	case 2:
		scrXY[0] = 0;
		scrXY[1] = 83;
		break;
	case 3:
		scrXY[0] = 0;
		scrXY[1] = 166;
		break;
	case 4:
		scrXY[0] = 0;
		scrXY[1] = 249;
		break;
	}

	SDL_Rect srcRect = { 0, 0, 0, 0 };
	SDL_Rect dstRect = { 0, 0, 0, 0 };

	//Draw Corners
	srcRect = { scrXY[0], scrXY[1], 8, 8 };
	dstRect = { x, y, 8, 8 };
	SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);

	srcRect = { scrXY[0] + 74, scrXY[1], 8, 8 };
	dstRect = { x + w - 8, y, 8, 8 };
	SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);

	srcRect = { scrXY[0], scrXY[1] + 74, 8, 8 };
	dstRect = { x, y + h - 8, 8, 8 };
	SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);

	srcRect = { scrXY[0] + 74, scrXY[1] + 74, 8, 8 };
	dstRect = { x + w - 8, y + h - 8, 8, 8 };
	SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);

	//Draw Top & Bottom Edge
	int w_bars = (w - 16) / 64;
	int w_remainder = (w - 16) % 64;

	for (int edge = 0; edge < 2; edge++)
	{
		srcRect = { scrXY[0] + 9, scrXY[1] + (74 * edge), 64, 8 };
		for (int i = 0; i < w_bars; i++)
		{
			dstRect = { x + 8 + (64 * i), y + ((h - 8)*edge), 64, 8 };
			SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);
		}
		srcRect = { scrXY[0] + 9, scrXY[1] + (74 * edge), w_remainder, 8 };
		dstRect = { x + 8 + (64 * w_bars), y + ((h - 8)*edge), w_remainder, 8 };
		SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);
	}

	//Draw Left & Right Edge
	int h_bars = (h - 16) / 64;
	int h_remainder = (h - 16) % 64;

	for (int edge = 0; edge < 2; edge++)
	{
		srcRect = { scrXY[0] + (74 * edge), scrXY[1] + 9, 8, 64 };
		for (int i = 0; i < h_bars; i++)
		{
			dstRect = { x + ((w - 8)*edge), y + 8 + (64 * i), 8, 64 };
			SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);
		}
		srcRect = { scrXY[0] + (74 * edge), scrXY[1] + 9, 8, h_remainder };
		dstRect = { x + ((w - 8)*edge), y + 8 + (64 * h_bars), 8, h_remainder };
		SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);
	}

	//Draw Center //Change if use textures
	srcRect = { scrXY[0] + 9, scrXY[1] + 9, 64, 64 };
	dstRect = { x + 8, y + 8, w - 16, h - 16 };
	SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);
}

void ResourceManager::drawMenu(int x, int y, int w, int h, bool titlebar, int barh)
{
	if (menuTemplateTexture != nullptr)
	{
		if (titlebar)
		{
			drawMenuPanel(x, y, w, barh, 1);
			drawMenuPanel(x, y + barh, w, h - barh, 2);
		}
		else
			drawMenuPanel(x, y, w, h, 2);
	}
	else
	{
		std::cout << "Error: Inventory Draw Call Failure!" << std::endl;
	}
}

//1 - helm, 2 - chest, 3 - gloves, 4 - feet, 5 - weapon, 6 - shield
void ResourceManager::drawEquipmentSlot(int x, int y, int variation)
{
	SDL_Rect srcRect = { 0, 0, 0, 0 };
	switch (variation)
	{
	case 1:
		srcRect = { 83, 60, 45, 45 };
		break;
	case 2:
		srcRect = { 129, 60, 45, 45 };
		break;
	case 3:
		srcRect = { 129, 106, 45, 45 };
		break;
	case 4:
		srcRect = { 129, 152, 45, 45 };
		break;
	case 5:
		srcRect = { 83, 106, 45, 45 };
		break;
	case 6:
		srcRect = { 83, 152, 45, 45 };
		break;
	default:
		srcRect = { 83, 198, 45, 45 };
		break;
	}

	SDL_Rect dstRect = { x, y, 45, 45 };
	SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);
}

void ResourceManager::drawItemContainerSlot(int x, int y, ItemStack* itemStack)
{
	SDL_Rect srcRect = {129, 198, 45, 45};
	SDL_Rect dstRect = { x, y, 45, 45 };
	SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);

	if (itemStack != nullptr)
	{
		drawItemStack(itemStack, x + ((45 - ICON_SIZE) / 2), y + ((45 - ICON_SIZE) / 2));
		/*
		Item* item = itemStack->_item;
		if (item != nullptr)
		{
			drawItem(item, x + ((45 - ICON_SIZE) / 2), y + ((45 - ICON_SIZE) / 2));

			if (itemStack->_size != 0)
			{
				char buff[5];
				_itoa_s(itemStack->_size, buff, 5, 10);

				drawText(font_ALPHABETA_22_BL, buff, x + 1, y - 1);
				drawText(font_ALPHABETA_22_BL, buff, x - 1, y - 1);
				drawText(font_ALPHABETA_20_W, buff, x, y);
			}
		}
		*/
	}
}

void ResourceManager::drawBackground()
{
	SDL_Rect dstRect = { 0, 0, windowSize[0], windowSize[1] };
	SDL_RenderCopy(_renderer, background, NULL, &dstRect);
}

void ResourceManager::drawEscapeButton(int x, int y, int state)
{
	SDL_Rect srcRect = {0,0,0,0};
	switch (state)
	{
	case 1:
		srcRect = { 83, 0, 18, 18 };
		break;
	case 2:
		srcRect = { 83, 20, 18, 18 };
		break;
	case 3:
		srcRect = { 83, 40, 18, 18 };
		break;
	default:
		srcRect = { 83, 0, 18, 18 };
	}

	SDL_Rect dstRect = {x, y, 18, 18};
	SDL_RenderCopy(_renderer, menuTemplateTexture, &srcRect, &dstRect);
}

void ResourceManager::init()
{
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	initialized = true;
}