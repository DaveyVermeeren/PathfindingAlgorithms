#pragma once
#include <simple2d.h>
#undef main

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