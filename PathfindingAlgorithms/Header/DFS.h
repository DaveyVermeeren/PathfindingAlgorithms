#pragma once
#include <stack>
#include <chrono>
#include <iostream>
#include <forward_list>
#include "Graphics.h"

struct DFSCoords
{
    int y;
    int x;
};


struct Tile;
struct ImageTile;
struct Node;
class DFS
{
public:
    DFS(int height, int width);
    ~DFS();

    bool IsValid(std::vector<std::vector<Node>> Visited, int row, int col);
    void Traverse(std::vector<std::vector<ImageTile>>& Map, int row, int col);

    std::vector<std::vector<Node>> HasVisited;
    int CurrentX;
    int CurrentY;
    bool TargetFound;
    int DirRow[4] = { 0, 1, 0, -1 };
    int DirCol[4] = { -1, 0, 1, 0 };


private:
    std::stack<DFSCoords> Line;
    std::forward_list<ImageTile> Path;
    bool Searching;
    bool First;
    bool Last;
    bool FirstPath;
    Node* CurNode;
    unsigned int MapHeight;
    unsigned int MapWidth;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
};