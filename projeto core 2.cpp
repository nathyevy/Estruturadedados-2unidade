#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    char nm[50];
    char sn[50];
    char eml[100];
} Usr;

typedef struct {
    char ttl[100];
    float prc;
} Lvr;

void cad();
void ed();
void listCad();
void acsCad();
void excCad();
int buscaBin(Lvr arr[], int l, int r, char chave[]);
void merge(Lvr arr[], int l, int m, int r);
void mergeSort(Lvr arr[], int l, int r);
int verUsr(char nm[], char sn[]);
void menu();

int main(void) {
    setlocale(LC_ALL, "Portuguese");

    int op;
    do {
        menu();
        scanf("%d", &op);

        switch (op) {
            case 1:
                cad();
                break;
            case 2:
                ed();
                break;
            case 3:
                listCad();
                break;
            case 4:
                acsCad();
                break;
            case 5:
                excCad();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (op != 0);

    return 0;
}

void cad() {
    FILE *fl = fopen("usuarios.txt", "a");
    if (fl == NULL) {
        printf("Erro ao abrir o arquivo para cadastro.\n");
        return;
    }

    Usr usr;
    do {
        printf("Digite o e-mail do usuário: ");
        scanf(" %99[^\n]", usr.eml);

        if (strchr(usr.eml, '@') != NULL) {
            break;
        } else {
            printf("E-mail inválido. O e-mail deve conter o caractere '@'.\n");
        }
    } while (1);

    printf("Digite o nome do usuário: ");
    scanf(" %49[^\n]", usr.nm);
    printf("Digite a senha do usuário: ");
    scanf(" %49[^\n]", usr.sn);

    fprintf(fl, "%s %s %s\n", usr.nm, usr.sn, usr.eml);
    fclose(fl);
}

void ed() {
    char nm[50], sn[50];
    printf("Digite o nome do usuário: ");
    scanf(" %49[^\n]", nm);
    printf("Digite a senha do usuário: ");
    scanf(" %49[^\n]", sn);

    if (verUsr(nm, sn)) {
        printf("Usuário autenticado. Escolha o que deseja editar:\n");
        printf("1 - Alterar nome\n");
        printf("2 - Alterar senha\n");

        int op;
        scanf("%d", &op);

        FILE *fl = fopen("usuarios.txt", "r");
        FILE *tmp = fopen("temp.txt", "w");
        if (fl == NULL || tmp == NULL) {
            printf("Erro ao abrir o arquivo para edição.\n");
            return;
        }

        Usr usr;
        while (fscanf(fl, " %49s %49s %99[^\n]", usr.nm, usr.sn, usr.eml) != EOF) {
            if (strcmp(usr.nm, nm) == 0 && strcmp(usr.sn, sn) == 0) {
                if (op == 1) {
                    printf("Digite o novo nome: ");
                    scanf(" %49[^\n]", usr.nm);
                } else if (op == 2) {
                    printf("Digite a nova senha: ");
                    scanf(" %49[^\n]", usr.sn);
                }
            }
            fprintf(tmp, "%s %s %s\n", usr.nm, usr.sn, usr.eml);
        }

        fclose(fl);
        fclose(tmp);
        remove("usuarios.txt");
        rename("temp.txt", "usuarios.txt");
        printf("Dados atualizados com sucesso.\n");
    } else {
        printf("Usuário ou senha incorretos.\n");
    }
}

void listCad() {
    FILE *fl = fopen("usuarios.txt", "r");
    if (fl == NULL) {
        printf("Erro ao abrir o arquivo para listagem.\n");
        return;
    }

    Usr usr;
    while (fscanf(fl, " %49s %49s %99[^\n]", usr.nm, usr.sn, usr.eml) != EOF) {
        printf("Usuário: %s\n", usr.nm);
    }
    fclose(fl);
}

void acsCad() {
    char nm[50], sn[50];
    printf("Digite o nome do usuário: ");
    scanf(" %49[^\n]", nm);
    printf("Digite a senha do usuário: ");
    scanf(" %49[^\n]", sn);

    if (verUsr(nm, sn)) {
        printf("Usuário autenticado. Lista de livros:\n");

        Lvr lvr[10] = {
            {"Livro1 - Introdução aos ponteiros", 29.99},
            {"Livro2 - Acessando o conteúdo apontado por um ponteiro - operador de indireção", 39.99},
            {"Livro3 - Imprimindo um ponteiro", 24.99},
            {"Livro4 - Ponteiros como parâmetros de funções", 19.99},
            {"Livro5 - Endereços e vetores", 49.99},
            {"Livro6 - Aritmética de ponteiros", 59.99},
            {"Livro7 - Lendo um vetor em uma função usando aritmética de ponteiros", 34.99},
            {"Livro8 - Imprimindo um vetor em uma função usando aritmética de ponteiros", 44.99},
            {"Livro9 - Incremento de ponteiros", 14.99},
            {"Livro10 - Vetores de ponteiros - notação vetores", 9.99}
        };

        float ttl = 0;
        int op;
        do {
            printf("Escolha um livro (1-10) ou 0 para finalizar:\n");
            for (int i = 0; i < 10; i++) {
                printf("%d - %s: R$%.2f\n", i + 1, lvr[i].ttl, lvr[i].prc);
            }
            scanf("%d", &op);

            if (op > 0 && op <= 10) {
                ttl += lvr[op - 1].prc;
                printf("Você escolheu: %s\n", lvr[op - 1].ttl);
            }
        } while (op != 0);

        printf("Total da compra: R$%.2f\n", ttl);
    } else {
        printf("Usuário ou senha incorretos.\n");
    }
}

void excCad() {
    char nm[50], sn[50];
    printf("Digite o nome do usuário: ");
    scanf(" %49[^\n]", nm);
    printf("Digite a senha do usuário: ");
    scanf(" %49[^\n]", sn);

    if (verUsr(nm, sn)) {
        FILE *fl = fopen("usuarios.txt", "r");
        FILE *tmp = fopen("temp.txt", "w");
        if (fl == NULL || tmp == NULL) {
            printf("Erro ao abrir o arquivo para exclusão.\n");
            return;
        }

        Usr usr;
        while (fscanf(fl, " %49s %49s %99[^\n]", usr.nm, usr.sn, usr.eml) != EOF) {
            if (!(strcmp(usr.nm, nm) == 0 && strcmp(usr.sn, sn) == 0)) {
                fprintf(tmp, "%s %s %s\n", usr.nm, usr.sn, usr.eml);
            }
        }

        fclose(fl);
        fclose(tmp);
        remove("usuarios.txt");
        rename("temp.txt", "usuarios.txt");
        printf("Cadastro excluído com sucesso.\n");
    } else {
        printf("Usuário ou senha incorretos.\n");
    }
}

int verUsr(char nm[], char sn[]) {
    FILE *fl = fopen("usuarios.txt", "r");
    if (fl == NULL) {
        printf("Erro ao abrir o arquivo para verificação.\n");
        return 0;
    }

    Usr usr;
    int found = 0;
    while (fscanf(fl, " %49s %49s %99[^\n]", usr.nm, usr.sn, usr.eml) != EOF) {
        if (strcmp(usr.nm, nm) == 0 && strcmp(usr.sn, sn) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fl);
    return found;
}

void merge(Lvr arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    Lvr L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i].prc <= R[j].prc) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(Lvr arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

int buscaBin(Lvr arr[], int l, int r, char chave[]) {
    if (r >= l) {
        int m = l + (r - l) / 2;

        if (strcmp(arr[m].ttl, chave) == 0)
            return m;

        if (strcmp(arr[m].ttl, chave) > 0)
            return buscaBin(arr, l, m - 1, chave);

        return buscaBin(arr, m + 1, r, chave);
    }

    return -1;
}

void menu() {
    printf("\n");
    printf("\n");
    printf("Bem vindo à nossa livraria online\n");
    printf("_______________________________________\n");
    printf("           Nathy's books                 \n");
    printf("_______________________________________\n");
    printf("Por favor, escolha uma opção abaixo\n");
    printf("\n");
    printf("1 - Cadastrar usuário\n");
    printf("2 - Editar cadastro\n");
    printf("3 - Listar cadastros\n");
    printf("4 - Acessar cadastro\n");
    printf("5 - Excluir cadastro\n");
    printf("0 - Sair\n");
}

