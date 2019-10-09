#pragma once
/// !WARNING
/*
This is not your typical TEST this is instead a place holder for me to create and test experimental classes before completely implementing them properly
*/


// A Dispatched job will receive this as function argument
//typedef unsigned int uint32_t;
//
//typedef void(*Jobfunction)();
//
//struct JobDispatchArgs
//{
//	uint32_t jobIndex;
//	uint32_t groupIndex;
//};
//
//namespace JobSystem
//{
//	// Create the internal resources such as worker threads, etc. Call it once when initializing the application.
//	void Initialize();
//
//	// Add a job to execute asynchronously. Any idle thread will execute this job.
//	void Execute(const std::function<void()>& job);
//
//	// Divide a job onto multiple jobs and execute in parallel.
//	//  jobCount    : how many jobs to generate for this task.
//	//  groupSize   : how many jobs to execute per thread. Jobs inside a group execute serially. It might be worth to increase for small jobs
//	//  func        : receives a JobDispatchArgs as parameter
//	void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job);
//
//	// Check if any threads are working currently or not
//	bool IsBusy();
//
//	// Wait until all threads become idle
//	void Wait();
//}



//https://www.youtube.com/watch?v=c1gO9aB9nbs&t=3246s
// Challenge.
// MakeLock free implementation of a singly linked list. 
// Implement Construct/Destruct : no problem;
// Implement Find and push_front;
class LLnode
{
public:
    LLnode *next;
};
class lfSLinkList
{
public:
    struct iterator
    {
    public:
        iterator(LLnode * ptr) : ptr(ptr) {}
        iterator operator++() { ++ptr; return *this; }
        bool operator!=(const iterator & other) const { return ptr != other.ptr; }
        const LLnode& operator*() const { return *ptr; }
    private:
        LLnode* ptr; /// NO THIS NEEDS TO TURN INTO NEXT each TIME.
    };
    iterator begin() const { return iterator(_first); }
    iterator end() const { return iterator(_first + length); }


    LLnode *root;



// void find(LLnode *_node)
// {
//     for(auto& N: root)
// }
    void push_front(LLnode *_node)
    {
        _node->next = root;
        root = _node;
        _first = root;
        ++length; /// CAS this

        /// if I can make a slot interface I can implement wait free here... 
        ///Perhaps CAS to decide which contented object becomes first allowing the addition of multiple objects at the same time. Extra step but allows multiple entries


    }

    LLnode *_last;

    size_t size() const
    {
        return length;
    }
private:
    unsigned int length;
       LLnode *_first; /// Make it atomic

};




































//  #include<vector>
//  #include <math.h>
//  
//  
//  const int WIDTH = 1024;
//  const int HEIGHT = 1024;
//  
//  struct Point
//  {
//  	int dx, dy;
//  
//  	int DistSq() const { return dx * dx + dy * dy; }
//  };
//  
//  Point inside = { 0, 0 };
//  Point empty = { 9999, 9999 };
//  
//  struct Grid
//  {
//  	std::vector<Point> grid;
//  	int width;
//  	int height;
//  	Grid() {
//  
//  	}
//  	Grid(int width, int height)
//  	{
//  		grid = std::vector<Point>(width*height);
//  		this->width = width;
//  		this->height = height;
//  	}
//  	Grid(int width, int height, std::vector<bool> inputVector)
//  	{
//  		grid = std::vector<Point>(width*height);
//  		this->width = width;
//  		this->height = height;
//  		int index = 0;
//  		for (size_t y = 0; y < height; y++)
//  		{
//  			for (size_t x = 0; x < width; x++)
//  			{
//  				grid[index] = (inputVector[index] ? inside : empty);
//  				index++;
//  			}
//  		}
//  	}
//  	Point get(int x, int y)
//  	{
//  		// OPTIMIZATION: you can skip the edge check code if you make your grid 
//  		// have a 1-pixel gutter.
//  		if (x >= 0 && y >= 0 && x < width && y < height)
//  			return grid[y * width + x];
//  		else
//  			return empty;
//  	}
//  	void put(int x, int y, const Point &p)
//  	{
//  		grid[y*width + x] = p;
//  	}
//  	void compare(Point &p, int x, int y, int offsetx, int offsety)
//  	{
//  		Point other = get(x + offsetx, y + offsety);
//  		other.dx += offsetx;
//  		other.dy += offsety;
//  
//  		if (other.DistSq() < p.DistSq())
//  			p = other;
//  	}
//  	void generateSDF()
//  	{
//  		// Pass 0
//  		for (int y = 0; y < height; y++)
//  		{
//  			for (int x = 0; x < width; x++)
//  			{
//  				Point p = get(x, y);
//  				compare(p, x, y, -1, 0);
//  				compare(p, x, y, 0, -1);
//  				compare(p, x, y, -1, -1);
//  				compare(p, x, y, 1, -1);
//  				put(x, y, p);
//  			}
//  
//  			for (int x = width - 1; x >= 0; x--)
//  			{
//  				Point p = get(x, y);
//  				compare(p, x, y, 1, 0);
//  				put(x, y, p);
//  			}
//  		}
//  
//  		// Pass 1
//  		for (int y = height - 1; y >= 0; y--)
//  		{
//  			for (int x = width - 1; x >= 0; x--)
//  			{
//  				Point p = get(x, y);
//  				compare(p, x, y, 1, 0);
//  				compare(p, x, y, 0, 1);
//  				compare(p, x, y, -1, 1);
//  				compare(p, x, y, 1, 1);
//  				put(x, y, p);
//  			}
//  
//  			for (int x = 0; x < width; x++)
//  			{
//  				Point p = get(x, y);
//  				compare(p, x, y, -1, 0);
//  				put(x, y, p);
//  			}
//  		}
//  	}
//  };
//  struct Vec2i
//  {
//  	int x, y;
//  };
//  struct Vec2f
//  {
//  	float x, y;
//  };
//  enum Direction { Right, Left, Up, Down };
//  enum PlateDynamics { Mountain, CoastLine, Ocean };
//  struct Cell
//  {
//  	int elevation = 0;
//  	int tectonicPlate;
//  	bool coastline = false;
//  	bool mountain = false;
//  	bool ocean = false;
//  	Cell()
//  	{
//  		tectonicPlate = 0;
//  	}
//  	Cell(int plate)
//  	{
//  		tectonicPlate = plate;
//  	}
//  	Cell(int plate, int elevation)
//  	{
//  		tectonicPlate = plate;
//  		this->elevation = elevation;
//  	}
//  };
//  
//  #include <random>
//  #include <vector>
//  #include <queue>
//  
//  #include <algorithm>
//  #include<iostream>
//  
//  class Planet
//  {
//  public:
//  	int worldSize = 1024;
//  	int plateCount = 17; //100 normally
//  	int elevationRange = 600;
//  	int oceanElevation = 400;
//  	Grid mountainGrid;
//  	Grid oceanGrid;
//  	Grid coastGrid;
//  
//  	bool tectonicPlatesComplete = false;
//  	bool rockTypesComplete = false;
//  	std::vector<Cell> cellMap;
//  	std::vector<std::vector<Vec2i>> plateEdges = std::vector<std::vector<Vec2i>>();
//  
//  	void init()
//  	{
//  		//Initial tectonic plate seeding
//  		std::random_device rd; // obtain a random number from hardware
//  		std::mt19937 eng(rd());
//  		std::uniform_int_distribution<> distr(0, worldSize*worldSize);
//  		std::uniform_int_distribution<> distr2(0, elevationRange);
//  		std::uniform_int_distribution<> distr3(0, 100);
//  
//  		int vectorSize = sqrt((worldSize*worldSize) / plateCount) * 4;
//  
//  		//Assign random initial elevation per plate
//  		elevationMap = std::vector<int>(plateCount);
//  		for (size_t i = 0; i < plateCount; i++)
//  		{
//  			elevationMap[i] = distr2(eng);
//  		}
//  
//  		//Assign movement vector per plate
//  		plateMovementMap = std::vector<Vec2f>(plateCount);
//  		for (size_t i = 0; i < plateCount; i++)
//  		{
//  			plateMovementMap[i].x = distr3(eng) / 100.0f;
//  			plateMovementMap[i].y = distr3(eng) / 100.0f;
//  		}
//  
//  		for (size_t i = 0; i < plateCount; i++)
//  		{
//  			int index = distr(eng);
//  			cellMap[index] = Cell(i + 1, elevationMap[i]);
//  			std::vector<Vec2i> edges = std::vector<Vec2i>();
//  			edges.reserve(vectorSize);
//  			edges.push_back(Vec2i{ getCellXFromIndex(index), getCellYFromIndex(index) });
//  			plateEdges.push_back(edges);
//  		}
//  	}
//  
//  	void generateTectonicPlates()
//  	{
//  
//  		int emptyRuns = 0;
//  		while (emptyRuns < 5)
//  		{
//  			bool expanded = generatePlatesTick();
//  			if (!expanded)
//  			{
//  				emptyRuns++;
//  			}
//  		}
//  		tectonicPlatesComplete = true;
//  	}
//  	bool generatePlatesTick()
//  	{
//  		//Go through all the plate edges
//  		std::random_device rd; // obtain a random number from hardware
//  		std::mt19937 eng(rd());
//  		bool expanded = false;
//  		for (size_t i = 0; i < plateCount; i++)
//  		{
//  			int size = plateEdges[i].size();
//  			std::uniform_int_distribution<> distr(0, worldSize*worldSize);
//  			for (size_t j = 0; j < std::min(size, 1000); j++)
//  			{
//  				int index = 0;
//  				int expansionIndex = distr(eng) % plateEdges[i].size();
//  				Vec2i pos = plateEdges[i][expansionIndex];
//  				index = getCellIndex(pos.x + 1, pos.y);
//  				if (cellMap[index].tectonicPlate == 0)
//  				{
//  					plateEdges[i].push_back(Vec2i{ moduloWrapper(pos.x + 1), pos.y });
//  					cellMap[index] = Cell(i + 1, elevationMap[i]);
//  					expanded = true;
//  					cellMap[index] = Cell(i + 1);
//  					expanded = true;
//  				}
//  				index = getCellIndex(pos.x - 1, pos.y);
//  				if (cellMap[index].tectonicPlate == 0)
//  				{
//  					plateEdges[i].push_back(Vec2i{ moduloWrapper(pos.x - 1), pos.y });
//  					cellMap[index] = Cell(i + 1, elevationMap[i]);
//  					expanded = true;
//  				}
//  				index = getCellIndex(pos.x, pos.y + 1);
//  				if (cellMap[index].tectonicPlate == 0)
//  				{
//  					plateEdges[i].push_back(Vec2i{ pos.x, moduloWrapper(pos.y + 1) });
//  					cellMap[index] = Cell(i + 1, elevationMap[i]);
//  					expanded = true;
//  				}
//  				index = getCellIndex(pos.x, pos.y - 1);
//  				if (cellMap[index].tectonicPlate == 0)
//  				{
//  					plateEdges[i].push_back(Vec2i{ pos.x, moduloWrapper(pos.y - 1) });
//  					cellMap[index] = Cell(i + 1, elevationMap[i]);
//  					expanded = true;
//  				}
//  				std::iter_swap(plateEdges[i].begin() + expansionIndex, plateEdges[i].end() - 1);
//  				plateEdges[i].pop_back();
//  			}
//  		}
//  		return expanded;
//  	}
//  	int getCellIndex(const int &x, const int &y)
//  	{
//  		return moduloWrapper(y) * worldSize + moduloWrapper(x);
//  	}
//  
//  	inline int moduloWrapper(int input)
//  	{
//  		return ((input % worldSize) + worldSize) % worldSize;
//  	}
//  
//  	int getCellXFromIndex(int index)
//  	{
//  		return index % worldSize;
//  	}
//  
//  	int getCellYFromIndex(int index)
//  	{
//  		return index / worldSize;
//  	}
//  
//  	std::vector<Vec2i> getPlateEdges() {
//  		std::vector<Vec2i> fixedPlateEdges = std::vector<Vec2i>();
//  		//int vectorSize = sqrt((worldSize*worldSize) / plateCount) * 4;
//  		for (int i = 0; i < cellMap.size(); i++) {
//  			int x = getCellXFromIndex(i);
//  			int y = getCellYFromIndex(i);
//  
//  			int plate = cellMap.at(i).tectonicPlate;
//  
//  			int rightPlate = cellMap.at(getCellIndex(x + 1, y)).tectonicPlate;
//  			int leftPlate = cellMap.at(getCellIndex(x - 1, y)).tectonicPlate;
//  			int topPlate = cellMap.at(getCellIndex(x, y + 1)).tectonicPlate;
//  			int backPlate = cellMap.at(getCellIndex(x, y - 1)).tectonicPlate;
//  
//  			if (rightPlate != plate || leftPlate != plate || topPlate != plate || backPlate != plate) {
//  				fixedPlateEdges.push_back(Vec2i{ x, y });
//  			}
//  		}
//  		return fixedPlateEdges;
//  	}
//  
//  	int getDistance(Point point, bool ocean) {
//  		float distance = sqrt(point.DistSq());
//  		//int heightExp = 100 * (1 / distance);
//  		int heightLin = 200 - distance * 2;
//  		int last = distance + heightLin;
//  		if (ocean) {
//  			last = -last;
//  		}
//  		if (last < 0 || last >255) {
//  			last = 1;
//  		}
//  		return last * 2;
//  	}
//  
//  	void simulateTectonicPlateMovement()
//  	{
//  		mountainMap = std::vector<bool>(worldSize*worldSize);
//  		oceanMap = std::vector<bool>(worldSize*worldSize);
//  		coastlineMap = std::vector<bool>(worldSize*worldSize);
//  		//Go through all edges and check for behaviour
//  		int index;
//  		Cell cell;
//  		Vec2i pos;
//  		Vec2f movement;
//  
//  		std::vector<Vec2i> fixedPlateEdges = getPlateEdges();
//  		std::cout << "Fixed Plate Edges size: " << fixedPlateEdges.size() << "\n";
//  		for (size_t j = 0; j < fixedPlateEdges.size(); j++)
//  		{
//  			pos = fixedPlateEdges[j];
//  			cell = cellMap[getCellIndex(pos.x, pos.y)];
//  			movement = plateMovementMap[cell.tectonicPlate - 1];
//  
//  			index = getCellIndex(pos.x + 1, pos.y);
//  
//  			if (cellMap[index].tectonicPlate != cell.tectonicPlate)
//  			{
//  				PlateDynamics state = comparePlateMovement(movement, plateMovementMap[cellMap[index].tectonicPlate - 1], cell.elevation, cellMap[index].elevation);
//  				switch (state)
//  				{
//  				case Ocean:
//  					oceanMap[index] = true;
//  					break;
//  				case Mountain:
//  					mountainMap[index] = true;
//  					break;
//  				case CoastLine:
//  					coastlineMap[index] = true;
//  					break;
//  				}
//  			}
//  			index = getCellIndex(pos.x - 1, pos.y);
//  			if (cellMap[index].tectonicPlate != cell.tectonicPlate)
//  			{
//  				PlateDynamics state = comparePlateMovement(movement, plateMovementMap[cellMap[index].tectonicPlate - 1], cell.elevation, cellMap[index].elevation);
//  				switch (state)
//  				{
//  				case Ocean:
//  					oceanMap[index] = true;
//  					break;
//  				case Mountain:
//  					mountainMap[index] = true;
//  					break;
//  				case CoastLine:
//  					coastlineMap[index] = true;
//  					break;
//  				}
//  			}
//  			index = getCellIndex(pos.x, pos.y + 1);
//  			if (cellMap[index].tectonicPlate != cell.tectonicPlate)
//  			{
//  				PlateDynamics state = comparePlateMovement(movement, plateMovementMap[cellMap[index].tectonicPlate - 1], cell.elevation, cellMap[index].elevation);
//  				switch (state)
//  				{
//  				case Ocean:
//  					oceanMap[index] = true;
//  					break;
//  				case Mountain:
//  					mountainMap[index] = true;
//  					break;
//  				case CoastLine:
//  					coastlineMap[index] = true;
//  					break;
//  				}
//  			}
//  			index = getCellIndex(pos.x, pos.y - 1);
//  			if (cellMap[index].tectonicPlate != cell.tectonicPlate)
//  			{
//  				PlateDynamics state = comparePlateMovement(movement, plateMovementMap[cellMap[index].tectonicPlate - 1], cell.elevation, cellMap[index].elevation);
//  				//std::cout << "State: " << state << std::endl;
//  				switch (state)
//  				{
//  				case Ocean:
//  					oceanMap[index] = true;
//  					break;
//  				case Mountain:
//  					mountainMap[index] = true;
//  					break;
//  				case CoastLine:
//  					coastlineMap[index] = true;
//  					break;
//  				}
//  			}
//  		}
//  
//  		mountainGrid = Grid(worldSize, worldSize, mountainMap);
//  		mountainGrid.generateSDF();
//  
//  		oceanGrid = Grid(worldSize, worldSize, oceanMap);
//  		oceanGrid.generateSDF();
//  
//  		coastGrid = Grid(worldSize, worldSize, coastlineMap);
//  		coastGrid.generateSDF();
//  
//  		int jndex = 0;
//  
//  		for (size_t x = 0; x < worldSize; x++)
//  		{
//  			for (size_t y = 0; y < worldSize; y++)
//  			{
//  				int averagePlateInfluence = .4;
//  				int gridAverage = (getDistance(mountainGrid.get(x, y), false) + getDistance(oceanGrid.get(x, y), true) + getDistance(coastGrid.get(x, y), false)) / 3;
//  				int average = (gridAverage + (cellMap.at(jndex).tectonicPlate*averagePlateInfluence)) / 2;
//  				cellMap.at(jndex).elevation = average;
//  				jndex++;
//  			}
//  		}
//  	}
//  
//  	PlateDynamics comparePlateMovement(Vec2f a, Vec2f b, int aHeight, int bHeight)
//  	{
//  		bool aOcean = (aHeight < oceanElevation);
//  		bool bOcean = (bHeight < oceanElevation);
//  
//  		float dot = a.x * b.x + a.y * b.y;
//  
//  
//  		if (dot <= .6) //Moving away, farther
//  		{
//  			if (!aOcean) //a is land
//  			{
//  				if (!bOcean) //b is land, continental divergence, coastline
//  				{
//  					return CoastLine;
//  				}
//  				else //b is ocean, oceanic divergence, ocean
//  				{
//  					return Ocean;
//  				}
//  			}
//  			else //a is ocean
//  			{
//  				if (!bOcean) //b is land, oceanic divergance, coastline
//  				{
//  					return Ocean;
//  				}
//  				else //b is ocean, oceanic divergence, ocean
//  				{
//  					return Ocean;
//  				}
//  			}
//  		}
//  		else if (dot > .6) //Moving toward each other, closer
//  		{
//  			if (!aOcean) //a is land
//  			{
//  				if (!bOcean) //b is land, continental convergance, mountain
//  				{
//  					return Mountain;
//  				}
//  				else //b is ocean, oceanic subduction, mountain
//  				{
//  					return Mountain;
//  				}
//  			}
//  			else //a is ocean
//  			{
//  				if (!bOcean) //b is land, oceanic subduction, mountain
//  				{
//  					return Mountain;
//  				}
//  				else //b is ocean, oceanic convergance, coastline
//  				{
//  					return CoastLine;
//  				}
//  			}
//  		}
//  		return CoastLine;
//  	}
//  
//  	Planet() {};
//  
//  	Planet(int worldSize)
//  	{
//  		this->worldSize = worldSize;
//  		cellMap = std::vector<Cell>(worldSize*worldSize);
//  	}
//  
//  	~Planet()
//  	{
//  	};
//  	std::vector<bool> mountainMap;
//  	std::vector<bool> oceanMap;
//  	std::vector<bool> coastlineMap;
//  private:
//  	std::vector<int> elevationMap;
//  	std::vector<Vec2f> plateMovementMap;
//  	//Used for distance fields
//  };
//  






/*




bool
{
GL_COLOR_LOGIC_OP
data returns a single boolean value indicating whether a fragment's RGBA color values are merged into the framebuffer using a logical operation. The initial value is GL_FALSE. See glLogicOp.


GL_CULL_FACE
data returns a single boolean value indicating whether polygon culling is enabled.The initial value is GL_FALSE.See glCullFace.

GL_DEPTH_TEST
data returns a single boolean value indicating whether depth testing of fragments is enabled.The initial value is GL_FALSE.See glDepthFunc and glDepthRange.

GL_DEPTH_WRITEMASK
data returns a single boolean value indicating if the depth buffer is enabled for writing.The initial value is GL_TRUE.See glDepthMask.

GL_DITHER
data returns a single boolean value indicating whether dithering of fragment colors and indices is enabled.The initial value is GL_TRUE.

GL_DOUBLEBUFFER
data returns a single boolean value indicating whether double buffering is supported.

}


int
{
GL_SMOOTH_LINE_WIDTH_GRANULARITY
data returns a single value indicating the level of quantization applied to smooth line width parameters.

GL_STENCIL_BACK_FAIL
data returns one value, a symbolic constant indicating what action is taken for back - facing polygons when the stencil test fails.The initial value is GL_KEEP.See glStencilOpSeparate.

GL_STENCIL_BACK_FUNC
data returns one value, a symbolic constant indicating what function is used for back - facing polygons to compare the stencil reference value with the stencil buffer value.The initial value is GL_ALWAYS.See glStencilFuncSeparate.

GL_STENCIL_BACK_PASS_DEPTH_FAIL
data returns one value, a symbolic constant indicating what action is taken for back - facing polygons when the stencil test passes, but the depth test fails.The initial value is GL_KEEP.See glStencilOpSeparate.

GL_STENCIL_BACK_PASS_DEPTH_PASS
data returns one value, a symbolic constant indicating what action is taken for back - facing polygons when the stencil test passes and the depth test passes.The initial value is GL_KEEP.See glStencilOpSeparate.

GL_STENCIL_BACK_REF
data returns one value, the reference value that is compared with the contents of the stencil buffer for back - facing polygons.The initial value is 0. See glStencilFuncSeparate.

GL_STENCIL_BACK_VALUE_MASK
data returns one value, the mask that is used for back - facing polygons to mask both the stencil reference value and the stencil buffer value before they are compared.The initial value is all 1's. See glStencilFuncSeparate.

GL_STENCIL_BACK_WRITEMASK
data returns one value, the mask that controls writing of the stencil bitplanes for back - facing polygons.The initial value is all 1's. See glStencilMaskSeparate.

GL_STENCIL_CLEAR_VALUE
data returns one value, the index to which the stencil bitplanes are cleared.The initial value is 0. See glClearStencil.

GL_STENCIL_FAIL
data returns one value, a symbolic constant indicating what action is taken when the stencil test fails.The initial value is GL_KEEP.See glStencilOp.This stencil state only affects non - polygons and front - facing polygons.Back - facing polygons use separate stencil state.See glStencilOpSeparate.

GL_STENCIL_FUNC
data returns one value, a symbolic constant indicating what function is used to compare the stencil reference value with the stencil buffer value.The initial value is GL_ALWAYS.See glStencilFunc.This stencil state only affects non - polygons and front - facing polygons.Back - facing polygons use separate stencil state.See glStencilFuncSeparate.

GL_STENCIL_PASS_DEPTH_FAIL
data returns one value, a symbolic constant indicating what action is taken when the stencil test passes, but the depth test fails.The initial value is GL_KEEP.See glStencilOp.This stencil state only affects non - polygons and front - facing polygons.Back - facing polygons use separate stencil state.See glStencilOpSeparate.

GL_STENCIL_PASS_DEPTH_PASS
data returns one value, a symbolic constant indicating what action is taken when the stencil test passes and the depth test passes.The initial value is GL_KEEP.See glStencilOp.This stencil state only affects non - polygons and front - facing polygons.Back - facing polygons use separate stencil state.See glStencilOpSeparate.

GL_STENCIL_REF
data returns one value, the reference value that is compared with the contents of the stencil buffer.The initial value is 0. See glStencilFunc.This stencil state only affects non - polygons and front - facing polygons.Back - facing polygons use separate stencil state.See glStencilFuncSeparate.

GL_STENCIL_VALUE_MASK
data returns one value, the mask that is used to mask both the stencil reference value and the stencil buffer value before they are compared.The initial value is all 1's. See glStencilFunc. This stencil state only affects non-polygons and front-facing polygons. Back-facing polygons use separate stencil state. See glStencilFuncSeparate.

GL_STENCIL_WRITEMASK
data returns one value, the mask that controls writing of the stencil bitplanes.The initial value is all 1's. See glStencilMask. This stencil state only affects non-polygons and front-facing polygons. Back-facing polygons use separate stencil state. See glStencilMaskSeparate.
GL_SUBPIXEL_BITS
data returns one value, an estimate of the number of bits of subpixel resolution that are used to position rasterized geometry in window coordinates.The value must be at least 4.

GL_TEXTURE_BINDING_1D
data returns a single value, the name of the texture currently bound to the target GL_TEXTURE_1D.The initial value is 0. See glBindTexture.

GL_TEXTURE_BINDING_1D_ARRAY
data returns a single value, the name of the texture currently bound to the target GL_TEXTURE_1D_ARRAY.The initial value is 0. See glBindTexture.

GL_TEXTURE_BINDING_2D
data returns a single value, the name of the texture currently bound to the target GL_TEXTURE_2D.The initial value is 0. See glBindTexture.

GL_TEXTURE_BINDING_2D_ARRAY
data returns a single value, the name of the texture currently bound to the target GL_TEXTURE_2D_ARRAY.The initial value is 0. See glBindTexture.

GL_TEXTURE_BINDING_2D_MULTISAMPLE
data returns a single value, the name of the texture currently bound to the target GL_TEXTURE_2D_MULTISAMPLE.The initial value is 0. See glBindTexture.

GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY
data returns a single value, the name of the texture currently bound to the target GL_TEXTURE_2D_MULTISAMPLE_ARRAY.The initial value is 0. See glBindTexture.

GL_TEXTURE_BINDING_3D
data returns a single value, the name of the texture currently bound to the target GL_TEXTURE_3D.The initial value is 0. See glBindTexture.

GL_TEXTURE_BINDING_BUFFER
data returns a single value, the name of the texture currently bound to the target GL_TEXTURE_BUFFER.The initial value is 0. See glBindTexture.

GL_TEXTURE_BINDING_CUBE_MAP
data returns a single value, the name of the texture currently bound to the target GL_TEXTURE_CUBE_MAP.The initial value is 0. See glBindTexture.

GL_TEXTURE_BINDING_RECTANGLE
data returns a single value, the name of the texture currently bound to the target GL_TEXTURE_RECTANGLE.The initial value is 0. See glBindTexture.

GL_TEXTURE_COMPRESSION_HINT
data returns a single value indicating the mode of the texture compression hint.The initial value is GL_DONT_CARE.

GL_TEXTURE_BINDING_BUFFER
data returns a single value, the name of the buffer object currently bound to the GL_TEXTURE_BUFFER buffer binding point.The initial value is 0. See glBindBuffer.

GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT
data returns a single value, the minimum required alignment for texture buffer sizes and offset.The initial value is 1. See glUniformBlockBinding.

GL_TIMESTAMP
data returns a single value, the 64 - bit value of the current GL time.See glQueryCounter.


GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS
data returns one value, the maximum number of active shader storage blocks that may be accessed by a fragment shader.

GL_MAX_FRAGMENT_INPUT_COMPONENTS
data returns one value, the maximum number of components of the inputs read by the fragment shader, which must be at least 128.

GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
data returns one value, the maximum number of individual floating - point, integer, or boolean values that can be held in uniform variable storage for a fragment shader.The value must be at least 1024. See glUniform.

GL_MAX_FRAGMENT_UNIFORM_VECTORS
data returns one value, the maximum number of individual 4 - vectors of floating - point, integer, or boolean values that can be held in uniform variable storage for a fragment shader.The value is equal to the value of GL_MAX_FRAGMENT_UNIFORM_COMPONENTS divided by 4 and must be at least 256. See glUniform.

GL_MAX_FRAGMENT_UNIFORM_BLOCKS
data returns one value, the maximum number of uniform blocks per fragment shader.The value must be at least 12. See glUniformBlockBinding.

GL_MAX_FRAMEBUFFER_WIDTH
data returns one value, the maximum width for a framebuffer that has no attachments, which must be at least 16384. See glFramebufferParameter.

GL_MAX_FRAMEBUFFER_HEIGHT
data returns one value, the maximum height for a framebuffer that has no attachments, which must be at least 16384. See glFramebufferParameter.

GL_MAX_FRAMEBUFFER_LAYERS
data returns one value, the maximum number of layers for a framebuffer that has no attachments, which must be at least 2048. See glFramebufferParameter.

GL_MAX_FRAMEBUFFER_SAMPLES
data returns one value, the maximum samples in a framebuffer that has no attachments, which must be at least 4. See glFramebufferParameter.

GL_MAX_GEOMETRY_ATOMIC_COUNTERS
data returns a single value, the maximum number of atomic counters available to geometry shaders.

GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS
data returns one value, the maximum number of active shader storage blocks that may be accessed by a geometry shader.

GL_MAX_GEOMETRY_INPUT_COMPONENTS
data returns one value, the maximum number of components of inputs read by a geometry shader, which must be at least 64.

GL_MAX_GEOMETRY_OUTPUT_COMPONENTS
data returns one value, the maximum number of components of outputs written by a geometry shader, which must be at least 128.

GL_MAX_GEOMETRY_TEXTURE_Image_UNITS
data returns one value, the maximum supported texture Image units that can be used to access texture maps from the geometry shader.The value must be at least 16. See glActiveTexture.

GL_MAX_GEOMETRY_UNIFORM_BLOCKS
data returns one value, the maximum number of uniform blocks per geometry shader.The value must be at least 12. See glUniformBlockBinding.

GL_MAX_GEOMETRY_UNIFORM_COMPONENTS
data returns one value, the maximum number of individual floating - point, integer, or boolean values that can be held in uniform variable storage for a geometry shader.The value must be at least 1024. See glUniform.

GL_MAX_INTEGER_SAMPLES
data returns one value, the maximum number of samples supported in integer format multisample buffers.

GL_MIN_MAP_BUFFER_ALIGNMENT
data returns one value, the minimum alignment in basic machine units of pointers returned fromglMapBuffer and glMapBufferRange.This value must be a power of two and must be at least 64.

GL_MAX_LABEL_LENGTH
data returns one value, the maximum length of a label that may be assigned to an object.See glObjectLabel and glObjectPtrLabel.

GL_MAX_PROGRAM_TEXEL_OFFSET
data returns one value, the maximum texel offset allowed in a texture lookup, which must be at least 7.

GL_MIN_PROGRAM_TEXEL_OFFSET
data returns one value, the minimum texel offset allowed in a texture lookup, which must be at most - 8.

GL_MAX_RECTANGLE_TEXTURE_SIZE
data returns one value.The value gives a rough estimate of the largest rectangular texture that the GL can handle.The value must be at least 1024. Use GL_PROXY_TEXTURE_RECTANGLE to determine if a texture is too large.See glTexImage2D.

GL_MAX_RENDERBUFFER_SIZE
data returns one value.The value indicates the maximum supported size for renderbuffers.See glFramebufferRenderbuffer.

GL_MAX_SAMPLE_MASK_WORDS
data returns one value, the maximum number of sample mask words.

GL_MAX_SERVER_WAIT_TIMEOUT
data returns one value, the maximum glWaitSync timeout interval.

GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS
data returns one value, the maximum number of shader storage buffer binding points on the context, which must be at least 8.

GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS
data returns a single value, the maximum number of atomic counters available to tessellation control shaders.

GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS
data returns a single value, the maximum number of atomic counters available to tessellation evaluation shaders.

GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS
data returns one value, the maximum number of active shader storage blocks that may be accessed by a tessellation control shader.

GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS
data returns one value, the maximum number of active shader storage blocks that may be accessed by a tessellation evaluation shader.

GL_MAX_TEXTURE_BUFFER_SIZE
data returns one value.The value gives the maximum number of texels allowed in the texel array of a texture buffer object.Value must be at least 65536.

GL_MAX_TEXTURE_Image_UNITS
data returns one value, the maximum supported texture Image units that can be used to access texture maps from the fragment shader.The value must be at least 16. See glActiveTexture.

GL_MAX_TEXTURE_LOD_BIAS
data returns one value, the maximum, absolute value of the texture level - of - detail bias.The value must be at least 2.0.

GL_MAX_TEXTURE_SIZE
data returns one value.The value gives a rough estimate of the largest texture that the GL can handle.The value must be at least 1024. Use a proxy texture target such as GL_PROXY_TEXTURE_1D or GL_PROXY_TEXTURE_2D to determine if a texture is too large.See glTexImage1D and glTexImage2D.

GL_MAX_UNIFORM_BUFFER_BINDINGS
data returns one value, the maximum number of uniform buffer binding points on the context, which must be at least 36.

GL_MAX_UNIFORM_BLOCK_SIZE
data returns one value, the maximum size in basic machine units of a uniform block, which must be at least 16384.

GL_MAX_UNIFORM_LOCATIONS
data returns one value, the maximum number of explicitly assignable uniform locations, which must be at least 1024.

GL_MAX_VARYING_COMPONENTS
data returns one value, the number components for varying variables, which must be at least 60.

GL_MAX_VARYING_VECTORS
data returns one value, the number 4 - vectors for varying variables, which is equal to the value of GL_MAX_VARYING_COMPONENTS and must be at least 15.

GL_MAX_VARYING_FLOATS
data returns one value, the maximum number of interpolators available for processing varying variables used by vertex and fragment shaders.This value represents the number of individual floating - point values that can be interpolated; varying variables declared as vectors, matrices, and arrays will all consume multiple interpolators.The value must be at least 32.

GL_MAX_VERTEX_ATOMIC_COUNTERS
data returns a single value, the maximum number of atomic counters available to vertex shaders.

GL_MAX_VERTEX_ATTRIBS
data returns one value, the maximum number of 4 - component generic vertex attributes accessible to a vertex shader.The value must be at least 16. See glVertexAttrib.

GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS
data returns one value, the maximum number of active shader storage blocks that may be accessed by a vertex shader.

GL_MAX_VERTEX_TEXTURE_Image_UNITS
data returns one value, the maximum supported texture Image units that can be used to access texture maps from the vertex shader.The value may be at least 16. See glActiveTexture.

GL_MAX_VERTEX_UNIFORM_COMPONENTS
data returns one value, the maximum number of individual floating - point, integer, or boolean values that can be held in uniform variable storage for a vertex shader.The value must be at least 1024. See glUniform.

GL_MAX_VERTEX_UNIFORM_VECTORS
data returns one value, the maximum number of 4 - vectors that may be held in uniform variable storage for the vertex shader.The value of GL_MAX_VERTEX_UNIFORM_VECTORS is equal to the value of GL_MAX_VERTEX_UNIFORM_COMPONENTS and must be at least 256.

GL_MAX_VERTEX_OUTPUT_COMPONENTS
data returns one value, the maximum number of components of output written by a vertex shader, which must be at least 64.

GL_MAX_VERTEX_UNIFORM_BLOCKS
data returns one value, the maximum number of uniform blocks per vertex shader.The value must be at least 12. See glUniformBlockBinding.

GL_MAX_VIEWPORT_DIMS
data returns two values : the maximum supported width and height of the viewport.These must be at least as large as the visible dimensions of the display being rendered to.See glViewport.

GL_MAX_VIEWPORTS
data returns one value, the maximum number of simultaneous viewports that are supported.The value must be at least 16. See glViewportIndexed.

GL_MINOR_VERSION
data returns one value, the minor version number of the OpenGL API supported by the current context.

GL_NUM_COMPRESSED_TEXTURE_FORMATS
data returns a single integer value indicating the number of available compressed texture formats.The minimum value is 4. See glCompressedTexImage2D.

GL_NUM_EXTENSIONS
data returns one value, the number of extensions supported by the GL implementation for the current context.See glGetString.

GL_NUM_PROGRAM_BINARY_FORMATS
data returns one value, the number of program binary formats supported by the implementation.

GL_NUM_SHADER_BINARY_FORMATS
data returns one value, the number of binary shader formats supported by the implementation.If this value is greater than zero, then the implementation supports loading binary shaders.If it is zero, then the loading of binary shaders by the implementation is not supported.

GL_PACK_ALIGNMENT
data returns one value, the byte alignment used for writing pixel data to memory.The initial value is 4. See glPixelStore.

GL_PACK_Image_HEIGHT
data returns one value, the Image height used for writing pixel data to memory.The initial value is 0. See glPixelStore.


	GL_LINE_SMOOTH_HINT
data returns one value, a symbolic constant indicating the mode of the line antialiasing hint.The initial value is GL_DONT_CARE.See glHint.

GL_LINE_WIDTH
data returns one value, the line width as specified with glLineWidth.The initial value is 1.

GL_LAYER_PROVOKING_VERTEX
data returns one value, the implementation dependent specifc vertex of a primitive that is used to select the rendering layer.If the value returned is equivalent to GL_PROVOKING_VERTEX, then the vertex selection follows the convention specified by glProvokingVertex.If the value returned is equivalent to GL_FIRST_VERTEX_CONVENTION, then the selection is always taken from the first vertex in the primitive.If the value returned is equivalent to GL_LAST_VERTEX_CONVENTION, then the selection is always taken from the last vertex in the primitive.If the value returned is equivalent to GL_UNDEFINED_VERTEX, then the selection is not guaranteed to be taken from any specific vertex in the primitive.

GL_LOGIC_OP_MODE
data returns one value, a symbolic constant indicating the selected logic operation mode.The initial value is GL_COPY.See glLogicOp.

GL_MAJOR_VERSION
data returns one value, the major version number of the OpenGL API supported by the current context.

GL_MAX_3D_TEXTURE_SIZE
data returns one value, a rough estimate of the largest 3D texture that the GL can handle.The value must be at least 64. Use GL_PROXY_TEXTURE_3D to determine if a texture is too large.See glTexImage3D.

GL_MAX_ARRAY_TEXTURE_LAYERS
data returns one value.The value indicates the maximum number of layers allowed in an array texture, and must be at least 256. See glTexImage2D.

GL_MAX_CLIP_DISTANCES
data returns one value, the maximum number of application - defined clipping distances.The value must be at least 8.

GL_MAX_COLOR_TEXTURE_SAMPLES
data returns one value, the maximum number of samples in a color multisample texture.


GL_DRAW_BUFFER
data returns one value, a symbolic constant indicating which buffers are being drawn to.See glDrawBuffer.The initial value is GL_BACK if there are back buffers, otherwise it is GL_FRONT.

GL_DRAW_BUFFER i
data returns one value, a symbolic constant indicating which buffers are being drawn to by the corresponding output color.See glDrawBuffers.The initial value of GL_DRAW_BUFFER0 is GL_BACK if there are back buffers, otherwise it is GL_FRONT.The initial values of draw buffers for all other output colors is GL_NONE.

GL_DRAW_FRAMEBUFFER_BINDING
data returns one value, the name of the framebuffer object currently bound to the GL_DRAW_FRAMEBUFFER target.If the default framebuffer is bound, this value will be zero.The initial value is zero.See glBindFramebuffer.

GL_READ_FRAMEBUFFER_BINDING
data returns one value, the name of the framebuffer object currently bound to the GL_READ_FRAMEBUFFER target.If the default framebuffer is bound, this value will be zero.The initial value is zero.See glBindFramebuffer.

GL_ELEMENT_ARRAY_BUFFER_BINDING
data returns a single value, the name of the buffer object currently bound to the target GL_ELEMENT_ARRAY_BUFFER.If no buffer object is bound to this target, 0 is returned.The initial value is 0. See glBindBuffer.

GL_FRAGMENT_SHADER_DERIVATIVE_HINT
data returns one value, a symbolic constant indicating the mode of the derivative accuracy hint for fragment shaders.The initial value is GL_DONT_CARE.See glHint.

GL_IMPLEMENTATION_COLOR_READ_FORMAT
data returns a single GLenum value indicating the implementation's preferred pixel data format. See glReadPixels.

GL_IMPLEMENTATION_COLOR_READ_TYPE
data returns a single GLenum value indicating the implementation's preferred pixel data type. See glReadPixels.

GL_CULL_FACE_MODE
data returns a single value indicating the mode of polygon culling.The initial value is GL_BACK.See glCullFace.

GL_ARRAY_BUFFER_BINDING
data returns a single value, the name of the buffer object currently bound to the target GL_ARRAY_BUFFER.If no buffer object is bound to this target, 0 is returned.The initial value is 0. See glBindBuffer.
GL_ACTIVE_TEXTURE
data returns a single value indicating the active multitexture unit.The initial value is GL_TEXTURE0.See glActiveTexture.

GL_BLEND_COLOR
data returns four values, the red, green, blue, and alpha values which are the components of the blend color.See glBlendColor.

GL_BLEND_DST_ALPHA
data returns one value, the symbolic constant identifying the alpha destination blend function.The initial value is GL_ZERO.See glBlendFunc and glBlendFuncSeparate.

GL_BLEND_DST_RGB
data returns one value, the symbolic constant identifying the RGB destination blend function.The initial value is GL_ZERO.See glBlendFunc and glBlendFuncSeparate.

GL_BLEND_EQUATION_RGB
data returns one value, a symbolic constant indicating whether the RGB blend equation is GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN or GL_MAX.See glBlendEquationSeparate.

GL_BLEND_EQUATION_ALPHA
data returns one value, a symbolic constant indicating whether the Alpha blend equation is GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN or GL_MAX.See glBlendEquationSeparate.

GL_BLEND_SRC_ALPHA
data returns one value, the symbolic constant identifying the alpha source blend function.The initial value is GL_ONE.See glBlendFunc and glBlendFuncSeparate.

GL_BLEND_SRC_RGB
data returns one value, the symbolic constant identifying the RGB source blend function.The initial value is GL_ONE.See glBlendFunc and glBlendFuncSeparate.

GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS
data returns one value, the maximum number of active shader storage blocks that may be accessed by a compute shader.

GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS
data returns one value, the maximum total number of active shader storage blocks that may be accessed by all active shaders.

GL_MAX_COMPUTE_UNIFORM_BLOCKS
data returns one value, the maximum number of uniform blocks per compute shader.The value must be at least 14. See glUniformBlockBinding.

GL_MAX_COMPUTE_TEXTURE_Image_UNITS
data returns one value, the maximum supported texture Image units that can be used to access texture maps from the compute shader.The value may be at least 16. See glActiveTexture.

GL_MAX_COMPUTE_UNIFORM_COMPONENTS
data returns one value, the maximum number of individual floating - point, integer, or boolean values that can be held in uniform variable storage for a compute shader.The value must be at least 1024. See glUniform.

GL_MAX_COMPUTE_ATOMIC_COUNTERS
data returns a single value, the maximum number of atomic counters available to compute shaders.

GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS
data returns a single value, the maximum number of atomic counter buffers that may be accessed by a compute shader.

GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS
data returns one value, the number of words for compute shader uniform variables in all uniform blocks(including default).The value must be at least 1. See glUniform.

GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS
data returns one value, the number of invocations in a single local work group(i.e., the product of the three dimensions) that may be dispatched to a compute shader.

GL_DISPATCH_INDIRECT_BUFFER_BINDING
data returns a single value, the name of the buffer object currently bound to the target GL_DISPATCH_INDIRECT_BUFFER.If no buffer object is bound to this target, 0 is returned.The initial value is 0. See glBindBuffer.

GL_MAX_DEBUG_GROUP_STACK_DEPTH
data returns a single value, the maximum depth of the debug message group stack.

GL_DEBUG_GROUP_STACK_DEPTH
data returns a single value, the current depth of the debug message group stack.

GL_CONTEXT_FLAGS
data returns one value, the flags with which the context was created(such as debugging functionality).

GL_CURRENT_PROGRAM
data returns one value, the name of the program object that is currently active, or 0 if no program object is active.See glUseProgram.

GL_DEPTH_CLEAR_VALUE
data returns one value, the value that is used to clear the depth buffer.Integer values, if requested, are linearly mapped from the internal floating - point representation such that 1.0 returns the most positive representable integer value, and −1.0 returns the most negative representable integer value.The initial value is 1. See glClearDepth.

GL_DEPTH_FUNC
data returns one value, the symbolic constant that indicates the depth comparison function.The initial value is GL_LESS.See glDepthFunc.
}

Vec2
{
GL_ALIASED_LINE_WIDTH_RANGE
data returns a pair of values indicating the range of widths supported for aliased lines.See glLineWidth.

GL_DEPTH_RANGE
data returns two values : the near and far mapping limits for the depth buffer.Integer values, if requested, are linearly mapped from the internal floating - point representation such that 1.0 returns the most positive representable integer value, and −1.0 returns the most negative representable integer value.The initial value is(0, 1).See glDepthRange.
}


Vec3
{

}

Vec4
{



}

GL_COLOR_CLEAR_VALUE
data returns four values : the red, green, blue, and alpha values used to clear the color buffers.Integer values, if requested, are linearly mapped from the internal floating - point representation such that 1.0 returns the most positive representable integer value, and −1.0 returns the most negative representable integer value.The initial value is(0, 0, 0, 0).See glClearColor.

Matrix
{

}

Other
{
	
	GL_COLOR_WRITEMASK
data returns four boolean values : the red, green, blue, and alpha write enables for the color buffers.The initial value is(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE).See glColorMask.

GL_COMPRESSED_TEXTURE_FORMATS
data returns a list of symbolic constants of length GL_NUM_COMPRESSED_TEXTURE_FORMATS indicating which compressed texture formats are available.See glCompressedTexImage2D.
GL_MAX_COMPUTE_WORK_GROUP_COUNT
Accepted by the indexed versions of glGet.data the maximum number of work groups that may be dispatched to a compute shader.Indices 0, 1, and 2 correspond to the X, Y and Z dimensions, respectively.

GL_MAX_COMPUTE_WORK_GROUP_SIZE
Accepted by the indexed versions of glGet.data the maximum size of a work groups that may be used during compilation of a compute shader.Indices 0, 1, and 2 correspond to the X, Y and Z dimensions, respectively.


}

GL_LINE_SMOOTH
data returns a single boolean value indicating whether antialiasing of lines is enabled.The initial value is GL_FALSE.See glLineWidth.


GL_MAX_COMBINED_ATOMIC_COUNTERS
data returns a single value, the maximum number of atomic counters available to all active shaders.

GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS
data returns one value, the number of words for fragment shader uniform variables in all uniform blocks(including default).The value must be at least 1. See glUniform.

GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS
data returns one value, the number of words for geometry shader uniform variables in all uniform blocks(including default).The value must be at least 1. See glUniform.

GL_MAX_COMBINED_TEXTURE_Image_UNITS
data returns one value, the maximum supported texture Image units that can be used to access texture maps from the vertex shader and the fragment processor combined.If both the vertex shader and the fragment processing stage access the same texture Image unit, then that counts as using two texture Image units against this limit.The value must be at least 48. See glActiveTexture.

GL_MAX_COMBINED_UNIFORM_BLOCKS
data returns one value, the maximum number of uniform blocks per program.The value must be at least 70. See glUniformBlockBinding.

GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS
data returns one value, the number of words for vertex shader uniform variables in all uniform blocks(including default).The value must be at least 1. See glUniform.

GL_MAX_CUBE_MAP_TEXTURE_SIZE
data returns one value.The value gives a rough estimate of the largest cube - map texture that the GL can handle.The value must be at least 1024. Use GL_PROXY_TEXTURE_CUBE_MAP to determine if a texture is too large.See glTexImage2D.

GL_MAX_DEPTH_TEXTURE_SAMPLES
data returns one value, the maximum number of samples in a multisample depth or depth - stencil texture.

GL_MAX_DRAW_BUFFERS
data returns one value, the maximum number of simultaneous outputs that may be written in a fragment shader.The value must be at least 8. See glDrawBuffers.

GL_MAX_DUAL_SOURCE_DRAW_BUFFERS
data returns one value, the maximum number of active draw buffers when using dual - source blending.The value must be at least 1. See glBlendFunc and glBlendFuncSeparate.

GL_MAX_ELEMENTS_INDICES
data returns one value, the recommended maximum number of vertex array indices.See glDrawRangeElements.

GL_MAX_ELEMENTS_VERTICES
data returns one value, the recommended maximum number of vertex array vertices.See glDrawRangeElements.

GL_MAX_FRAGMENT_ATOMIC_COUNTERS
data returns a single value, the maximum number of atomic counters available to fragment shaders.
GL_PACK_LSB_FIRST
data returns a single boolean value indicating whether single - bit pixels being written to memory are written first to the least significant bit of each unsigned byte.The initial value is GL_FALSE.See glPixelStore.

GL_PACK_ROW_LENGTH
data returns one value, the row length used for writing pixel data to memory.The initial value is 0. See glPixelStore.

GL_PACK_SKIP_ImageS
data returns one value, the number of pixel Images skipped before the first pixel is written into memory.The initial value is 0. See glPixelStore.

GL_PACK_SKIP_PIXELS
data returns one value, the number of pixel locations skipped before the first pixel is written into memory.The initial value is 0. See glPixelStore.

GL_PACK_SKIP_ROWS
data returns one value, the number of rows of pixel locations skipped before the first pixel is written into memory.The initial value is 0. See glPixelStore.

GL_PACK_SWAP_BYTES
data returns a single boolean value indicating whether the bytes of two - byte and four - byte pixel indices and components are swapped before being written to memory.The initial value is GL_FALSE.See glPixelStore.

GL_PIXEL_PACK_BUFFER_BINDING
data returns a single value, the name of the buffer object currently bound to the target GL_PIXEL_PACK_BUFFER.If no buffer object is bound to this target, 0 is returned.The initial value is 0. See glBindBuffer.

GL_PIXEL_UNPACK_BUFFER_BINDING
data returns a single value, the name of the buffer object currently bound to the target GL_PIXEL_UNPACK_BUFFER.If no buffer object is bound to this target, 0 is returned.The initial value is 0. See glBindBuffer.

GL_POINT_FADE_THRESHOLD_SIZE
data returns one value, the point size threshold for determining the point size.See glPointParameter.

GL_PRIMITIVE_RESTART_INDEX
data returns one value, the current primitive restart index.The initial value is 0. See glPrimitiveRestartIndex.

GL_PROGRAM_BINARY_FORMATS
data an array of GL_NUM_PROGRAM_BINARY_FORMATS values, indicating the proram binary formats supported by the implementation.

GL_PROGRAM_PIPELINE_BINDING
data a single value, the name of the currently bound program pipeline object, or zero if no program pipeline object is bound.See glBindProgramPipeline.

GL_PROGRAM_POINT_SIZE
data returns a single boolean value indicating whether vertex program point size mode is enabled.If enabled, then the point size is taken from the shader built - in gl_PointSize.If disabled, then the point size is taken from the point state as specified by glPointSize.The initial value is GL_FALSE.

GL_PROVOKING_VERTEX
data returns one value, the currently selected provoking vertex convention.The initial value is GL_LAST_VERTEX_CONVENTION.See glProvokingVertex.

GL_POINT_SIZE
data returns one value, the point size as specified by glPointSize.The initial value is 1.

GL_POINT_SIZE_GRANULARITY
data returns one value, the size difference between adjacent supported sizes for antialiased points.See glPointSize.

GL_POINT_SIZE_RANGE
data returns two values : the smallest and largest supported sizes for antialiased points.The smallest size must be at most 1, and the largest size must be at least 1. See glPointSize.

GL_POLYGON_OFFSET_FACTOR
data returns one value, the scaling factor used to determine the variable offset that is added to the depth value of each fragment generated when a polygon is rasterized.The initial value is 0. See glPolygonOffset.

GL_POLYGON_OFFSET_UNITS
data returns one value.This value is multiplied by an implementation - specific value and then added to the depth value of each fragment generated when a polygon is rasterized.The initial value is 0. See glPolygonOffset.

GL_POLYGON_OFFSET_FILL
data returns a single boolean value indicating whether polygon offset is enabled for polygons in fill mode.The initial value is GL_FALSE.See glPolygonOffset.

GL_POLYGON_OFFSET_LINE
data returns a single boolean value indicating whether polygon offset is enabled for polygons in line mode.The initial value is GL_FALSE.See glPolygonOffset.

GL_POLYGON_OFFSET_POINT
data returns a single boolean value indicating whether polygon offset is enabled for polygons in point mode.The initial value is GL_FALSE.See glPolygonOffset.

GL_POLYGON_SMOOTH
data returns a single boolean value indicating whether antialiasing of polygons is enabled.The initial value is GL_FALSE.See glPolygonMode.

GL_POLYGON_SMOOTH_HINT
data returns one value, a symbolic constant indicating the mode of the polygon antialiasing hint.The initial value is GL_DONT_CARE.See glHint.

GL_READ_BUFFER
data returns one value, a symbolic constant indicating which color buffer is selected for reading.The initial value is GL_BACK if there is a back buffer, otherwise it is GL_FRONT.See glReadPixels.

GL_RENDERBUFFER_BINDING
data returns a single value, the name of the renderbuffer object currently bound to the target GL_RENDERBUFFER.If no renderbuffer object is bound to this target, 0 is returned.The initial value is 0. See glBindRenderbuffer.

GL_SAMPLE_BUFFERS
data returns a single integer value indicating the number of sample buffers associated with the framebuffer.See glSampleCoverage.

GL_SAMPLE_COVERAGE_VALUE
data returns a single positive floating - point value indicating the current sample coverage value.See glSampleCoverage.

GL_SAMPLE_COVERAGE_INVERT
data returns a single boolean value indicating if the temporary coverage value should be inverted.See glSampleCoverage.

GL_SAMPLER_BINDING
data returns a single value, the name of the sampler object currently bound to the active texture unit.The initial value is 0. See glBindSampler.

GL_SAMPLES
data returns a single integer value indicating the coverage mask size.See glSampleCoverage.

GL_SCISSOR_BOX
data returns four values : the x and y window coordinates of the scissor box, followed by its width and height.Initially the x and y window coordinates are both 0 and the width and height are set to the size of the window.See glScissor.

GL_SCISSOR_TEST
data returns a single boolean value indicating whether scissoring is enabled.The initial value is GL_FALSE.See glScissor.

GL_SHADER_COMPILER
data returns a single boolean value indicating whether an online shader compiler is present in the implementation.All desktop OpenGL implementations must support online shader compilations, and therefore the value of GL_SHADER_COMPILER will always be GL_TRUE.

GL_SHADER_STORAGE_BUFFER_BINDING
When used with non - indexed variants of glGet(such as glGetIntegerv), data returns a single value, the name of the buffer object currently bound to the target GL_SHADER_STORAGE_BUFFER.If no buffer object is bound to this target, 0 is returned.When used with indexed variants of glGet(such as glGetIntegeri_v), data returns a single value, the name of the buffer object bound to the indexed shader storage buffer binding points.The initial value is 0 for all targets.See glBindBuffer, glBindBufferBase, and glBindBufferRange.

GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT
data returns a single value, the minimum required alignment for shader storage buffer sizes and offset.The initial value is 1. See glShaderStorageBlockBinding.

GL_SHADER_STORAGE_BUFFER_START
When used with indexed variants of glGet(such as glGetInteger64i_v), data returns a single value, the start offset of the binding range for each indexed shader storage buffer binding.The initial value is 0 for all bindings.See glBindBufferRange.

GL_SHADER_STORAGE_BUFFER_SIZE
When used with indexed variants of glGet(such as glGetInteger64i_v), data returns a single value, the size of the binding range for each indexed shader storage buffer binding.The initial value is 0 for all bindings.See glBindBufferRange.

GL_SMOOTH_LINE_WIDTH_RANGE
data returns a pair of values indicating the range of widths supported for smooth(antialiased) lines.See glLineWidth.

GL_STENCIL_TEST
data returns a single boolean value indicating whether stencil testing of fragments is enabled.The initial value is GL_FALSE.See glStencilFunc and glStencilOp.
GL_STEREO
data returns a single boolean value indicating whether stereo buffers(left and right) are supported.


GL_TRANSFORM_FEEDBACK_BUFFER_BINDING
When used with non - indexed variants of glGet(such as glGetIntegerv), data returns a single value, the name of the buffer object currently bound to the target GL_TRANSFORM_FEEDBACK_BUFFER.If no buffer object is bound to this target, 0 is returned.When used with indexed variants of glGet(such as glGetIntegeri_v), data returns a single value, the name of the buffer object bound to the indexed transform feedback attribute stream.The initial value is 0 for all targets.See glBindBuffer, glBindBufferBase, and glBindBufferRange.

GL_TRANSFORM_FEEDBACK_BUFFER_START
When used with indexed variants of glGet(such as glGetInteger64i_v), data returns a single value, the start offset of the binding range for each transform feedback attribute stream.The initial value is 0 for all streams.See glBindBufferRange.

GL_TRANSFORM_FEEDBACK_BUFFER_SIZE
When used with indexed variants of glGet(such as glGetInteger64i_v), data returns a single value, the size of the binding range for each transform feedback attribute stream.The initial value is 0 for all streams.See glBindBufferRange.

GL_UNIFORM_BUFFER_BINDING
When used with non - indexed variants of glGet(such as glGetIntegerv), data returns a single value, the name of the buffer object currently bound to the target GL_UNIFORM_BUFFER.If no buffer object is bound to this target, 0 is returned.When used with indexed variants of glGet(such as glGetIntegeri_v), data returns a single value, the name of the buffer object bound to the indexed uniform buffer binding point.The initial value is 0 for all targets.See glBindBuffer, glBindBufferBase, and glBindBufferRange.

GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT
data returns a single value, the minimum required alignment for uniform buffer sizes and offset.The initial value is 1. See glUniformBlockBinding.

GL_UNIFORM_BUFFER_SIZE
When used with indexed variants of glGet(such as glGetInteger64i_v), data returns a single value, the size of the binding range for each indexed uniform buffer binding.The initial value is 0 for all bindings.See glBindBufferRange.

GL_UNIFORM_BUFFER_START
When used with indexed variants of glGet(such as glGetInteger64i_v), data returns a single value, the start offset of the binding range for each indexed uniform buffer binding.The initial value is 0 for all bindings.See glBindBufferRange.

GL_UNPACK_ALIGNMENT
data returns one value, the byte alignment used for reading pixel data from memory.The initial value is 4. See glPixelStore.

GL_UNPACK_Image_HEIGHT
data returns one value, the Image height used for reading pixel data from memory.The initial is 0. See glPixelStore.

GL_UNPACK_LSB_FIRST
data returns a single boolean value indicating whether single - bit pixels being read from memory are read first from the least significant bit of each unsigned byte.The initial value is GL_FALSE.See glPixelStore.

GL_UNPACK_ROW_LENGTH
data returns one value, the row length used for reading pixel data from memory.The initial value is 0. See glPixelStore.

GL_UNPACK_SKIP_ImageS
data returns one value, the number of pixel Images skipped before the first pixel is read from memory.The initial value is 0. See glPixelStore.

GL_UNPACK_SKIP_PIXELS
data returns one value, the number of pixel locations skipped before the first pixel is read from memory.The initial value is 0. See glPixelStore.

GL_UNPACK_SKIP_ROWS
data returns one value, the number of rows of pixel locations skipped before the first pixel is read from memory.The initial value is 0. See glPixelStore.

GL_UNPACK_SWAP_BYTES
data returns a single boolean value indicating whether the bytes of two - byte and four - byte pixel indices and components are swapped after being read from memory.The initial value is GL_FALSE.See glPixelStore.

GL_VERTEX_ARRAY_BINDING
data returns a single value, the name of the vertex array object currently bound to the context.If no vertex array object is bound to the context, 0 is returned.The initial value is 0. See glBindVertexArray.

GL_VERTEX_BINDING_DIVISOR
Accepted by the indexed forms.data returns a single integer value representing the instance step divisor of the first element in the bound buffer's data store for vertex attribute bound to index.

GL_VERTEX_BINDING_OFFSET
Accepted by the indexed forms.data returns a single integer value representing the byte offset of the first element in the bound buffer's data store for vertex attribute bound to index.

GL_VERTEX_BINDING_STRIDE
Accepted by the indexed forms.data returns a single integer value representing the byte offset between the start of each element in the bound buffer's data store for vertex attribute bound to index.

GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET
data returns a single integer value containing the maximum offset that may be added to a vertex binding offset.

GL_MAX_VERTEX_ATTRIB_BINDINGS
data returns a single integer value containing the maximum number of vertex buffers that may be bound.

GL_VIEWPORT
When used with non - indexed variants of glGet(such as glGetIntegerv), data returns four values : the x and y window coordinates of the viewport, followed by its width and height.Initially the x and y window coordinates are both set to 0, and the width and height are set to the width and height of the window into which the GL will do its rendering.See glViewport.

When used with indexed variants of glGet(such as glGetIntegeri_v), data returns four values : the x and y window coordinates of the indexed viewport, followed by its width and height.Initially the x and y window coordinates are both set to 0, and the width and height are set to the width and height of the window into which the GL will do its rendering.See glViewportIndexedf.

GL_VIEWPORT_BOUNDS_RANGE
data returns two values, the minimum and maximum viewport bounds range.The minimum range should be at least[-32768, 32767].

GL_VIEWPORT_INDEX_PROVOKING_VERTEX
data returns one value, the implementation dependent specifc vertex of a primitive that is used to select the viewport index.If the value returned is equivalent to GL_PROVOKING_VERTEX, then the vertex selection follows the convention specified by glProvokingVertex.If the value returned is equivalent to GL_FIRST_VERTEX_CONVENTION, then the selection is always taken from the first vertex in the primitive.If the value returned is equivalent to GL_LAST_VERTEX_CONVENTION, then the selection is always taken from the last vertex in the primitive.If the value returned is equivalent to GL_UNDEFINED_VERTEX, then the selection is not guaranteed to be taken from any specific vertex in the primitive.

GL_VIEWPORT_SUBPIXEL_BITS
data returns a single value, the number of bits of sub - pixel precision which the GL uses to interpret the floating point viewport bounds.The minimum value is 0.

GL_MAX_ELEMENT_INDEX
data returns a single value, the maximum index that may be specified during the transfer of generic vertex attributes to the GL.

Many of the boolean parameters can also be queried more easily using glIsEnabled.

Notes
The following parameters return the associated value for the active texture unit : GL_TEXTURE_1D, GL_TEXTURE_BINDING_1D, GL_TEXTURE_2D, GL_TEXTURE_BINDING_2D, GL_TEXTURE_3D and GL_TEXTURE_BINDING_3D.

GL_MAX_VIEWPORTS, GL_VIEWPORT_SUBPIXEL_BITS, GL_VIEWPORT_BOUNDS_RANGE, GL_LAYER_PROVOKING_VERTEX, and GL_VIEWPORT_INDEX_PROVOKING_VERTEX are available only if the GL version is 4.1 or greater.

GL_MAX_VERTEX_ATOMIC_COUNTERS, GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS, GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS, GL_MAX_GEOMETRY_ATOMIC_COUNTERS, GL_MAX_FRAMGENT_ATOMIC_COUNTERS, and GL_MIN_MAP_BUFFER_ALIGNMENT are accepted by pname only if the GL version is 4.2 or greater.

GL_MAX_ELEMENT_INDEX is accepted by pname only if the GL version is 4.3 or greater.

GL_MAX_COMPUTE_UNIFORM_BLOCKS, GL_MAX_COMPUTE_TEXTURE_Image_UNITS, GL_MAX_COMPUTE_UNIFORM_COMPONENTS, GL_MAX_COMPUTE_ATOMIC_COUNTERS, GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS, GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS, GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, GL_MAX_COMPUTE_WORK_GROUP_COUNT, and GL_MAX_COMPUTE_WORK_GROUP_SIZE and GL_DISPATCH_INDIRECT_BUFFER_BINDING are available only if the GL version is 4.3 or greater.

GL_MAX_DEBUG_GROUP_STACK_DEPTH, GL_DEBUG_GROUP_STACK_DEPTH and GL_MAX_LABEL_LENGTH are accepted only if the GL version is 4.3 or greater.

GL_MAX_UNIFORM_LOCATIONS is accepted only if the GL version is 4.3 or greater.

GL_MAX_FRAMEBUFFER_WIDTH, GL_MAX_FRAMEBUFFER_HEIGHT, GL_MAX_FRAMEBUFFER_LAYERS, and GL_MAX_FRAMEBUFFER_SAMPLES are available only if the GL version is 4.3 or greater.

GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS, GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS, GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS, GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, and GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS are available only if the GL version is 4.3 or higher.

GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT is available only if the GL version is 4.3 or greater.

GL_VERTEX_BINDING_DIVISOR, GL_VERTEX_BINDING_OFFSET, GL_VERTEX_BINDING_STRIDE, GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET and GL_MAX_VERTEX_ATTRIB_BINDINGS are available only if the GL version is 4.3 or greater.

*/