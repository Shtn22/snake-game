#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define rows 20
#define columns 20
#define number_of_food 2
#define MAX_STACK_SIZE 1000

typedef struct {
    int Scores[MAX_STACK_SIZE];
    int nb_values;
}stack;

typedef struct SnakeNode {
    int x, y;
    struct SnakeNode* next;
} SnakeNode;

typedef struct {
    SnakeNode* head;
    SnakeNode* tail;
    char direction;
    int length;
} Snake;

typedef struct {
    int x, y;
} coordinates;

typedef struct {
    int number;
    coordinates f[number_of_food];
} Food;

char name[25];
char board[columns * rows];
int game_over = 0;
Snake snake;
Food foods;

int heighest_score(stack*s);
int previous_score(stack *s);
void file_save(stack*s,char* player_name);
int stack_is_empty(stack*s);
int stack_size(stack *s);
int stack_pop(stack*s);
stack *stack_new();
void stack_free(stack*s);
int stack_push(stack* s, int p);
char get_char_with_timeout(double timeout_in_seconds);
void fill_board();
void print_board();
void draw_snake();
void draw_food();
void setup_game();
void move_snake(int deltaX, int deltaY);
void read_keyboard();
void add_snake_node(int x, int y);
void remove_snake_tail();
void free_snake();


int main() {
    setup_game();
    stack *s=stack_new();
    while (!game_over) {
        fill_board();
        draw_snake();
        draw_food();
        print_board();    
        int score=(snake.length)*10;
        stack_push(s,score);
        file_save(s,name);
        printf("Score: %d\n", score);
        previous_score(s);
        heighest_score(s);
        read_keyboard();
    }
    fill_board();    
    print_board();
    printf("LOSER HAHAHAHA! Final Score: %d\n", (snake.length) * 10);
    printf("%d\n",previous_score(s));
    printf("%d\n",heighest_score(s));
    free_snake();
    stack_free(s);
    return 0;
}

stack* stack_new(void){
    stack* s;
    s=malloc(sizeof(stack));
    if(!s) return NULL;
    s->nb_values=0;
    return s;

}

void stack_free(stack*s){
    free(s);
}

int stack_push(stack* s, int p){
    if (stack_size(s)>=MAX_STACK_SIZE) return -1;
    s->Scores[s->nb_values++]=p;return 0;
}
int stack_pop(stack*s){
    if (!(s->nb_values)) return -1;
    return s->Scores[--(s->nb_values)]; 

}
int stack_size(stack *s){
    return s->nb_values;
}
int stack_is_empty(stack*s){
    return s->nb_values==0;
}

void file_save(stack*s,char* player_name){
FILE* file=fopen("scores.txt","a");
if (file==NULL) printf("unable to open file!\n");
for(int i=0;i<s->nb_values;i++){
    fprintf(file,"%d %s\n",s->Scores[i],player_name);
}
fclose(file);
}

int previous_score(stack *s){
    if (stack_is_empty(s)) printf("there is no previous score!\n");
    else printf("previous score: %d",s->Scores[s->nb_values-1]);
}

int heighest_score(stack*s){
    if (stack_is_empty(s)) printf("there is no heighest score yet!\n");
    else {
    int h_s = s->Scores[0];
    for (int i = 1; i < s->nb_values; i++) {
        if (s->Scores[i] > h_s) {
            h_s = s->Scores[i];
        }
    }
        printf("heighest score: %d\n",h_s);
    }   
}


void fill_board() {
    int x, y;
    for (x = 0; x < rows; x++) {
        for (y = 0; y < columns; y++) {
            if (x == 0 || y == 0 || x == rows - 1 || y == columns - 1) {
                board[x * columns + y] = '#';
            } else {
                board[x * columns + y] = ' ';
            }
        }
    }
}

void print_board() {
    system("cls");
    int x, y;
    for (x = 0; x < rows; x++) {
        for (y = 0; y < columns; y++) {
            putch(board[x * columns + y]);
        }
        putch('\n');
    }
}


void draw_snake() {
    SnakeNode* current = snake.head->next; 
    while (current != NULL) {
        board[current->x * columns + current->y] = '*';
        current = current->next;
    }
    if (snake.direction == 'U') {
        board[snake.head->x * columns + snake.head->y] = '^';
    } else if (snake.direction == 'D') {
        board[snake.head->x * columns + snake.head->y] = 'v';
    } else if (snake.direction == 'L') {
        board[snake.head->x * columns + snake.head->y] = '<';
    } else if (snake.direction == 'R') {
        board[snake.head->x * columns + snake.head->y] = '>';
    }
}


void draw_food() {
    for (int i = 0; i < foods.number; i++) {
        board[foods.f[i].x * columns + foods.f[i].y] = '+';
    }
}


void add_snake_node(int x, int y) {
    SnakeNode* new_node = (SnakeNode*)malloc(sizeof(SnakeNode));
    new_node->x = x;
    new_node->y = y;
    new_node->next = snake.head;
    snake.head = new_node;
    if (snake.tail == NULL) {
        snake.tail = new_node;
    }
}


void remove_snake_tail() {
    if (snake.tail == NULL) return;

    SnakeNode* current = snake.head;
    while (current->next != snake.tail) {
        current = current->next;
    }
    free(snake.tail);
    snake.tail = current;
    snake.tail->next = NULL;
}


void move_snake(int deltaX, int deltaY) {
    int newX = snake.head->x + deltaX;
    int newY = snake.head->y + deltaY;

    if (newX <= 0 || newX >= rows - 1 || newY <= 0 || newY >= columns - 1) {
        game_over = 1;
        return;
    }

    SnakeNode* current = snake.head->next;
    while (current != NULL) {
        if (current->x == newX && current->y == newY) {
            game_over = 1;
            return;
        }
        current = current->next;
    }
    add_snake_node(newX, newY);
    int ate_food = 0;
    for (int i = 0; i < foods.number; i++) {
        if (foods.f[i].x == newX && foods.f[i].y == newY) {
            foods.f[i].x = (rand() % (rows - 2)) + 1;
            foods.f[i].y = (rand() % (columns - 2)) + 1;
            ate_food = 1;
            break;
        }
    }
    if (!ate_food) {
        remove_snake_tail();
    } else {
        snake.length++;
    }
}


char get_char_with_timeout(double timeout_in_seconds) {
    clock_t start_time = clock();
    static int k = 0;
    if (k == 0) {
        k++;
        return _getch();
    } else {
        while ((clock() - start_time) < timeout_in_seconds * CLOCKS_PER_SEC) {
            if (_kbhit()) {
                return _getch();
            }
            Sleep(10);
        }
        return '\0';
    }
}


void read_keyboard() {
    static int deltaX = 0, deltaY = 1;
    char ch = get_char_with_timeout(0.5);
    if (ch != '\0') {
        switch (ch) {
            case 72: case 'Z': case 'z': deltaX = -1; deltaY = 0;  snake.direction = 'U'; break;
            case 80: case 'S': case 's': deltaX = 1; deltaY = 0;   snake.direction = 'D'; break;
            case 77: case 'D': case 'd': deltaX = 0; deltaY = 1;   snake.direction = 'R'; break;
            case 75: case 'Q': case 'q': deltaX = 0; deltaY = -1;  snake.direction = 'L'; break;
        }
    }
        

    move_snake(deltaX, deltaY);
}

void setup_game() {
    printf("Enter player name!\n");
    fgets(name,sizeof(name),stdin);
    name [strcspn(name,"\n")]='\0';
    snake.head = NULL;
    snake.tail = NULL;
    snake.length = 0;
    snake.direction = 'R';
    add_snake_node((rand() % (rows - 6)) + 1, (rand() % (columns - 6)) + 1);
    foods.number = number_of_food;
    srand(time(NULL));
    for (int i = 0; i < foods.number; i++) {
        foods.f[i].x = (rand() % (rows - 2)) + 1;
        foods.f[i].y = (rand() % (columns - 2)) + 1;
    }
}

void free_snake() {
    while (snake.head != NULL) {
        remove_snake_tail();
    }
}

