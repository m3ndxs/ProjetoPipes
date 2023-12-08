#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define MAXBUFF 1024

//pipe[0] -> Leitura
//pipe[1] -> Escrita

main(){
    int descritor,
        pipe1[2],
        pipe2[2];

    if(pipe(pipe1)<0 || pipe(pipe2)<0) {
        printf("Erro na chamada PIPE");
        exit(0);
    }

    // Criando o processo filho
    // id = fork()
    // if(id > 0 ) -> processo pai
    // if(id = 0 ) -> processo filho 
    // if(id > 0 ) -> erro na chamada fork

    if((descritor = fork())<0) {
        printf("Erro chamada FORK");
        exit(0);
    }
    else if(descritor > 0) {
        close(pipe1[0]);
        close(pipe2[1]);
        close(pipe1[1]);

        senhaArquivo(pipe2[0]); // faz a Leitura da senha no processo pai

        close(pipe2[0]);

        exit(0);
    }
    else {
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe1[0]);

        leSenha(pipe2[1]);  // Realiza somente a leitura da senha em um arquivo txt

        close(pipe2[1]);
        
        exit(0);
    }    
}


senhaArquivo(readfd)
int readfd; //leitura do pipe2[0]

{
    char senha[100];
    read(readfd, senha, 100);

    char senhaUsuario[100];
    
    do{
        printf("Digite a senha: ");
        fgets(senhaUsuario, 100, stdin);

        if(strcmp(senha, senhaUsuario) == 0){
            printf("\nSenha Correta! ");
        }
        else{
            printf("\nSenha incorreta, tente novamente!");
        }
    }while(1);

}

leSenha(writefd)
int writefd;

{
    FILE *arquivoSenha = fopen("arquivo.txt", "r");

    char senha[100];
    fgets(senha, 100, arquivoSenha);

    fclose(arquivoSenha);

    write(writefd, senha, strlen(senha));
}

void menu() {
    int opcao;

    do{
        printf("Seja bem vindo(a)!\n");
        printf("\n1 - Criar Arquivo");
        printf("\n2 - Abrir Arquivo");
        printf("\n3 - Alterar Arquivo");
        printf("\n4 - Deletar Arquivo");
        printf("\n5- Sair");

        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                printf("\nCriando arquivo...");  
                break;
            case 2:
                printf("\nAbrindo arquivo...");
                break;
            case 3:
                printf("\nAlterando arquivo...");
                break;
            case 4:
                printf("\nDeletando arquivo...");
                break;
            case 5:
                printf("\nVolte sempre...");
                break;
            default:
                printf("\nOpção invalida!");        
        }

    }while(opcao != 5);
}