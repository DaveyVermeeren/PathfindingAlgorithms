#pragma once
#include <vector>
#include <queue>
#include <chrono>
#include <iostream>
#include "Graphics.h"
#include <forward_list>

struct BFSCoords
{
    int y;
    int x;
};

struct Vector2Int;
struct Tile;
struct ImageTile;
struct Node;
class BFS
{
public:
    BFS(int height, int width);
    ~BFS();

    bool IsValid(std::vector<std::vector<Node>> Visited, int row, int col);
    void Traverse(std::vector<std::vector<ImageTile>>& Map, int row, int col);

    std::vector<std::vector<Node>> HasVisited;
    int CurrentX;
    int CurrentY;
    bool TargetFound;
    int DirRow[4] = { 0, 1, 0, -1 };
    int DirCol[4] = { -1, 0, 1, 0 };

private:
    std::queue<BFSCoords> Line;
    std::forward_list<ImageTile> Path;
    bool First;
    bool Last;
    bool Searching;
    bool FirstPath;
    Node* CurNode;
    unsigned int MapHeight;
    unsigned int MapWidth;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
};
