#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_USERNAME_LEN 20
#define MAX_PASSWORD_LEN 10

// Declaração de funções

// Funcoes para cadastro do usuario
void cadastraUsuario();
void cadastro(int readfd, int writefd);
void salvaCadastro(int readfd, int writefd);

//funcoes login
void login();
void realizaLogin(int readfd, int writefd);
void leLogin(int readfd, int writefd);

struct Usuario {
    char nomeUsuario[MAX_USERNAME_LEN];
    char senha[MAX_PASSWORD_LEN];
};

int main() {
    int opcao;

    do {
        printf("\n------------ Gerenciamento de arquivos --------------\n");
        printf("\n1 - Cadastrar");
        printf("\n2 - Login");
        printf("\n3 - Sair");
        printf("\nEscolha sua opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            cadastraUsuario();
            break;

        case 2:
            login();
            break;

        case 3:
            printf("Saindo do programa...");
            break;

        default:
            break;
        }
    } while (opcao != 3);

   return 0;
}

void cadastraUsuario() {
    int descritor,
        pipe1[2],
        pipe2[2];

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        printf("Erro na chamada PIPE");
        exit(0);
    }

    if ((descritor = fork()) < 0) {
        printf("Erro na chamada FORK");
        exit(0);
    } else if (descritor > 0) {
        close(pipe1[0]);
        close(pipe2[1]);

        cadastro(pipe2[0], pipe1[1]);

        close(pipe1[1]);
        close(pipe2[0]);
    } else {
        close(pipe1[1]);
        close(pipe2[0]);

        salvaCadastro(pipe1[0], pipe2[1]);

        close(pipe1[0]);
        close(pipe2[1]);

        exit(0);
    }
}

void cadastro(int readfd, int writefd) {
    struct Usuario novoUsuario;

    char confirmacaoSenha[MAX_PASSWORD_LEN];
    char buffer[200];
    char buff[200];

    printf("\nInsira seu nome de usuario: ");
    scanf("%s", novoUsuario.nomeUsuario);
    printf("\nInsira uma senha: ");
    scanf("%s", novoUsuario.senha);

    printf("\nConfirme a senha digitada: ");
    scanf("%s", confirmacaoSenha);

    if (strcmp(novoUsuario.senha, confirmacaoSenha) != 0) {
        printf("Erro: As senhas nao sao iguais. Tente Novamente!\n");
        return;
    }
    snprintf(buffer, sizeof(buffer), "%s %s", novoUsuario.nomeUsuario, novoUsuario.senha);

    write(writefd, buffer, sizeof(buffer));

    read(readfd, buff, sizeof(buff));
    printf("\n%s", buff);
}

void salvaCadastro(int readfd, int writefd) {
    char buffer[200];

    read(readfd, buffer, sizeof(buffer));
    printf("\nInformacao recebida -> %s\n", buffer);

    FILE *arquivoUsuarios = fopen("usuarios.txt", "a");

    if (arquivoUsuarios == NULL) {
        perror("Erro ao abrir o arquivo de usuarios");
        return;
    }

    fprintf(arquivoUsuarios, "%s\n", buffer);

    fclose(arquivoUsuarios);

    write(writefd, "Cadastro Salvo...", sizeof("Cadastro Salvo..."));
}

void login(){
    int descritor,
        pipe1[2],
        pipe2[2];

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        printf("Erro na chamada PIPE");
        exit(0);
    }

    if ((descritor = fork()) < 0) {
        printf("Erro na chamada FORK");
        exit(0);
    } else if (descritor > 0) {
        close(pipe1[0]);
        close(pipe2[1]);

        realizaLogin(pipe2[0], pipe1[1]);

        close(pipe1[1]);
        close(pipe2[0]);
    } else {
        close(pipe1[1]);
        close(pipe2[0]);

        leLogin(pipe1[0], pipe2[1]);

        close(pipe1[0]);
        close(pipe2[1]);

        exit(0);
    }
}

void realizaLogin(int readfd, int writefd){
    char usuario[MAX_USERNAME_LEN];
    char senha[MAX_PASSWORD_LEN];
    char infoLogin[50];
    char buffer[50];

    printf("\nUsuario: ");
    scanf("%s", &usuario);
    printf("\nSenha: ");
    scanf("%s", &senha);

    snprintf(infoLogin, sizeof(infoLogin), "%s %s", usuario, senha);

    write(writefd, infoLogin, sizeof(infoLogin));

    read(readfd, buffer, sizeof(buffer));
    printf("%s", buffer);
}

void leLogin(int readfd, int writefd){
    struct Usuario usuarioAtual;
    char infoLogin[50];
    int loginFeito = 0;
    
    read(readfd, infoLogin, sizeof(infoLogin));
    printf("\nConferindo as informações recebidas...");
    printf("\n%s", infoLogin);

    sleep(5);

    FILE *arquivoUsuarios = fopen("usuarios.txt", "r");
    if(arquivoUsuarios == NULL){
        perror("\n\nArquivo nao encontrado");
        return NULL;
    }

    while(fscanf(arquivoUsuarios, "%s %s", usuarioAtual.nomeUsuario, usuarioAtual.senha)){
        if(strcmp(usuarioAtual.nomeUsuario, infoLogin) == 0){
            if(strcmp(usuarioAtual.senha, infoLogin + strlen(usuarioAtual.nomeUsuario) + 1) == 0){
                loginFeito = 1;

                break;
            }
        }
    }

    fclose(arquivoUsuarios);

    if(loginFeito){
        char resposta[] = "\nLogin realizado com sucesso!";

        write(writefd, resposta, sizeof(resposta));
    }
    else{
        char resposta[] = "\nLogin falhou...";

        write(writefd, resposta, sizeof(resposta));
    }

    return 0;
}