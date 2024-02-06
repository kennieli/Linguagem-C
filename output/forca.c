#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define TAMANHO_PALAVRA 20

//Declaração Funções -----------------------------------------------------------------------------------------------------------------------------------------------------------------
//--> Para não precisar se preocupar com a ordem em que chamamos as funções
void header();
void chuta();
void desenhaforca();
void escolhepalavra();
int acertou();
int enforcou ();
int jachutou (char letra);
void adicionapalavra();
int chuteserrados();

//Variáveis Globais ------------------------------------------------------------------------------------------------------------------------------------------------------------------
size_t tamanho;
char palavrasecreta[TAMANHO_PALAVRA];
char chutes[26];
int chutesdados = 0;

//Funções ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//--> Header do Jogo
void header(){
    printf("*********************\n");
    printf("*   JOGO DA FORCA   *\n");
    printf("*********************\n");
}

//--> Para pegar a letra digitada pelo user e armazenar na variável chute
void chuta(){
    char chute;
    scanf(" %c", &chute); //Usamos espaço antes do %c para que o scanf entenda que o enter não é um char

    chutes[chutesdados] = chute;
    chutesdados++;
}

//--> Identifica as letras já chutadas
int jachutou(char letra) {
    int achou = 0;
    for(int j = 0; j < chutesdados; j++) {
        if(chutes[j] == letra) {
            achou = 1;
            break;
        }
    }
    return achou;
}

// --> Adiciona uma nova pavra para a lista da forca
void adicionapalavra(){
    char quer;

    printf("Voce deseja adicionar uma nova palavra no jogo? (S/N):\n");
    scanf(" %c", &quer);

        if (quer == 'S'){
            char novapalavra[TAMANHO_PALAVRA];

            printf("Qual a nova palavra?\n");
            scanf("%s", novapalavra);

            FILE* f;
            f = fopen("palavras.txt", "r+"); //Lê o arquivo e permite escrever nele
            
            if(f == 0){
                printf("Desculpe, banco de dados nao disponivel!\n\n");
                exit(1);
            }

            int qtd = 0;
            fscanf(f,"%d", &qtd); //Lê a quantidade palavras que o arquivo tem (que está na primeira linha)
            qtd++; //Soma + 1 na quantidade - pois foi acrescentada uma palavra

            fseek(f, 0, SEEK_SET); //Vai voltar para a primeira linha, para setar o valor de quantidade de palavras
            fprintf(f,"%d", qtd);

            fseek(f, 0, SEEK_END); //Posiciona novamente a seta no final do arquivo
            fprintf(f, "\n%s", novapalavra); //Escreve a nova palavra

            fclose(f);
        }
}

//--> Palavra do jogo
void escolhepalavra(){

    FILE* f;
    f = fopen("palavras.txt","r");//fopen -> função para abrir um arquivo, o primeiro parâmetro é o nome do arquivo, o seguindo é referente ao que queremos fazer com ele, nesse caso "r" para ler

    if(f == 0){
        printf("Desculpe, banco de dados nao disponivel!\n\n");
        exit(1);
    }

    int qtddepalavras;
    fscanf(f, "%d", &qtddepalavras);

    srand(time(0));
    int randomico = rand() % qtddepalavras; //para sortear a palavra do arquivo txt

    for (int i=0; i <= randomico;i++){
        fscanf(f, "%s", palavrasecreta);
    }

    tamanho = strlen(palavrasecreta);

    fclose(f);//é necessário fechar o arquivo
}

//--> Desenha Forca e Letras acertadas
void desenhaforca() {

    int erros = chuteserrados();

//if ternário -> (condicao ? valor verdadeiro : valor falso)
    printf("  _______       \n");
    printf(" |/      |      \n");
    printf(" |      %c%c%c  \n", (erros>=1?'(':' '), 
        (erros>=1?'_':' '), (erros>=1?')':' '));
    printf(" |      %c%c%c  \n", (erros>=3?'\\':' '), 
        (erros>=2?'|':' '), (erros>=3?'/': ' '));
    printf(" |       %c     \n", (erros>=2?'|':' '));
    printf(" |      %c %c   \n", (erros>=4?'/':' '), 
        (erros>=4?'\\':' '));
    printf(" |              \n");
    printf("_|___           \n");
    printf("\n\n");

    printf("Voce ja deu %d chutes\n", chutesdados);

    for(int i = 0; i < (int)tamanho; i++) {

        if(jachutou(palavrasecreta[i])) {
            printf("%c ", palavrasecreta[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

// --> Acertos da palavra
int acertou(){
    for (int i = 0; i < (int)tamanho; i++){
        if(!jachutou(palavrasecreta[i])){
            return 0;
        }
    }
    return 1;
}

// --> Identifica os chutes errados
int chuteserrados(){
    int erros = 0;
    for (int i = 0; i < chutesdados; i++){
        int existe = 0;
        
        for (int j = 0; j < (int)tamanho; j++){
            if(chutes[i] == palavrasecreta[j]){
                existe = 1;
                break;
            }
        }
        if(!existe) erros++;
    }

    return erros;
}

//--> Identifica se o número de chutes foi atingido
int enforcou(){

    return chuteserrados() >= 5;
}

//Main -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main() {

    escolhepalavra();
    header();

    do{
        desenhaforca();
        chuta();
  
    }while(!acertou() && !enforcou());

    if(acertou()){
        printf("PARABENS PELA VITORIA! VOCE GANHOU!!!!!!! :D\n");
    }else{
        printf("VOCE PERDEU! TENTE NA PROXIMA *_*\n");
    }

    adicionapalavra();
}