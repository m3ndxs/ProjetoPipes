#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAXBUFF 1024

// pipe[0] -> Leitura
// pipe[1] -> Escrita

main()
{
    loginPipes();   

    return 0;
}

void *timerThread(void *arg) {
    sleep(10);  // Tempo de espera: 10 segundos
    printf("\nTempo esgotado. Tente novamente.\n");
    system("clear");
    loginPipes(); 
}

void *passwordAttemptsThread(void *args){
    int *tentativas = (int *)arg;

    while(*tentatiavs < 5){
        sleep(1); //Aguarda 1 segundo

        *tentativas++;

        if(*tentativas == 5){
            printf("Numero maximo de tentativas incorretas atingido!\nPrograma encerrado...\n");
            exit(0);
        }
    }

    return NULL;
}

void loginPipes(){
    int descritor,
            pipe1[2],
            pipe2[2];

        if (pipe(pipe1) < 0 || pipe(pipe2) < 0)
        {
            printf("Erro na chamada PIPE");
            exit(0);
        }

        // Criando o processo filho
        // id = fork()
        // if(id > 0 ) -> processo pai
        // if(id = 0 ) -> processo filho
        // if(id < 0 ) -> erro na chamada fork

        if ((descritor = fork()) < 0)
        {
            printf("Erro chamada FORK");
            exit(0);
        }
        else if (descritor > 0)
        {
            close(pipe1[0]);
            close(pipe2[1]);
            close(pipe1[1]);

            senhaArquivo(pipe2[0]); // faz a Leitura da senha no processo pai

            close(pipe2[0]);

            exit(0);
        }
        else
        {
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe1[0]);

            leSenha(pipe2[1]); // Realiza somente a leitura da senha em um arquivo txt
            
            close(pipe2[1]);

            exit(0);
        }
}

senhaArquivo(readfd) int readfd; // leitura do pipe2[0]

{
    char senha[100];
    read(readfd, senha, 100);

    char senhaUsuario[100];

    do
    {
        printf("Digite a senha: ");
        fgets(senhaUsuario, 100, stdin);

        int tentativas;
        pthread_t verificaTentativas;
        pthread_create(&verificaTentativas, NULL, passwordAttemptsThread, &tentativas);

        if (strcmp(senha, senhaUsuario) == 0)
        {
            printf("\nSenha Correta! ");

            menu();
        }
        else
        {
            printf("\nSenha incorreta, tente novamente!");
        }
    } while (1);
}

leSenha(writefd) int writefd;

{
    FILE *arquivoSenha = fopen("arquivo.txt", "r");

    char senha[100];
    fgets(senha, 100, arquivoSenha);

    fclose(arquivoSenha);

    write(writefd, senha, strlen(senha));
}

void menu()
{
    int opcao;

    do
    {
        printf("Seja bem vindo(a)!\n");
        printf("\n1 - Abrir Arquivo");
        printf("\n2 - Sair");

        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\nAbrindo arquivo...\n\n");

            comunicacaoArquivo();
    
            break;
        case 2:
            printf("\nSaindo arquivo...");
            break;
        default:
            printf("\nOpção invalida!");
        }

    } while (opcao != 2);
}

void comunicacaoArquivo(){
    pthread_t timer;
    pthread_create(&timer, NULL, timerThread, NULL);

    int descritor2,
        pipe3[2],
        pipe4[2];

    if (pipe(pipe3) < 0 || pipe(pipe4) < 0)
            {
                printf("Erro na chamada PIPE");
                exit(0);
            }

            if ((descritor2 = fork()) < 0)
            {
                printf("Erro chamada FORK");
                exit(0);
            }
            else if (descritor2 > 0)
            {
                close(pipe3[0]);
                close(pipe4[1]);
                close(pipe3[1]);

                mostraArquivo(pipe4[0]);

                pthread_join(timer, NULL);

                close(pipe4[0]);

                exit(0);
            }
            else
            {
                close(pipe3[1]);
                close(pipe4[0]);
                close(pipe3[0]);

                leArquivo(pipe4[1]);

                pthread_join(timer, NULL);

                close(pipe4[1]);

                exit(0);
            }

    
}

mostraArquivo(readfd) int readfd;

{
    char arquivo[300];
    read(readfd, arquivo, 300);

    printf("%s", &arquivo);
}

leArquivo(writefd) int writefd;

{
    FILE *arquivo = fopen("arquivo.txt", "r");

    char arquivoUsuario[300];
    fgets(arquivoUsuario, 300, arquivo);

    fclose(arquivo);

    write(writefd, arquivoUsuario, strlen(arquivoUsuario));
}