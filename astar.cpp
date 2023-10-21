#include <iostream>
#include <fstream>
#include <string>

enum {
    OPEN, WALL=5, NONE = NULL
};


typedef struct {
    int posx;
    int posy;
    int value;
} S_CELL;


typedef struct {
    S_CELL start;
    S_CELL end;
    S_CELL map[20][20];
    S_CELL currentCell;
    S_CELL neighbours[4];
    int openList[20];
    int closedList[20];
} S_GRID;

S_GRID parseGrid(){
    S_GRID grid;
    std::string nazwap="grid.txt";
    int wym2=20;
    int wym1=20;

    std::ifstream plik(nazwap.c_str());
    
    for ( int i=0;i<wym2;i++)    
    {
        for ( int j=0;j<wym1;j++) 
        {
            plik >> grid.map[i][j].value;
            grid.map[i][j].posx = i;
            grid.map[i][j].posy = j;
        }
    }  
    plik.close();
    return grid;
}

S_GRID checkNeighbours(S_GRID grid){
    for (int i = 0 ;i<4; i++) {
        int checkBottom = grid.map[grid.currentCell.posx + 1][grid.currentCell.posy].value;
        int checkTop = grid.map[grid.currentCell.posx - 1][grid.currentCell.posy].value;
        int checkLeft = grid.map[grid.currentCell.posx][grid.currentCell.posy - 1].value;;
        int checkRight = grid.map[grid.currentCell.posx][grid.currentCell.posy + 1].value;;
        switch (i)
        {
        case 0:
            if (checkBottom != NULL && checkBottom != 5)
                grid.neighbours[i] = grid.map[grid.currentCell.posx][grid.currentCell.posy+1];
            break;
        case 1:
            if (checkTop != NULL && checkTop != 5)
                grid.neighbours[i] = grid.map[grid.currentCell.posx][grid.currentCell.posy-1];
            break;
        case 2:
            if (checkLeft != NULL && checkLeft != 5)
                grid.neighbours[i] = grid.map[grid.currentCell.posx - 1][grid.currentCell.posy];
        case 3:
            if (checkRight != NULL && checkRight != 5)
                grid.neighbours[i] = grid.map[grid.currentCell.posx + 1][grid.currentCell.posy+1];
            break;
        }
        
    }
    return grid
}


S_GRID aStar(S_GRID grid, S_CELL start, S_CELL end) {
    grid.start = start;
    grid.end = end;
    double heuristic = ((grid.start.posx + grid.end.posy) << 1 + (grid.start.posy - grid.end.posy) << 1) >> 1;
    while (grid.currentCell.posx != end.posx && grid.currentCell.posy != end.posy){
        grid = checkNeighbours(grid);       
    } 
    return grid;
}


int main(){
    S_GRID grid = parseGrid();
    
    return 0;
}