#include <stdio.h>

int solveSudoku(int,int);
void printSudoku(void);
int sameColumn(int,int,int);
int sameSquare(int,int,int);
int sameRow(int,int,int);


int sudoku[9][9]=
        {0,0,3,0,2,0,6,0,0,
         9,0,0,3,0,5,0,0,1,
         0,0,1,8,0,6,4,0,0,
         0,0,8,1,0,2,9,0,0,
         7,0,0,0,0,0,0,0,8,
         0,0,6,7,0,8,2,0,0,
         0,0,2,6,0,9,5,0,0,
         8,0,0,2,0,3,0,0,9,
         0,0,5,0,1,0,3,0,0}; //remplacer par le sudoku detecter par RN


int main()
{
    int x = 0;
    int y=0;
    printf("\n");
    printf(" First Grid...\n");
    printSudoku();
    solveSudoku(x,y);
    printf(" The Sudoku Solved !\n");
    printSudoku();
    return 0;
}





void printSudoku()
{
    for(int i=0; i<9; i++)
    {
        if (i%3==0)
            printf("\n");
        for (int j=0;j<9; j++)
        {
            if (j%3==0)
                printf(" ");
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

int solveSudoku(int x, int y)
{
    int num =1;
    int tx=0;
    int ty=0;

    if (sudoku[x][y] != 0)
    {
        if (x==8 && y==8){
            return 1;
        }
        if (x<8)
        {
            x++;
        }
        else{
            x=0;
            y++;
        }
        if(solveSudoku(x,y)){
            return 1;
        }else {
            return 0;
        }
    }
    else{
        while (num<10){
            if (!sameSquare(x,y,num)&& !sameRow(x,y,num) && !sameColumn(x,y,num)){
                sudoku[x][y] = num;
                if (x==8 &&y ==8){
                    return 1;
                }

                if (x<8)
                {
                    tx=x+1;
                }
                else{
                    tx=0;
                    ty = y+1;
                }
                if(solveSudoku(tx,ty)){
                    return 1;
                }

            }
            num++;
        }
        sudoku[x][y] =0;
        return 0;
    }
}


int sameColumn(int x,int y,int num) //deja numero dans la colonne
{
    for(int i=0;i<9; i++)
    {
        if (sudoku[x][i] == num)
        {
            return 1;
        }
    }
    return 0;
}

int sameRow(int x,int y,int num)
{
    for(int i=0;i<9; i++)
    {
        {
            return 1;
        }
    }
    return 0;
}

int sameSquare(int x,int y,int num) //deja numero dans la colonne
{
    if (x < 3)
    {
        x=0;
    }
    else if (x<6) {
        x=3;
    }
    else{
        x=6;
    }
    if (y < 3)
    {
        y=0;
    }
    else if (y<6) {
        y=3;
    }
    else{
        y=6;
    }

    for(int i=x; i<x+3; i++) //8 car 9 cases
    {
        for(int j=y;j<y+3; j++)
        {

            if (sudoku[i][j] == num)
            {
                return 1;

            }
        }
    }
    return 0;
}
