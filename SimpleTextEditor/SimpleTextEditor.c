#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

struct Node {
    char text[256];     // Данні: масив для зберігання одного рядка
    struct Node* next;  // Вказівник на наступний рядок (вузол)
};

void appendText(struct Node** head) {
    char buffer[256];
    printf("Enter text to append: ");

    // 1. Зчитуємо текст від користувача та прибираємо символ переносу рядка
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    // 2. Виділяємо пам'ять під новий вагон і заповнюємо його даними
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strncpy(newNode->text, buffer, sizeof(newNode->text) - 1);
    newNode->text[sizeof(newNode->text) - 1] = '\0';
    newNode->next = NULL; // За ним поки нічого немає

    // 3. Чіпляємо новий вагон до потяга
    if (*head == NULL) {
        // Якщо потяг порожній, новий вагон стає першим (головою)
        *head = newNode;
    }
    else {
        // Якщо вагони вже є, шукаємо останній
        struct Node* current = *head;
        while (current->next != NULL) {
            current = current->next; // Переходимо до наступного вагона
        }
        // Цикл завершився, current — це останній вагон. 
        // Приєднуємо наш новий вагон одразу за ним:
        current->next = newNode;
    }
}

void printText(struct Node* head) {
    struct Node* current = head; // Стартуємо з самого першого вагона

    while (current != NULL) {
        printf("%s\n", current->text); // Друкуємо рядок
        current = current->next;       // Переходимо до наступного вагона
    }
}


int main()
{
    struct Node* head = NULL; // Наш текст на початку порожній

    int command;

    // Перша безпечна перевірка вводу на старті програми
    if (scanf("%d", &command) != 1)
    {
        printf("Invalid input! Please enter a number.\n");
        while (getchar() != '\n');
        command = -1; // Присвоюємо значення, щоб увійти в цикл і попросити ввід знову
    }
    else
    {
        while (getchar() != '\n');
    }

    while (command != 0)
    {
        switch (command)
        {
        case 1: // Append text symbols to the end
            appendText(&head);            
            break;

        case 2: // Start the new line 
            printf("Command 2 is not implemented yet\n");
            break;

        case 3: // Use files to load/save the information 
            printf("Command 3 is not implemented yet\n");
            break;

        case 4: // Print the current text to console 
            printText(head);
            break;

        case 5: // Insert the text by line and symbol index 
            printf("Command 5 is not implemented yet\n");
            break;

        case 6: //  Search (please note that the text can be found more than once) 
            printf("Command 6 is not implemented yet\n");
            break;

        case 7: // (Optional) Clearing the console
            printf("Command 7 is not implemented yet\n");
            break;

        default:
            printf("This command does not exist\n");
            break;
        }

        // Безпечний ввід у кінці циклу — захищає від літер та очищає буфер
        if (scanf("%d", &command) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
        }
        else
        {
            while (getchar() != '\n');
        }
    }

    return 0;
}