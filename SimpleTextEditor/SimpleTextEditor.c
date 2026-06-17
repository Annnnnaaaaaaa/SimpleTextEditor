#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

struct Node {
    char* text;        // посилання на першу комірку тексту
    int length;        // індекс останнього символа 
    int capacity;      // максимальна кількість символів
    struct Node* next_pointer; // Вказівник на наступну ноду
};

#define MAX_HISTORY 10 // те саме, що const int MAX_HISTORY = 10; тільки працює  // глибина undo/redo

// Один знімок стану — масив рядків (по одному на кожен Node)
struct Snapshot {
    char** lines;   // масив рядків
    int count;      // кількість рядків
};

// Стек для undo і redo
struct Snapshot undo_stack[MAX_HISTORY];
int undo_top = -1; // індекс ноди (-1 = порожній)

struct Snapshot redo_stack[MAX_HISTORY];
int redo_top = -1; // індекс ноди (-1 = порожній)





// Функція підбирає найменше capacity (через степінь двійки)
int calculate_capacity(int target_length) {
    int capacity = 1;
    while (capacity <= target_length){
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
        printf("DEBUG: realloc з %d до %d\n", (*node_pointer).capacity, new_capacity);


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
        (*node_pointer).text + index + insert_len, // Distination 
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

struct Node* get_last_node(struct Node* head) {
    if (head == NULL) {
        return NULL;
    }

    struct Node* current_node_pointer = head;
    while ( (*current_node_pointer).next_pointer != NULL ) {
        current_node_pointer = (*current_node_pointer).next_pointer;
    }

    return current_node_pointer;
}

void print_text(struct Node* head) {
    if (head == NULL) {
        printf("Немає тексту для друку.");
        return;
    }

    struct Node* current_node_pointer = head;
    while (current_node_pointer != NULL) {
        printf( "%s\n", (*current_node_pointer).text );
        current_node_pointer = (*current_node_pointer).next_pointer;
    }
}

void file_write_implementation(struct Node* head) {
    char file_name[256];
    printf("Enter the file name for saving: ");

    if (scanf("%s", file_name) != 1) {
        printf("Invalid input! Please enter a file name.\n");
        while (getchar() != '\n');
        return;
    }
    else {
        while (getchar() != '\n');
    }

    struct Node* current_node_pointer = head;

    FILE* file;
    file = fopen(file_name, "w");

    if (file != NULL) {
        while (current_node_pointer != NULL) {
            fputs((*current_node_pointer).text, file);
            fputs("\n", file);
            current_node_pointer = (*current_node_pointer).next_pointer;
        }

        fclose(file);
        printf("Text has been saved successfully\n");
    }
    else {
        printf("Error: File can not be opened!\n");
    }
}

void clean_list(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        struct Node* next = (*current).next_pointer;
        free( (*current).text ); 
        free(current);     
        current = next;
    }
}

struct Node* file_read_implementation(struct Node* head) {
    char file_name[256];
    printf("Enter file name for loading: ");

    if (scanf("%s", file_name) != 1) {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return head; // повертаємо старий head
    }
    while (getchar() != '\n');

    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file or file does not exist!\n");
        return head; // повертаємо старий head
    }

    if (head != NULL) {
        clean_list(head);
        head = NULL;
    }

    char buffer[512]; // буфер для читання рядка
    struct Node* tail_pointer = NULL;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {

        int len = strcspn(buffer, "\n");
        buffer[len] = '\0';

        struct Node* new_node = create_node(buffer);

        if (head == NULL) {
            head = new_node;       
            tail_pointer = head;
        }
        else {
            (*tail_pointer).next_pointer = new_node; 
            tail_pointer = new_node;               
        }
    }

    fclose(file);
    printf("Text has been loaded successfully from '%s'\n", file_name);
    return head;
}

void search_text(struct Node* head) {
    if (head == NULL) {
        printf("Немає тексту, в якому шукати.\n");
        return;
    }

    char search_buffer[512]; //
    printf("Enter text to search: ");

    fgets(search_buffer, sizeof(search_buffer), stdin); // просимо інпут
    int len = strcspn(search_buffer, "\n");
    search_buffer[len] = '\0'; // прибираємо \n з кінця інпуту

    if (strlen(search_buffer) == 0) {
        printf("Помилка. Пустий пошуковий запит.\n");
        return;
    }

    struct Node* current = head;
    int line_index = 0;       
    bool found_any = false;

    while (current != NULL) {
        char* text = (*current).text;
        int text_len = (*current).length;
        int search_len = strlen(search_buffer); // вводимо спеціальну змінну, щоб не писати декілька раз strlen(search_buffer)

        for (int i = 0; i <= text_len - search_len; i++) { // рух по тексту
            bool match = true;

            for (int j = 0; j < search_len; j++) { // рух по шуканому слову
                if (text[i + j] != search_buffer[j]) {
                    match = false; 
                    break;         
                }
            }

            if (match) {
                if (!found_any) {
                    printf("Text is present in this position: ");
                    found_any = true;
                }

                printf("%d %d, ", line_index, i); // прибрати кому
                i += search_len - 1;
            }
        }

        current = (*current).next_pointer;
        line_index++;
    }

    if (found_any) {
        printf("\n");
    }
    else {
        printf("Текст не знайдено.\n");
    }
}

void insert_with_replacement(struct Node* head, int line_index, int char_index, char* new_text) {
    struct Node* cur = head;
    int cur_line = 0;
    while (cur != NULL && cur_line < line_index) {
        cur = (*cur).next_pointer;
        cur_line++;
    }
    if (cur == NULL) {
        printf("Error: Line out of range.\n");
        return;
    }
    if (char_index < 0 || char_index > (*cur).length) {
        printf("Error: Char index out of range.\n");
        return;
    }

    int new_len = strlen(new_text);
    int overwrite_count = (*cur).length - char_index; // скільки символів переписуємо
    if (overwrite_count > new_len) overwrite_count = new_len;

    int extra = new_len - overwrite_count; // додаткові символи після overwrite зони, якщо треба збільшити місце
    if (extra > 0) {
        ensure_capacity(cur, extra);
        // зсуваємо хвіст вправо (створюємо дирку)
        memmove((*cur).text + char_index + new_len,
            (*cur).text + char_index + overwrite_count,
            (*cur).length - char_index - overwrite_count + 1);
        (*cur).length += extra;
    }

    // копіюємо новий текст в дирку
    memcpy((*cur).text + char_index, new_text, new_len);
}

struct Snapshot make_snapshot(struct Node* head) {
    struct Snapshot snap;
    snap.count = 0;
    snap.lines = NULL;

    struct Node* cur = head;
    while (cur != NULL) { // рахуємо кількість рядків
        snap.count++;
        cur = (*cur).next_pointer;
    }

    if (snap.count == 0) return snap;

    snap.lines = (char**)malloc(snap.count * sizeof(char*));
    cur = head;
    for (int i = 0; i < snap.count; i++) {
        snap.lines[i] = (char*)malloc((*cur).length + 1);
        strcpy(snap.lines[i], (*cur).text);
        cur = (*cur).next_pointer;
    }
    return snap;
}

void free_snapshot(struct Snapshot* snap) {
    for (int i = 0; i < (*snap).count; i++) {
        free((*snap).lines[i]);
    }
    if ((*snap).lines != NULL) free((*snap).lines);
    (*snap).lines = NULL;
    (*snap).count = 0;
}

struct Node* restore_snapshot(struct Snapshot snap) {
    struct Node* new_head = NULL;
    struct Node* tail = NULL;
    for (int i = 0; i < snap.count; i++) {
        struct Node* n = create_node(snap.lines[i]);
        if (new_head == NULL) {
            new_head = n;
            tail = n;
        }
        else {
            (*tail).next_pointer = n;
            tail = n;
        }
    }
    return new_head;
}

// записуємо поточний стан у undo перед будь-якою зміною (1, 2, 6, 14)
void push_undo(struct Node* head) {
    if (undo_top == MAX_HISTORY - 1) { // видаляємо найстаріший запис, якщо стек переповнено
        free_snapshot(&undo_stack[0]);
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            undo_stack[i] = undo_stack[i + 1];
        }
        undo_top--;
    }
    undo_top++;
    undo_stack[undo_top] = make_snapshot(head);

    // очищуємо redo після нової дії
    for (int i = 0; i <= redo_top; i++) {
        free_snapshot(&redo_stack[i]);
    }
    redo_top = -1;
}

struct Node* undo_command(struct Node* head) {
    if (undo_top < 0) {
        printf("Undo: нічого скасовувати.\n");
        return head;
    }
    if (redo_top == MAX_HISTORY - 1) {
        free_snapshot(&redo_stack[0]);
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            redo_stack[i] = redo_stack[i + 1]; // зберігаємо поточний стан у redo
        }
        redo_top--;
    }
    redo_top++;
    redo_stack[redo_top] = make_snapshot(head);

    // відновлюємо попередній стан
    struct Snapshot snap = undo_stack[undo_top];
    struct Node* new_head = restore_snapshot(snap);
    free_snapshot(&undo_stack[undo_top]);
    undo_top--;

    clean_list(head);
    printf("Undo виконано.\n");
    return new_head;
}

struct Node* redo_command(struct Node* head) {
    if (redo_top < 0) {
        printf("Redo: нічого повертати.\n");
        return head;
    }
    if (undo_top == MAX_HISTORY - 1) {
        free_snapshot(&undo_stack[0]);
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            undo_stack[i] = undo_stack[i + 1]; // Зберігаємо поточний стан у undo
        }
        undo_top--;
    }
    undo_top++;
    undo_stack[undo_top] = make_snapshot(head);

    // повертаємо текст до стану який був після скасованої дії
    struct Snapshot snap = redo_stack[redo_top];
    struct Node* new_head = restore_snapshot(snap);
    free_snapshot(&redo_stack[redo_top]);
    redo_top--;

    clean_list(head);
    printf("Redo виконано.\n");
    return new_head;
}

// видаляє num_chars символів у рядку line_index починаючи з char_index
void delete_text(struct Node* head, int line_index, int char_index, int num_chars) {
    struct Node* cur = head;
    int cur_line = 0;
    while (cur != NULL && cur_line < line_index) {
        cur = (*cur).next_pointer;
        cur_line++;
    }
    if (cur == NULL) {
        printf("Error: Line out of range.\n");
        return;
    }
    if (char_index < 0 || char_index > (*cur).length) {
        printf("Error: Char index out of range.\n");
        return;
    }
    // обрізаємо num_chars щоб не вийти за межі рядка
    if (char_index + num_chars > (*cur).length) {
        num_chars = (*cur).length - char_index;
    }
    // зсуваємо решту рядка вліво
    memmove((*cur).text + char_index,
        (*cur).text + char_index + num_chars,
        (*cur).length - char_index - num_chars + 1);
    (*cur).length -= num_chars;
}



int main()
{
    SetConsoleCP(1251); // кодування, щоб нормально відображалась кирилиця
    SetConsoleOutputCP(1251);
    
    struct Node* head = NULL; // Наш текст на початку порожній    

    int command;

    printf("0 - Exit\n");
    printf("1 - Append text symbols to the end\n");
    printf("2 - Start the new line\n");
    printf("3 - Use files to load the information\n");
    printf("4 - Use files to save the information\n");
    printf("5 - Print the current text to console\n");
    printf("6 - Insert the text by line and symbol index\n");
    printf("7 - Search (please note that the text can be found more than once)\n");
    printf("8  - Delete (line, index, num_chars)\n");
    printf("9  - Undo\n");
    printf("10 - Redo\n");
    printf("11 - Cut (line, index, num_chars)\n");
    printf("12 - Paste (line, index)\n");
    printf("13 - Copy (line, index, num_chars)\n");
    printf("14 - Insert with replacement (line, index, text)\n\n");

    while (1)
    {
        command = -1;
        printf("\n> ");
        

        // Перевірка input
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
        

        // парсинг
        switch (command)
        {
        case 0:
            return 0; // exit


        case 1: // Append text symbols to the end
        {
            char input_buffer[512];
            printf("Enter text to append: ");
            fgets(
                input_buffer,           // save to
                sizeof(input_buffer),   // of size
                stdin                   // from (standart input)
            );

            int input_len = strcspn(input_buffer, "\n");
            input_buffer[input_len] = '\0';

            push_undo(head); // щоб зберегти стан до змін

            if (head == NULL) {
                head = create_node(input_buffer);
            }
            else {

                struct Node* tail_pointer = get_last_node(head);
                append_text_to_node(tail_pointer, input_buffer);
            }
            break;
        }

        case 2: // Start the new line
        {
            push_undo(head);

            if (head == NULL) {
                head = create_node("");
            }
            else {
                struct Node* tail_pointer = get_last_node(head);
                (*tail_pointer).next_pointer = create_node("");
            }
            printf("New line is started\n");
            break;
        }

        case 3: // Use files to load the information
            head = file_read_implementation(head);
            break;

        case 4: // Use files to save the information 
            file_write_implementation(head);
            break;

        case 5: // Print the current text to console 
            print_text(head);
            break;

        case 6: // Insert the text by line and symbol index
        {
            int line_index;
            int char_index;
            char input_buffer[512];

            printf("Choose line and index: "); // start from 0 0
            
            if (scanf("%d %d", &line_index, &char_index) != 2) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                break;
            }

            while (getchar() != '\n');

            printf("Enter text to insert: ");

            fgets(
                input_buffer,           // save to
                sizeof(input_buffer),   // of size
                stdin                   // from (standart input)
            );

            int input_len = strcspn(input_buffer, "\n");
            input_buffer[input_len] = '\0';

            struct Node* current_node_pointer = head; // доходимо до потрібного рядку
            int current_line = 0; 

            while (current_node_pointer != NULL && current_line < line_index) {
                current_node_pointer = (*current_node_pointer).next_pointer;
                current_line++;
            }

            if (current_node_pointer == NULL) {
                printf("Error: Line out of range.\n");
            }
            else {
                push_undo(head);

                insert_text_at(current_node_pointer, char_index, input_buffer);
            }
            break;
        }

        case 7: // Search 
            search_text(head);
            break;

        case 8: // Delete
        {
            int line_index, char_index, num_chars;
            printf("Choose line, index and number of symbols: ");
            if (scanf("%d %d %d", &line_index, &char_index, &num_chars) != 3) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                break;
            }
            while (getchar() != '\n');

            push_undo(head);
            delete_text(head, line_index, char_index, num_chars);
            break;
            break;
        }

        case 9: // Undo
            head = undo_command(head);
            break;

        case 10: // Redo
            head = redo_command(head);
            break;

        case 11: // Cut
            break;

        case 12: // Paste
            break;

        case 13: // Copy
            break;

        case 14: // Insert with replacement
        {
            int line_index, char_index;
            char input_buffer[512];
            printf("Choose line and index: ");
            if (scanf("%d %d", &line_index, &char_index) != 2) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                break;
            }
            while (getchar() != '\n');
            printf("Write text: ");
            fgets(input_buffer, sizeof(input_buffer), stdin);
            int input_len = strcspn(input_buffer, "\n");
            input_buffer[input_len] = '\0';

            push_undo(head);

            insert_with_replacement(head, line_index, char_index, input_buffer);
            break;
        }

        default:
            printf("This command does not exist\n");
            break;
        }
    }
}