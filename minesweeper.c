#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>



/* 
    Array values:
    1 - 8: Number tile closed
    9: Mine tile (closed)
    10: Empty tile closed
    11 - 18: Number tile opened
    20: Empty tile opened
    21 - 28: Number tile marked
    29: Mine tile marked
    30: Empty tile marked
*/


void clear_keyboard_cache();
int mines(int x);
void showboard(int **sboard, int x, int y);
int command(int flag);
void checktile(int tile);
int filler(int **sboard, int a1 ,int b1, int x, int y);
void gameover(int s);
void open(int **sboard, int a, int b, int x, int y);
int check_win(int **sboard, int x, int y);

int main(void)
{
    int **board, n, m, i, j, s = 0, k, l, ig, jg, com, flag;
    
    char dif[13];
    
    srand((int) time(NULL));
    
    
    printf("Give the size of the board:\n");
    do
    {
        printf("Number of lines (9 - 20): ");
        scanf("%d", &n);
        clear_keyboard_cache();
    }while(n < 9 || n > 20);
    do
    {
        printf("Number of cols (9 - 20): ");
        scanf("%d", &m);
        clear_keyboard_cache();
    }while(m < 9 || m > 20);
    
    
    printf("Chose a difficulty\n");
    do
    {
        printf("Easy-Medium-Hard-Impossible: ");
        fgets(dif, sizeof(dif), stdin);
        
        if (strlen(dif) > 0 && dif[strlen(dif) - 1] == '\n')    // Removes newline character from input
            dif[strlen(dif) - 1] = '\0';
        
        
        // The following makes the input not case sensitive
        for(i = 0; dif[i]; i++)
        {
            dif[i] = tolower(dif[i]);
        }

        if(strcmp(dif, "easy") == 0)
        {
            s = 10;
        }
        else if(strcmp(dif, "medium") == 0)
        {
            s = 15;
        } 
        else if(strcmp(dif, "hard") == 0)
        {
            s = 20;
        } 
        else if(strcmp(dif, "impossible") == 0)
        {
           s = 25;
        } 
    }while(!s);
    
    
    won:
    board = (int **)malloc(sizeof(int *) * n);
    for(i = 0; i < n; i++)
    {
        board[i] = (int *)malloc(sizeof(int) * m);
		if(board[i] == NULL) 
		{
			printf("Memory allocation failed!");
			exit(1);
		}
    }
    
    
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            board[i][j] = 0;
        }
    }
    
    
    k = ((n * m) * s) / 100;
    for(l = 0; l < k; l++)
    {
        i = mines(n);
        j = mines(m);
        if(board[i][j] == 9)
            l--;
        else
            board[i][j] = 9;
    }
    
    
    for(i = 0; i < n; i++) // Fills the board with the numbers (amount of mines in neighbouring tiles)
    {
        for(j = 0; j < m; j++)
        {
            if(board[i][j] != 9)
                board[i][j] = filler(board, i, j, n, m);
        }
    }
    
    
    showboard(board, n, m);
    
    flag = 0; // Used to prevent player from losing from the first open
    while(1)
    {
        
        com = command(1);
        ig = command(2);
        jg = command(3);
      
        
        if(ig > n || jg > m || ig < 0 || jg < 0)
            printf("The numbers you gave are outside of the board!\n");
        else if(board[ig][jg] > 10 && board[ig][jg] < 21)
            printf("This tile is already open, please choose another tile!\n");
        else
        {
            if(com == 1)
            {
                if(flag)
                {
                    checktile(board[ig][jg]);
                }
                else
                {
                    flag = 1;
                    if(board[ig][jg] == 9)
                    {
                        board[ig][jg] = 0;
                        while(1)
                        {
                            i = mines(n);
                            j = mines(m);
                            if(board[i][j] != 9 && (i != ig && j != jg))
                            {
                                board[i][j] == 9;
                                break;
                            }
                        }
                        for(i = 0; i < n; i++)
                        {
                            for(j = 0; j < m; j++)
                            {
                                if(board[i][j] != 9)
                                {
                                    board[i][j] = 0;
                                    board[i][j] = filler(board, i, j, n, m);
                                }
                            }
                        }
                    }
                }
                
                
                if(board[ig][jg] == 10)
                {
                    open(board,ig,jg,n,m);
                }
                else
                    board[ig][jg] += 10;
                
                showboard(board, n, m);
            }
            else if(com == 2)
            {
            
                if(board[ig][jg] > 0 && board[ig][jg] <= 10)
                {
                    board[ig][jg] += 20;
                    showboard(board, n, m);
                }
                else if(board[ig][jg] > 21 || board[ig][jg] <= 30) // 9 (bomb) + 21 (mark) == 30
                {
                    board[ig][jg] -= 20;
                    showboard(board, n, m);
                }
                else
                    printf("This tile is already open, please choose another tile!\n");
            }
            else if(com == 3)
            {
                if(board[ig][jg] == 9)
                    printf("There is a mine behind this tile!\n");
                else if(board[ig][jg] > 0 && board[ig][jg] <= 10)
                {
                    if(board[ig][jg] == 10)
                        printf("This tile is empty!\n");
                    else
                        printf("Behind this tile there is the number %d!\n", board[ig][jg]);
                }
                else if(board[ig][jg] > 10 && board[ig][jg] < 21)
                    printf("This tile is already open, please choose another tile!\n");
                else
                    printf("This tile is marked, unmark before cheating!\n");
            }
            if(check_win(board, n, m))
            {
                for(i = 0; i < m; i++)
                {
                    free(board[i]);
                }
                free(board);
                
                n += 5;
                m += 5;
                if(n > 30 || m > 30)
                {
                    gameover(2);
                }
                else
                {
                    printf("Level won! New level challenge!\n");
                    goto won;
                }
            }
        }
    }
    
    return 0;
}

void clear_keyboard_cache()
{
    char ch;
    
    while((ch = getchar()) != '\n' && ch != EOF);
}


int mines(int x)
{ 
    return 0 + rand() % x;
}


void showboard(int **sboard, int x, int y)
{
    int i, j, k = 0;
    
    printf("   ");
    if(y <= 9)
        printf(" ");
    
    
    for(j = 0; j < y; j++)
    {
        if(y > 9 && j < 9)
            printf(" ");
        printf("%d ", j + 1);
    }
    printf("\n");
    
    printf("---");
    for(j = 0; j < y; j++)
    {
        if(y > 9)
            printf("---");
        else
            printf("--");
    }
    printf("\n");
    
    for(i = 0; i < x; i++)
    {
        if(i < 9)
            printf(" ");
        for(j = 0; j < y; j++)
        {
            if(!j)
                printf("%d| ", i + 1);
            if(sboard[i][j] > 0 && sboard[i][j] <= 10)
            {
                printf("# ");
                if(y > 9)
                    printf(" ");
            }
            else if(sboard[i][j] > 10 && sboard[i][j] < 19)
            {
                printf("%d ", sboard[i][j] - 10);
                k++;
                if(y > 9)
                    printf(" ");
            }
            else if(sboard[i][j] == 20)
            {
                printf(". ");
                k++;
                if(y > 9)
                    printf(" ");
            }
            else if(sboard[i][j] > 20)
            {
                printf("@ ");
                if(y > 9)
                    printf(" ");
            }
        }
        printf("\n");
    }
    
    printf("\n%d blocks are open.\n", k);
    printf("Make your move(s)\n");
}


int command(int flag)
{ 
    int i, j, k = 0, check = 1;
    char comm[13];
    static int gi, gj;
 
    if(flag == 1)
    {
        while(check)
        {
            printf("Available commands: Open/Mark/Cheat\n");
            printf("Type your command or type exit - ex. open(4,4): ");
            
            fgets(comm, sizeof(comm), stdin);
            
            if (strlen(comm) > 0 && comm[strlen(comm) - 1] == '\n')
                comm[strlen(comm) - 1] = '\0';
           
            for(i = 0; comm[i]; i++)
            {
                comm[i] = tolower(comm[i]);
            }
            
            if(!strncmp(comm, "open(", 5))
                k = 1;
            else if(!strncmp(comm, "mark(", 5))
                k = 2;
            else if(!strncmp(comm, "cheat(", 6))
                k = 3;
            else if(!strcmp(comm, "exit"))
                exit(0);
            
            
            i = 5; // The 5th element of the array is the first digit if the user chose open or mark
            if(k > 0)
            {
                if(k == 3)
                    i++;
            
                if(isdigit(comm[i]) && comm[i + 1] == ',' && isdigit(comm[i + 2]) && (comm[i + 3] == ')' || (isdigit(comm[i + 3]) && comm[i + 4] == ')')))
                { 
                    gi = atoi(comm + i);
                    gj = atoi(comm + i + 2);
                    check = 0;
                    
                }
                else if(isdigit(comm[i]) && isdigit(comm[i + 1 ]) && comm[i + 2] == ',' && isdigit(comm[i + 3]) && (comm[i + 4] == ')' || (isdigit(comm[i + 4]) && comm[i + 5 ] == ')')))
                {
                    gi = atoi(comm + i);
                    gj = atoi(comm + i + 3);
                    check = 0;
                }
                
            }
        }
        
        gi--;
        gj--;
        
        return k;
    }
    else if(flag == 2)
    {
        return gi;
    }    
    else if(flag == 3)
    {
         return gj;
    }
}


void checktile(int tile)
{
    if(tile == 9)
        gameover(1);
}


int filler(int **sboard, int a1, int b1, int x, int y)
{
    if (a1 != 0) // up
    {
        if (sboard[a1 - 1][b1] == 9)
            sboard[a1][b1]++;
    }
    if (a1 != x - 1) // down
    {
        if (sboard[a1 + 1][b1] == 9)
            sboard[a1][b1]++;
    }
    if (b1 != 0) // left
    {
        if (sboard[a1][b1 - 1] == 9)
            sboard[a1][b1]++;
    }
    if (b1 != y - 1) // right
    {
        if (sboard[a1][b1 + 1] == 9)
            sboard[a1][b1]++;
    }
    if (a1 != 0 && b1 != 0) // up + left
    {
        if (sboard[a1 - 1][b1 - 1] == 9)
            sboard[a1][b1]++;
    }
    if (a1 != 0 && b1 != y - 1) // up + right
    {
        if (sboard[a1 - 1][b1 + 1] == 9)
            sboard[a1][b1]++;
    }
    if (a1 != x - 1 && b1 != 0) // down + left
    {
        if (sboard[a1 + 1][b1 - 1] == 9)
            sboard[a1][b1]++;
    }
    if (a1 != x - 1 && b1 != y - 1) // down + right
    {
        if (sboard[a1 + 1][b1 + 1] == 9)
            sboard[a1][b1]++;
    }
    
    
    if(sboard[a1][b1] == 0)
    {
        sboard[a1][b1] = 10;
        return sboard[a1][b1];
    }
    else
        return  sboard[a1][b1];
}


void open(int **sboard, int a, int b, int x, int y)
{
    int i, j;
    if(a < 0 || a >= x || b < 0 || b >= y)
    {
        return;
    }
    
    if(sboard[a][b] >= 1 && sboard[a][b] < 9  )
    {
        sboard[a][b]+=10;
        return;
    }
    
    if(sboard[a][b] >=11)
    {
        if(sboard[a][b] > 20 && sboard[a][b] != 29)
        {
            sboard[a][b] -= 20;
        }
        else
            return;
    }
    
    if(sboard[a][b] == 9)
    {
        return;
    }
    
    sboard[a][b]+=10;
    
    for(i = -1; i <=1; i++)
    {
        for(j = -1; j <= 1; j++)
        {
            if(i==0 && j==0)
            {
                continue;
            }
            open(sboard, a + i, b + j, x, y);
        }
    }
    
    
}


int check_win(int **sboard, int x, int y)
{
    int i, j;
    
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            if((sboard[i][j] > 0 && sboard[i][j] < 9) || (sboard[i][j] > 20 && sboard[i][j] < 29) || (sboard[i][j] == 10) || (sboard[i][j] == 30))
                return 0;
        }
    }
    
    return 1;
}


void gameover(int f)
{
    if(f == 1)
    {
        printf("     _.-^^---....,,--\n");
        printf(" _--                  --_\n");
        printf("<                        >)\n");
        printf("|                         |\n");
        printf(" \\._                   _./\n");
        printf("    ```--. . , ; .--'''\n");
        printf("          | |   |\n");
        printf("       .-=||  | |=-.\n");
        printf("       `-=#$#&#$#=-'\n");
        printf("          | ;  :|\n");
        printf(" _____.,-#=&$@=#&#~,._____\n");
    }
    else if(f == 2)
    {
        printf("         ______________\n");
        printf("        /             /|\n");
        printf("       /             / |\n");
        printf("      /____________ /  |\n");
        printf("     | ___________ |   |\n");
        printf("     ||           ||   |\n");
        printf("     ||   You     ||   |\n");
        printf("     ||   Won!    ||   |\n");
        printf("     ||___________||   |\n");
        printf("     |   _______   |  /\n");
        printf("    /|  (_______)  | /\n");
        printf("   ( |_____________|/\n");
        printf("    \\\n");
        printf(".=======================.\n");
        printf("| ::::::::::::::::  ::: |\n");
        printf("| ::::::::::::::[]  ::: |\n");
        printf("|   -----------     ::: |\n");
        printf("`-----------------------'\n");
    }
    else
    {
        printf(" __     __           _               _\n");
        printf(" \\ \\   / /          | |             | |\n");
        printf("  \\ \\_/ /__  _   _  | |     ___  ___| |_\n");
        printf("   \\   / _ \\| | | | | |    / _ \\/ __| __|\n");
        printf("    | | (_) | |_| | | |___| (_) \\__ \\ |_\n");
        printf("    |_|\\___/ \\__,_| |______\\___/|___/\\__|\n");
    }
    
    exit(0);
}