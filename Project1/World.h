#ifndef _WORLD_
#define _WORLD_

class World
{
public:
	//Create an instance of the world
	static World& create(unsigned int width, unsigned int height);
	
	//Get the singleton instance of the world
	static World& getInstance();
public:
	//Get the terrain at a position
	int getTerrain(const Position& in_worldPosition) const;
	//Set the terrain at a position
	void setTerrain(const Position& in_worldPosition, int in_newValue);
	
	//Tests if a point is in the world
	bool isInWorld(const Position& in_worldPosition) const;
private:
	World(unsigned int in_width, unsigned int in_height);
	
	unsigned int getArrayIndex(const Position& in_worldPosition) const;
private:
	static World* m_instance;
	
	int* m_worldGrid;
	unsigned int m_width;
	unsigned int m_height;
}

#endif