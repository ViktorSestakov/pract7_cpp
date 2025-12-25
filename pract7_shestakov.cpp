#include <iostream>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;
const int INITIAL_SPEED_MS = 150;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

struct Segment {
    int x, y;
    Segment* next;

    Segment(int _x, int _y) : x(_x), y(_y), next(nullptr) {}
};

Segment* head = nullptr;
Segment* tail = nullptr;
int length = 1;
int foodX, foodY;
Direction dir;
bool gameOver;
int score = 0;

void Setup() {
    gameOver = false;
    dir = STOP;
    score = 0;

    head = new Segment(WIDTH / 2, HEIGHT / 2);
    tail = head;

    length = 1;

    srand(time(NULL));

    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;
}

void Draw() {
    system("cls");

    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH + 2; j++) {
            if (j == 0 || j == WIDTH + 1) {
                cout << "#";
            }
            else {
                bool isSnakePart = false;
                Segment* current = head;

                while (current != nullptr) {
                    if (current->x == j - 1 && current->y == i) {
                        if (current == head) {
                            cout << "O";
                        }
                        else {
                            cout << "o";
                        }
                        isSnakePart = true;
                        break;
                    }
                    current = current->next;
                }

                if (!isSnakePart) {
                    if (j - 1 == foodX && i == foodY) {
                        cout << "*";
                    }
                    else {
                        cout << " ";
                    }
                }
            }
        }
        cout << endl;
    }

    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        char key = _getch();

        switch (key) {
        case 'a':
        case 75:
            if (dir != RIGHT) dir = LEFT;
            break;
        case 'd':
        case 77:
            if (dir != LEFT) dir = RIGHT;
            break;
        case 'w':
        case 72:
            if (dir != DOWN) dir = UP;
            break;
        case 's':
        case 80:
            if (dir != UP) dir = DOWN;
            break;
        }
    }
}

void Logic() {
    if (dir == STOP) return;

    int newHeadX = head->x;
    int newHeadY = head->y;

    switch (dir) {
    case LEFT:  newHeadX--; break;
    case RIGHT: newHeadX++; break;
    case UP:    newHeadY--; break;
    case DOWN:  newHeadY++; break;
    }

    if (newHeadX < 0 || newHeadX >= WIDTH || newHeadY < 0 || newHeadY >= HEIGHT) {
        gameOver = true;
        return;
    }

    Segment* current = head->next;
    while (current != nullptr) {
        if (current->x == newHeadX && current->y == newHeadY) {
            gameOver = true;
            return;
        }
        current = current->next;
    }

    Segment* newHead = new Segment(newHeadX, newHeadY);

    newHead->next = head;
    head = newHead;

    if (newHeadX == foodX && newHeadY == foodY) {
        score += 10;
        length++;

        bool placed = false;
        while (!placed) {
            foodX = rand() % (WIDTH - 2) + 1;
            foodY = rand() % (HEIGHT - 2) + 1;

            placed = true;
            current = head;
            while (current != nullptr) {
                if (current->x == foodX && current->y == foodY) {
                    placed = false;
                    break;
                }
                current = current->next;
            }
        }

    }
    else {
        Segment* currentTail = head;
        while (currentTail->next != tail) {
            currentTail = currentTail->next;
        }

        delete tail;
        tail = currentTail;
        tail->next = nullptr;
    }
}

void Clean() {
    Segment* current = head;
    Segment* next;
    while (current != nullptr) {
        next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
}


int main() {
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();

        Sleep(INITIAL_SPEED_MS - (length * 5));
        if (INITIAL_SPEED_MS - (length * 5) < 50) {
            Sleep(10);
        }
    }

    system("cls");
    cout << "GAME OVER!" << endl;
    cout << "Score: " << score << endl;
    cout << "Press any key to exit...";
    _getch();

    Clean();
    return 0;
}