#include "BFS.h"

BFS::BFS(int height, int width)
{
    First = false;
    Last = false;
    Searching = true;
    TargetFound = false;
    FirstPath = false;
    CurNode = nullptr;
    CurrentX = 0;
    CurrentY = 0;
    MapHeight = height;
    MapWidth = width;

    for (unsigned int i = 0; i < MapHeight; i++)
    {
        std::vector<Node> TempVec;
        ImageTile EmptyImage = {nullptr, 0, 0, 0};
        for (unsigned int j = 0; j < MapWidth; j++)
        {
            TempVec.push_back({ false, false, EmptyImage });
        }
        HasVisited.push_back(TempVec);
    }
}

BFS::~BFS()
{
    
}


bool BFS::IsValid(std::vector<std::vector<Node>> Visited, int row, int col)
{
    if (row < 0 || col < 0 || row >= static_cast<int>(MapHeight) || col >= static_cast<int>(MapWidth))
    {
        return false;
    }

    if (Visited[row][col].Visited)
    {
        return false;
    }

    return true;
}

void BFS::Traverse(std::vector<std::vector<ImageTile>>& Map, int row, int col)
{
    if (Searching)
    {
        if (!First)
        {
            begin = std::chrono::steady_clock::now();
            Line.push(Vector2Int{ row, col });
            HasVisited[row][col].Visited = true;
            HasVisited[row][col].Changed = true;
            First = true;
        }

        if (!Line.empty())
        {
            Vector2Int cell = Line.front();
            int x = cell.x;
            int y = cell.y;
            CurrentX = x;
            CurrentY = y;

            printf("Coords(%i, %i)\n", cell.x, cell.y);

            Line.pop();

            if (!TargetFound)
            {
                for (int i = 0; i < 4; i++)
                {
                    int adjx = x + DirRow[i];
                    int adjy = y + DirCol[i];

                    if (IsValid(HasVisited, adjy, adjx))
                    {
                        if (Map[adjy][adjx].Type == 1)
                        {
                            continue;
                        }

                        if (Map[adjy][adjx].Type == 2)
                        {
                            CurNode = &HasVisited[y][x];
                            Path.push_front(Map[y][x]);
                            TargetFound = true;
                        }

                        Line.push({ adjy, adjx });
                        HasVisited[adjy][adjx].Visited = true;
                        HasVisited[adjy][adjx].Parent.Pos.y = y;
                        HasVisited[adjy][adjx].Parent.Pos.x = x;
                        HasVisited[adjy][adjx].Parent.Type = Map[y][x].Type;
                    }
                }
            }
        }

        if (TargetFound && FirstPath)
        {
            if (CurNode->Parent.Type != 3 && CurNode->Parent.Type != 2)
            {
                printf("PathCoords(%i, %i)\n", CurNode->Parent.Pos.x, CurNode->Parent.Pos.y);

                CurrentX = CurNode->Parent.Pos.x;
                CurrentY = CurNode->Parent.Pos.y;

                Path.push_front(Map[CurNode->Parent.Pos.y][CurNode->Parent.Pos.x]);
                CurNode = &HasVisited[CurNode->Parent.Pos.y][CurNode->Parent.Pos.x];
            }
            else
            {
                Searching = false;
            }
        }
        else if (TargetFound)
        {
            FirstPath = true;
        }
    }
    else if (!Last)
    {
        end = std::chrono::steady_clock::now();
        int Min = static_cast<int>(std::chrono::duration_cast<std::chrono::minutes>(end - begin).count());
        int Sec = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count());
        int MilSec = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

        std::cout << "Time difference = " << Min << "[m]" << std::endl;
        std::cout << "Time difference = " << Sec % 60 << "[s]" << std::endl;
        std::cout << "Time difference = " << MilSec % 1000 << "[ms]" << std::endl;
        Last = true;
    }
}
