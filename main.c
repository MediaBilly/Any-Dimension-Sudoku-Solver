#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int **readBoard(FILE *in,int *dim) {
  int i,j,**board;
  //Read dimension
  fscanf(in,"%d",dim);
  //Read current placed numbers
  if((board = (int**)malloc(*dim * sizeof(int*))) == NULL)
    return NULL;
  for(i = 0;i < *dim;i++)
  {
    if((board[i] = (int*)malloc(*dim * sizeof(int))) == NULL)
      return NULL;
    for(j = 0;j < *dim;j++)
      fscanf(in,"%d",&board[i][j]);
  }
  return board;
}

void printBoard(int **board,int dim)
{
  int i,j;
  for(i = 0;i < dim;i++)
  {
    for(j = 0;j < dim;j++)
      printf("%d ",board[i][j]);
    printf("\n");
  }
}

int solve(int **board,int dim)
{
  int i,j,k,num,found,fits,x,y,dr;
  //Get next empty cell
  found = 0;
  for(i = 0;i < dim && !found;i++)
  {
    for(j = 0;j < dim && !found;j++)
      if(board[i][j] == 0)
        found = 1;
  }
  i--;
  j--;
  //Sudoku filled
  if(!found)
    return 1;
  dr = (int)sqrt(dim);
  //Find a number that fits this cell
  for(num = 1;num <= dim;num++)
  {
    fits = 1;
    //Check if it fits horizontally
    for(k = 0;k < dim && fits;k++)
      if(board[i][k] == num)
        fits = 0;
        //Check if it fits horizontally
    for(k = 0;k < dim && fits;k++)
      if(board[k][j] == num)
        fits = 0;
    //Check if it fits in the box
    for(x = i - (i % dr); x < i - (i % dr) + dr && fits;x++)
      for(y = j - (j % dr); y < j - (j % dr) + dr && fits;y++)
        if(board[x][y] == num)
          fits = 0;
    if(fits)
    {
      //Place fitting number and solve the other cells
      board[i][j] = num;
      //If success return 1
      if(solve(board,dim))
        return 1;
      else
        board[i][j] = 0;
      //Ohterwise try new number
    }
  }
  //Out of options
  return 0;
}

void freeBoard(int **board,int dim)
{
  int i;
  for(i = 0;i < dim;i++)
    free(board[i]);
  free(board);
}

int main(int argc, char const *argv[]) {
  int **board,dim;
  FILE *in;
  //Open file and check for errors
  if(argc < 2)
  {
    printf("Error: Not enough arguments.\n");
    return 0;
  }
  if((in = fopen(argv[1],"r")) == NULL)
  {
    printf("Error:Could not open puzzle.txt file.\n");
    return 0;
  }
  //Read board and check for errors
  if((board = readBoard(in,&dim)) == NULL)
  {
    printf("Error:Not enough memory.\n");
    return 0;
  }
  if(solve(board,dim))
    printBoard(board,dim);
  else
    printf("No solution for this puzzle.\n");
  return 0;
}
