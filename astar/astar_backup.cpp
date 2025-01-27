#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>
#include <set>
#include <stack>
#include <cfloat>
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
    double gValue; // Cost from the start node to this node
    double hValue; // Estimated cost from this node to the goal
    double fValue; // f(x) = g(x) + h(x)

    Cell *parent;

    void generateNeighbors(std::vector<std::vector<Cell>> &grid, int maxX, int maxY)
    {
        int x = posx;
        int y = posy;

        // Define possible moves (up, down, left, and right)
        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

        neighbours.clear();

        for (int i = 0; i < 4; ++i)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            // Check if the new position is within the bounds of the grid
            if (newX >= 0 && newX < maxX && newY >= 0 && newY < maxY)
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
        if (value == 5) // Assuming 0 indicates an obstacle
        {
            return false;
        }

        if (posx < 0 || posy < 0 || posx >= XMAX || posy >= YMAX)
        {
            return false;
        }

        return true;
    }

    bool isDestination(Cell dest)
    {
        return posx == dest.posx && posy == dest.posy;
    }

    double heuristic(int endX, int endY)
    {
        // Euclidean distance heuristic
        double dx = posx - endX;
        double dy = posy - endY;
        return std::sqrt(dx * dx + dy * dy);
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
        return posx != other.posx or posy != other.posy;
    }
    bool operator<(const Cell &other) const
    {
        return fValue < other.fValue;
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

    // Function to parse the grid from a file
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
                map[i][j].heuristic(end.posx, end.posy);
                map[i][j].updateFValue();
            }
        }
    }

    bool isNotInClosedList(Cell cell)
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

        // Reverse the path to start from the beginning
        std::reverse(path.begin(), path.end());
    }

    void aStar()
    {
        openList.clear();
        closedList.clear();
        resetCellValues();
        start.gValue = 0;
        start.calculateFunctionValue(end.posx, end.posy);

        // Use a priority queue to store cells
        std::priority_queue<Cell *, std::vector<Cell *>, std::greater<Cell *>> openQueue;

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

            current->generateNeighbors(map, XMAX, YMAX);

            for (Cell *neighbor : current->neighbours)
            {
                if (neighbor == &start)
                {
                    continue;
                }

                if (!isNotInClosedList(*neighbor))
                {
                    continue;
                }

                double tentativeGValue = current->gValue + 1.0; // Assuming a cost of 1 to move to a neighboring cell

                if (tentativeGValue < neighbor->gValue)
                {
                    neighbor->parent = current;
                    neighbor->gValue = tentativeGValue;                   // Update the gValue with the new, better value
                    neighbor->calculateFunctionValue(end.posx, end.posy); // Recalculate fValue based on the new gValue
                    openQueue.push(neighbor);
                    openList.push_back(neighbor);
                }
            }
        }
    }
};

void printPath(std::vector<Cell> &path)
{
    for (const Cell &cell : path)
    {
        std::cout << "Position: (" << cell.posx << "," << cell.posy << ")\n";
    }
}
void printGridWithPath(const Grid &grid)
{
    for (int i = 0; i < YMAX; i++)
    {
        for (int j = 0; j < XMAX; j++)
        {
            const Cell &currentCell = grid.map[i][j];

            if (currentCell == grid.start)
            {
                std::cout << "S "; // Start
            }
            else if (currentCell == grid.end)
            {
                std::cout << "E "; // End
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

    // Call your A* algorithm to find the path, which results in a vector of cells.
    grid.aStar();

    // Print the path and grid
    printPath(grid.path);
    printGridWithPath(grid);

    return 0;
}
