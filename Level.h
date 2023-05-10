#pragma once
//Level maps 
class Levels
{
public:
	int  getTileContent(int level, int x, int y);
	void setTileContent(int level, int x, int y, int newObject);
	
	//level variables
	bool levelComplete = false;

private:

	// Map Data
// 0 - Empty
// 1 - tile
// 2 - PC 
// 3 - NPC
// 4 - Pickup

	int levelMaps[3][18][25] =
	{
		{
			{1,1,1,4,0,0,1,1,1,0,3,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,0,0,3,1},
			{1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,2,0,1,1,1,1,0,0,0,1},
			{1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,0,0,4,1},
			{1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,3,0,0,3,0,0,0,0,0,1,1,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,1,1,0,0,0,1},
			{1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,4,1},
			{1,1,1,1,1,1,0,3,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1},
			{1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1},
			{1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1}
		},
		{
			{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,0,0,0,1,1,1,1,0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,0,2,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
			{0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,1,1,0,3,0,1},
			{0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1},
			{0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1},
			{0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1},
			{0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{0,0,0,3,0,0,0,4,0,0,0,0,0,4,0,0,0,0,0,4,0,3,0,0,1}, 
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,4,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1}

		},
		{
			{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1},
			{1,1,0,0,0,0,4,0,1,0,0,0,1,0,0,4,0,0,0,1,1,0,0,0,1},
			{1,1,0,0,3,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1},
			{1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1,1,0,0,0,1},
			{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
			{1,1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,4,0,1,1,0,4,0,1},
			{1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1},
			{1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1},
			{1,1,0,0,3,0,0,0,1,0,0,0,1,0,0,0,3,0,0,0,0,0,0,0,1},
			{1,1,0,0,0,0,4,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,3,0,0,1},
			{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,0,2,0,1,1,1,1,1,1,1,1,1,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1}
		}
	};
};