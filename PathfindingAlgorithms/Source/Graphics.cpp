#include "Graphics.h"

Graphics* Graphic;

Graphics::Graphics(std::string Filename, SearchTypes Newtype)
{
    LoadMap(Filename);

    Type = Newtype;

    //Using in case of future expansion
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

void Graphics::Render()
{
    for (auto CurVec : ImageMap)
    {
        for (auto Image : CurVec)
        {
            S2D_DrawImage(Image.Image);
        }
    }
}


void Graphics::Update()
{
    //Using in case of future expansion
    switch (Type)
    {
    case SearchTypes::TypeBFS:
        BreadthFirst->Traverse(ImageMap, 
                              StartPoint.y,
                              StartPoint.x);
        
        UpdateMap(BreadthFirst->HasVisited, 
                     BreadthFirst->CurrentX,
                     BreadthFirst->CurrentY,
                     BreadthFirst->DirRow, 
                     BreadthFirst->DirCol,
                     BreadthFirst->TargetFound);
        break;

    case SearchTypes::TypeDFS:
        DepthFirst->Traverse(ImageMap,
                            StartPoint.y,
                            StartPoint.x);
            
        UpdateMap(DepthFirst->HasVisited,
                     DepthFirst->CurrentX,
                     DepthFirst->CurrentY,
                     DepthFirst->DirRow,
                     DepthFirst->DirCol,
                     DepthFirst->TargetFound);
        break;

    case SearchTypes::TypeAStar:
        AStarSearch->Traverse(ImageMap,
            { StartPoint.y, StartPoint.x },
            {EndPoint.y, EndPoint.x});

        UpdateMapAStar(AStarSearch->ClosedList,
                             AStarSearch->Current.x,
                             AStarSearch->Current.y,
                             AStarSearch->DirRow,
                             AStarSearch->DirCol,
                             (AStarSearch->TargetFound &&
                              AStarSearch->FirstPath),
                             AStarSearch->Path);
        break;
    }
}

void Graphics::ReplaceTile(std::string Filename, Vector2Int Coords)
{
    S2D_Image* Temp = S2D_CreateImage(Filename.c_str());
    S2D_Image* Old = ImageMap[Coords.y][Coords.x].Image;
    Temp->x = Old->x;
    Temp->y = Old->y;
    Temp->width = Old->width;
    Temp->height = Old->height;
    S2D_FreeImage(ImageMap[Coords.y][Coords.x].Image);
    ImageMap[Coords.y][Coords.x] = { Temp, 0 };
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
                ReplaceTile("Resource/Yellow.png", { adjy, adjx });
                Visited[adjy][adjx].Changed = true;
            }
        }
    }
    else
    {
        ReplaceTile("Resource/Orange.png", { y, x });
    }
}

void Graphics::UpdateMapAStar(std::vector<std::vector<Node>>& Visited, int x, int y, int DirRow[], int DirCol[], bool Found, std::forward_list<ImageTile>& Path)
{
    if (!Found)
    {
        for (int i = 0; i < AStarSearch->Directions; i++)
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
                ReplaceTile("Resource/Yellow.png", { adjy, adjx });
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
            ReplaceTile("Resource/Orange.png", CurrentTile.Pos);
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

