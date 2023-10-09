#include <iostream>
#include <fstream>

typedef struct {
    int pos_x;
    int pos_y;
    int end_x;
    int end_y;
    int array[20][20];
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
            file >> grid.array[i][j];
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
            std::cout << grid.array[i][j] << " ";
        }
        std:: cout << '\n';
    }
    return 0;
}
