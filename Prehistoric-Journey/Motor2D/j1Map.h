#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"


struct Properties
{
	struct Property {
		p2SString name;
		float value = 0.0f;
	};

	p2List<Property*> properties;

	~Properties() {
		p2List_item<Property*>* item = properties.start;


		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}
		properties.clear();
	}
	int Get(const char* value, int default_value) const;
	float LookForPropertyValue(const p2SString& name) const;
};


enum LayerType {
	LAYER_NONE = -1,
	LAYER_BG_FRONT,
	LAYER_GROUND,
	LAYER_BG_1,
	LAYER_BG_2,
	LAYER_BG_3,
	LAYER_COLLIDER
};


struct MapLayer {
	p2SString name;
	uint width = 0u;
	uint height = 0u;
	uint* data = nullptr;
	LayerType type = LAYER_NONE;
	float parallax_vel = 1.0f;
	Properties	properties;
	MapLayer() : data(NULL){}

	~MapLayer() {
		if (data != nullptr)
			delete[] data;
	}

	inline uint Get(int x, int y) const {

 		return data[(y * width) + x];
	}
};
	




struct TileSet
{
	
	SDL_Rect GetTileRect(int id) const;

	p2SString				name;
	int						firstgid = 0;
	uint					margin = 0u;
	uint					spacing = 0u;
	uint					tile_width = 0u;
	uint					tile_height = 0u;
	SDL_Texture*			texture=nullptr;
	uint					tex_width = 0u;
	uint					tex_height = 0u;
	uint					num_tiles_width = 0u;
	uint					num_tiles_height = 0u;
	uint					offset_x = 0u;
	uint					offset_y = 0u;

	~TileSet();
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct MapData
{
	uint					width = 0u;
	uint					height = 0u;
	uint					tile_width = 0u;
	uint					tile_height = 0u;
	SDL_Color			background_color;
	MapTypes			type = MAPTYPE_UNKNOWN;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;

};


class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	
	iPoint MapToWorld(const int x, const int y) const;
	iPoint WorldToMap(const int x, const int y) const;

	void setColliders();
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(const pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(const pugi::xml_node& tileset_node, TileSet* set);
	
	bool LoadLayer(const pugi::xml_node& node, MapLayer* layer);
	bool LoadLayerProperties(const pugi::xml_node& node, Properties& properties);
	TileSet* GetTilesetFromTileId(int id) const;

	
	

public:

	MapData data;
	iPoint spawn_pos = {0, 0};
	uint debug_camera_culling = 0u;
private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded = false;
};

#endif // __j1MAP_H__