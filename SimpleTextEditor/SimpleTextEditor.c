#include <stdio.h>
#include <string.h>



int main()
{
    int command;
    scanf(" %d", &command);

    while (command != 0)
    {
        switch (command)
        {
        case 1:
            printf("Command 1 is not implemented yet");
            break;

        case 2:
            printf("Command 2 is not implemented yet");
            break;

        case 3:
            printf("Command 3 is not implemented yet");
            break;

        case 4:
            printf("Command 4 is not implemented yet");
            break;

        case 5:
            printf("Command 5 is not implemented yet");
            break;

        case 6:
            printf("Command 6 is not implemented yet");
            break;

        case 7:
            printf("Command 7 is not implemented yet");
            break;

        default:
            printf("This command does not exist");
            scanf(" %d", &command);
            break;
        }

        if (scanf("%d", &command) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Викидаємо літеру 'а' з буфера, щоб не зациклювалось
        }
        else
        {
            while (getchar() != '\n'); // Викидаємо Enter (\n) після успішного числа
        }
    }
}