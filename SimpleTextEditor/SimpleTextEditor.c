#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char* text;        // посилання на першу комірку тексту
    int length;        // індекс останнього символа 
    int capacity;      // максимальна кількість символів
    struct Node* next_pointer; // Вказівник на наступну ноду
};

// Функція підбирає найменше capacity (через найбільший степінь двійки)
int calculate_capacity(int target_length) {
    int capacity = 1;
    while (capacity < target_length) {
        capacity *= 2;
    }
    return capacity;
}


struct Node* create_node(char* initial_text) {
    struct Node* current_node_pointer = (struct Node*)malloc(sizeof(struct Node));

    (*current_node_pointer).length = strlen(initial_text);
    (*current_node_pointer).capacity = calculate_capacity( (*current_node_pointer).length );

    (*current_node_pointer).text = (char*)malloc( (*current_node_pointer).capacity );  
    

    strcpy( (*current_node_pointer).text, initial_text ); // strcpy безпечний, бо ми виділили достатньо місця
    (*current_node_pointer).next_pointer = NULL;

    return current_node_pointer;
}


void ensure_capacity(struct Node* node_pointer, int additional_length) {
    int required_length = (*node_pointer).length + additional_length;

    if (required_length >= (*node_pointer).capacity) {
        int new_capacity = calculate_capacity(required_length); 

        (*node_pointer).text = (char*)realloc( (*node_pointer).text, new_capacity ); // з полички на полички

        if ( (*node_pointer).text == NULL ) {
            printf("Критична помилка: Не вдалося перерозподілити пам'ять. Text = null\n");
            exit(1); // exeption
        }

        (*node_pointer).capacity = new_capacity;
    }
}

void append_text_to_node(struct Node* node_pointer, char* new_text) { //char*??? new_text - посиланняна перший елемент масиву char
    int new_len = strlen(new_text);
    ensure_capacity(node_pointer, new_len);

    strcat( (*node_pointer).text, new_text ); // concatenation
    (*node_pointer).length = (*node_pointer).length + new_len;
}

void insert_text_at(struct Node* node_pointer, int index, char* insert_text) {
    if (index > (*node_pointer).length || index < 0) {
        printf("Error: Out-of-range insert index\n");
        return;
    }

    int insert_len = strlen(insert_text);
    ensure_capacity(node_pointer, insert_len);

    // робимо дирку
    memmove(
        (*node_pointer).text + index + insert_len, // Distanation 
        (*node_pointer).text + index,              // Source
        (*node_pointer).length - index + 1         // Size
    );

    // вставляємо у дирку
    memcpy(
        (*node_pointer).text + index, // Distanation 
        insert_text,                  // Source
        insert_len                    // Size
    );
    
    (*node_pointer).length = (*node_pointer).length + insert_len;
}


//void save_to_file(struct Node* head, const char* filename) {
//    FILE* file = fopen(filename, "w");
//    if (!file) {
//        printf("Error: Не вдалося відкрити файл для запису.\n");
//        return;
//    }
//
//    struct Node* current = head;
//    while (current != NULL) {
//        fprintf(file, "%s\n", current->text);
//        current = current->next;
//    }
//    fclose(file);
//    printf("Text has been saved successfully\n");
//}

//struct Node* load_from_file(const char* filename) {
//    FILE* file = fopen(filename, "r");
//    if (!file) {
//        // Якщо файл відсутній, залишаємо поточний буфер без змін 
//        printf("Error: File not found.\n");
//        return NULL;
//    }
//
//    struct Node* head = NULL;
//    struct Node* tail = NULL;
//    char buffer[1024]; // Тимчасовий буфер для читання рядків
//
//    while (fgets(buffer, sizeof(buffer), file) != NULL) {
//        // Видаляємо символ переносу рядка, якщо він є
//        buffer[strcspn(buffer, "\n")] = '\0';
//
//        struct Node* new_node = create_node(buffer);
//        if (head == NULL) {
//            head = new_node;
//            tail = new_node;
//        }
//        else {
//            tail->next = new_node;
//            tail = new_node;
//        }
//    }
//
//    fclose(file);
//    printf("Text has been loaded successfully\n");
//    return head; // Повертаємо нову "голову" списку
//}




int main()
{
    struct Node* head = NULL; // Наш текст на початку порожній    

    int command;

    printf("0 - Exit\n");
    printf("1 - Append text symbols to the end\n");
    printf("2 - Start the new line\n");
    printf("3 - Use files to load the information\n");
    printf("4 - Use files to save the information\n");
    printf("5 - Print the current text to console\n");
    printf("6 - Insert the text by line and symbol index\n");
    printf("7 - Search (please note that the text can be found more than once)\n\n");
    while (1)
    {
        command = -1;
        printf("\n> ");
        

        // Перша безпечна перевірка вводу на старті програми
        if (scanf("%d", &command) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        else
        {
            while (getchar() != '\n');
            // додати перевірку на 1.фві21
        }
        

        switch (command)
        {
        case 0:
            return 0; // exit

        case 1: // Append text symbols to the end
            printf("Command 1 is not implemented yet\n");
            //appendText(&head);       
            break;

        case 2: // Start the new line 
            printf("Command 2 is not implemented yet\n");
            break;

        case 3: // Use files to load/save the information 
            printf("Command 3 is not implemented yet\n");
            break;

        case 4: // Print the current text to console 
            printf("Command 4 is not implemented yet\n");
            //printText(head);
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
    }
}