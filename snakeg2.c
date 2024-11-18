#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#define rows 20
#define columns 20
#define number_of_food 2

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
} Food;

typedef struct {
    int number;
    Food food[number_of_food];
} Foods;


char board[columns * rows];
int game_over = 0;
Snake snake;
Foods foods;


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
    while (!game_over) {
        fill_board();
        draw_snake();
        draw_food();
        print_board();    
        printf("Score: %d\n", (snake.length) * 10);
        read_keyboard();
    }
    fill_board();    
    print_board();
    printf("LOSER HAHAHAHA! Final Score: %d\n", (snake.length) * 10);
    free_snake();
    return 0;
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
        board[foods.food[i].x * columns + foods.food[i].y] = '+';
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
        if (foods.food[i].x == newX && foods.food[i].y == newY) {
            foods.food[i].x = (rand() % (rows - 2)) + 1;
            foods.food[i].y = (rand() % (columns - 2)) + 1;
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
    snake.head = NULL;
    snake.tail = NULL;
    snake.length = 0;
    snake.direction = 'R';
    add_snake_node((rand() % (rows - 6)) + 1, (rand() % (columns - 6)) + 1);
    foods.number = number_of_food;
    srand(time(NULL));
    for (int i = 0; i < foods.number; i++) {
        foods.food[i].x = (rand() % (rows - 2)) + 1;
        foods.food[i].y = (rand() % (columns - 2)) + 1;
    }
}



void free_snake() {
    while (snake.head != NULL) {
        remove_snake_tail();
    }
}

