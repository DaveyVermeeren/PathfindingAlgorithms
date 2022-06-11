#include "AStar.h"

AStar::AStar(int height, int width)
{
    Searching = true;
    First = false;
    Last = false;
    TargetFound = false;
    FirstPath = false;
    Heuristic = 0.f;
    Current = { 0, 0 };
    MapHeight = height;
    MapWidth = width;
    D = 1.f;
    D2 = sqrtf(2.f);
    Greed = 1.f; //What to multiply H with
    Humble = 1.f; //What to multiply G with
    Directions = 4;

    for (unsigned int i = 0; i < MapHeight; i++)
    {
        std::vector<Node> TempVec;
        ImageTile EmptyImage = { nullptr, 0, 0, 0 };
        for (unsigned int j = 0; j < MapWidth; j++)
        {
            TempVec.push_back({ false, false, EmptyImage });
        }
        ClosedList.push_back(TempVec);
    }
}

AStar::~AStar()
{
    
}

bool AStar::IsValid(int col, int row)
{
    if (col < 0 || row < 0 || col >= static_cast<int>(MapHeight) || row >= static_cast<int>(MapWidth))
    {
        return false;
    }

    return true;
}

bool AStar::IsFree(std::vector<std::vector<ImageTile>>& Map, int col, int row)
{
    if (IsValid(col, row) && Map[col][row].Type == 0)
    {
        return true;
    }

    return false;
}

void AStar::Traverse(std::vector<std::vector<ImageTile>>& Map, ASCoords Start, ASCoords Target)
{
    if (Searching)
    {
        if (!First)
        {
            begin = std::chrono::steady_clock::now();

            if (!IsValid(Start.y, Start.x))
            {
                printf("Invalid Start\n");
                Searching = false;
                return;
            }

            if (!IsValid(Target.y, Target.x))
            {
                printf("Invalid Target\n");
                Searching = false;
                return;
            }

            if (Start.x == Target.x && Start.y == Target.y)
            {
                printf("Target has already been reached\n");
                Searching = false;
                return;
            }

            for (unsigned int i = 0; i < MapHeight; i++)
            {
                std::vector<AStarValues> tempvec;
                for (unsigned int j = 0; j < MapWidth; j++)
                {
                    tempvec.push_back({ FLT_MAX,FLT_MAX, FLT_MAX, {-1, -1} });
                }
                CellDetails.push_back(tempvec);
            }

            CellDetails[Start.y][Start.x] = { 0.f, 0.f, 0.f, Start };

            OpenList.push_back({ 0.f, Start });
            ClosedList[Start.y][Start.x].Changed = true;

            First = true;
        }

        if (!OpenList.empty())
        {
            if (!TargetFound)
            {
                NodeInfo currentinfo = OpenList.front();
                Current = currentinfo.Pos;

                OpenList.pop_front();
                ClosedList[Current.y][Current.x].Visited = true;
                printf("CurrentCoords: %i, %i\n", Current.x, Current.y);

            
                for (int i = 0; i < Directions; i++)
                {
                    ASCoords Neighbour = { Current.y + DirCol[i], Current.x + DirRow[i] };

                    if (IsValid(Neighbour.y, Neighbour.x))
                    {
                        if (Neighbour.x == Target.x && Neighbour.y == Target.y)
                        {
                            CellDetails[Neighbour.y][Neighbour.x].Parent = Current;
                            TargetFound = true;
                            return;
                        }

                        // if not on the closed list and not a wall, do this
                        if (!ClosedList[Neighbour.y][Neighbour.x].Visited && Map[Neighbour.y][Neighbour.x].Type == 0)
                        {
                            AStarValues TempValues = {0.f, 0.f, 0.f, {-1, -1}};
                            TempValues.g = (CellDetails[Current.y][Current.x].g + 1.f) * Humble;
                            TempValues.h = CalculateEuclideanHeuristic(Neighbour, Target) * Greed;

                            TempValues.f = TempValues.g + TempValues.h;
                            TempValues.Parent = Current;

                            //Checks if it is on the open list, if it isn't it gets added
                            //If it is but the f value is better then it gets placed in the list
                            if (CellDetails[Neighbour.y][Neighbour.x].f == FLT_MAX ||
                                CellDetails[Neighbour.y][Neighbour.x].f > TempValues.f)
                            {
                                CellDetails[Neighbour.y][Neighbour.x] = TempValues;

                                NodeInfo TempInfo = { TempValues.f, Neighbour };
                                OpenList.push_back(TempInfo);
                                InsertionSort(OpenList);
                            }

                        }
                    } 
                }
            }

            if (TargetFound && FirstPath)
            {
                while(!(CellDetails[Current.y][Current.x].Parent.x == Current.x &&
                        CellDetails[Current.y][Current.x].Parent.y == Current.y)) 
                {
                    printf("PathCoords are: (%i,%i)\n", Current.x, Current.y);

                    Path.push_front({ Map[Current.y][Current.x].Image, 0, {Current.y, Current.x}});

                    Current = { CellDetails[Current.y][Current.x].Parent };
                }

                Path.push_front({ Map[Current.y][Current.x].Image, 0, {Current.y, Current.x} });
                Path.reverse();

                Searching = false;
            }
            else if (TargetFound)
            {
                FirstPath = true;
                Current = Target;
            }
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

        PrintCellDetails();

        Last = true;
    }
}

float AStar::CalculateManhattanHeuristic(ASCoords CurrentCoords, ASCoords TargetCoords)
{
    float dx = static_cast<float>(abs(CurrentCoords.x - TargetCoords.x));
    float dy = static_cast<float>(abs(CurrentCoords.y - TargetCoords.y));
    Heuristic = dx + dy;
    return Heuristic;
}

float AStar::CalculateDiagonalHeuristic(ASCoords CurrentCoords, ASCoords TargetCoords)
{
    float dx = static_cast<float>(abs(CurrentCoords.x - TargetCoords.x));
    float dy = static_cast<float>(abs(CurrentCoords.y - TargetCoords.y));

    float xy = dx + dy;
    float Dxy = D * xy;
    float twoD = 2 * D;
    float D2D = D2 - twoD;
    float minD = D2D * min(dx, dy);
    Heuristic = Dxy +  minD;

    return Heuristic;
}

float AStar::CalculateEuclideanHeuristic(ASCoords CurrentCoords, ASCoords TargetCoords)
{
    float dx = static_cast<float>(abs(CurrentCoords.x - TargetCoords.x));
    float dy = static_cast<float>(abs(CurrentCoords.y - TargetCoords.y));

    Heuristic = D * sqrtf(dx * dx + dy * dy);

    return Heuristic;
}

float AStar::CalculateEuclideanSquaredHeuristic(ASCoords CurrentCoords, ASCoords TargetCoords)
{
    float dx = static_cast<float>(abs(CurrentCoords.x - TargetCoords.x));
    float dy = static_cast<float>(abs(CurrentCoords.y - TargetCoords.y));

    Heuristic = D * (dx * dx + dy * dy);

    return Heuristic;
}

void AStar::InsertionSort(std::deque<NodeInfo>& a_OpenList)
{
    for (int i = 1; i < a_OpenList.size(); i++)
    {
        NodeInfo key = a_OpenList[i];
        int j = i - 1;

        while (j >= 0 && a_OpenList[j].f > key.f)
        {
            a_OpenList[j + 1] = a_OpenList[j];
            j--;
        }
        a_OpenList[j + 1] = key;
    }
}

void AStar::PrintCellDetails()
{
    std::ofstream myfile;
    myfile.open("CellDetails.txt");

    myfile << "\n\nG Values:\n\n";

    for (int y = 0; y < CellDetails.size(); y++)
    {
        myfile << "Line" << y << ": { ";
        for (int x = 0; x < CellDetails[y].size(); x++)
        {
            if (CellDetails[y][x].g == FLT_MAX)
            {
                myfile << "NaN,";
            }
            else
            {
                myfile << CellDetails[y][x].g << ",";
            }
        }
        myfile << "},\n";
    }

    myfile << "\n\nH Values:\n\n";

    for (int y = 0; y < CellDetails.size(); y++)
    {
        myfile << "Line" << y << ": { ";
        for (int x = 0; x < CellDetails[y].size(); x++)
        {
            if (CellDetails[y][x].h == FLT_MAX)
            {
                myfile << "NaN,";
            }
            else
            {
                myfile << CellDetails[y][x].h << ",";
            }
        }
        myfile << "},\n";
    }

    myfile << "\n\nF Values:\n\n";

    for (int y = 0; y < CellDetails.size(); y++)
    {
        myfile << "Line" << y << ": { ";
        for (int x = 0; x < CellDetails[y].size(); x++)
        {
            if (CellDetails[y][x].f == FLT_MAX)
            {
                myfile << "NaN,";
            }
            else
            {
                myfile << CellDetails[y][x].f << ",";
            }
        }
        myfile << "},\n";
    }

    myfile.close();
}
