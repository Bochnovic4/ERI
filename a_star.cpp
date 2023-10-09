#include <iostream>
#include <fstream>

typedef struct {
    int value;
    int pos_x;
    int pos_y;
} S_GRID_CELL;

typedef struct {
    int pos_x;
    int pos_y;
    int end_x;
    int end_y;
    S_GRID_CELL cells[20][20];
    int closed[20][20];
    int open[20][20];
    int parent[20][20];
    int function[20][20];
} S_GRID;

S_GRID readFile(std::string fileName){
    S_GRID grid;

    std::ifstream file(fileName.c_str());

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            file >> grid.cells[i][j].value;
            grid.cells[i][j].pos_x = i;
            grid.cells[i][j].pos_y = j;
        }
    }

    return grid;
}

int main(){
    S_GRID grid;
    grid = readFile("grid.txt");

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            std::cout << grid.cells[i][j].value << " ";
        }
        std:: cout << '\n';
    }
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            std::cout << "("<<grid.cells[i][j].pos_x << ","<< grid.cells[i][j].pos_y << ")" << " ";
        }
        std:: cout << '\n';
    }

    return 0;
}
