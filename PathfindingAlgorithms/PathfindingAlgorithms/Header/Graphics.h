#pragma once
#include <vector>
#include <simple2d.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "BFS.h"
#include "DFS.h"
#include "AStar.h"
#undef main

constexpr int HEIGHT = 768;
constexpr int WIDTH = 1024;
constexpr int HalfHeight = HEIGHT / 2;
constexpr int HalfWidth = WIDTH / 2;
constexpr int TileSize = 32;
constexpr int HalfTileSize = TileSize / 2;

// Tile Types:
// 0 - Air
// 1 - Wall
// 2 - Target
// 3 - AI

//TODO: Refactor to get rid of circular dependencies, it's annoying

enum class SearchTypes
{
    TypeBFS,
    TypeDFS,
    TypeAStar,
};

struct Vector2Int
{
    int y;
    int x;

    Vector2Int& operator=(const Vector2Int& a_Vec)
    {
        this->x = a_Vec.x;
        this->y = a_Vec.y;
        return *this;
    }
};

struct Tile
{
    Vector2Int Index;
    std::string Filename;
    int Type;
};

struct ImageTile
{
    S2D_Image* Image;
    int Type;
    Vector2Int Pos;
};

struct Node
{
    bool Visited;
    bool Changed;
    ImageTile Parent;

    Node& operator=(const Node& a_Node)
    {
        this->Visited = a_Node.Visited;
        this->Parent.Type = a_Node.Parent.Type;
        this->Parent.Image = a_Node.Parent.Image;
        this->Parent.Pos.x = a_Node.Parent.Pos.x;
        this->Parent.Pos.y = a_Node.Parent.Pos.y;
        return *this;
    }
};

struct BFSCoords;
struct DFSCoords;
struct ASCoords;
class BFS;
class DFS;
class AStar;
class Graphics
{
public:
    Graphics(std::string Filename, SearchTypes Type);
    ~Graphics();

    void Create2DWindow();
    void AddTiles();
    void LoadMap(std::string Filename);
    void UpdateMap(std::vector<std::vector<Node>>& Visited, int x, int y, int DirRow[4], int DirCol[4], bool Found);
    void UpdateMapAStar(std::vector<std::vector<Node>>& Visited, int x, int y, int DirRow[4], int DirCol[4], bool Found, std::forward_list<ImageTile>& Path);

    std::vector<std::vector<ImageTile>> ImageMap;
    std::vector<Tile> Tiles;
    SearchTypes Type;
    BFS* BreadthFirst;
    DFS* DepthFirst;
    AStar* AStarSearch;
    Vector2Int StartPoint;
    Vector2Int EndPoint;

private:
    S2D_Window* Window;
    int MapHeight;
    int MapWidth;
    bool AStarSkip;
};

extern Graphics* Graphic;