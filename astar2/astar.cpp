#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>

#define YMAX 20
#define XMAX 20

class Cell
{
public:
    int posx;
    int posy;
    int value;
    std::vector<Cell *> neighbours;
    double gValue;
    double hValue;
    double fValue;

    Cell *parent;

    void generateNeighbors(std::vector<std::vector<Cell>> &grid)
    {
        int x = posx;
        int y = posy;

        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

        neighbours.clear();

        for (int i = 0; i < 4; ++i)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (newX >= 0 && newX < XMAX && newY >= 0 && newY < YMAX)
            {
                if (grid[newX][newY].isValid())
                {
                    neighbours.push_back(&grid[newX][newY]);
                }
            }
        }
    }

    void updateFValue()
    {
        fValue = gValue + hValue;
    }

    bool isValid()
    {
        if (value == 5)
        {
            return false;
        }

        if (posx < 0 || posy < 0 || posx >= XMAX || posy >= YMAX)
        {
            return false;
        }

        return true;
    }

    bool isDestination(const Cell &dest)
    {
        return posx == dest.posx && posy == dest.posy;
    }

    double heuristic(int endX, int endY)
    {
        double dx = posx - endX;
        double dy = posy - endY;
        return std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    }

    void calculateFunctionValue(int endX, int endY)
    {
        gValue = parent->gValue + 1.0;
        hValue = heuristic(endX, endY);
        fValue = gValue + hValue;
    }

    bool operator==(const Cell &other) const
    {
        return posx == other.posx && posy == other.posy;
    }

    bool operator!=(const Cell &other) const
    {
        return posx != other.posx || posy != other.posy;
    }

    bool operator<(const Cell &other) const
    {
        return fValue < other.fValue;
    }

    bool operator>(const Cell &other) const
    {
        return fValue > other.fValue;
    }

    bool isBetterThan(const Cell &other) const
    {
        return fValue > other.fValue;
    }
};

class Grid
{
public:
    Cell start;
    Cell end;
    std::vector<std::vector<Cell>> map;
    std::vector<Cell *> openList;
    std::vector<Cell *> closedList;
    std::vector<Cell> path;

    void parseGrid()
    {
        std::string filename = "grid.txt";
        int wym2 = 20;
        int wym1 = 20;

        map = std::vector<std::vector<Cell>>(wym2, std::vector<Cell>(wym1));

        std::ifstream file(filename);

        if (!file.is_open())
        {
            std::cerr << "Failed to open the file." << std::endl;
            return;
        }

        for (int i = 0; i < wym2; i++)
        {
            for (int j = 0; j < wym1; j++)
            {
                file >> map[i][j].value;
                map[i][j].posx = i;
                map[i][j].posy = j;
                if (map[i][j].value == 1)
                {
                    start.value = map[i][j].value;
                    start.posx = i;
                    start.posy = j;
                }
                else if (map[i][j].value == 2)
                {
                    end.value = map[i][j].value;
                    end.posx = i;
                    end.posy = j;
                }
            }
        }

        file.close();
    }

    void resetCellValues()
    {
        for (int i = 0; i < YMAX; i++)
        {
            for (int j = 0; j < XMAX; j++)
            {
                map[i][j].gValue = std::numeric_limits<double>::infinity();
                map[i][j].hValue = map[i][j].heuristic(end.posx, end.posy);
                map[i][j].updateFValue();
            }
        }
    }

    bool isNotInClosedList(const Cell &cell)
    {
        return std::find(closedList.begin(), closedList.end(), &cell) == closedList.end();
    }

    void reconstructPath(Cell &current)
    {
        path.clear();
        while (current != start)
        {
            path.push_back(current);
            current = *current.parent;
        }
        path.push_back(start);
    }

    void aStar()
    {
        openList.clear();
        closedList.clear();
        resetCellValues();
        start.gValue = 0;
        start.calculateFunctionValue(end.posx, end.posy);

        auto compareCells = [](const Cell *a, const Cell *b)
        {
            return a->isBetterThan(*b);
        };
        std::priority_queue<Cell *, std::vector<Cell *>, decltype(compareCells)> openQueue(compareCells);

        openQueue.push(&start);

        while (!openQueue.empty())
        {
            Cell *current = openQueue.top();
            openQueue.pop();

            if (current->isDestination(end))
            {
                reconstructPath(*current);
                return;
            }

            closedList.push_back(current);

            current->generateNeighbors(map);

            for (Cell *neighbor : current->neighbours)
            {
                if (*neighbor == start)
                {
                    continue;
                }

                if (!isNotInClosedList(*neighbor))
                {
                    continue;
                }

                double tentativeGValue = current->gValue + 1.0;

                if (tentativeGValue < neighbor->gValue)
                {
                    neighbor->parent = current;
                    neighbor->gValue = tentativeGValue;
                    neighbor->calculateFunctionValue(end.posx, end.posy);
                    openQueue.push(neighbor);
                    openList.push_back(neighbor);
                }
            }
        }
    }
};

void printPath(const std::vector<Cell *> &path)
{
    for (const Cell *cell : path)
    {
        std::cout << "Position: (" << cell->posx << "," << cell->posy << ")\n";
    }
}

void printGridWithPath(const Grid &grid)
{
    for (int i = 0; i < YMAX; i++)
    {
        for (int j = 0; j < XMAX; j++)
        {
            const Cell &currentCell = grid.map[i][j];

            if (currentCell == grid.end)
            {
                std::cout << "E "; // End
            }
            else if (currentCell == grid.start)
            {
                std::cout << "S "; // Start
            }
            else if (std::find(grid.path.begin(), grid.path.end(), currentCell) != grid.path.end())
            {
                std::cout << "* "; // Path
            }
            else if (std::find(grid.closedList.begin(), grid.closedList.end(), &currentCell) != grid.closedList.end())
            {
                std::cout << "P "; // Closed path
            }
            else if (std::find(grid.openList.begin(), grid.openList.end(), &currentCell) != grid.openList.end())
            {
                std::cout << "O "; // Open path
            }
            else if (currentCell.value == 5)
            {
                std::cout << "X "; // Obstacle
            }
            else
            {
                std::cout << ". "; // Empty cell
            }
        }
        std::cout << std::endl;
    }
}

int main()
{
    Grid grid;
    grid.parseGrid();

    grid.aStar();

    //printPath(grid.closedList);
    for(int i=0 ;i<20;i++){
        std::cout<<grid.path[i].posx << grid.path[i].posy;
    }
    //printGridWithPath(grid);

    return 0;
}
