#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define N  20
#define D  3
#define A  4


typedef struct _agent{
	int line;
	int column;
} Agent;

Agent leader;
Agent agents[A];
//char board[N][N];


void init(){
	srand(time(NULL));
	int line = rand() % N;
	int column = rand() % N;
	
	//leader = malloc(sizeof(Agent));
	
	leader.line = line;
	leader.column = column;
	
	/*for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			board[i][j] = "  ";
		}
	}*/
}

void initAgents(){
	int r = 360/Aa;
	
	
	agents[0].line = leader.line - D;
	agents[0].column = leader.column;
	
	agents[1].line = leader.line;
	agents[1].column = leader.column - D;
	
	agents[2].line = leader.line;
	agents[2].column = leader.column + D;
	
	agents[3].line = leader.line + D;
	agents[3].column = leader.column;
}

void print(){
	int l,c,k;
	char p;
	
	for(l = 0; l < N; l++){
		for(c = 0; c < N; c++){
			p = ' ';
			for(k = 0; k < A; k++){
				if((l == agents[k].line) && (c == agents[k].column)){
					p = '*';
				}
			}	
			if(l == leader.line && c == leader.column){
				p = '@';
			}
			printf(" %c", p);	
		}
		printf("\n");
	}
}

int main(){
	init();
	initAgents();
		
	print();

	return (0);
}
