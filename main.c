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
	#define wallid 35
	#define ladderid 61
#elif asciiext == 1
	#define characterid 148
	#define wallid 219
	#define ladderid 240
#endif

#if __WIN32
	#define clearscr "cls"
#else
	#define clearscr "clear"
#endif

/*stuff variables; will almost CERTAINLY change later*/
int dx, dy, kx, ky, mx, my;

int newfloor(int sizex, int sizey, int mat[sizex][sizey])
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
			int dir = rand() % 4;
			if(dir == 0)
			{
				mat[floory-1][floorx] = 45;
			} else if(dir == 1)
			{
				mat[floory+1][floorx] = 45;
			} else if(dir == 2)
			{
				mat[floory][floorx-1] = 45;
			}	else if(dir == 3)
			{
				mat[floory][floorx+1] = 45;
			}
		}
	}
	
	dx = rand() % sizex, dy = rand() % sizey; /*ladder*/
	kx = rand() % sizex, ky = rand() % sizey; /*key*/
	mx = rand() % sizex, my = rand() % sizey; /*money*/
	
	/*ladder*/
	mat[dy][dx] = ladderid;
	mat[ky][kx] = 33;
	
	/*money*/
	int l;
	for(l=0; l<5; l++)
	{
		mx = rand() % sizex, my = rand() % sizey;
		mat[mx][my] = 36;
	}
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
	newfloor(sizex+1, sizey+1, mat);
	
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
				} else if(mat[i][k] == 45) /*ground*/
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
			mat[my][mx] = 45;
			money+=1;
		}
		
		/*collect key*/
		if(mat[ypos][xpos] == 33) 
		{
			mat[ky][kx] = 45;
			haskey = 1;
		}
	}
	
	return 0;
}