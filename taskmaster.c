
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 100
#define MAX_TASK_LEN 100
#define MAX_QUOTE_LEN 200
#define TASK_FILE "tasks.txt"

typedef struct {
    char description[MAX_TASK_LEN];
    int completed;
} Task;

Task tasks[MAX_TASKS];
int task_count = 0;

const char *quotes[] = {
    "Stay focused and never give up!",
    "You’re one step closer to your goals!",
    "Keep pushing, you’ve got this!",
    "Small progress is still progress!",
    "Turn your dreams into plans!"
};
const int quote_count = 5;

void load_tasks() {
    FILE *file = fopen(TASK_FILE, "r");
    if (file == NULL) return;

    task_count = 0;
    while (task_count < MAX_TASKS && fgets(tasks[task_count].description, MAX_TASK_LEN, file)) {
        tasks[task_count].description[strcspn(tasks[task_count].description, "\n")] = 0;
        fscanf(file, "%d\n", &tasks[task_count].completed);
        task_count++;
    }
    fclose(file);
}

void save_tasks() {
    FILE *file = fopen(TASK_FILE, "w");
    if (file == NULL) {
        printf("Error saving tasks!\n");
        return;
    }

    for (int i = 0; i < task_count; i++) {
        fprintf(file, "%s\n%d\n", tasks[i].description, tasks[i].completed);
    }
    fclose(file);
}

void add_task(char *description) {
    if (task_count >= MAX_TASKS) {
        printf("Task list is full!\n");
        return;
    }
    strncpy(tasks[task_count].description, description, MAX_TASK_LEN - 1);
    tasks[task_count].description[MAX_TASK_LEN - 1] = 0;
    tasks[task_count].completed = 0;
    task_count++;
    save_tasks();
    printf("Task added: %s\n", description);
}

void list_tasks() {
    if (task_count == 0) {
        printf("No tasks yet! Add some to get started.\n");
        return;
    }
    printf("\nYour Tasks:\n");
    for (int i = 0; i < task_count; i++) {
        printf("%d. [%c] %s\n", i + 1, tasks[i].completed ? 'X' : ' ', tasks[i].description);
    }
}

void complete_task(int index) {
    if (index < 1 || index > task_count) {
        printf("Invalid task number!\n");
        return;
    }
    tasks[index - 1].completed = 1;
    save_tasks();
    printf("Task %d marked as completed!\n", index);
}

void pomodoro_timer(int minutes) {
    printf("\nStarting %d-minute Pomodoro session...\n", minutes);
    printf("Focus! Here's a motivational quote: %s\n", quotes[rand() % quote_count]);

    time_t start = time(NULL);
    int seconds = minutes * 60;

    while (seconds > 0) {
        printf("\rTime left: %02d:%02d", seconds / 60, seconds % 60);
        fflush(stdout);
        sleep(1);
        seconds--;
    }

    printf("\nPomodoro complete! Take a 5-minute break.\a\n");
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    srand(time(NULL));
    load_tasks();
    char input[256];
    char command;
    char description[MAX_TASK_LEN];
    int task_index;

    while (1) {
        clear_screen();
        printf("=== TaskMaster ===\n");
        printf("1. List tasks\n");
        printf("2. Add task\n");
        printf("3. Complete task\n");
        printf("4. Start Pomodoro (25 min)\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");

        fgets(input, sizeof(input), stdin);
        command = input[0];

        switch (command) {
            case '1':
                list_tasks();
                break;
            case '2':
                printf("Enter task description: ");
                fgets(description, MAX_TASK_LEN, stdin);
                description[strcspn(description, "\n")] = 0;
                add_task(description);
                break;
            case '3':
                list_tasks();
                printf("Enter task number to complete: ");
                scanf("%d", &task_index);
                getchar(); // Clear newline
                complete_task(task_index);
                break;
            case '4':
                pomodoro_timer(25);
                break;
            case '5':
                printf("Goodbye! Stay productive!\n");
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}
