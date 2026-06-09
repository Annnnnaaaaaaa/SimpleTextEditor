#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main()
{
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
        case 1:
            printf("Command 1 is not implemented yet\n");
            break;

        case 2:
            printf("Command 2 is not implemented yet\n");
            break;

        case 3:
            printf("Command 3 is not implemented yet\n");
            break;

        case 4:
            printf("Command 4 is not implemented yet\n");
            break;

        case 5:
            printf("Command 5 is not implemented yet\n");
            break;

        case 6:
            printf("Command 6 is not implemented yet\n");
            break;

        case 7:
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