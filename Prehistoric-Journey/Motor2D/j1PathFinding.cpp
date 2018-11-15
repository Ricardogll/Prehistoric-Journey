#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
const p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	/*//Diagonals
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));
	*/

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	if (!IsWalkable(origin) || !IsWalkable(destination)) {
		return -1;
		LOG("Path not found");
	}
	else {
		// TODO 2: Create two lists: open, close
		// Add the origin tile to open
		// Iterate while we have tile in the open list

		open.list.clear();
		closed.list.clear();

		int g = 0, h = origin.DistanceTo(destination);

		PathNode curr(g, h, origin, nullptr);
		open.list.add(curr);

		PathList neighbours;

		uint neighbours_last_size = 0u;

		while (p2List_item<PathNode>* item = open.GetNodeLowestScore()) {
			// TODO 3: Move the lowest score cell from open list to the closed list
			//const PathNode* parent = new PathNode(item->)
			curr = item->data;


			closed.list.add(curr);
			open.list.del(item);





			uint neighbours_size = closed.list.end->data.FindWalkableAdjacents(neighbours);

			for (int i = 0; i < neighbours_size; i++) {

				//if (closed.list.find(neighbours.list[i]) != -1) continue;

				if (closed.Find(neighbours.list[i].pos) != NULL) continue;

				const p2List_item<PathNode>* repeated_node = open.Find(neighbours.list[i].pos);

				if (repeated_node == NULL) {

					neighbours.list[i].CalculateF(destination);
					open.list.add(neighbours.list[i]);
				}
				else
				{
					//if (repeated_node->data.g > neighbours.list[i].g) {

						//open.list[open.list.find(repeated_node->data)].parent = neighbours.list[i].parent;

						//repeated_node->data.parent = neighbours.list[i].parent;
					//}

				}



				/*open_pos = open.list.find(neighbours.list[i]);

				if (open_pos != -1) {

					neighbours.list[i].CalculateF(destination);
					open.list.add(neighbours.list[i]);

				}
				else
				{
					if (open.list[open_pos].g > neighbours.list[i].g) {
						open.list[open_pos].parent = neighbours.list[i].parent;
					}


				}*/


				if (neighbours.list[i].pos == destination) {
					//closed.list.add(neighbours.list[i]);
					break;
				}
			}



			// TODO 4: If we just added the destination, we are done!
			// Backtrack to create the final path
			// Use the Pathnode::parent and Flip() the path when you are finish
			if (curr.pos == destination) {
				last_path.Clear();

				p2List_item<PathNode>* item_path = closed.list.end;
				for (item_path; item_path->data.parent != nullptr; item_path->data = *item_path->data.parent) {
					last_path.PushBack(item_path->data.pos);
				}
				last_path.PushBack(item_path->data.pos);
				last_path.Flip();

				break;
			}
			// TODO 5: Fill a list of all adjancent nodes

			// TODO 6: Iterate adjancent nodes:
			// ignore nodes in the closed list
			// If it is NOT found, calculate its F and add it to the open list
			// If it is already in the open list, check if it is a better path (compare G)
			// If it is a better path, Update the parent

			neighbours.list.clear();
		}
	}

}

