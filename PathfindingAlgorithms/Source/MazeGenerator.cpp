#include "MazeGenerator.h"
#include <random>

MazeGenerator* Maze;

MazeGenerator::MazeGenerator(int height, int width, int pathwidth, int wallwidth, std::string& filename)
{
    DirRow.insert(std::make_pair(CardinalDirection::East, 1));
    DirRow.insert(std::make_pair(CardinalDirection::West, -1));
    DirRow.insert(std::make_pair(CardinalDirection::North, 0));
    DirRow.insert(std::make_pair(CardinalDirection::South, 0));

    DirCol.insert(std::make_pair(CardinalDirection::East, 0));
    DirCol.insert(std::make_pair(CardinalDirection::West, 0));
    DirCol.insert(std::make_pair(CardinalDirection::North, -1));
    DirCol.insert(std::make_pair(CardinalDirection::South, 1));

    Opposite.insert(std::make_pair(CardinalDirection::East, CardinalDirection::West));
    Opposite.insert(std::make_pair(CardinalDirection::West, CardinalDirection::East));
    Opposite.insert(std::make_pair(CardinalDirection::North, CardinalDirection::South));
    Opposite.insert(std::make_pair(CardinalDirection::South, CardinalDirection::North));

    PathWidth = pathwidth;
    WallWidth = wallwidth;
    TileGroupSize = PathWidth + WallWidth;
    GridHeight = height + 1; //To give it an outer border
    GridWidth = width + 1; //To give it an outer border

    MazeHeight = height / TileGroupSize;
    MazeWidth = width / TileGroupSize;

    for (int i = 0; i < MazeHeight + 1; i++)
    {
        std::vector<TreeNode*> TempVec;
        for (int j = 0; j < MazeWidth + 1; j++)
        {
            TempVec.push_back(new TreeNode());
        }

        MazeGrid.push_back(TempVec);
    }

    for (int y = 0; y < GridHeight; y++)
    {
        std::vector<int> TempGrid;
        for (int x = 0; x < GridWidth; x++)
        {
            if (y == 0 || y % TileGroupSize == 0)
            {
                TempGrid.push_back(1);
                continue;
            }

            if (x == 0 || x % TileGroupSize == 0)
            {
                TempGrid.push_back(1);
                continue;
            }

            TempGrid.push_back(0);
            
        }

        TileGrid.push_back(TempGrid);
    }

    Create();

    int HeightQuarter = static_cast<int>(0.25 * height);
    int WidthQuarter = static_cast<int>(0.25 * width);

    bool SetStart = false;
    bool SetTarget = false;

    int rngx = 0;
    int rngy = 0;
    unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    std::default_random_engine generator(seed);

    while (!SetStart)
    {
        std::uniform_int_distribution<int> distributionx(1, WidthQuarter);
        std::uniform_int_distribution<int> distributiony(1, HeightQuarter);
        rngx = distributionx(generator);
        rngy = distributiony(generator);

        if (TileGrid[rngy][rngx] == 0)
        {
            Start = { rngx, rngy };
            TileGrid[rngy][rngx] = 3;
            SetStart = true;
        }
    }

    while (!SetTarget)
    {
        std::uniform_int_distribution<int> distributionx(3 * WidthQuarter, width);
        std::uniform_int_distribution<int> distributiony(3 * HeightQuarter, height);
        rngx = distributionx(generator);
        rngy = distributiony(generator);

        if (TileGrid[rngy][rngx] == 0)
        {
            Target = { rngx, rngy };
            TileGrid[rngy][rngx] = 2;
            SetTarget = true;
        }
    }

    SaveMaze(filename);
    
}

MazeGenerator::~MazeGenerator()
{
    
}

void MazeGenerator::CreateMaze()
{
    while (!Edges.empty())
    {
        //Gets edge from the back of the vector.
        Edge Current = Edges.back();
        Edges.pop_back();

        //Adds value to the current coordinates based on current Direction
        EdgeCoords NewCoords = { Current.Coords.x + DirRow[Current.Direction],
                                 Current.Coords.y + DirCol[Current.Direction] };

        //Makes sure the new coordinates aren't out of bounds
        NewCoords.x = std::clamp(NewCoords.x, 0, MazeWidth);
        NewCoords.y = std::clamp(NewCoords.y, 0, MazeHeight);

        //Stores current and new IDs
        int CurID = MazeGrid[Current.Coords.y][Current.Coords.x]->ID;
        int NewID = MazeGrid[NewCoords.y][NewCoords.x]->ID;

        //If current group of tiles and the new tile isn't connected, merge them.
        if (!MazeGrid[Current.Coords.y][Current.Coords.x]->Connected(MazeGrid[NewCoords.y][NewCoords.x]))
        {
            //Edges are connected and the groups merged
            MazeGrid[Current.Coords.y][Current.Coords.x]->Connect(MazeGrid[NewCoords.y][NewCoords.x]);
            MergeGroup(CurID, NewID);

            //Stores what directions need to be opened up for each tile using bitwise OR assignment
            MazeGrid[Current.Coords.y][Current.Coords.x]->direction |= Current.Direction;
            MazeGrid[NewCoords.y][NewCoords.x]->direction |= Opposite[Current.Direction];
        }
    }
}

void MazeGenerator::CreateEdges(int height, int width)
{
    int GroupID = 0;
    //Creates edges for each cell based on given height and width
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++, GroupID++)
        {
            MazeGrid[y][x]->ID = GroupID;
            //Adds two separate edges, each with their own direction
            if (x + 1 < width)
            {
                Edges.push_back(Edge{ {x, y}, CardinalDirection::North });
            }
            if (y + 1 < height)
            {
                Edges.push_back(Edge{ {x, y}, CardinalDirection::West });
            }
        }
    }

    //Shuffles stored edges for random maze generation
    std::shuffle(Edges.begin(), Edges.end(), std::mt19937(std::random_device()()));
}

void MazeGenerator::CreateTileGrid()
{
    //Changing TileGrid based on mazeheight and mazewidth 
    for (int CurrentY = 0; CurrentY < MazeHeight; CurrentY++)
    {
        for (int CurrentX = 0; CurrentX < MazeWidth; CurrentX++)
        {
            //Store direction for later use.
            CardinalDirection Dir = MazeGrid[CurrentY][CurrentX]->direction;

            //TileGroupSize is used to scale the MazeGrid to TileGrid size
            if (CurrentY % TileGroupSize != 0 ||
                CurrentX % TileGroupSize != 0)
            {
                //Starts at 1 so it will ignore the corner
                for (int i = 1; i < TileGroupSize; i++)
                {
                    int TileX = CurrentX * TileGroupSize;
                    int TileY = CurrentY * TileGroupSize;

                    //If the Dir contains the direction and is not out of bounds
                    //Remove the wall of the current position in TileGrid
                    if (Dir == CardinalDirection::North &&
                        TileX != GridWidth - 1)
                    {
                        TileGrid[TileY][TileX + i] = 0;
                    }

                    if (Dir == CardinalDirection::East &&
                        TileY != GridHeight - 1)
                    {
                        TileX += TileGroupSize;
                        TileGrid[TileY + i][TileX] = 0;
                    }

                    if (Dir == CardinalDirection::South &&
                        TileX != GridWidth - 1)
                    {
                        TileY += TileGroupSize;
                        TileGrid[TileY][TileX + i] = 0;
                    }

                    if (Dir == CardinalDirection::West &&
                        TileY != GridHeight - 1)
                    {
                        TileGrid[TileY + i][TileX] = 0;
                    }
                }
            }
        
        }
    }
}

void MazeGenerator::MergeGroup(int fromID, int toID)
{
    if (fromID == toID)
    {
        return;
    }

    for (int y = 0; y < MazeHeight; y++)
    {
        for (int x = 0; x < MazeWidth; x++)
        {
            if (MazeGrid[y][x]->ID == fromID)
            {
                MazeGrid[y][x]->ID = toID;
            }
        }
    }
}

void MazeGenerator::SaveMaze(std::string &filename)
{
    nlohmann::json JsonMap;

    JsonMap["height"] = GridHeight;
    JsonMap["width"] = GridWidth;

    auto TileArray = nlohmann::json::array();

    for (int y = 0; y < GridHeight; y++)
    {
        for (int x = 0; x < GridWidth; x++)
        {
            TileArray.push_back(std::to_string(TileGrid[y][x]));
        }
    }

    nlohmann::json value = { {"grid", TileArray} };

    auto LayerArray = nlohmann::json::array();
    LayerArray.push_back(value);

    JsonMap["layers"] = LayerArray;
    
    std::cout << "Please insert filename\n";

    std::cin >> filename;

    if (filename == "PathfindingBase")
    {
        std::cout << "Sorry, that name is not allowed, using 'Testing' instead\n";
        filename = "Testing";
    }

    filename = "Resource/" + filename + ".json";

    std::ofstream i(filename);
    i << JsonMap;
    i.close();
}


void MazeGenerator::Create()
{
    CreateEdges(MazeHeight, MazeWidth);
    CreateMaze();
    CreateTileGrid();
}

void MazeGenerator::PrintGrid()
{
    std::ofstream myfile;
    myfile.open("TileGrid.txt");

    myfile << "\n\nTile Values:\n\n";

    for (int y = 0; y < TileGrid.size(); y++)
    {
        myfile << "{ ";
        for (int x = 0; x < TileGrid[y].size(); x++)
        {
            myfile << TileGrid[y][x] << ",";
        }
        myfile << "},\n";
    }

    myfile.close();
}

