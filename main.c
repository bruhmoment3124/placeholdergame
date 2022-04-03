#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*monochrome and extended ascii characters*/
#define monochrome 0
#define asciiext 1

/*define colors*/
#if monochrome == 0
	#define red "\033[0;31m"
	#define green "\033[0;32m"
	#define yellow "\033[0;33m"
	#define blue "\033[0;34m"
	#define grey "\033[0;37m"
	#define reset "\033[0m"
#elif monochrome == 1
	#define red
	#define green
	#define yellow
	#define blue
	#define grey
	#define reset
#endif

#if asciiext == 0
	#define characterid 64
	#define wallid 124
	#define ladderid 61
#elif asciiext == 1
	#define characterid 148
	#define wallid 177
	#define ladderid 240
#endif

#if __WIN32
	#define clearscr "cls"
#else
	#define clearscr "clear"
#endif

void newfloor(int mat[21][21], int xpos, int ypos, int dx, int dy, int ky, int kx)
{
	/*print wall tiles*/
	int wallx, wally;
	for(wallx=1; wallx<21; wallx++)
	{
		for(wally=1; wally<21; wally++)
		{		
			mat[wally][wallx] = wallid;
			printf("%c", mat[wally][wallx]);
		}
	}
	
	/*print floor tiles*/
	int floorx, floory;
	for(floorx=1; floorx<21; floorx++)
	{
		for(floory=1; floory<21; floory++)
		{
			int dir = rand() % 4;
			if(dir == 0)
			{
				mat[floory-1][floorx] = 46;
			} else if(dir == 1)
			{
				mat[floory+1][floorx] = 46;
			} else if(dir == 2)
			{
				mat[floory][floorx-1] = 46;
			}	else if(dir == 3)
			{
				mat[floory][floorx+1] = 46;
			}
		}
	}
	
	mat[ypos][xpos] = characterid;
	mat[dy][dx] = ladderid;
	mat[ky][kx] = 33;
}

int main(void)
{
	srand(time(0)); /*generate seeds*/
	
	/*map variables*/
	int mat[21][21];
	
	/*player variables*/
	int xpos = 1, ypos = 1;
	int health = 5, money = 0;
	int haskey = 0;
	
	int dx = rand() % 20, dy = rand() % 20; 
	int kx = rand() % 20, ky = rand() % 20;
	newfloor(mat, xpos, ypos, dx, dy, kx, ky);
	
	/*money*/
	int mx, my;
	
	int l;
	for(l=0; l<5; l++)
	{
		mx = rand() % 20, my = rand() % 20;
		mat[mx][my] = 36;
	}
	
	while(1)
	{		
		system(clearscr); /*clear screen*/

		int i, k;
		for(i=1; i<21; i++)
		{
			for(k=1; k<21; k++)
			{
				/*print in color*/
				if(mat[i][k] == wallid) /*bricks*/
				{
					printf(blue "%c" reset, mat[i][k]); /*blue*/
				} else if(mat[i][k] == 46) /*ground*/
				{
					printf(grey "%c" reset, mat[i][k]); /*grey*/
				} else if(mat[i][k] == 33) /*key*/
				{
					printf(yellow "%c" reset, mat[i][k]); /*yellow*/
				} else if(mat[i][k] == 36) /*money*/
				{
					printf(green "%c" reset, mat[i][k]); /*green*/
				} else
				{
					printf("%c", mat[i][k]);		
				}
			}
			printf("\n");
		}
		printf(red "%%:%d " reset green "$:%d " reset, health, money); /*health and money*/
		if(haskey == 1) printf(yellow "!\n" reset); /*key*/
		
		/*tile swaping*/
		int temp = mat[ypos][xpos];
		mat[ypos][xpos] = mat[1][1];
		mat[1][1] = temp;
		
		/*input*/
		char press = getchar();
		
		/*movement and collision*/
		if(press == 'a' && mat[ypos][xpos-1] != wallid) /*left*/
		{
			xpos-=1;
		} else if(press == 'd' && mat[ypos][xpos+1] != wallid) /*right*/
		{
			xpos+=1;
		} else if(press == 'w' && mat[ypos-1][xpos] != wallid) /*up*/
		{
			ypos-=1;
		} else if(press == 's' && mat[ypos+1][xpos] != wallid) /*down*/
		{
			ypos+=1;
		}
		
		/*go to next level*/
		if(mat[ypos][xpos] == ladderid && haskey == 1)
		{
			newfloor(mat, xpos, ypos, dx, dy, kx, ky);
			haskey-=1;
		}
		
		/*screen wrap*/
		if(xpos == 0) /*left side*/
		{
			xpos = 20;
		} else if(xpos == 21) /*right side*/
		{
			xpos = 1;
		} else if(ypos == 0) /*top side*/
		{
			ypos = 20;
		} else if(ypos == 21) /*bottom side*/
		{
			ypos = 1;
		}
		
		/*collect money*/
		if(mat[ypos][xpos] == 36) 
		{
			mat[my][mx] = 46;
			money+=1;
		}
		
		/*collect key*/
		if(mat[ypos][xpos] == 33) 
		{
			mat[ky][kx] = 46;
			haskey = 1;
		}
	}
	
	return 0;
}