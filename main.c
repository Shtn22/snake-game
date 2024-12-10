#include<stdlib.h>
#include <windows.h>
#include <stdio.h>
#include<conio.h>
#include <time.h>
#define rows 20
#define columns 20
#define Initial_length 1
#define number_of_food 2

typedef struct{
    int x,y;
}Coordinates;
typedef struct{
    int number;
    Coordinates coordinates[2]; 
}Food; 
typedef struct Snake_part {
    int x,y;
    struct Snake_part *next,*prev;
    
}Snake_part;
typedef struct Snake{
    int length;
    struct  Snake_part  *head;
    struct Snake_part *tail;

}Snake;

char board[columns*rows];
int game_over=0;
int number_of_files=0;
int still_playing=1;
char H='@';
Food food;

Snake_part *create_snake_part(int x,int y);
Snake * create_snake(int x,int y);
void add_to_the_tail(Snake *snake,int x,int y);
char get_char_with_timeout(double timeout_in_seconds,Snake *snake);
void fill_board();
void print_board();
void draw_food();
void draw_snake(Snake *snake);
void move_snake(Snake *snake,int deltaX,int deltaY);
void update_saved_game(int x);
Snake* setup_game();
void save_game(Snake *snake);
Snake* setup_saved_game();
void check_for_best_score(Snake *snake);
int get_max_score();
void free_snake(Snake *snake);
void read_keyboard(Snake *snake);
int check_for_saved_game();

int main(){
    while(still_playing){
    int best_score=get_max_score();
    printf("*****************************MENU*******************************\n");
    printf("               Meilleur Score : %i               \n",best_score);
    printf("               1-Continuer la Partie                        \n");
    printf("               2-Nouvelle Partie                            \n");
    printf("               3-Quitter le jeu                              \n");
    char ch;
    ch=_getch();
    if(ch=='3'){still_playing=0;}
    else if(ch=='2'){
    Snake *snake=setup_game();
    game_over=0;
    while(!game_over){
        fill_board();
        draw_snake(snake);
        draw_food();
        print_board();
        printf("Votre score est : %i \n",(snake->length-1)*10);
        read_keyboard(snake);
    }
    draw_snake(snake);
    draw_food();
    print_board();
    printf("votre score finale est : %i\n",(snake->length-1)*10);
    check_for_best_score(snake);
    free_snake(snake);
    system("cls");}
    
    else if(ch=='1'){
        
        if(check_for_saved_game()==0){system("cls");printf("There is no saved games!!\nPlease begin a new one!!\n");}
        else {
        Snake *snake=setup_saved_game();
        game_over=0;
        while(!game_over){
            fill_board();
            draw_snake(snake);
            draw_food();
            print_board();
            printf("Votre score est : %i \n",(snake->length-1)*10);
            read_keyboard(snake);
        }
        draw_snake(snake);
        draw_food();
        print_board();
        printf("votre score finale est : %i\n",(snake->length-1)*10);
        check_for_best_score(snake);
        free_snake(snake);
        system("cls");}}
        else system("cls");}
    return 0;
    
}



Snake_part *create_snake_part(int x,int y){
    Snake_part *head=malloc(sizeof(Snake_part));
    head->x=x;
    head->y=y;
    head->prev=NULL;
    head->next=NULL;
    return head;

}
Snake * create_snake(int x,int y){
    Snake *snake=malloc(sizeof(Snake));
    snake->length=Initial_length;
    snake->head=create_snake_part(x,y);
    snake->tail=snake->head;
    return snake;

}
void add_to_the_tail(Snake *snake,int x,int y){
    Snake_part *new_part=create_snake_part(x,y);
    snake->tail->next=new_part;
    new_part->prev=snake->tail;
    snake->tail=new_part;
    snake->length++;

}
char get_char_with_timeout(double timeout_in_seconds,Snake *snake){
    clock_t start_time=clock();
    static int k=0;
    if (k==0){k++;add_to_the_tail(snake,snake->tail->x,snake->tail->y);return _getch();}
    else {
        while((clock()-start_time)<timeout_in_seconds*CLOCKS_PER_SEC){
        if(_kbhit()){
            return _getch();
        }
        Sleep(10);}
    return '\0';
}   

}
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
void draw_food(){
    int i;
    for(i=0;i<food.number;i++){
        board[food.coordinates[i].x*columns+food.coordinates[i].y]='+';

    }

}
void draw_snake(Snake *snake){
    Snake_part *current=snake->tail;
    while(current!=NULL){
        if(current==snake->head){
            board[current->x*columns+current->y]=H;
        }
        else{
            board[current->x*columns+current->y]='*';
        }
        current=current->prev;
        
    }

}
void move_snake(Snake *snake,int deltaX,int deltaY){
    int i;
    snake->head->x+=deltaX;
    snake->head->y+=deltaY;
    Snake_part *current=snake->head->next;
    while(current!=NULL){
        if(snake->head->x==current->x&&snake->head->y==current->y)game_over=1;
        current=current->next;
    }
    if(snake->head->x==rows-1|snake->head->y==columns-1||snake->head->x==0||snake->head->y==0) game_over=1;
    free(current);
    current=snake->tail;
    while(current->prev!=NULL){
        current->x=current->prev->x;
        current->y=current->prev->y;
        current=current->prev;
    }
    
    for(i=0;i<food.number;i++){
        if(snake->head->x==food.coordinates[i].x&&snake->head->y==food.coordinates[i].y){
            food.coordinates[i].x=(rand()%(rows-2))+1;
            food.coordinates[i].y=(rand()%(columns-2))+1;
            add_to_the_tail(snake,snake->tail->x,snake->tail->y);
        }
    }

}    
void update_saved_game(int x){
    FILE *file;
    file=fopen("saved_game.txt","w");
    fprintf(file,"%i",x);
    fclose(file);

}  
Snake* setup_game(){
    food.number=number_of_food;
    srand(time(NULL));
    Snake* snake=create_snake((rand()%(rows-4))+1,(rand()%(columns-4))+1);
    int i;
    for(i=0;i<food.number;i++){
        food.coordinates[i].x=(rand()%(rows-2))+1;
        food.coordinates[i].y=(rand()%(columns-2))+1;
    }
    return snake;

}        
void save_game(Snake *snake){
    FILE *file;
    file=fopen("last_saved_game.txt","w");
    if(file==NULL){
        return ;
    }
    fprintf(file,"%i %i %i %i\n",food.coordinates[0].x,food.coordinates[0].y,food.coordinates[1].x,food.coordinates[1].y);
    Snake_part *current=snake->head;
    while(current!=NULL){
        fprintf(file,"%i %i\n",current->x,current->y);
        current=current->next;
    }
    fclose(file);
    update_saved_game(1);

}
Snake* setup_saved_game(){
    food.number=number_of_food;
    FILE *file;
    file=fopen("last_saved_game.txt","r");
    int a,b,c,d;
    char line[50];
    fgets(line,sizeof(line),file);
    sscanf(line,"%i %i %i %i ",&a,&b,&c,&d);
    food.coordinates[0].x=a;food.coordinates[0].y=b;food.coordinates[1].x=c;food.coordinates[1].y=d;
    fgets(line,sizeof(line),file);
    sscanf(line,"%i %i ",&a,&b);
    Snake* snake=create_snake(a,b);
    while(fgets(line,sizeof(line),file)){
        sscanf(line,"%i %i ",&a,&b);
        add_to_the_tail(snake,a,b);}
    update_saved_game(0);
    return snake;

}
void check_for_best_score(Snake *snake){
    FILE *file;
    file=fopen("best_score.txt","r");
    char line[10];
    int new_score=(snake->length-1)*10;
    fgets(line,sizeof(line),file);
    int score;
    fclose(file);
    sscanf(line,"%i",&score);
    if (new_score>score){
        file=fopen("best_score.txt","w");
        fprintf(file,"%i",new_score);
        fclose(file);
    }
        
}
int get_max_score(){
    FILE *file;
    file=fopen("best_score.txt","r");
    char line[10];
    fgets(line,sizeof(line),file);
    int best_score;
    sscanf(line,"%i",&best_score);
    return best_score;

} 
void free_snake(Snake *snake){
    Snake_part *current=snake->tail;
    while(current!=NULL){
        Snake_part *prev=current->prev;
        free(current);
        current=prev;
    }
    free(snake);

}
void read_keyboard(Snake *snake){
    static int deltaX=0,deltaY=0;
    char ch;
    static char prev=' ';
    ch=get_char_with_timeout(0.2,snake);
    if (ch!='\0'&&ch!=prev){
        switch(ch){
        
        case 'a':save_game(snake);game_over=1;break;
        case 'z':deltaX=-1;deltaY=0;H='^';break;
        case 's':deltaX=1;deltaY=0;H='v';break;
        case 'd':deltaX=0;deltaY=1;H='>';break;
        case 'q':deltaX=0;deltaY=-1;H='<';break;

    }
    }
    prev=ch;
    move_snake(snake,deltaX,deltaY);

}
int check_for_saved_game(){
    FILE *file;
    file=fopen("saved_game.txt","r");
    char line[10];
    fgets(line,sizeof(line),file);
    fclose(file);
    int saved_game;
    sscanf(line,"%i",&saved_game);
    return saved_game;
    
}