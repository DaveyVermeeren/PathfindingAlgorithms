#pragma once
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <deque>
#include <forward_list>
#include "UserTypes.h"

struct AStarValues
{
    float f;
    float g;
    float h;
    Vector2Int Parent;

    AStarValues& operator=(const AStarValues& a_Values)
    {
        this->f = a_Values.f;
        this->g = a_Values.g;
        this->h = a_Values.h;
        this->Parent.x = a_Values.Parent.x;
        this->Parent.y = a_Values.Parent.y;
        return *this;
    }
};

struct NodeInfo
{
    float f;
    Vector2Int Pos;

    bool operator<(const NodeInfo& a_Right) const
    {
        return this->f < a_Right.f;
    }

    bool operator>(const NodeInfo& a_Right) const
    {
        return this->f > a_Right.f;
    }
};

class AStar
{
public:
    AStar(int height, int width);
    ~AStar();

    std::vector<std::vector<Node>> ClosedList;
    std::vector<std::vector<AStarValues>> CellDetails;
    std::forward_list<ImageTile> Path;

    bool IsValid(int col, int row);
    bool IsFree(std::vector<std::vector<ImageTile>>& Map, int col, int row);
    void Traverse(std::vector<std::vector<ImageTile>>& Map, Vector2Int Start, Vector2Int Target);
    float CalculateManhattanHeuristic(Vector2Int CurrentCoords, Vector2Int TargetCoords);
    float CalculateDiagonalHeuristic(Vector2Int CurrentCoords, Vector2Int TargetCoords);
    float CalculateEuclideanHeuristic(Vector2Int CurrentCoords, Vector2Int TargetCoords);
    float CalculateEuclideanSquaredHeuristic(Vector2Int CurrentCoords, Vector2Int TargetCoords);
    void InsertionSort(std::deque<NodeInfo>& a_OpenList);
    void PrintCellDetails();

    Vector2Int Current;
    bool TargetFound;
    bool FirstPath;
    //int DirRow[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    //int DirCol[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    int Directions;
    int DirRow[4] = { 0, 1, 0, -1 };
    int DirCol[4] = { -1, 0, 1, 0 };

private:
    std::deque<NodeInfo> OpenList;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    float Heuristic;
    unsigned int MapHeight;
    unsigned int MapWidth;
    bool First;
    bool Last;
    bool Searching;
    float D;
    float D2;
    float Greed;
    float Humble;
};