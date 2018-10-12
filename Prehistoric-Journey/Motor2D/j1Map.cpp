#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1Scene.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

TileSet::~TileSet() {
	App->tex->UnLoad(texture);
}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	// TODO 5: Prepare the loop to draw all tilesets + Blit
	p2List_item<TileSet*>* tileset_item = data.tilesets.end;//to print bg first and blit platforms on top of it
	p2List_item<MapLayer*>* layers_item = data.layers.start;

	/*uint camera_x, camera_y, camera_w, camera_h;
	App->win->GetWindowSize(camera_w, camera_h);
	camera_x = App->render->camera.x;
	camera_y = App->render->camera.y;*/
	
	//if (tileset_item->data->Get(i, j) != 0) 
	while (tileset_item != NULL) {

		layers_item = data.layers.start;

		while (layers_item != NULL) {

			uint w, h;
			
			App->win->GetWindowSize(w, h);

			iPoint top_left_camera = WorldToMap(-(App->render->camera.x) * layers_item->data->parallax_vel / App->win->GetScale(), 0);
			iPoint bottom_right_camera = WorldToMap(-(App->render->camera.x) * layers_item->data->parallax_vel + (int)w / App->win->GetScale(), (int)h / App->win->GetScale());

			if (top_left_camera.x < 0)
				top_left_camera.x = 0;

			if (bottom_right_camera.x > layers_item->data->width)
				bottom_right_camera.x = layers_item->data->width;

			for(int x = top_left_camera.x; x <= bottom_right_camera.x; x++) {
				

				for (int y = 0; y < layers_item->data->height; y++) {

					SDL_Rect rect = tileset_item->data->GetTileRect(layers_item->data->Get(x, y));
					iPoint world_coords = MapToWorld(x, y);
					/*SDL_Rect tile_pos_world;
					tile_pos_world.x = world_coords.x;
					tile_pos_world.y = world_coords.y;
					tile_pos_world.w = rect.w;
					tile_pos_world.h = rect.h;

					if (RectInsideCamera(camera_x, camera_y, camera_w, camera_h, rect)) {*/



						if (layers_item->data->type == LAYER_BG_FRONT) {
							App->render->Blit(tileset_item->data->texture, world_coords.x, world_coords.y, &rect, layers_item->data->parallax_vel);
						}
						else if (layers_item->data->type == LAYER_GROUND) {
							App->render->Blit(tileset_item->data->texture, world_coords.x, world_coords.y, &rect, layers_item->data->parallax_vel);
						}
						//else if (layers_item->data->type == LAYER_COLLIDER) {
						//	App->render->Blit(tileset_item->data->texture, world_coords.x, world_coords.y, &rect, layers_item->data->parallax_vel);
						//}
						else if (layers_item->data->type == LAYER_BG_1) {
							App->render->Blit(tileset_item->data->texture, world_coords.x, world_coords.y, &rect, layers_item->data->parallax_vel);
						}
						else if (layers_item->data->type == LAYER_BG_2) {
							App->render->Blit(tileset_item->data->texture, world_coords.x, world_coords.y, &rect, layers_item->data->parallax_vel);
						}
						else if (layers_item->data->type == LAYER_BG_3) {
							App->render->Blit(tileset_item->data->texture, world_coords.x, world_coords.y, &rect, layers_item->data->parallax_vel);
						}
					//}
				}

			}
			layers_item = layers_item->next;
		}
		tileset_item = tileset_item->prev;
	}

}

bool j1Map::RectInsideCamera(int camera_x, int camera_y, uint camera_w, uint camera_h, SDL_Rect &rect) {
	bool ret = false;
	//camera_x = -camera_x;
	///*if ((rect.x > camera_x && rect.x < camera_x + camera_w)) {
	//	ret = true;
	//}*/
	////if (rect.x < camera_x+camera_w)
	////	ret = true;
	//if (rect.x + rect.w > camera_x)
	//	ret = true;

	return ret;
}


TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	int gid_prev = 0;
	int gid_next = 0;

	for (item; item != NULL; item = item->next) {
		if (item->prev == NULL)
			gid_prev = 0;
		else
			gid_prev = item->data->firstgid;

		if (item->next == NULL)
			return item->data;
		else
			gid_next = item->next->data->firstgid;

		if (gid_prev <= id && gid_next > id)
			return item->data;
	}

	return item->data;

}
		// TODO 9: Complete the draw function
void j1Map::setColliders()
{
	p2List_item<TileSet*>* tile_item = this->data.tilesets.start;

	
		p2List_item<MapLayer*>* layer_item = this->data.layers.end;

		
			for (int y = 0; y < data.height; y++)
			{
				for (int x = 0; x < data.width; x++)
				{
					if (layer_item->data->Get(x, y) != 0)
					{
						int id = layer_item->data->Get(x, y);

						if (layer_item->data->type == LAYER_COLLIDER)
						{
							if ((id == 48 && App->scene->current_map == 1) || (id == 348 && App->scene->current_map == 2))
							{
								SDL_Rect rect = tile_item->data->GetTileRect(id);
								iPoint worldcoord = MapToWorld(x, y);
								rect.x = worldcoord.x;
								rect.y = worldcoord.y;
								App->collision->AddCollider(rect, COLLIDER_WALL);
							}
							if ((id == 77 && App->scene->current_map == 1) || (id == 377 && App->scene->current_map == 2))
							{
								spawn_pos = MapToWorld(x, y);

							}
							if ((id == 63 && App->scene->current_map == 1) || (id == 363 && App->scene->current_map == 2))
							{
								SDL_Rect rect = tile_item->data->GetTileRect(id);
								iPoint worldcoord = MapToWorld(x, y);
								rect.x = worldcoord.x;
								rect.y = worldcoord.y;
								App->collision->AddCollider(rect, COLLIDER_LEDGE);

							}
						}
					}
				}
			
		
	}
}



iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);
	// TODO 2: Add orthographic world to map coordinates
	if (App->map->data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// TODO 2: clean up all layer data
	// Remove all layers

	p2List_item<MapLayer*>* item_layer;
	item_layer = data.layers.start;

	while (item_layer != NULL)
	{
		RELEASE(item_layer->data);
		item_layer = item_layer->next;
	}
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// TODO 4: Iterate all layers and load each of them
	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* yer = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, yer);
		}

		

		data.layers.add(yer);
	}


	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// TODO 4: Add info here about your loaded layers
		// Adapt this vcode with your own variables
		
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

// TODO 3: Create the definition for a function that loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	
	const int size = layer->height*layer->width;
	
	LoadLayerProperties(node, layer->properties);
	
	layer->name = node.attribute("name").as_string();
	if (layer->name == "MainGround")
		layer->type = LAYER_GROUND;
	else if (layer->name == "Forest") {
		layer->type = LAYER_BG_1;
		//layer->parallax_vel = 0.8f;
	}
	else if (layer->name == "Mountains") {
		layer->type = LAYER_BG_2;
		//layer->parallax_vel = 0.5f; // 
	}
	else if (layer->name == "Sky") {
		layer->type = LAYER_BG_3;
		//layer->parallax_vel = 0.3f;
	}
	else if (layer->name == "Rocks") {
		layer->type = LAYER_BG_FRONT;
		//layer->parallax_vel = 1.1f;
	}
	else if (layer->name == "Mainground")
		layer->type = LAYER_GROUND;

	else if (layer->name == "Background") {
		layer->type = LAYER_BG_1;
		//layer->parallax_vel = 0.8f;
	}
	else if (layer->name == "Colliders")
		layer->type = LAYER_COLLIDER;

	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->parallax_vel = layer->properties.LookForPropertyValue("parallax_vel");
	
	layer->data = new uint[layer->height * layer->width];
	memset(layer->data, 0, sizeof(uint)*layer->height * layer->width);
	
	uint i = 0;
	for (pugi::xml_node tile_gid = node.child("data").child("tile"); tile_gid; tile_gid = tile_gid.next_sibling("tile")) {
		layer->data[i++] = tile_gid.attribute("gid").as_uint();
	}
	return true;
}



bool j1Map::LoadLayerProperties(pugi::xml_node& node, Properties& properties) {

	bool ret = true;

	pugi::xml_node node_properies = node.child("properties");

	if (node_properies == NULL) {
		LOG("ERROR Loading Layer Properties");
		ret = false;
	}
	else {
		pugi::xml_node aux;

		for (aux = node_properies.child("property"); aux; aux = aux.next_sibling("property")) {
			Properties::Property* prop = new Properties::Property;
			prop->value = aux.attribute("value").as_float();
			prop->name = aux.attribute("name").as_string();
			properties.properties.add(prop);

		}
	}

	return ret;
}

float Properties::LookForPropertyValue(p2SString name) {

	p2List_item<Property*>* item = properties.start;
	while (item != NULL) {

		if (item->data->name == name) {
			return item->data->value;
		}

		item = item->next;
	}

	return 0.0f;

}