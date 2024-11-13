#include<stdlib.h>
#include <stdio.h>
#include<conio.h>
#include <time.h>
#define rows 20
#define columns 20
#define Initial_length 2
#define number_of_food 2
char board[columns*rows];
int game_over=0;
typedef struct{
    int x,y;

}snake_part;
typedef struct {
    int length;
    snake_part part[256]; 
}Snake;
typedef struct{
    int x,y;
}Food;
typedef struct{
    int number;
    Food food[2]; 
}Foods;

void fill_board(){
    int x,y;
    for (x=0;x<rows;x++){
        for(y=0;y<columns;y++){
            if(x==0||y==0||x==rows-1||y==columns-1){
                board[x*columns+y]='#';
                
            }
            else{board[x*columns+y]=' ';}
        }
    }
            }
void print_board(){
    system("cls");
    int x,y;
    for (x=0;x<rows;x++){
        for(y=0;y<columns;y++){
            putch(board[x*columns+y]);
                
            }
            putch('\n');
        }
    }
Snake snake;
Foods foods;
void draw_food(){
    int i;
    for(i=0;i<foods.number;i++){
        board[foods.food[i].x*columns+foods.food[i].y]='+';

    }

}

void draw_snake(){
    int i;
    for (i=snake.length-1;i>=0;i--){
        if (i==0)board[snake.part[i].x*columns+snake.part[i].y]='@';
        else  board[snake.part[i].x*columns+snake.part[i].y]='*';
    }
}

void move_snake(int deltaX,int deltaY){
    int i;
    snake.part[0].x+=deltaX;
    snake.part[0].y+=deltaY;
    for(i=1;i<snake.length;i++){
        if(snake.part[0].x==snake.part[i].x&&snake.part[0].y==snake.part[i].y) game_over=1;
    }
    for(i=snake.length-1;i>=1;i--){
        snake.part[i].x=snake.part[i-1].x;
        snake.part[i].y=snake.part[i-1].y;
    }
    if(snake.part[0].x==rows-1||snake.part[0].y==columns-1||snake.part[0].x==0||snake.part[0].y==0) game_over=1;
    for(i=0;i<foods.number;i++){
        if(snake.part[0].x==foods.food[i].x&&snake.part[0].y==foods.food[i].y){
            foods.food[i].x=(rand()%(rows-2))+1;
            foods.food[i].y=(rand()%(columns-2))+1;
            snake.length++;
            snake.part[snake.length-1].x=snake.part[snake.length-2].x;
            snake.part[snake.length-1].y=snake.part[snake.length-2].y;



        }
    }
}    
    
void read_keyboard(){
    char ch;
    ch=getch();
    switch(ch){
        case 'z':move_snake(-1,0);break;
        case 's':move_snake(1,0);break;
        case 'd':move_snake(0,1);break;
        case 'q':move_snake(0,-1);break;
    }

}
void setup_game(){
    snake.length=Initial_length;
    foods.number=number_of_food;
    srand(time(NULL));
    snake.part[0].x=(rand()%(rows-4))+1;
    snake.part[0].y=(rand()%(columns-4))+1;
    

    int i;
    for(i=0;i<foods.number;i++){
        foods.food[i].x=(rand()%(rows-2))+1;
        foods.food[i].y=(rand()%(columns-2))+1;
    }
}        
int main(){
    setup_game();
    while(!game_over){
        fill_board();
        draw_snake();
        draw_food();
        print_board();
        printf("Votre score est : %i \n",(snake.length-1)*10);
        read_keyboard();
    }
    



    
    
    return 0;
}