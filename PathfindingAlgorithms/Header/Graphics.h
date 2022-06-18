#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "UserTypes.h"
#include "BFS.h"
#include "DFS.h"
#include "AStar.h"

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

enum class SearchTypes
{
    TypeBFS,
    TypeDFS,
    TypeAStar,
};

class Graphics
{
public:
    Graphics(std::string Filename, SearchTypes Type);
    ~Graphics();

    void Update();
    void Render();
    void AddTiles();
    void ReplaceTile(std::string Filename, Vector2Int Coords);
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
    int MapHeight;
    int MapWidth;
    bool AStarSkip;
};

extern Graphics* Graphic;