#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600
#define GRID_SIZE 20

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[100];
    int length;
    Point direction;
} Snake;

typedef struct {
    Point position;
} Food;

Snake snake;
Food food;
bool gameOver = false;

void init() {
    snake.length = 5;
    snake.direction.x = 1;
    snake.direction.y = 0;
    for (int i = 0; i < snake.length; ++i) {
        snake.body[i].x = GRID_SIZE - i - 1;
        snake.body[i].y = GRID_SIZE / 2;
    }
    srand(time(0));
    food.position.x = rand() % (WIDTH / GRID_SIZE);
    food.position.y = rand() % (HEIGHT / GRID_SIZE);
}

void drawRectangle(int x, int y, int size) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size, y + size);
    glVertex2f(x, y + size);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Desenha a cobra
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < snake.length; ++i) {
        drawRectangle(snake.body[i].x * GRID_SIZE, snake.body[i].y * GRID_SIZE, GRID_SIZE);
    }

    // Desenha a comida
    glColor3f(1.0, 0.0, 0.0);
    drawRectangle(food.position.x * GRID_SIZE, food.position.y * GRID_SIZE, GRID_SIZE);

    glFlush();
}

void updateSnake() {
    for (int i = snake.length - 1; i > 0; --i) {
        snake.body[i] = snake.body[i - 1];
    }
    snake.body[0].x += snake.direction.x;
    snake.body[0].y += snake.direction.y;

    // Verifica se a cobra comeu a comida
    if (snake.body[0].x == food.position.x && snake.body[0].y == food.position.y) {
        snake.length++;
        food.position.x = rand() % (WIDTH / GRID_SIZE);
        food.position.y = rand() % (HEIGHT / GRID_SIZE);
    }

    // Verifica colisão com paredes
    if (snake.body[0].x < 0 || snake.body[0].x >= WIDTH / GRID_SIZE ||
        snake.body[0].y < 0 || snake.body[0].y >= HEIGHT / GRID_SIZE) {
        gameOver = true;
    }

    // Verifica colisão com o próprio corpo
    for (int i = 1; i < snake.length; ++i) {
        if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
            gameOver = true;
            break;
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_UP:
                if (snake.direction.y != -1) {
                    snake.direction.x = 0;
                    snake.direction.y = 1;
                }
                break;
            case GLFW_KEY_DOWN:
                if (snake.direction.y != 1) {
                    snake.direction.x = 0;
                    snake.direction.y = -1;
                }
                break;
            case GLFW_KEY_LEFT:
                if (snake.direction.x != 1) {
                    snake.direction.x = -1;
                    snake.direction.y = 0;
                }
                break;
            case GLFW_KEY_RIGHT:
                if (snake.direction.x != -1) {
                    snake.direction.x = 1;
                    snake.direction.y = 0;
                }
                break;
        }
    }
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Snake Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glfwSetKeyCallback(window, keyCallback);

    init();

    while (!glfwWindowShouldClose(window) && !gameOver) {
        display();
        updateSnake();
        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(0.1); // Atualiza a cada 100ms
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}