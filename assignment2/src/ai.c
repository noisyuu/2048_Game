#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "ai.h"
#include "utils.h"
#include "priority_queue.h"
#include <math.h>

struct heap h;
uint32_t generated = 0;
uint32_t expanded = 0;
float seconds = 0;


void initialize_ai(){
	heap_init(&h);
}
/**
 * Find best action by building all possible paths up to depth max_depth
 * and back propagate using either max or avg
 */

move_t get_next_move( uint8_t board[SIZE][SIZE], int max_depth, propagation_t propagation ){
	FILE *outputFile;
	outputFile = fopen("output.txt","w	");
	clock_t start_time = 1000*clock();
	move_t best_action = rand() % 4;
	node_t* start;
	start = (node_t *)malloc(sizeof(node_t));
	if(!start){	
		exit(1);
	}
	//initialize the start component
	start->priority = 0;
	start->depth = 0;
	start->num_childs = 4;
	copy_board(board,start->board);
	
	uint32_t arraysize = (uint64_t) pow(4, max_depth+2);
	uint32_t k; 
	node_t* visited[arraysize];
	initialize_ai();
	heap_push(&h, start);
	
	//don't forget to free all the memory
	uint32_t visited_Index = 0;
	int maxScore[4] = {0,0,0,0};
	int avgMaxScore[4] = {0,0,0,0};
	int avgTotalScore[4] = {0,0,0,0};
	int visitedNode[4] = {0,0,0,0};
	int move;
	while(h.count != 0){
		node_t* current = heap_delete(&h);
		visited[visited_Index++] = current;
		
		if(current-> depth <= max_depth){

			for(move = left; move <= down; move++){
				node_t* newNode;
				bool success = false;
				newNode = (node_t *)malloc(sizeof(node_t));
				if(newNode == NULL){
					exit(1);
				}
				generated++;
				newNode->parent = current;
				newNode->priority = current -> priority;
				copy_board(current->board,newNode->board);
				newNode->move = move;
				newNode->depth = current->depth;
				newNode->num_childs = current->num_childs;
				
				success = execute_move_t(newNode->board,&(newNode->priority),move);
				newNode->depth += 1;
				
				if(success){
					expanded++;
					addRandom(newNode->board); 
					heap_push(&h,newNode);
					node_t* temp = newNode;
					while(temp->depth > 1){
						temp = temp->parent;
						
					}
					if(propagation == 0 && newNode->priority >= temp->priority){
							temp->priority = newNode->priority;
							maxScore[temp->move] = newNode->priority;
					}
					if(propagation == 1){
						visitedNode[temp->move] += 1;
						avgTotalScore[temp->move] += newNode->priority;
						if (avgTotalScore[temp->move]/visitedNode[temp->move] >= avgMaxScore[temp->move]){
							avgMaxScore[temp->move] = avgTotalScore[temp->move]/visitedNode[temp->move];
						}
					}
				}
				else{
					free(newNode);
					
				}
			}
		} 	
		
	}
	uint32_t tempMax = 1;
	for(move = left; move <= down; move++){
			if(propagation == 0 && maxScore[move] >= tempMax){
				tempMax = maxScore[move];
				best_action = (move_t)move;
			}
			if(propagation == 1 && avgMaxScore[move] >= tempMax){
				tempMax = avgMaxScore[move];
				best_action = (move_t)move;
			}
		}
		
	emptyPQ(&h);		
	for(k=0; k< visited_Index;k++){
		free(visited[k]);
	}
	
	clock_t end_time = 1000 * clock();
	seconds += (end_time - start_time)/CLOCKS_PER_SEC;
	float eovertime = expanded/seconds;
	fprintf(outputFile,"MaxDepth = %d \n",max_depth);
	fprintf(outputFile,"Generated = %d \n",	generated);
	fprintf(outputFile,"Expanded = %d \n",expanded);
	fprintf(outputFile,"Time = %f \n",seconds/1000);
	fprintf(outputFile,"Expanded/Second = %f \n 	",eovertime*1000);
	fclose(outputFile);
	return best_action;
	
}

	
void copy_board(uint8_t source[SIZE][SIZE],uint8_t target[SIZE][SIZE]){
	int i,j;
	for(i = 0; i <SIZE; i++){
		for(j  = 0; j< SIZE; j++){
			target[i][j] = source[i][j];
		}
	}
}



