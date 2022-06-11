#include "MazeGenerator.h"
#include "Graphics.h"

int main()
{
    //Some simple console stuff for easier usability
    std::string Filename = "Resource/PathfindingBase.json";
    SearchTypes TypeOfSearch = SearchTypes::TypeAStar;
    std::string MazeAnswer = "";
    std::string TypeAnswer = "";
    
    bool AnsweredMaze = false;
    bool AnsweredType = false;
    bool AllowMaze = false;

    //Use random maze?

    std::cout << "Create random maze? y/n?\n";

    while(!AnsweredMaze)
    {
        std::cin >> MazeAnswer;

        if (MazeAnswer == "y")
        {
            AllowMaze = true;
            AnsweredMaze = true;
            break;
        }

        if (MazeAnswer == "n")
        {
            AnsweredMaze = true;
            break;
        }

        std::cout << "Please insert either y or n\n";
    }

    //What algorithm?

    std::cout << "Please insert algorithm type: \nFill in either 'BFS', 'DFS' or 'AStar'\n";

    while (!AnsweredType)
    {
        std::cin >> TypeAnswer;

        if (TypeAnswer == "BFS")
        {
            TypeOfSearch = SearchTypes::TypeBFS;
            AnsweredType = true;
            break;
        }

        if (TypeAnswer == "DFS")
        {
            TypeOfSearch = SearchTypes::TypeDFS;
            AnsweredType = true;
            break;
        }

        if (TypeAnswer == "AStar")
        {
            TypeOfSearch = SearchTypes::TypeAStar;
            AnsweredType = true;
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

    Graphic->Create2DWindow();

    return 0;
}
