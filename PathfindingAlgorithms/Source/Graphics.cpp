#include "Graphics.h"

Graphics* Graphic;

Graphics::Graphics(std::string Filename, SearchTypes Newtype)
{
    LoadMap(Filename);

    Type = Newtype;

    switch (Type)
    {
    case SearchTypes::TypeBFS:
        BreadthFirst = new BFS(MapHeight, MapWidth);
        break;

    case SearchTypes::TypeDFS:
        DepthFirst = new DFS(MapHeight, MapWidth);
        break;

    case SearchTypes::TypeAStar:
        AStarSearch = new AStar(MapHeight, MapWidth);
        AStarSkip = false;
        break;
    }
}

Graphics::~Graphics()
{
    
}

void Render()
{
    for (auto CurVec : Graphic->ImageMap)
    {
        for (auto Image : CurVec)
        {
            S2D_DrawImage(Image.Image);
        }
    }
}


void Update()
{
    switch (Graphic->Type)
    {
    case SearchTypes::TypeBFS:
        Graphic->BreadthFirst->Traverse(Graphic->ImageMap, 
                                       Graphic->StartPoint.y,
                                       Graphic->StartPoint.x);
        
        Graphic->UpdateMap(Graphic->BreadthFirst->HasVisited, 
                          Graphic->BreadthFirst->CurrentX,
                          Graphic->BreadthFirst->CurrentY,
                          Graphic->BreadthFirst->DirRow, 
                          Graphic->BreadthFirst->DirCol,
                          Graphic->BreadthFirst->TargetFound);
        break;

    case SearchTypes::TypeDFS:
        Graphic->DepthFirst->Traverse(Graphic->ImageMap,
            Graphic->StartPoint.y,
            Graphic->StartPoint.x);
            
        Graphic->UpdateMap(Graphic->DepthFirst->HasVisited,
                             Graphic->DepthFirst->CurrentX,
                             Graphic->DepthFirst->CurrentY,
                             Graphic->DepthFirst->DirRow,
                             Graphic->DepthFirst->DirCol,
                             Graphic->DepthFirst->TargetFound);
        break;

    case SearchTypes::TypeAStar:
        Graphic->AStarSearch->Traverse(Graphic->ImageMap,
            { Graphic->StartPoint.y, Graphic->StartPoint.x },
            {Graphic->EndPoint.y, Graphic->EndPoint.x});

        Graphic->UpdateMapAStar(Graphic->AStarSearch->ClosedList,
                             Graphic->AStarSearch->Current.x,
                             Graphic->AStarSearch->Current.y,
                             Graphic->AStarSearch->DirRow,
                             Graphic->AStarSearch->DirCol,
                             (Graphic->AStarSearch->TargetFound &&
                              Graphic->AStarSearch->FirstPath),
                             Graphic->AStarSearch->Path);
        break;
    }
}

void Graphics::Create2DWindow()
{
    Window = S2D_CreateWindow(
        "Pathfinding Algorithms Test",
        WIDTH, HEIGHT,
        Update, Render,
        S2D_RESIZABLE
    );

    S2D_Show(Window);
}

void Graphics::AddTiles()
{
    for (Tile tile : Tiles)
    {
        S2D_Image* Temp = S2D_CreateImage(tile.Filename.c_str());
        S2D_Image* Old = ImageMap[tile.Index.y][tile.Index.x].Image;
        Temp->x = Old->x;
        Temp->y = Old->y;
        Temp->width = Old->width;
        Temp->height = Old->height;
        S2D_FreeImage(ImageMap[tile.Index.y][tile.Index.x].Image);
        ImageMap[tile.Index.y][tile.Index.x] = { Temp, tile.Type };
    }
}

void Graphics::UpdateMap(std::vector<std::vector<Node>>& Visited, int x, int y, int DirRow[], int DirCol[], bool Found)
{
    if (!Found)
    {
        for (int i = 0; i < 4; i++)
        {
            int adjx = x + DirRow[i];
            int adjy = y + DirCol[i];

            if (adjx < 0)
            {
                adjx = 0;
            }

            if (adjy < 0)
            {
                adjy = 0;
            }

            if (Visited[adjy][adjx].Visited && !Visited[adjy][adjx].Changed)
            {
                S2D_Image* Temp = S2D_CreateImage("Resource/Yellow.png");
                S2D_Image* Old = ImageMap[adjy][adjx].Image;
                Temp->x = Old->x;
                Temp->y = Old->y;
                Temp->width = Old->width;
                Temp->height = Old->height;
                S2D_FreeImage(ImageMap[adjy][adjx].Image);
                ImageMap[adjy][adjx] = { Temp, 0 };
                Visited[adjy][adjx].Changed = true;
            }
        }
    }
    else
    {
        S2D_Image* Temp = S2D_CreateImage("Resource/Orange.png");
        S2D_Image* Old = ImageMap[y][x].Image;
        Temp->x = Old->x;
        Temp->y = Old->y;
        Temp->width = Old->width;
        Temp->height = Old->height;
        S2D_FreeImage(ImageMap[y][x].Image);
        ImageMap[y][x] = { Temp, 0 };
    }
}

void Graphics::UpdateMapAStar(std::vector<std::vector<Node>>& Visited, int x, int y, int DirRow[], int DirCol[], bool Found, std::forward_list<ImageTile>& Path)
{
    if (!Found)
    {
        for (int i = 0; i < Graphic->AStarSearch->Directions; i++)
        {
            int adjx = x + DirRow[i];
            int adjy = y + DirCol[i];

            if (adjx < 0)
            {
                adjx = 0;
            }

            if (adjy < 0)
            {
                adjy = 0;
            }

            if (Visited[adjy][adjx].Visited && !Visited[adjy][adjx].Changed)
            {
                S2D_Image* Temp = S2D_CreateImage("Resource/Yellow.png");
                S2D_Image* Old = ImageMap[adjy][adjx].Image;
                Temp->x = Old->x;
                Temp->y = Old->y;
                Temp->width = Old->width;
                Temp->height = Old->height;
                S2D_FreeImage(ImageMap[adjy][adjx].Image);
                ImageMap[adjy][adjx] = { Temp, 0 };
                Visited[adjy][adjx].Changed = true;
            }
        }
    }
    else if (!AStarSkip)
    {
        AStarSkip = true;
    }
    else if (!Path.empty())
    {
        ImageTile CurrentTile = Path.front();
        if (!(CurrentTile.Pos.x == StartPoint.x && CurrentTile.Pos.y == StartPoint.y))
        {
            S2D_Image* Temp = S2D_CreateImage("Resource/Orange.png");
            S2D_Image* Old = ImageMap[CurrentTile.Pos.y][CurrentTile.Pos.x].Image;
            Temp->x = Old->x;
            Temp->y = Old->y;
            Temp->width = Old->width;
            Temp->height = Old->height;
            S2D_FreeImage(ImageMap[CurrentTile.Pos.y][CurrentTile.Pos.x].Image);
            ImageMap[CurrentTile.Pos.y][CurrentTile.Pos.x] = { Temp, 0 };
        }
        Path.pop_front();
    }
}

void Graphics::LoadMap(std::string Filename)
{
    std::ifstream i(Filename);
    nlohmann::json JsonMap;
    i >> JsonMap;

    MapHeight = JsonMap["height"];
    MapWidth = JsonMap["width"];
    std::vector<std::vector<int>> TemporaryArray;
    for (int i = 0; i < MapHeight; i++)
    {
        std::vector<int> TempVec;
        for (int j = 0; j < MapWidth; j++)
        {
            TempVec.push_back(0);
        }
        TemporaryArray.push_back(TempVec);
    }

    int x = 0;
    int y = 0;

    for (const auto& Item : JsonMap["layers"]) //Gets the Layers array which has all the information
    {
        if (Item.find("grid") != Item.end()) //Looks for the 1D array called grid
        {
            for (const auto& GridTile : Item["grid"]) //Loops through the 1D array
            {
                //Gets the amount of elements the size of the Map width and stores it in the 2DArray
                if (x < MapWidth)
                {
                    std::string TempString = GridTile;
                    TemporaryArray[y][x] = std::stoi(TempString);
                    x++;
                }
                //Once a row is filled it goes to the next line and slowly turns 1D to 2D Array
                if (y < MapHeight && x >= MapWidth)
                {
                    y++;
                    x = 0;
                }

                if (y == MapHeight)
                {
                    break;
                }
            }
        }
    }

    float AvailableHeight = static_cast<float>(HEIGHT - TileSize) / MapHeight;
    float AvailableWidth = static_cast<float>(WIDTH - TileSize) / MapWidth;
    float QuadHeight = 0.9f * AvailableHeight;
    float QuadSpaceHeight = AvailableHeight - QuadHeight;
    float QuadWidth = 0.9f * AvailableWidth;
    float QuadSpaceWidth = AvailableWidth - QuadWidth;

    //It is not the best but works for intended purposes
    for (int i = 0; i < MapHeight; i++)
    {
        std::vector<ImageTile> TempVec;
        for (int j = 0; j < MapWidth; j++)
        {
            int value = TemporaryArray[i][j];
            S2D_Image* Temp = nullptr;

            if (value == 0)
            {
                Temp = S2D_CreateImage("Resource/Red.png");
            }

            if (value == 1)
            {
                Temp = S2D_CreateImage("Resource/Gray.png");
            }

            if (value == 2)
            {
                Temp = S2D_CreateImage("Resource/Green.png");
                EndPoint = { i, j };
            }

            if (value == 3)
            {
                Temp = S2D_CreateImage("Resource/Blue.png");
                StartPoint = { i, j };
            }

            if (Temp != nullptr)
            {
                Temp->x = static_cast<int>(HalfTileSize + j * QuadWidth + j * QuadSpaceWidth);
                Temp->y = static_cast<int>(HalfTileSize + i * QuadHeight + i * QuadSpaceHeight);
                Temp->width = static_cast<int>(QuadWidth);
                Temp->height = static_cast<int>(QuadHeight);
                TempVec.push_back({ Temp, value, i, j });
            }
        }
        ImageMap.push_back(TempVec);
    }
    i.close();
}

