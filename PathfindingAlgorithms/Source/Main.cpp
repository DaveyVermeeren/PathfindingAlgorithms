#include "MazeGenerator.h"
#include "Graphics.h"

void Update()
{
    Graphic->Update();
}

void Render()
{
    Graphic->Render();
}

int main()
{
    //Some simple console stuff for easier usability
    std::string Filename = "Resource/PathfindingBase.json";
    SearchTypes TypeOfSearch = SearchTypes::TypeAStar;
    std::string MazeAnswer = "";
    std::string TypeAnswer = "";
    
    bool AllowMaze = false;

    //Use random maze?

    std::cout << "Create random maze? y/n?\n";

    while(true)
    {
        std::cin >> MazeAnswer;

        if (MazeAnswer == "y")
        {
            AllowMaze = true;
            break;
        }

        if (MazeAnswer == "n")
        {
            break;
        }

        std::cout << "Please insert either y or n\n";
    }

    //What algorithm?

    std::cout << "Please insert algorithm type: \nFill in either 'BFS', 'DFS' or 'AStar'\n";

    while (true)
    {
        std::cin >> TypeAnswer;

        if (TypeAnswer == "BFS")
        {
            TypeOfSearch = SearchTypes::TypeBFS;
            break;
        }

        if (TypeAnswer == "DFS")
        {
            TypeOfSearch = SearchTypes::TypeDFS;
            break;
        }

        if (TypeAnswer == "AStar")
        {
            TypeOfSearch = SearchTypes::TypeAStar;
            break;
        }

        std::cout << "Please insert algorithm type\n";
    }
    
    //What map needs to be loaded?

    if (!AllowMaze)
    {
        std::string TempFilename = "";
        std::cout << "Please insert filename\nFill in 'Base' to use basemap\n";
        std::cin >> TempFilename;

        if (TempFilename == "Base")
        {
            std::cout << "Using base map\n";
        }
        else
        {
            TempFilename = "Resource/" + TempFilename + ".json";
            std::ifstream test(TempFilename);

            if (!test)
            {
                std::cout << "Filename does not exist, using base instead\n";
            }
            else
            {
                Filename = TempFilename;
            }
        }
    }
    else
    {
        Maze = new MazeGenerator(50, 50, 4, 1, Filename);
    }

    Graphic = new Graphics(Filename, TypeOfSearch);

    if (AllowMaze)
    {
        Graphic->StartPoint = { Maze->Start.y, Maze->Start.x };
        Graphic->EndPoint = { Maze->Target.y, Maze->Target.x };
    }

    S2D_Window* Window;

    Window = S2D_CreateWindow(
        "Pathfinding Algorithms Test",
        WIDTH, HEIGHT,
        Update, Render,
        S2D_RESIZABLE
    );

    S2D_Show(Window);

    return 0;
}
