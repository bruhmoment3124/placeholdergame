#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*for monochrome screens*/
#define monochrome 0

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

#define characterid 64
#define wallid 47
#define ladderid 61

#if __WIN32
	#define clearscr "cls"
#else
	#define clearscr "clear"
#endif

/*interactable items*/
struct items
{
	/*x and y coordinates*/
	int dx, dy; /*door*/
	int kx, ky; /*key*/
	int mx, my; /*money*/
} items;


int newfloor(int sizex, int sizey, int mat[sizey+1][sizex+1])
{
	/*print wall tiles*/
	int wallx, wally;
	for(wallx=1; wallx<sizex+1; wallx++)
	{
		for(wally=1; wally<sizey+1; wally++)
		{		
			mat[wally][wallx] = wallid;
			printf("%c", mat[wally][wallx]);
		}
	}
	
	/*print floor tiles*/
	int floorx, floory;
	for(floorx=1; floorx<sizex+1; floorx++)
	{
		for(floory=1; floory<sizey+1; floory++)
		{
			int dir = rand() % 2;
			if(dir == 0)
			{
				mat[floory][floorx-1] = 46;
			} else if(dir == 1)
			{
				mat[floory][floorx+1] = 46;
			}
		}
	}
	
	/*set random points of each item*/
	items.dx = rand() % sizex, items.dy = rand() % sizey; /*ladder*/
	items.kx = rand() % sizex, items.ky = rand() % sizey; /*key*/
	items.mx = rand() % sizex, items.my = rand() % sizey; /*money*/
	
	/*ladder*/
	mat[items.dy][items.dx] = ladderid;
	
	/*key*/
	if(mat[items.ky][items.kx] != ladderid) mat[items.ky][items.kx] = 33;
}

int main(int argc, char *argv[])
{
	srand(time(0)); /*generate seeds*/
	
	/*map variables*/
	int sizex, sizey;
	if(argv[1] != NULL) /*if arguments are supplied to the console*/
	{
		sizex = atoi(argv[1]);
		sizey = atoi(argv[2]);
	} else
	{
		printf("enter map size (x/y):\n");
		scanf("%d", &sizex);
		scanf("%d", &sizey);
	}
	
	int mat[sizey+2][sizex+2];
	int level = 1;
	
	newfloor(sizex+1, sizey+1, mat); /*create 1st floor*/
	
	/*player*/
	int xpos = 1, ypos = 1;	
	int health = 5, money = 0;
	int haskey = 0;
	mat[ypos][xpos] = characterid; /*set player position*/

	while(1)
	{		
		system(clearscr); /*clear screen*/

		int i, k;
		for(i=1; i<sizex+1; i++)
		{
			for(k=1; k<sizey+1; k++)
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
		
		if(haskey == 1) /*health, money, level, and key*/
		{
			printf(red "%%:%d " reset green "$:%d " reset blue "lvl:%d " reset yellow "!\n" reset, health, money, level);
		} else
		{
			printf(red "%%:%d " reset green "$:%d " reset blue "lvl:%d\n" reset, health, money, level);
		}
		
		/*tile swaping*/
		int temp = mat[ypos][xpos];
		mat[ypos][xpos] = mat[1][1];
		mat[1][1] = temp;
		
		/*input*/
		char press = getchar();
		
		/*movement and collision*/
		switch(press)
		{
			case 'a':
				if(mat[ypos][xpos-1] != wallid) xpos-=1; /*left*/
			break;
			
			case 'd':
				if(mat[ypos][xpos+1] != wallid) xpos+=1; /*right*/
			break;
			
			case 'w':
				if(mat[ypos-1][xpos] != wallid) ypos-=1; /*up*/
			break;
			
			case 's':
				if(mat[ypos+1][xpos] != wallid) ypos+=1; /*down*/
			break;
		}
		
		/*go to next level*/
		if(mat[ypos][xpos] == ladderid && haskey == 1)
		{
			level+=1; /*increment level int*/
			newfloor(sizex+1, sizey+1, mat);
		
			xpos = 1, ypos = 1;
			haskey-=1;
			mat[ypos][xpos] = characterid; /*set player position*/
		}
		
		/*screen wrap*/
		if(xpos == 0) /*left side*/
		{
			xpos = sizex;
		} else if(xpos == sizex+1) /*right side*/
		{
			xpos = 1;
		} else if(ypos == 0) /*top side*/
		{
			ypos = sizey;
		} else if(ypos == sizey+1) /*bottom side*/
		{
			ypos = 1;
		}
		
		/*collect money*/
		if(mat[ypos][xpos] == 36) 
		{
			mat[items.my][items.mx] = 46;
			money+=1;
		}
		
		/*collect key*/
		if(mat[ypos][xpos] == 33) 
		{
			mat[items.ky][items.kx] = 46;
			haskey = 1;
		}
	}
	
	return 0;
}