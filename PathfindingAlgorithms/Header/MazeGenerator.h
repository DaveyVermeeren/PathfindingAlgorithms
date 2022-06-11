#pragma once
#include <vector>
#include <map>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

enum class CardinalDirection
{
    North = 1,
    South = 2,
    East = 4,
    West = 8
};

inline CardinalDirection operator ~(CardinalDirection a)
{
    return static_cast<CardinalDirection>(~static_cast<int>(a));
}

inline CardinalDirection operator ^(CardinalDirection a, CardinalDirection b)
{
    return static_cast<CardinalDirection>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline CardinalDirection operator &(CardinalDirection a, CardinalDirection b)
{
    return static_cast<CardinalDirection>(static_cast<int>(a) & static_cast<int>(b));
}

inline CardinalDirection operator |(CardinalDirection a, CardinalDirection b)
{
    return static_cast<CardinalDirection>(static_cast<int>(a) | static_cast<int>(b));
}

inline CardinalDirection& operator |=(CardinalDirection& a, CardinalDirection b)
{
    return a = a | b;
}

inline bool operator ==(CardinalDirection a, CardinalDirection b)
{
    return static_cast<int>(a & b) != 0;
}

struct TreeNode
{
    int ID = 0;
    CardinalDirection direction; //Keep uninitialized to prevent issues

    bool Connected(TreeNode* Node)
    {
        return ID == Node->ID;
    }

    void Connect(TreeNode* Node)
    {
        Node->ID = this->ID;
    }
};

struct EdgeCoords
{
    int x;
    int y;
};

struct Edge
{
    EdgeCoords Coords;
    CardinalDirection Direction;
};

class MazeGenerator
{
public:
    //Height and Width NEED to be divisible by Pathwidth + (2 * Wallwidth)
    //Height and Width will be increased by 1 to give an outer border
    MazeGenerator(int height, int width, int pathwidth, int wallwidth, std::string& filename);
    ~MazeGenerator();

    void Create();
    void CreateMaze();
    void CreateEdges(int height, int width);
    void CreateTileGrid();
    void MergeGroup(int fromID, int toID);
    void PrintGrid();
    void SaveMaze(std::string& filename);

    EdgeCoords Start;
    EdgeCoords Target;

private:
    std::vector<std::vector<TreeNode*>> MazeGrid;
    std::vector<std::vector<int>> TileGrid;
    std::vector<Edge> Edges;
    std::map<CardinalDirection, int> DirRow;
    std::map<CardinalDirection, int> DirCol;
    std::map<CardinalDirection, CardinalDirection> Opposite;
    int PathWidth;
    int WallWidth;
    int TileGroupSize;
    int GridHeight;
    int GridWidth;
    int MazeHeight;
    int MazeWidth;
};

extern MazeGenerator* Maze;