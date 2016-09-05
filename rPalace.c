#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

struct Client{
    long CPF;
    char nome[15];
    char sobrenome[30];
    long telefone;
};
typedef struct Client client;

void newClient(FILE *ptrCadastros); // Adiciona um novo cliente
void showClients(FILE *ptrCadastros); // Mostra todos os clientes
void showOneClient(client theClient); // Imprime dados de um cliente
void updateClient(FILE *ptrCadastros, long CPF); // Atualiza o cadastro de um CPF
int menu(); // Apresenta o menu e retorna a opção do usuário
int findClient(FILE *ptrCadastros, long CPF, client *theClient); // Procura por um cliente retorna o 1 se encontrar e 0 se não
client getClientData(); // recolhe e retorna os dados de um cliente
void removeClient(FILE *ptrCadastros, long CPF); // remove cliente
int fptrToRemovedClient(FILE *ptrCadastros); // Move ponteiro FILE para o primeiro cadastro removido
void datToTxt(FILE *ptrCadastros); // Gera um arquivo txt com todos os dados


int main() {
    FILE *ptrCadastros;
    client theClient;
    int using = 1;
    long aux;

    setlocale(LC_ALL,"");

    // Se não houver arquivo, cria um.
    if ((ptrCadastros = fopen("cadastros.dat", "r+")) == NULL){
        ptrCadastros = fopen("cadastros.dat", "w+");
    }
    // Loop de funcionamento
    while (using){
        switch (menu()) {
            case 1:
                printf("CPF: ");
                scanf("%ld", &aux);
                if (!findClient(ptrCadastros, aux, &theClient)){
                    printf("CPF não cadastrado\n");
                } else {
                    showOneClient(theClient);
                }
            break;
            case 2:
                newClient(ptrCadastros);
            break;
            case 3:
                printf("CPF: ");
                scanf("%ld", &aux);
                updateClient(ptrCadastros, aux);
            break;

            case 4:
                printf("CPF: ");
                scanf("%ld", &aux);
                removeClient(ptrCadastros, aux);
            break;

            case 5:
                showClients(ptrCadastros);
            break;
            case 6:
                datToTxt(ptrCadastros);
            break;

            case 9:
                using = 0;
                printf("Saiu\n");
            break;

            default:
            printf("Opção inválida...\n");
        }
    }
    fclose(ptrCadastros);
    return 0;
}
// mostra menu e retorna opção do usuário
int menu(){
    int choice;
    printf("// ----- // ----- // CADASTRO HOSPEDE // ----- // ----- //\n");
    printf("[1] - Buscar por cadastro\n");
    printf("[2] - Cadastrar novo\n");
    printf("[3] - Atualizar cadastro\n");
    printf("[4] - Remover Cadastro\n");
    printf("[5] - Imprimir todos cadastrados\n");
    printf("[6] - Gerar relatório\n");
    printf("[9] - Finalizar\n");
    printf("\nEntre com a sua opção: ");
    scanf("\n%d", &choice);

    system("clear");

    return choice;
}
// Cadastra novo cliente
void newClient(FILE *ptrCadastros){
    char aux;
    client auxClient;

    printf("CPF: ");
    scanf("%ld", &auxClient.CPF);
    if (!findClient(ptrCadastros, auxClient.CPF, NULL)){
        printf("Nome: ");
        scanf("\n%[^\n]s", auxClient.nome);
        printf("Sobrenome: ");
        scanf("\n%[^\n]s", auxClient.sobrenome);
        printf("Telefone: ");
        scanf("\n%ld", &auxClient.telefone);

        printf("'S' para salvar 'C' para cancelar\n");
        scanf("\n%c", &aux);
        if (aux == 'S' || aux == 's'){
            printf("Cadastro salvo!\n");
            if (fptrToRemovedClient(ptrCadastros)){
                fwrite(&auxClient, sizeof(client), 1, ptrCadastros);
            } else {
                fseek(ptrCadastros, 0, SEEK_END);
                fwrite(&auxClient, sizeof(client), 1, ptrCadastros);
            }
        } else if (aux == 'C' || aux == 'c'){
            printf("Cancelando.\n");
        } else {
            printf("Opção inválida, considerando 'C' e cancelando.\n");
        }
    } else {
        printf("CPF já cadastrado, dados acima.\n");
    }
}
// Mostra todos os clientes cadastrados
void showClients(FILE *ptrCadastros){
    client auxClient;

    rewind(ptrCadastros);
    fread(&auxClient, sizeof(client), 1, ptrCadastros);
    while (!feof(ptrCadastros)){
        if (auxClient.CPF != 0)
            showOneClient(auxClient);
        // printf("\nCPF:       %ld\nNome:      %s\nSobrenome: %s\nTelefone:  %ld\n", auxClient.CPF, auxClient.nome, auxClient.sobrenome, auxClient.telefone);
        fread(&auxClient, sizeof(client), 1, ptrCadastros);
    }
}
// retorna 1 se encontrar um cpf retorna 0 se não encontrar
int findClient(FILE *ptrCadastros, long CPF, client *theClient){
    client auxClient;

    rewind(ptrCadastros);
    while (!feof(ptrCadastros)){
        fread(&auxClient, sizeof(client), 1, ptrCadastros);
        if (CPF == auxClient.CPF){
            if (theClient != NULL){
                (*theClient) = auxClient;
            }
            return 1;
        }
    }
    return 0;
}
// Atualiza dados um por um do cliente com este CPF, se existir
void updateClient(FILE *ptrCadastros, long CPF){
    client auxClient;
    char aux;

    if (findClient(ptrCadastros, CPF, NULL)){
        fseek(ptrCadastros, -sizeof(client), SEEK_CUR);
        fread(&auxClient, sizeof(client), 1, ptrCadastros);
        printf("Cliente encontrado!\n\n");
        // Atualiza ou não CPF
        printf("CPF: %ld\n", auxClient.CPF);
        printf("'C' para confirmar e 'A' para atualizar.\n");
        scanf("\n%c", &aux);
        if (aux == 'C' || aux == 'c') {
            printf("Confirmado.\n\n");
        } else if (aux == 'A' || aux == 'a'){
            printf("Novo CPF: ");
            scanf("\n%ld", &auxClient.CPF);
            printf("Atualizado para valor acima.\n\n");
        } else {
            printf("Opção inválida, considerando 'C' e mantendo valor.\n");
        }
        // Atualiza ou não nome
        printf("Nome: %s\n", auxClient.nome);
        printf("'C' para confirmar e 'A' para atualizar.\n");
        scanf("\n%c", &aux);
        if (aux == 'C' || aux == 'c') {
            printf("Confirmado.\n\n");
        } else if (aux == 'A' || aux == 'a'){
            printf("Novo nome: ");
            scanf("\n%[^\n]s", auxClient.nome);
            printf("Atualizado para valor acima.\n\n");
        } else {
            printf("Opção inválida, considerando 'C' e mantendo valor.\n");
        }
        // Aualiza ou não sobrenome
        printf("Sobrenome: %s\n", auxClient.sobrenome);
        printf("'C' para confirmar e 'A' para atualizar.\n");
        scanf("\n%c", &aux);
        if (aux == 'C' || aux == 'c') {
            printf("Confirmado.\n\n");
        } else if (aux == 'A' || aux == 'a'){
            printf("Novo sobrenome: ");
            scanf("\n%[^\n]s", auxClient.sobrenome);
            printf("Atualizado para valor acima.\n\n");
        } else {
            printf("Opção inválida, considerando 'C' e mantendo valor.\n");
        }
        // Atualiza ou não telefone
        printf("Telefone: %ld\n", auxClient.telefone);
        printf("'C' para confirmar e 'A' para atualizar.\n");
        scanf("\n%c", &aux);
        if (aux == 'C' || aux == 'c') {
            printf("Confirmado.\n\n");
        } else if (aux == 'A' || aux == 'a'){
            printf("Novo telefone: ");
            scanf("\n%ld", &auxClient.telefone);
            printf("Atualizado para valor acima.\n\n");
        } else {
            printf("Opção inválida, considerando 'C' e mantendo valor.\n");
        }
        // Grava em cadastros.dat seja lá como estiver
        fseek(ptrCadastros, -sizeof(client), SEEK_CUR);
        fwrite(&auxClient, sizeof(client), 1, ptrCadastros);
        printf("Atualizado com sucesso!\n");
    } else {
        printf("Cliente não encontrado.\n");
    }
}
// Recolhe um por um dos dados de um cliente
client getClientData(){
    client auxClient;

    printf("CPF: ");
    scanf("\n%ld", &auxClient.CPF);

    printf("Nome: ");
    scanf("\n%[^\n]s", auxClient.nome);

    printf("Sobrenome: ");
    scanf("\n%[^\n]s", auxClient.sobrenome);

    printf("Telefone: ");
    scanf("\n%ld", &auxClient.telefone );
    return auxClient;
}
// imprime os dados de um cliente
void showOneClient(client theClient){
    printf("\nCPF:       %ld\nNome:      %s\nSobrenome: %s\nTelefone:  %ld\n", theClient.CPF, theClient.nome, theClient.sobrenome, theClient.telefone);
}
// Remove um cliente
void removeClient(FILE *ptrCadastros, long CPF){
    client auxClient;
    char aux;

    if (findClient(ptrCadastros, CPF, &auxClient)){
        showOneClient(auxClient);
        printf("Deseja remover esse cadastro? ('S' para sim | 'N' para não)\n");
        scanf("\n%c", &aux);

        if (aux == 'S' || aux == 's'){
            auxClient.CPF = 0;
            fseek(ptrCadastros, -sizeof(client), SEEK_CUR);
            fwrite(&auxClient, sizeof(client), 1, ptrCadastros);
            printf("Removido com sucesso!\n");
        } else if (aux == 'N' || aux == 'n'){
            printf("Cadastro não removido\n");
        } else {
            printf("Opção inválida! Considerando 'N' e não removendo\n");
        }
    } else {
        printf("CPF não cadastrado.\n");
    }
}
// Procura por algum cadastro removido, se encontrado retorna 1 do contrário 0 (move ponteiro FILE para o cadastro removido)
int fptrToRemovedClient(FILE *ptrCadastros){
    client auxClient;

    rewind(ptrCadastros);

    while(!feof(ptrCadastros)){
        fread(&auxClient, sizeof(client), 1, ptrCadastros);
        if (auxClient.CPF == 0){
            fseek(ptrCadastros, -sizeof(client), SEEK_CUR);
            return 1;
        }
    }
    return 0;
}
// Gera um arquivo .txt com todos os dados
void datToTxt(FILE *ptrCadastros){
    FILE *ptrRelatorio;
    client cAux;

    rewind(ptrCadastros);
    if ((ptrRelatorio = fopen("relatorio.txt", "w")) == NULL){
        printf("Arquivo não pôde ser gerado.\n");
    } else {
        fprintf(ptrRelatorio,"CPF         NOME            SOBRENOME                      TELEFONE\n");
        fread(&cAux, sizeof(client), 1, ptrCadastros);
        while(!feof(ptrCadastros)){
            if(cAux.CPF != 0){
                fprintf(ptrRelatorio, "%-12ld%-16s%-31s%ld\n", cAux.CPF, cAux.nome, cAux.sobrenome, cAux.telefone);
            }
            fread(&cAux, sizeof(client), 1, ptrCadastros);
        }
        printf("Relatório gerado com sucesso!\n");
    }
    fclose(ptrRelatorio);
}
