#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// pipe[0] -> Leitura
// pipe[1] -> Escrita


int attempts = 0;

main()
{
    loginPipes();

    return 0;
}

void *timerThread(void *arg) {
    sleep(10);  // Tempo de espera: 10 segundos
    printf("\nTime ended. Try again.\n");
    system("clear");

    attempts = 0;

    loginPipes();
}

void *passwordAttemptsThread(int *args){
    if(attempts < 5){
        sleep(1); //Aguarda 1 segundo

        if(attempts == 5){
            system("clear");
            printf("Maximum number of attempts reached!!\nClosing program...\n");

            attempts = 0;
            exit(0);
        }
    }

    return NULL;
}

void loginPipes(){
    int descriptor,
            pipe1[2];

        if (pipe(pipe1) < 0)
        {
            printf("Error PIPE");
            exit(0);
        }

        // Criando o processo filho
        // id = fork()
        // if(id > 0 ) -> processo pai
        // if(id = 0 ) -> processo filho
        // if(id < 0 ) -> erro na chamada fork

        if ((descriptor = fork()) < 0)
        {
            printf("Error FORK");
            exit(0);
        }
        else if (descriptor > 0)
        {
            close(pipe1[1]);

            passwordFile(pipe1[0]); // faz a Leitura da password no processo pai

            close(pipe1[0]);

            exit(0);
        }
        else
        {
            close(pipe1[0]);

            readPassword(pipe1[1]); // Realiza somente a leitura da password em um file txt

            close(pipe1[1]);

            exit(0);
        }
}

passwordFile(readfd) int readfd; // leitura do pipe2[0]

{
    char password[100];
    read(readfd, password, 100);

    char userPassword[100];

    do
    {
        printf("Password: ");
        fgets(userPassword, 100, stdin);

        pthread_t checkAttempts;
        pthread_create(&checkAttempts, NULL, passwordAttemptsThread, attempts);

        if (strcmp(password, userPassword) == 0)
        {
            printf("\nCorrect password!\n\n ");

            menu();

            pthread_t checkAttempts;
            pthread_create(&checkAttempts, NULL, passwordAttemptsThread, attempts);
        }
        else
        {
            printf("\nWrong password. Try again! \n\n");
            attempts++;
        }
    } while (1);
}

readPassword(writefd) int writefd;

{
    FILE *passwordFile = fopen("arquivo.txt", "r");

    char password[100];
    fgets(password, 100, passwordFile);

    fclose(passwordFile);

    write(writefd, password, strlen(password));
}

void menu()
{
    int option;

    do
    {
        system("clear");

        printf("Welcome!\n");
        printf("\n1 - Open file");
        printf("\n2 - Logoff");

        printf("\nChoose one option: ");
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            system("clear");
            printf("Opening file...\n\n");

            comunicationFile();

            break;
        case 2:
            system("clear");
            printf("Leaving file...");
            break;
        default:
            printf("Invalid option!\n");
        }

    } while (option != 2);
}

void comunicationFile(){
    pthread_t timer;
    pthread_create(&timer, NULL, timerThread, NULL);

    int descriptor2,
        pipe2[2];

    if (pipe(pipe2) < 0)
            {
                printf("Error PIPE");
                exit(0);
            }

            if ((descriptor2 = fork()) < 0)
            {
                printf("Error FORK");
                exit(0);
            }
            else if (descriptor2 > 0)
            {
                close(pipe2[1]);

                showFile(pipe2[0]);

                pthread_join(timer, NULL);

                close(pipe2[0]);

                exit(0);
            }
            else
            {
                close(pipe2[0]);

                readFile(pipe2[1]);

                pthread_join(timer, NULL);

                close(pipe2[1]);

                exit(0);
            }


}

showFile(readfd) int readfd;

{
    char file[300];
    read(readfd, file, 300);

    printf("%s", &file);
}

readFile(writefd) int writefd;
{
    FILE *file = fopen(".infos.txt", "r");

    char userFile[300];
    fgets(userFile, 300, file);

    fclose(file);

    write(writefd, userFile, strlen(userFile));
}
