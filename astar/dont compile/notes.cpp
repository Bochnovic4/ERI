#include <iostream>       // Include the C++ standard input/output library
#include <fstream>        // Include the C++ file input/output library
#include <string>         // Include the C++ string library
#include <cmath>          // Include the C++ math library
#include <vector>         // Include the C++ vector library
#include <algorithm>      // Include the C++ algorithm library
#include <limits>         // Include the C++ limits library
#include <stack>          // Include the C++ stack library
#include <cfloat>         // Include the C++ floating-point constants library
#include <queue>          // Include the C++ queue library

#define YMAX 20  // Define a constant for the maximum y dimension of the grid
#define XMAX 20  // Define a constant for the maximum x dimension of the grid

// Definition of the Cell class
class Cell
{
public:
    int posx;            // X position of the cell
    int posy;            // Y position of the cell
    int value;           // Value of the cell (1=start, 2=end, 5=obstacle)
    std::vector<Cell *> neighbours;  // List of neighboring cells
    double gValue;        // Cost from the start node to this node
    double hValue;        // Estimated cost from this node to the goal
    double fValue;        // f(x) = g(x) + h(x)
    Cell *parent;         // Parent cell in the path

    // Function to generate neighbors for the current cell based on grid boundaries
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
            // Check if the new position is within the bounds of the grid and is a valid cell
            if (newX >= 0 && newX < maxX && newY >= 0 && newY < maxY)
            {
                if (grid[newX][newY].isValid())
                {
                    neighbours.push_back(&grid[newX][newY]);
                }
            }
        }
    }

    // Function to update the fValue of the cell
    void updateFValue()
    {
        fValue = gValue + hValue;
    }

    // Function to check if the cell is valid (not an obstacle and within grid boundaries)
    bool isValid()
    {
        if (value == 5) // Assuming 5 indicates an obstacle
        {
            return false;
        }
        if (posx < 0 || posy < 0 || posx >= XMAX || posy >= YMAX)
        {
            return false;
        }
        return true;
    }

    // Function to check if the cell is the destination
    bool isDestination(const Cell &dest)
    {
        return posx == dest.posx && posy == dest.posy;
    }

    // Heuristic function (Euclidean distance) to estimate the cost to the goal
    double heuristic(int endX, int endY)
    {
        double dx = posx - endX;
        double dy = posy - endY;
        return std::sqrt(dx * dx + dy * dy);
    }

    // Function to calculate fValue, gValue, and hValue
    void calculateFunctionValue(int endX, int endY)
    {
        gValue = parent->gValue + 1.0;
        hValue = heuristic(endX, endY);
        fValue = gValue + hValue;
    }

    // Overloaded comparison operators for cells
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

    // Custom function to check if the current cell is better than another cell
    bool isBetterThan(const Cell &other) const
    {
        return fValue > other.fValue;
    }
};

// Definition of the Grid class
class Grid
{
public:
    Cell start;           // Start cell
    Cell end;             // End cell
    std::vector<std::vector<Cell>> map;  // 2D grid of cells
    std::vector<Cell *> openList;        // List of open cells
    std::vector<Cell *> closedList;      // List of closed cells (explored cells)
    std::vector<Cell> path;              // Path from start to end

    // Function to parse the grid from a file
    void parseGrid()
    {
        std::string filename = "grid.txt";  // Name of the file containing the grid
        int wym2 = 20;  // Maximum y dimension of the grid
        int wym1 = 20;  // Maximum x dimension of the grid
        map = std::vector<std::vector<Cell>>(wym2, std::vector<Cell>(wym1));  // Initialize the grid

        std::ifstream file(filename);  // Open the file for reading

        if (!file.is_open())  // Check if the file failed to open
        {
            std::cerr << "Failed to open the file." << std::endl;  // Print an error message
            return;
        }

        for (int i = 0; i < wym2; i++)
        {
            for (int j = 0; j < wym1; j++)
            {
                file >> map[i][j].value;  // Read the value from the file into the grid cell
                map[i][j].posx = i;       // Set the x position of the cell
                map[i][j].posy = j;       // Set the y position of the cell
                if (map[i][j].value == 1)
                {
                    start.value = map[i][j].value;  // Set the start cell
                    start.posx = i;                 // Set the x position of the start cell
                    start.posy = j;                 // Set the y position of the start cell
                }
                else if (map[i][j].value == 2)
                {
                    end.value = map[i][j].value;  // Set the end cell
                    end.posx = i;                 // Set the x position of the end cell
                    end.posy = j;                 // Set the y position of the end cell
                }
            }
        }
        file.close();  // Close the file
    }

    // Function to reset cell values
    void resetCellValues()
    {
        for (int i = 0; i < YMAX; i++)
        {
            for (int j = 0; j < XMAX; j++)
            {
                map[i][j].gValue = std::numeric_limits<double>::infinity();  // Set gValue to infinity
                map[i][j].heuristic(end.posx, end.posy);  // Calculate heuristic value
                map[i][j].updateFValue();  // Update fValue
            }
        }
    }

    // Function to check if a cell is not in the closed list
    bool isNotInClosedList(const Cell &cell)
    {
        return std::find(closedList.begin(), closedList.end(), &cell) == closedList.end();
    }

    // Function to reconstruct the path
    void reconstructPath(Cell &current)
    {
        path.clear();
        while (current != start)
        {
            path.push_back(current);
            current = *current.parent;
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());  // Reverse the path to start from the beginning
    }

    // A* algorithm implementation
    void aStar()
{
    openList.clear(); // Clear the open list
    closedList.clear(); // Clear the closed list
    resetCellValues(); // Reset the values of all cells
    start.gValue = 0; // Set the gValue of the start cell to 0
    start.calculateFunctionValue(end.posx, end.posy); // Calculate and set the hValue and fValue of the start cell

    auto compareCells = [](const Cell *a, const Cell *b)
    {
        return a->isBetterThan(*b);
    };
    // Create a priority queue (openQueue) to maintain cells with the lowest fValue at the top
    std::priority_queue<Cell *, std::vector<Cell *>, decltype(compareCells)> openQueue(compareCells);

    openQueue.push(&start); // Add the start cell to the openQueue

    while (!openQueue.empty()) // Continue until the openQueue is not empty
    {
        Cell *current = openQueue.top(); // Get the cell with the lowest fValue from the openQueue
        openQueue.pop(); // Remove the current cell from the openQueue

        if (current->isDestination(end)) // Check if the current cell is the destination
        {
            reconstructPath(*current); // Reconstruct the path from the current cell to the start
            return; // Exit the A* algorithm
        }

        closedList.push_back(current); // Add the current cell to the closed list

        current->generateNeighbors(map, XMAX, YMAX); // Generate neighbors for the current cell

        for (Cell *neighbor : current->neighbours) // Loop through the neighbors of the current cell
        {
            if (*neighbor == start) // Skip the start cell
            {
                continue;
            }

            if (!isNotInClosedList(*neighbor)) // Skip cells that are already in the closed list
            {
                continue;
            }

            double tentativeGValue = current->gValue + 1.0; // Calculate the tentative gValue for the neighbor

            if (tentativeGValue < neighbor->gValue) // Check if the tentative gValue is better than the neighbor's current gValue
            {
                neighbor->parent = current; // Update the parent of the neighbor to be the current cell
                neighbor->gValue = tentativeGValue; // Update the gValue of the neighbor
                neighbor->calculateFunctionValue(end.posx, end.posy); // Update the hValue and fValue of the neighbor
                openQueue.push(neighbor); // Add the neighbor to the openQueue
                openList.push_back(neighbor); // Add the neighbor to the open list
			}
		}
	}
}

};

// Function to print the path of cells
void printPath(const std::vector<Cell *> &path)
{
    for (const Cell *cell : path)
    {
        std::cout << "Position: (" << cell->posx << "," << cell->posy << ")\n";  // Print the position of the cell
    }
}

// Function to print the grid with path markers
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
    Grid grid;        // Create a Grid object
    grid.parseGrid(); // Parse the grid from a file

    grid.aStar();  // Run the A* algorithm to find the path

    printPath(grid.closedList); // Print the list of cells in the closed list (explored cells)
    printGridWithPath(grid);    // Print the grid with path markers

    return 0;
}
