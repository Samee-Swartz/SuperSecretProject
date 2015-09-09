#ifndef _WORLD_
#define _WORLD_

class World
{
public:
	static World& create(unsigned int width, unsigned int height);
	static World& getInstance();
public:
	int getTerrain(const Position& in_worldPosition) const;
	void setTerrain(const Position& in_worldPosition, int in_newValue);
	
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