#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<windows.h>

char view[10][10];

typedef struct {
	char shape;
	int x, y;
	int is_body;
}object_2d;

int game_score = 0, best_score = 0;

size_t game_objects_size;

object_2d *game_objects;


////snake body size 

size_t snake_body_size = 0;

////the snake body
int *snake_body;


///player model
object_2d *player;

////fruit for achiving more score

object_2d fruit = (object_2d){'Q',4,8,0};

//stores the player last input
char player_last_input;

void display_view();
void assign_background();
void clean_view();
void render_objects();
void debug_logs();
void game_on();
void player_movement(object_2d *player);
void keep_going();
void tail_follow_player();
void snake_assign_body();
void snake_add_body();
void remove_object(int index_location);
void add_new_object();
void loss_conditions();
void close_game();




void assign_background(){
	for(int i = 0; i < 10; ++i){
		for(int j = 0; j < 10; ++j){
			view[i][j] = '.';
		}
	}
}

void display_view(){
	for(int i = 0; i < 10; ++i){
		for(int j = 0; j < 10; ++j){
			printf("  %c  ",view[i][j]);
		}
		printf("\n\n");
	}
}

void clean_view(){
	system("cls");
}

void render_objects(){
	//assign_background();
	for(int i = 0; i < game_objects_size; ++i){
		int x = game_objects[i].x, y = game_objects[i].y;
		if((x >= 0 && x <= 9) && (y >= 0 && y <= 9)){
			view[x][y] = game_objects[i].shape;
		}
	}
	
	view[fruit.x][fruit.y] = fruit.shape;
	//display_view();
}

void debug_logs(){
	
	printf("your score : %d\n",game_score * 33);
	printf("best_score score : %d\n",best_score);
	/*
	printf("x : %d , y : %d\n",game_objects[0].x,game_objects[0].y);
	for(int i = 0; i < snake_body_size; ++i){
		printf("body %d x : %d , y : %d\n",i,game_objects[snake_body[i]].x,game_objects[snake_body[i]].y);
	}*/
}

void game_on(){
	clean_view();
	assign_background();
	render_objects();
	display_view();
	debug_logs();
	loss_conditions();
}

void loss_conditions(){///also used for collision
	if(player->x == fruit.x && player->y == fruit.y){
		++game_score;
		snake_add_body();
		srand(time(NULL));
		int x = rand() % 9, y = rand() % 9;
		fruit.x = x;
		fruit.y = y;
		
	}
	if(player->x == 10 || player->x == -1){
		printf("you have lost, the snake hit it's head!\n");
		close_game();
		exit(1);
	}
	else if(player->y == 10 || player->y == -1){
		printf("you have lost, the snake hit it's head!");
		close_game();
		exit(1);
	}
	
	for(int i = 0; i < snake_body_size - 1; ++i){
		if((player->x == game_objects[snake_body[i]].x && player->y == game_objects[snake_body[i]].y) && i != 0){///additional condition since the neck is too close and trigers loss
			printf("you have lost, the snake hit it's head! it was %d\n",i);
			close_game();
			exit(1);
		}
	}
}

void close_game(){
	if(game_score * 33 > best_score){
		printf("new score!");
		FILE *temp_fp = fopen("data.txt","w");
		if(temp_fp != NULL){
			fprintf(temp_fp,"gmd:%d",game_score * 33);
			fclose(temp_fp);
		}
		else{
			perror("error");
		}
	}
}


void player_movement(object_2d *player){
	char getch_input = getch();
	switch(getch_input){
		case 'W':
			if(player->x != 0 && player_last_input != 'S'){
				player_last_input = getch_input;
				//tail_follow_player();
				//--player->x;
			} 
			break;
		case 'S':
			if(player->x != 9 && player_last_input != 'W'){
				player_last_input = getch_input;
				//tail_follow_player();
				//++player->x;
			} 
			break;
		
		case 'A':
			if(player->y != 0 && player_last_input != 'D'){
				player_last_input = getch_input;
				//tail_follow_player();
				//--player->y;
			} 
			break;
		case 'D':
			if(player->y != 9 && player_last_input != 'A'){
				player_last_input = getch_input;
				//tail_follow_player();
				//++player->y;
			} 
			break;
		case 'E':
			snake_add_body();
			break;
		case 'H':
			free(game_objects);
			exit(1);
			break;
	}
}


void add_new_object(){
	++game_objects_size;
	object_2d *temp_obj = realloc(game_objects,game_objects_size * sizeof(object_2d));
	game_objects = temp_obj;
	player = &game_objects[0];
}

void remove_object(int index_location){
	for(int i = index_location; i < game_objects_size; ++i){
		game_objects[i] = game_objects[i + 1];
	}
	--game_objects_size;
	object_2d *temp_obj = realloc(game_objects,game_objects_size * sizeof(object_2d));
	game_objects = temp_obj;
	player = &game_objects[0];
	snake_assign_body();
}

void keep_going(){
	switch(player_last_input){
		case 'W':
			if(player->x != -1){
				tail_follow_player();
				--player->x;
			} 
			break;
		case 'S':
			if(player->x != 10){
				tail_follow_player();
				++player->x;
			} 
			break;
		case 'A':
			if(player->y != -1){
				tail_follow_player();
				--player->y;
			} 
			break;
		case 'D':
			if(player->y != 10){
				tail_follow_player();
				++player->y;
			} 
			break;
		case 'H':
			free(game_objects);
			exit(1);
			break;
	}
}

void snake_assign_body(){
	int index = 0;
	for(int i = 0; i < game_objects_size; ++i){
		if(game_objects[i].is_body){
			snake_body[index] = i;
			index++;
		}
			
	}
}

void snake_add_body(){
	/////add the snake body into the objects array
	add_new_object();
	int x, y;///place the snake recent body
	if(snake_body_size == 1){
		x = player->x;
		y = player->y - 1;
	}
	else{
		x = game_objects[snake_body[snake_body_size - 1]].x;
		y = game_objects[snake_body[snake_body_size - 1]].y - 1;
	}
	game_objects[game_objects_size - 1] = (object_2d){'W',x,y,1};
	///reassign the snake body array
	snake_body_size++;
	int *temp = realloc(snake_body,snake_body_size * sizeof(int));
	snake_body = temp;
	snake_body[snake_body_size - 1] = game_objects_size - 1;
}

void tail_follow_player(){
	if(snake_body_size == 1){
		game_objects[snake_body[0]].x = player->x;
		game_objects[snake_body[0]].y = player->y;
	}
	else{
		
		for(int i = snake_body_size - 1; i > -1; --i){
			if(i == 0){
				game_objects[snake_body[i]].x = player->x;
				game_objects[snake_body[i]].y = player->y;
			}
			else{
				game_objects[snake_body[i]].x = game_objects[snake_body[i - 1]].x;
				game_objects[snake_body[i]].y = game_objects[snake_body[i - 1]].y;
			}
		}
	}
	
	/*s
	for(int i = snake_body_size; i > 0; --i){
		
		if(i == 0){
			game_objects[snake_body[i]].x = player->x;
			game_objects[snake_body[i]].y = player->y;
		}
		//game_objects[snake_body[i]].x = game_objects[i - 1].x;
		//game_objects[snake_body[i]].y = game_objects[i - 1].y;
		
		//game_objects[snake_body[i]].x = player->x;
		//game_objects[snake_body[i]].y = player->y;
	}*/
}




int main(){
	
	FILE *temp_fp = fopen("data.txt","r");
	if(temp_fp != NULL){
		fscanf(temp_fp,"gmd : %d",&best_score);
		fclose(temp_fp);
	}
	else{
		fclose(temp_fp);
	}
	
	game_objects_size = 1;
	game_objects = malloc(game_objects_size * sizeof(object_2d));
	game_objects[0] = (object_2d){'@',4,4,0};
	
	snake_body_size++;
	snake_body = malloc(snake_body_size * sizeof(int));
	snake_body[0] = 1;
	add_new_object();
	game_objects[1] = (object_2d){'W',4,3,1};
	
	snake_add_body();
	
	//snake_body_size++;
	//int *temp = realloc(snake_body,snake_body_size * sizeof(int));
	
	//snake_add_body();
	
	
	player = &game_objects[0];
	
	
	//remove_object(1);
	
	while(1){
		game_on();
		if(kbhit())player_movement(player);
		keep_going();
		Sleep(100);
		
	}
	
	free(game_objects);
	free(snake_body);
	
	return 0;
}