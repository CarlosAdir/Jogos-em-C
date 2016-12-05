
/**      @file: 150121059.c
  *     @author: Carlos Adir Ely Murussi Leite (carlos.adir.leite@gmail.com)
  *  @matricula: 150121059
  * @disciplina: Algoritmos e Programação de Computadores
  *
  * Esse algoritmo implementado em C é para o trabalho 3. Desde a implementação do trabalho 2, foram feitos 
  * mais dois jogos: Cobrinha e Pacman. Pela falta de tempo, o Pacman ficou sem monstros e o labirinto só existe o menu.
  * Somente os jogos Senha e Cobrinha atualizam a pontuacao, isso é, após jogar o valor da pontuacao pode mudar. 
  * Nos outros jogos não há pontuacao, mas o sistema para atualizar já está feito.
  * No jogo da cobrinha, além de ter mapas padrões, é possivel importar dados, como um exemplo como mostra link abaixo:
  *
  *               https://github.com/CarlosAdir/Jogos-em-C
  *
  * O jogo do pacman só funciona com um arquivo especifico, que também é dado no github. É necessário que o usuario entre com o arquivo.
  * O jogo roda roda ainda não tem arquivo, mas no github já tem o arquivo com palavras necessárias, quando estiver pronto, estará disponibilizado
  * a versão em que pega-se arquivos. Mas como é para entregar cedo, então todas as palavras já estão no programa.
   */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#define ENTER 10
#define ESC 27
#define SAIR 0
#define espaco 20
#define player 10
#define carac 10
#define jogos 12
#define bin "ranking"
#define read "rb"
#define write "wb+"
#define tele1 '='
#define tele2 '|'
#define parede '&'
#define comida 'k'
#define corpo '@'
#define monstro '@'
#define corpo_pacman '$'
#define x_m 50
#define y_m 30

typedef struct
{
  char nome[carac];
  int point[jogos], total;
} geral;
typedef struct
{
  char nome[carac];
  int valor;
} individual;
typedef struct
{
  individual w_[player];
} completo;
char getch()/*le um caracter da entrada padrão sem o bloqueio de entrada(nao necessita apertar enter) */
{
    int ch;
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO,&oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}
void limpa_buffer()
{
    while(kbhit())
    {
        getch();
    }
}
int abs_int(int numero)
{
  return (numero < 0 ? -numero : numero);
}
void aperte_tecla(char tecla)
{
  char caracter;
  limpa_buffer();
  if(!tecla)
  {
    printf("\n\nAperte qualquer tecla para continuar...");
    getch();
  }
  else
  {
    if(tecla == ESC)
      printf("\n\nAperte ESC para continuar..."); 
    else if(tecla == ENTER)
      printf("\n\nAperte ENTER para continuar...");
    else
      printf("\n\nAperte %c para continuar...", tecla);
    do
    {
      caracter = getch();
    }while(caracter != tecla);
  }
}
int pega_algarismo(int minimo, int maximo)
{
    char caracter;
    limpa_buffer();
    do
    {
        caracter = getch();
    }while(!( caracter >= '0' + minimo && caracter <= '0' + maximo ) && caracter != ESC);
    if(caracter == ESC)
        return 0;
    return caracter - '0';
}
char pega_letra(char inicial, char final, char inicial2, char final2)
{
  char caracter;
  limpa_buffer();
  do
  {
    caracter = getch();
  }while(!(caracter <= final && caracter >= inicial) && !(caracter <= final2 && caracter >= inicial2) && caracter != ESC);
  if(caracter == ESC)
    return 0;
  return caracter;
}
int pega_numero_grande()
{
  char numero[11];
  int soma=0, contador = 0;
  scanf(" %9s", numero);
  limpa_buffer();
  do
  {
    soma*=10;
    if(!(numero[contador]>= '0' && numero[contador] <= '9'))
      return -1;
    soma+=(numero[contador] - '0');
    contador++;
  }while(numero[contador] != '\0');
  return soma;
}
void pega_palavra_no_arquivo(FILE *arquivo, char limite, char *palavra)
{
  int i = -1;
  char c;
  do
  {
    i++;
    c = fgetc(arquivo);
    *(palavra+i) = c;
  }while( c != limite);
  *(palavra+i) = '\0';
}
void copia(char *str1, char *str2)
{
  int i = -1;
  do
  {
    i++;
    *(str2+i) = *(str1+i);
  }while(*(str2+i) != '\0');
}
int pega_quatro_setas() /* Retorna CIMA = 1, BAIXO = 2, ESQUERDA = 4, DIREITA = 3.*/
{
  char car[3];
  do
  {
    do
    {
      do
      {
        car[0] = getch();
      }while(car[0] != 27);
      if(!kbhit())
        return 0;
      car[1] = getch();
    }while(car[1] != 91);
    car[2] = getch();
  }while(car[2] < 65 || car[2] > 68);
  return car[2] - 64;
}
int pega_duas_setas()
{
  char car[3];
  do
  {
    do
    {
      do
      {
        car[0] = getch();
      }while(car[0] != 27);
      if(!kbhit())
        return 0;
      car[1] = getch();
    }while(car[1] != 91);
    car[2] = getch();
  }while(car[2] < 65 || car[2] > 66);
  return car[2] - 64;
}
int mede_tamanho_string(char *string)
{
  int tamanho = 0;
  while(*(string+tamanho) != '\0')
    tamanho++;
  return tamanho;
}
void imprime_string(char *str, int tam)
{
  int i, tamanho = mede_tamanho_string(str);
  if(tamanho < espaco)
  {
    for(i = 0; i < tam - tamanho; i++)
      putchar(' ');
    printf("%s", str);
  }
}
void troca_inteiro(int *p, int *q)
{
  int aux = *p;
  *p = *q;
  *q = aux;
}
void troca_nomes(char *str1, char *str2)
{
  char c;
  int i;
  for(i = 0; i < carac; i++)
  {
    c = *(str1+i);
    *(str1+i) = *(str2+i);
    *(str2+i) = c;
  }
}
int compara_string(char *str1, char *str2) /* Retorna 1 caso sejam iguais */
{
  int i = 0;
  while(*(str1+i) == *(str2+i) && (*(str1+i) != '\0' || *(str2+i) != '\0'))
    i++;
  if(*(str1+i) == '\0' && *(str2+i) == '\0')
    return 1;
  return 0;
}
int existe_arquivo(char *str) /* Retorna 1 caso exista */
{
  FILE *arquivo = fopen(str, read);
  if(arquivo == NULL)
    return 0;
  else
    fclose(arquivo);
  return 1;
}
int ler_numero(FILE *arquivo)
{
  int numero;
  fread(&numero, sizeof(numero), 1, arquivo);
  return numero;
}
char ler_letra(FILE *arquivo)
{
  char letra;
  fread(&letra, sizeof(letra), 1, arquivo);
  return letra;
}
void escreve_numero(int numero, FILE *arquivo)
{
  fwrite(&numero, sizeof(numero), 1, arquivo);
}
void escreve_letra(char letra, FILE *arquivo)
{
  fwrite(&letra, sizeof(letra), 1, arquivo);
}
void limpa_tudo(geral *score, completo *record)
{
  int i, j, k;
  for(i = 0; i < player; i++)
  {
    for(j = 0; j < carac; j++)
      *((score+i)->nome+j) = '\0';
    for(j = 0; j < jogos; j++)
      *((score+i)->point+j) = 0;
    (score+i)->total = 0;
  }
  for(i = 0; i < jogos; i++)
  {
    for(j = 0; j < player; j++)
    {
      for(k = 0; k < carac; k++)
        *(((record+i)->w_+j)->nome+k) = '\0';
      ((record+i)->w_+j)->valor = 0;
    }
  }
}
void limpar_um(geral *dado)
{
  int i;
  for(i = 0; i < carac; i++)
    *(dado->nome+i) = '\0';
  *(dado->nome+1) = '\0';
  for(i = 0; i < jogos; i++)
    *(dado->point+i) = 0;
  dado->total = 0;
}
int importar_dados(geral *a_, completo *b_)
{
  FILE *arquivo = fopen(bin, read);
  int i, j, k;
  if(!arquivo)
    return 1;
  for(i = 0; i < player; i++)
  {
    for(j = 0; j < carac; j++)
      *((a_+i)->nome+j) = ler_letra(arquivo);
    for(j = 0; j < jogos; j++)
      *((a_+i)->point+j) = ler_numero(arquivo);
    (a_+i)->total = ler_numero(arquivo);
  }
  for(i = 0; i < jogos; i++)
    for(j = 0; j < player; j++)
    {
      for(k = 0; k < carac; k++)
        *(((b_+i)->w_+j)->nome+k) = ler_letra(arquivo);
      ((b_+i)->w_+j)->valor = ler_numero(arquivo);
    }
  return 0;
  fclose(arquivo);
}
void escreve_tudo(geral *a_, completo *b_)
{
  FILE *arquivo;
  arquivo = fopen(bin, write);
  int i, j, k;
  for(i = 0; i < player; i++)
  {
    for(j = 0; j < carac; j++)
      escreve_letra(*((a_+i)->nome+j), arquivo);
    for(j = 0; j < jogos; j++)
      escreve_numero(*((a_+i)->point+j), arquivo);
    escreve_numero((a_+i)->total, arquivo);
  }
  for(i = 0; i < jogos; i++)
    for(j = 0; j < player; j++)
    {
      for(k = 0; k < carac; k++)
        escreve_letra(*(((b_+i)->w_+j)->nome+k), arquivo);
      escreve_numero(((b_+i)->w_+j)->valor, arquivo);
    }
  fclose(arquivo);
}
void organizar_geral(geral *a_)
{
  int i, j, posicaomaior, maior;
  for(i = 0; i < player; i++)
  {
    posicaomaior = i;
    maior = (a_+i)->total;
    for(j = i+1; j< player; j++)
    {
      if((a_+j)->total > maior)
      {
        maior = (a_+j)->total;
        posicaomaior = j;
      }
    }
    if(maior != (a_+i)->total)
    {
      troca_nomes((a_+i)->nome, (a_+posicaomaior)->nome);
      troca_inteiro(&(a_+i)->total, &(a_+posicaomaior)->total);
      for(j = 0; j < jogos; j++)
        troca_inteiro((a_+i)->point+j, (a_+posicaomaior)->point+j);
    }
  }
} /* Apagar isso */
int procura_posicao(geral *a_, geral *b_)
{
  int i, j, controle;
  for(i = 0; i < player; i++)
  {
    controle = 1;
    if(b_->total == (a_+i)->total)
      if(compara_string(b_->nome, (a_+i)->nome))
      {
        for(j = 0; j < jogos && controle; j++)
        {
          if(*(b_->point+j) != *((a_+i)->point+j))
            controle = 0;
        }
        if(controle)
          return i + 1;
      }
  }
  return 0;
}
void copia_geral(geral *a_, geral *b_) /* A struct a_ recebe todos os dados de b */
{
  int i;
  for(i = 0; i < carac; i++)
    *(a_->nome+i) = *(b_->nome+i);
  for(i = 0; i < jogos; i++)
    *(a_->point+i) = *(b_->point+i);
  a_->total = b_->total;
}
void copia_individual(individual *a_, individual *b_)
{
  int i;
  for(i = 0; i < carac; i++)
    *(a_->nome+i) = *(b_->nome+i);
  a_->valor = b_->valor;
}
int organiza_posicao_geral(geral *score, geral *dado, int posicao_antiga) /* Tem como melhorar essa funcao */
{
  int posicao_nova = 0, i;
  if(!posicao_antiga && dado->total <= (score+player-1)->total) /* Se o dado for menor que o ultimo valor, irá retornar 0 */
    return 0;
  if(posicao_antiga /* Ver se isso é realmente necessario */ && dado->total == (score+posicao_antiga-1)->total) /* Nesse caso o total for equivalente(pois nunca sera menor), irá retornar a antiga posicao */
    return posicao_antiga;
  while((score+posicao_nova)->total >= dado->total) /* Aqui achará a posicao que for menor que tal valor de dado->total */
        posicao_nova++;
  if(posicao_antiga == 0)
    posicao_antiga = player-1;
  else
    posicao_antiga--;
  for(i = posicao_antiga; i>posicao_nova; i--)
    copia_geral(score+i, score+i-1);
  copia_geral(score+posicao_nova, dado);
  return posicao_nova+1;
}
int organiza_posicao_completo(completo *record, geral *dado, int posicao_antiga, int jogo)
{
  int posicao_nova = 0, i;
  jogo--;
  if(!posicao_antiga && *(dado->point+jogo) <= ((record+jogo)->w_+player-1)->valor)
    return 0; /* Se o dado for menor que o ultimo valor, irá retornar 0 */
  if(*(dado->point+jogo) == ((record+jogo)->w_+posicao_antiga-1)->valor)
    return posicao_antiga;
  while(((record+jogo)->w_+posicao_nova)->valor >= *(dado->point+jogo)) /* Aqui achará a posicao que for menor que tal valor de dado->total */
        posicao_nova++;
  if(posicao_antiga == 0)
    posicao_antiga = player-1;
  else
    posicao_antiga--;
  for(i = posicao_antiga; i>posicao_nova; i--)
    copia_individual((record+jogo)->w_+i, (record+jogo)->w_+i-1);
  for(i = 0; i < carac; i++)
  {
    *(((record+jogo)->w_+posicao_nova)->nome+i) = *(dado->nome+i);
  }
  ((record+jogo)->w_+posicao_nova)->valor = *(dado->point+jogo);
  return posicao_nova+1; 
}
void verifica_se_ja_jogou(geral *score, geral *dado, completo *record, char *nome, int *posicao)
{
  int i = 0, j=0;
  while(i < player)
  {
    if(compara_string((score+i)->nome, nome))
    {
      copia_geral(dado, score+i);
      *posicao = i+1;
      i = player;
      j = 1;
    }
    i++;
  }
  if(!j)
    for(i = 0; i < jogos; i++)
      for(j = 0; j < player; j++)
        if(compara_string(((record+i)->w_+j)->nome, nome))
        {
          *(posicao+i+1) = j+1;
          *(dado->point+i) = ((record+i)->w_+j)->valor;
          j = player;
        }
  else
    for(i = 0; i < jogos; i++)
      for(j = 0; j < player; j++)
        if(compara_string(((record+i)->w_+j)->nome, nome))
        {
          *(posicao+i+1) = j+1;
          j = player;
        }
}
int ranks(geral *score)
{
	int a=0, b=3, direcao;
	void menu()
	{
		system("clear");
		printf("*********************************************\n");
		printf("*****    Você está na toca dos feras!   *****\n");
		printf("*********************************************\n");
		printf("Os controles para subir e descer são as setas\n");
		printf("*********************************************\n\n");
	}
	void imprime(int comeco, int fim)
	{
		int i;
		for(i = comeco; i < fim; i++)
		{
			printf("%dº Lugar, co%s", i+1, i == 9 ? "m" : "m ");
			if((score+i)->total < 10)
				printf("     ");
			else if((score+i)->total < 100)
				printf("    ");
			else if((score+i)->total < 1000)
				printf("   ");
			else if((score+i)->total < 10000)
				printf("  ");
			else if((score+i)->total < 100000)
				printf(" ");
			printf("%d pontos: %s\n", (score+i)->total, (score+i)->nome);
			printf("\t              Senha: %d\n", *((score+i)->point+0));
			printf("\t           Charadas: %d\n", *((score+i)->point+1));
			printf("\tPedra Papel Tesoura: %d\n", *((score+i)->point+2));
			printf("\t               2048: %d\n", *((score+i)->point+3));
			printf("\t      Jogo da Velha: %d\n", *((score+i)->point+4));
			printf("\t    Jogo da Memoria: %d\n", *((score+i)->point+5));
			printf("\t             Xadrez: %d\n", *((score+i)->point+6));
			printf("\t       Campo Minado: %d\n", *((score+i)->point+7));
			printf("\t        Roda a Roda: %d\n", *((score+i)->point+8));
			printf("\t           Cobrinha: %d\n", *((score+i)->point+9));
			printf("\t            Pac Man: %d\n", *((score+i)->point+10));
			printf("\t          Labirinto: %d\n\n", *((score+i)->point+11));
		}
	}
	do
	{
		menu();
		imprime(a, b);
		printf("Digite ESC para sair, ou as setas para subir ou descer o rank.");
		direcao = pega_duas_setas();
		if(direcao == 1)
		{
			a--;
			b--;
		}
		else
		{
			a++;
			b++;
		}
		if(a < 0)
		{
			a++;
			b++;
		}
		else if(b > player)
		{
			a--;
			b--;
		}
	}while(direcao);
	return 0;
}
int senha(completo *record, geral *dado, int *posicao)
{
  int numero_maximo = 0, pontuacao=0;
  int senha, numero_do_usuario, tentativas;
  srand((unsigned)time(NULL));
  void menu_opcoes()
  {
    system("clear");
    printf("*********\n");
    printf("* Senha *\n");
    printf("*********\n");
    printf("\n");
    printf("1) Jogar\n");
    printf("2) Configurações\n");
    printf("3) Instrucoes\n");
    printf("4) Arrombadores de cofres\n\n");
    printf("0) Sair\n");
    printf("\n");
    printf("Digite uma opcao:  ");
  }
  void jogar()
  {
    void menu_jogar()
    {
      system("clear");
      printf("*********\n");
      printf("* Senha *\n");
      printf("*********\n");
      printf("\n");
    }
    int valor_da_pontuacao(int a, int b)
    {
      if(a < 10)
      	a = 1;
      else if(a < 100)
      	a = 2;
      else if(a < 1000)
      	a = 4;
      else if(a < 10000)
      	a = 8;
      else if(a < 100000)
      	a = 16;
      else
      	a = 32;
      return a*10 - b;
    }
    tentativas = pontuacao = 0;
    senha = rand()%(numero_maximo + 1);
    menu_jogar();
    printf("Já escolhi o numero!\n\n");
    aperte_tecla(ENTER);
    menu_jogar();
    printf("Digite seu palpite:  ");
    numero_do_usuario = pega_numero_grande();
    while(numero_do_usuario >= 0 && numero_do_usuario != senha)
    {
      tentativas++;
      menu_jogar();
      if(abs_int(numero_do_usuario - senha) > numero_maximo/3)
        printf("Estás frio! ");
      else if(abs_int(numero_do_usuario - senha) > numero_maximo/9)
        printf("Estás morno! ");
      else if(abs_int(numero_do_usuario- senha) > numero_maximo/27)
        printf("Estás quente! ");
      else
        printf("Estás  muito perto! ");
      if(numero_do_usuario < senha)
        printf("O resultado está acima do valor!\n\n");
      else
        printf("O resultado está abaixo desse valor!\n\n");
      printf("Digite um valor:  ");
      numero_do_usuario = pega_numero_grande();
    }
    if(numero_do_usuario == senha)
    {
      menu_jogar();
      printf("Parabéns, a senha é %d! Como descobriu com apenas %d tentativas?\n\n", senha, tentativas+1);
      pontuacao = valor_da_pontuacao(numero_maximo, tentativas); /* Para melhorar aqui, basta mudar os valores, para teste usaremos esse */
      if(pontuacao > *(dado->point+0)){
        dado->total -= *(dado->point+0);
        *(dado->point+0) = pontuacao;
        dado->total += pontuacao;
      }
    }
    else
    {
      menu_jogar();
      printf("Que pena! Você desistiu! Vencedores não são os que nunca perdem, mas sim aqueles que nunca param de tentar.\n");
      printf("A senha era %d e foram %d tentativas no total\n\n", senha, tentativas+1);
    }
    aperte_tecla(0);
  }
  int configuracoes()
  {
    void menu_configuracoes()
    {
      system("clear");
      printf("*********\n");
      printf("* Senha *\n");
      printf("*********\n");
      printf("\n");
      printf("1) Alterar o numero maximo(atual: %d)\n\n\n\n", numero_maximo);
      printf("0) Voltar ao menu alterior\n");
    }
    while(1)
    {
      menu_configuracoes();
      numero_do_usuario = pega_algarismo(0, 1);
      if(numero_do_usuario == 1)
      {
        menu_configuracoes();
        printf("\n\nDigite o novo numero maximo:  ");
        numero_do_usuario = pega_numero_grande();
        if(numero_do_usuario > 1)
          numero_maximo = numero_do_usuario;
        aperte_tecla(ENTER);
      }
      else
        return 0;
    }
  }
  void instrucoes()
  {
    system("clear");
    printf("Esse jogo é o jogo da senha. O computador sorteia um numero, e o usuario deve achar o numero.\n");
    printf("Se por exemplo o numero maximo é 5, então o computador poderá sortear os numeros 0, 1, 2, 3, 4 ou 5.\n");
    printf("Esse valor de numero maximo pode ser alterado no menu configuracoes.\n");
    printf("O usuario após digitar um palpite, o computador irá dizer se o valor do computador está acima ou abaixo do valor digitado\n");
    printf("O jogo acaba quando o usuario acerta o numero, ou então o usuario digita algum caracter ou numero negativo.\n");
    printf("Neste segundo caso, tem-se que o usuario receberá a pontuacao zero.\n\n");
    aperte_tecla(ENTER);
  }
  void rank_jogo()
  {
    int i;
    system("clear");
    printf("***********************************************\n");
    printf("* Bem vindo à sala dos Arrombadores de Cofres *\n");
    printf("***********************************************\n");
    printf("\n");
    for(i = 0; i < player; i++)
    {
      printf("%dº Lugar - ", i+1);
      imprime_string(((record+0)->w_+i)->nome,espaco);
      printf(": %d\n", ((record+0)->w_+i)->valor );
    }
    aperte_tecla(0);
  }
  while(1)
  {
    menu_opcoes();
    numero_do_usuario = pega_algarismo(0, 4);
    if(numero_do_usuario == 1)
      jogar();
    else if(numero_do_usuario == 2)
      configuracoes();
    else if(numero_do_usuario == 3)
      instrucoes();
    else if(numero_do_usuario == 4)
      rank_jogo();
    else
    {
      limpa_buffer();
      return 0;
    }
    *(posicao+1) = organiza_posicao_completo(record, dado, *(posicao+1), 1);
  }
}
int charadas(completo *record, geral *dado, int *posicao)
{
	int numero_do_usuario, numero_erros_maximo = 2, tentativas, gabarito, pontuacao;
	void menu_opcoes()
	{
		system("clear");
		printf("************\n");
		printf("* Charadas *\n");
		printf("************\n");
		printf("\n");
		printf("1) Jogar\n");
		printf("2) Configurações\n");
		printf("3) Instrucoes\n");
		printf("4) Os coringas da vez\n\n");
		printf("0) Sair\n");
		printf("\n");
		printf("Digite uma opcao:  ");
	}
	void jogar()
	{
		void menu_jogar()
		{
			system("clear");
			printf("************\n");
			printf("* Charadas *\n");
			printf("************\n");
			printf("\n");
		}
		int valor_pontuacao()
		{
			if(tentativas == 1)
				return 50;
			else
				return 25;
		}
		int charada(int numero_da_charada)
		{
			if(numero_da_charada == 1)
			{
				printf("Um \"amigo\" foi lhe visitar e levou duas de suas taças\n");
				printf("favoritas, qual o nome do filme?\n");
				printf("1) Exterminador do Futuro 2\n");
				printf("2) O roubo do Banco Central\n");
				printf("3) RoboCop 2\n");
				printf("4) Roubo a Máfia\n");
				return 3;
			}
			return 0;
		}
		tentativas = 0;
		menu_jogar();
		gabarito = charada(1);
		printf("Digite sua opcao:  ");
		numero_do_usuario = pega_algarismo(1, 4);
		while(tentativas < numero_erros_maximo && numero_do_usuario != gabarito)
		{
			menu_jogar();
			charada(1);
			tentativas++;
			printf("Errado! Como diria o profeta: \"tente outra vez\"...\n\n");
			aperte_tecla(ENTER);
			menu_jogar();
			charada(1);
			numero_do_usuario = pega_algarismo(1, 4);
		}
		menu_jogar();
		charada(1);
		if(tentativas == 2)
			printf("Você perdeu! Fora jogador!\n\n");
		else
		{
			pontuacao = valor_pontuacao();
			if(pontuacao > *(dado->point+1))
			{
				dado->total -= *(dado->point+1);
				*(dado->point+1) = pontuacao;
			    dado->total += pontuacao;
		    }
			printf("Parabéns, você ganhou!\n\n");
		}
		aperte_tecla(ENTER);
	}
	int configuracoes()
	{
		void menu_configuracoes()
		{
			system("clear");
			printf("************\n");
			printf("* Charadas *\n");
			printf("************\n");
			printf("\n");
			printf("1) Alterar os numero de erros maximo(atual: %d)\n\n", numero_erros_maximo);
			printf("0) Voltar ao menu alterior\n");
		}
		while(1)
		{
			menu_configuracoes();
			numero_do_usuario = pega_algarismo(0, 1);
			if(numero_do_usuario == 1)
			{
				menu_configuracoes();
				printf("Digite o novo numero maximo:  ");
				numero_do_usuario = pega_numero_grande();
				if(numero_do_usuario > 1)
					numero_erros_maximo = numero_do_usuario;
				aperte_tecla(ESC);
			}
			else
				return 0;
		}
	}
	void instrucoes()
	{
		system("clear");
	    printf("Esse é o jogo de charadas. É possivel adicionar outras charadas ao codigo, mas isso é outra historia.\n");
	    printf("No momento, somente há uma charada. Para jogar, será apresentada uma charada(ou seja, uma questao) e\n");
	    printf("pede-se para adivinhar a resposta correta dentro de alternativas. Caso o usuario erre mais de duas vezes,\n");
	    printf("ele perde e o jogo e recebe pontuacao zero. Caso acerte a charada de primeira, recebe 50 pontos, e de \n");
	    printf("se acertar de segunda vez, somente 25 pontos.");
	    aperte_tecla(ENTER);
	}
	void rank_jogo()
	{
	  int i;
	  system("clear");
	  printf("*********************************\n");
	  printf("* Bem vindo à sala dos Coringas *\n");
	  printf("*********************************\n");
	  printf("\n");
	  for(i = 0; i < player; i++)
	  {
	    printf("%dº Lugar - ", i+1);
	    imprime_string(((record+1)->w_+i)->nome,espaco);
	    printf(": %d\n", ((record+1)->w_+i)->valor );
	  }
	  aperte_tecla(0);
	}
	while(1)
	{
		menu_opcoes();
		numero_do_usuario = pega_algarismo(0, 4);
		if(numero_do_usuario == 1)
			jogar();
		else if(numero_do_usuario == 2)
			configuracoes();
		else if(numero_do_usuario == 3)
			instrucoes();
		else if(numero_do_usuario == 4)
			rank_jogo();
		else
		{
			limpa_buffer();
			return 0;
		}
		*(posicao+2) = organiza_posicao_completo(record, dado, *(posicao+2), 2);
	}
}
int pedra_papel_tesoura(completo *record, geral *dado, int *posicao)
{
	int numero_do_usuario;
	char pedra = 'p', papel = 'P', tesoura = 'T', jogador[3];
	void menu_opcoes()
	{
		system("clear");
		printf("***********************\n");
		printf("* Pedra Papel Tesoura *\n");
		printf("***********************\n");
		printf("\n");
		printf("1) Jogar\n");
		printf("2) Configurações\n");
		printf("3) Instrucoes\n");
		printf("4) 'Nao sei o que colocar aqui'\n\n");
		printf("0) Sair\n");
		printf("\n");
		printf("Digite uma opcao:  ");
	}
	void jogar()
	{
		int i, terminar;
		void menu_jogar()
		{
			system("clear");
			printf("\t***********************\n");
			printf("\t* Pedra Papel Tesoura *\n");
			printf("\t***********************\n");
			printf("\n");
			printf("\t Pedra  - %c\n", pedra);
			printf("\t Papel  - %c\n", papel);
			printf("\tTesoura - %c\n\n", tesoura);
		}
		int verifica_vencedor()
		{
			if(!jogador[0] || !jogador[1] || !jogador[2])
				return 4;
			if(jogador[0] != jogador[1] && jogador[0] != jogador[2] && jogador[1] != jogador[2])
				return 0;
			if(jogador[0] == jogador[1] && jogador[0] == jogador[2])
				return 0;
			if(jogador[0] == jogador[1])
			{
				if(jogador[2] == pedra && jogador[1] == tesoura)
					return 3;
				else if(jogador[2] == papel && jogador[1] == pedra)
					return 3;
				else if(jogador[2] == tesoura && jogador[1] == papel)
					return 3;
			}
			else if(jogador[1] == jogador[2])
			{
				if(jogador[0] == pedra && jogador[2] == tesoura)
					return 1;
				else if(jogador[0] == papel && jogador[2] == pedra)
					return 1;
				else if(jogador[0] == tesoura && jogador[2] == papel)
					return 1;
			}
			else if(jogador[0] == jogador[2])
			{
				if(jogador[1] == pedra && jogador[2] == tesoura)
					return 2;
				else if(jogador[1] == papel && jogador[2] == pedra)
					return 2;
				else if(jogador[1] == tesoura && jogador[2] == papel)
					return 2;
			}
			return 0;
		}
		int nao_terminou()
		{
			int vencedor = verifica_vencedor();
			if(vencedor)
				return 0;
			return 1;
		}
		char pega_entrada(char a, char b, char c)
		{
			char caracter;
			do
			{
				caracter = getch();
			}while(caracter != a && caracter != b && caracter != c && caracter != ESC);
			if(caracter == ESC)
				return 0;
			return caracter;
		}
		jogador[0] = jogador[1] = jogador[2] = 'a';
		terminar = nao_terminou();
		while(terminar)
		{
			for(i = 0; i<3 && jogador[0] && jogador[1] && jogador[2]; i++)
			{
				menu_jogar();
				printf("Jogador %d, o que deseja jogar?  ", i+1);
				jogador[i] = pega_entrada(pedra, papel, tesoura);
			}
			terminar = nao_terminou();
			printf("\n\nResultado:\n");
			printf("Jogador 1: %s\n", (jogador[0] == pedra ? "Pedra" : (jogador[0] == papel ? "Papel" : "Tesoura")));
			printf("Jogador 2: %s\n", (jogador[1] == pedra ? "Pedra" : (jogador[1] == papel ? "Papel" : "Tesoura")));
			printf("Jogador 3: %s\n", (jogador[2] == pedra ? "Pedra" : (jogador[2] == papel ? "Papel" : "Tesoura")));
			if(terminar)
			{
				printf("\n\nHouve empate, denovo!");
				aperte_tecla(ENTER);
			}
		}
		terminar = verifica_vencedor();
		if(terminar != 4)
		{
			printf("\n\nParabéns ao Jogador %d por ter ganho a partida!\n\n", terminar);
			aperte_tecla(ENTER);
		}
	}
	int configuracoes()
	{
		void menu_configuracoes()
		{
			system("clear");
			printf("***********************\n");
			printf("* Pedra Papel Tesoura *\n");
			printf("***********************\n");
			printf("\n");
			printf("1) Alterar os caracteres de cada opcao:\n Pedra  - %c\n Papel  - %c\nTesoura - %c\n\n", pedra, papel, tesoura);
			printf("0) Voltar ao menu alterior\n");
		}
		while(1)
		{
			menu_configuracoes();
			numero_do_usuario = pega_algarismo(0, 1);
			if(numero_do_usuario == 1)
			{
				menu_configuracoes();
				printf("\nDigite o caracter da pedra:  ");
				pedra = pega_letra('a', 'z', 'A', 'Z');
				putchar(pedra);
				printf("\nDigite o caracter do papel:  ");
				papel = pega_letra('a', 'z', 'A', 'Z');
				putchar(papel);
				printf("\nDigite o caracter da tesoura:  ");
				tesoura = pega_letra('a', 'z', 'A', 'Z');
				putchar(tesoura);
				if(papel == pedra || papel == tesoura || pedra == tesoura)
				{
					printf("\nNão é possivel que duas opcoes tenham os mesmo caracteres. Padrões originais restaurados.");
					pedra = 'p';
					papel = 'P';
					tesoura = 'T';
				}
				aperte_tecla(ENTER);
			}
			else
				return 0;
		}
	}
	void instrucoes()
	{
		system("clear");
		printf("Instrucoes!\n");
	}
	void rank_jogo()
	{
	  int i;
	  system("clear");
	  printf("*****************************************************\n");
	  printf("* Bem vindo à sala dos 'Nao sei o que colocar aqui' *\n");
	  printf("*****************************************************\n");
	  printf("\n");
	  for(i = 0; i < player; i++)
	  {
	    printf("%dº Lugar - ", i+1);
	    imprime_string(((record+2)->w_+i)->nome,espaco);
	    printf(": %d\n", ((record+2)->w_+i)->valor );
	  }
	  aperte_tecla(0);
	}
	while(1)
	{
		menu_opcoes();
		numero_do_usuario = pega_algarismo(0, 4);
		if(numero_do_usuario == 1)
			jogar();
		else if(numero_do_usuario == 2)
			configuracoes();
		else if(numero_do_usuario == 3)
			instrucoes();
		else if(numero_do_usuario == 4)
			rank_jogo();
		else
		{
			limpa_buffer();
			return 0;
		}
		*(posicao+3) = organiza_posicao_completo(record, dado, *(posicao+3), 3);
	}
}
int jogo_2048(completo *record, geral *dado, int *posicao)
{
	int numero_do_usuario, probabilidade=0, recorde=0, pontuacao, tamanho = 6;
	int campo[10][10];
	void menu_opcoes()
	{
	    system("clear");
	    printf("\t********\n");
	    printf("\t* 2048 *\n");
	    printf("\t********\n");
	    printf("\n");
	    printf("  Recorde:  %d\n", recorde);
	    printf("\n");
	    printf("1) Iniciar novo jogo\n");
	    printf("2) Configuracoes\n");
	    printf("3) Instrucoes\n");
	    printf("4) Os Calculistas\n\n");
	    printf("0) Sair\n");
	    printf("\n");
	    printf("Digite a opcao:  ");
	}
	void jogar()
	{
		void menu_jogar()
		{
			system("clear");
			printf("\t********\n");
			printf("\t* 2048 *\n");
			printf("\t********\n");
			printf("\n");
			printf("\t Recorde:    %d\n", recorde);
			printf("\t Pontuacao:  %d\n", pontuacao);
	    }
	    void limpar()
	    {
	      int i, j;
	      for(i = 0; i<tamanho; i++)
	        for(j = 0; j<tamanho; j++)
	          campo[i][j] = 0;
	    }
	    void poe_aleatorio(int quantidade, int maximo_iteracoes)
	    {
	      int i, x, y, quantidade_iteracoes=0;
	      srand((unsigned)time(NULL));
	      for(i = 0; i<quantidade; i++)
	      {
	        do
	        {
	          x = rand()%tamanho;
	          y = rand()%tamanho;
	          quantidade_iteracoes++;
	          if(quantidade_iteracoes > maximo_iteracoes)
	          	return;
	        }while(campo[x][y] != 0);
	        campo[x][y] = (rand()%10 <= probabilidade) ? 4 : 2;
	      }
	    }
	    int pega_direcao()
	    {
	      char caracter;
	      do
	      {
	        caracter = getch();
	      }while(caracter != 'a' && caracter != 's' && caracter != 'd' && caracter != 'w' && caracter != ESC);
	      limpa_buffer();
	      if(caracter == 'a')
	        return 1;
	      else if(caracter == 's')
	        return 2;
	      else if(caracter == 'd')
	        return 3;
	      else if(caracter == 'w')
	        return 4;
	      else
	        return 0;
	    }
	    int mover(int direcao)
	    {
			int i, x, y, moveu = 1;
			if(direcao == 1) /* esquerda */
			{
				for(i = 0; i < tamanho; i++)
				{
					x = 0;
					do
					{
						while(x < tamanho && !campo[i][x])
							x++;
						y = x+1;
						while(y < tamanho && !campo[i][y])
							y++;
						if(x < tamanho && y < tamanho)
						{
							if(campo[i][x] == campo[i][y])
							{
								campo[i][x] *= 2;
								campo[i][y] = 0;
								pontuacao += campo[i][x];
								moveu++;
							}
							else
							{
								x = y;
								y ++;
							}
						}
					}while(x < tamanho && y < tamanho);
				}
				for(i = 0; i < tamanho; i++)
				{
					x = 0;
					do
					{
						while(campo[i][x] && x < tamanho)
							x++;
						y = x+1;
						while(!campo[i][y] && y < tamanho)
							y++;
						if(x < tamanho && y < tamanho && !campo[i][x] && campo[i][y])
						{
							campo[i][x] = campo[i][y];
							campo[i][y] = 0;
							moveu++;
						}
						x++;
					}while(y < tamanho && x < tamanho);
				}
			}
			else if(direcao == 2) /* baixo */
			{
				for(i = 0; i < tamanho; i++)
				{
					x = tamanho-1;
					do
					{
						while(x >= 0 && !campo[x][i])
							x--;
						y = x-1;
						while(y >= 0 && !campo[y][i])
							y--;
						if(x >= 0 && y >= 0)
						{
							if(campo[x][i] == campo[y][i])
							{
								campo[x][i] *= 2;
								campo[y][i] = 0;
								pontuacao += campo[x][i];
								moveu++;
							}
							else
							{
								x = y;
								y--;
							}
						}
					}while(x >= 0 && y >= 0);
				}
				for(i = 0; i < tamanho; i++)
				{
					x = tamanho - 1;
					do
					{
						while(campo[x][i] && x >= 0)
							x--;
						y = x-1;
						while(!campo[y][i] && y >= 0)
							y--;
						if(x >= 0 && y >= 0 && !campo[x][i] && campo[y][i])
						{
							campo[x][i] = campo[y][i];
							campo[y][i] = 0;
							moveu++;
						}
						x--;
					}while(y >= 0 && x >=0 );
				}	
			}
			else if(direcao == 3) /* direita */
			{
				for(i = 0; i < tamanho; i++)
				{
					x = tamanho-1;
					do
					{
						while(x >= 0 && !campo[i][x])
							x--;
						y = x-1;
						while(y >= 0 && !campo[i][y])
							y--;
						if(x >= 0 && y >= 0)
						{
							if(campo[i][x] == campo[i][y])
							{
								campo[i][x] *= 2;
								campo[i][y] = 0;
								pontuacao += campo[i][x];
								moveu++;
							}
							else
							{
								x = y;
								y--;
							}
						}
					}while(x >= 0 && y >= 0);
				}
				for(i = 0; i < tamanho; i++)
				{
					x = tamanho - 1;
					do
					{
						while(campo[i][x] && x >= 0)
							x--;
						y = x-1;
						while(!campo[i][y] && y >= 0)
							y--;
						if(x >= 0 && y >= 0 && !campo[i][x] && campo[i][y])
						{
							campo[i][x] = campo[i][y];
							campo[i][y] = 0;
							moveu++;
						}
						x--;
					}while(y >= 0 && x >= 0);
				}
			}
			else if(direcao == 4)/* cima */
			{
				for(i = 0; i < tamanho; i++)
				{
					x = 0;
					do
					{
						while(x < tamanho && !campo[x][i])
							x++;
						y = x+1;
						while(y < tamanho && !campo[y][i])
							y++;
						if(x < tamanho && y < tamanho)
						{
							if(campo[x][i] == campo[y][i])
							{
								campo[x][i] *= 2;
								campo[y][i] = 0;
								pontuacao += campo[x][i];
								moveu++;
							}
							else
							{
								x = y;
								y++;
							}
						}
					}while(x < tamanho && y < tamanho);
				}
				for(i = 0; i < tamanho; i++)
				{
					x = 0;
					do
					{
						while(campo[x][i] && x < tamanho)
							x++;
						y = x+1;
						while(!campo[y][i] && y < tamanho)
							y++;
						if(x < tamanho && y < tamanho && !campo[x][i] && campo[y][i])
						{
							campo[x][i] = campo[y][i];
							campo[y][i] = 0;
							moveu++;
						}
						x++;
					}while(y < tamanho && x < tamanho);
				}
			}
			else
				return 0;
			return moveu;
	    }
	    void imprime()
	    {
	      int i, j;
	      printf("\n\n");
	      for(i=0; i<tamanho; i++)
	      {
	        for(j=0; j<tamanho; j++)
	        {
	          if(campo[i][j] < 10)
	            printf("  %d  ",campo[i][j]);
	          else if(campo[i][j] < 100)
	            printf(" %d  ", campo[i][j]);
	          else if(campo[i][j] < 1000)
	            printf(" %d ", campo[i][j]);
	          else
	            printf("%d ", campo[i][j]);
	        }
	        printf("\n\n");
	      }
	      printf("\n\n");
	    }
	    int nao_terminou()
	    {
	    	int i, j;
	    	if(!numero_do_usuario)
	    		return 0;
	    	for(i = 0; i<tamanho; i++)
	    		for(j = 0; j < tamanho; j++)
	    			if(campo[i][j] == 2048)
	    				return 0;
	    	for(i = 0; i < tamanho; i++)
	    		for(j = 0; j < tamanho; j++)
	    			if(!campo[i][j])
	    				return 1;
	    	i = j = 0;
	    	if(campo[i][j] == campo[i][j+1] || campo[i][j] == campo[i+1][j])
	    		return 1;
	    	i = tamanho-1;
	    	if(campo[i][j] == campo[i][j+1] || campo[i][j] == campo[i-1][j])
	    		return 1;
	    	j = tamanho-1;
	    	if(campo[i][j] == campo[i][j-1] || campo[i][j] == campo[i-1][j])
	    		return 1;
	    	i = 0;
	    	if(campo[i][j] == campo[i][j-1] || campo[i][j] == campo[i+1][j])
	    		return 1;
	    	for(i = 1; i < tamanho-1; i++)
	    		for(j = 1; j < tamanho-1; j++)
	    		{
	    			if(campo[i][j] == campo[i][j+1] || campo[i][j] == campo[i+1][j])
	    				return 1;
	    			if(campo[i][j] == campo[i][j-1] || campo[i][j] == campo[i-1][j])
	    				return 1;
	    		}
	    	return 0;
	    }
	    void fim_de_jogo()
	    {
	    	int i, j;
	    	menu_jogar();
		    printf("\ta - Esquerda\n");
	    	printf("\td - Direita \n");
	    	printf("\tw - Subir   \n");
	    	printf("\ts - Descer  \n");
	    	printf("\tESC - Sair\n\n");
	    	numero_do_usuario = 0;
	    	for(i = 0; i<tamanho; i++)
		    		for(j = 0; j < tamanho; j++)
		    			if(campo[i][j] == 2048)
		    				numero_do_usuario = 1;
		    printf("%s", numero_do_usuario ? "Parabéns, você ganhou!" : "Que pega, você perdeu :( ");
		    printf("\nSua pontuação é %d. %s", pontuacao, pontuacao == recorde ? "Você quebrou o recorde" : "Ainda não conseguistes quebrar o recorde");
		    imprime();
		    aperte_tecla(ENTER);
	    }
	    pontuacao = 0;
	    numero_do_usuario = 2;
	    limpar();
	    poe_aleatorio(4, 100);
	    printf("%d\n", numero_do_usuario);
	    while(nao_terminou())
	    {
	    	menu_jogar();
	    	printf("\ta - Esquerda\n");
	    	printf("\td - Direita \n");
	    	printf("\tw - Subir   \n");
	    	printf("\ts - Descer  \n");
	    	printf("\tESC - Sair\n\n");
	    	printf("%s\n", numero_do_usuario == 1 ? "Nao foi possivel fazer tal movimento, digite um movimento valido" : " ");
	    	imprime();
	    	numero_do_usuario = mover(pega_direcao());
	    	limpa_buffer();
	    	if(numero_do_usuario > 1)
	    	{
		    	poe_aleatorio(1, 100);
		    	recorde = (pontuacao > recorde ? pontuacao : recorde);
	    	}
	    }
	    fim_de_jogo();
	}
	int configuracoes()
	{
		void menu_configuracoes()
		{
		  system("clear");
		  printf("\t********\n");
		  printf("\t* 2048 *\n");
		  printf("\t********\n");
		  printf("\n");
		  printf("1) Alterar a probabilidade de aparecer 4(atual: %d0%%)\n\n", probabilidade+1);
		  printf("0) Voltar ao menu alterior\n");
		}
		while(1)
		{
		  menu_configuracoes();
		  numero_do_usuario = pega_algarismo(0, 1);
		  if(numero_do_usuario == 1)
		  {
		    menu_configuracoes();
		    printf("\nDigite a nova probabilidade(entre 1 e 9):  ");
		    numero_do_usuario = pega_algarismo(0, 9);
		    if(numero_do_usuario)
		    {
		      probabilidade = numero_do_usuario;
		    }
		    aperte_tecla(ENTER);
		  }
		  else
		    return 0;
		}
	}
	void instrucoes()
	{
		system("clear");
		printf("Instrucoes!\n");
		aperte_tecla(ENTER);
	}
	void rank_jogo()
	{
	  int i;
	  system("clear");
	  printf("************************************\n");
	  printf("* Bem vindo à sala dos Calculistas *\n");
	  printf("************************************\n");
	  printf("\n");
	  for(i = 0; i < player; i++)
	  {
	    printf("%dº Lugar - ", i+1);
	    imprime_string(((record+3)->w_+i)->nome,espaco);
	    printf(": %d\n", ((record+3)->w_+i)->valor );
	  }
	  aperte_tecla(0);
	}
	while(1)
	{
		menu_opcoes();
		numero_do_usuario = pega_algarismo(0, 4);
		if(numero_do_usuario == 1)
			jogar();
		else if(numero_do_usuario == 2)
			configuracoes();
		else if(numero_do_usuario == 3)
			instrucoes();
		else if(numero_do_usuario == 4)
			rank_jogo();
		else
		{
			limpa_buffer();
			return 0;
		}
		*(posicao+4) = organiza_posicao_completo(record, dado, *(posicao+4), 4);
	}
}
void jogo_da_velha(completo *record, geral *dado, int *posicao) /* totalmente terminado */
{
  char position[3][3], resposta[5];
  int res, pont_1=0, pont_2=0, pont_3=0, limite=0;
  void imprime(int jogador)
  {
    system("clear");
    printf("\n");
    printf("\n");
    printf("*****************\n");
    printf("* Jogo da velha *\n");
    printf("*****************\n");
    printf("\n");
    printf("   Limite:  %d jogos\n", limite);
    printf("Jogador 1:  %d vitorias\n", pont_1);
    printf("Jogador 2:  %d vitorias\n", pont_2);
    printf("  Empates:  %d\n", pont_3);
    printf("\n");
    printf("     1     2     3\n");
    printf("        |     |    \n");
    printf("a    %c  |  %c  |  %c \n", position[0][0], position[0][1], position[0][2]);
    printf("        |     |    \n");
    printf("   -----+-----+-----\n");
    printf("        |     |    \n");
    printf("b    %c  |  %c  |  %c \n", position[1][0], position[1][1], position[1][2]);
    printf("        |     |    \n");
    printf("   -----+-----+-----\n");
    printf("        |     |    \n");
    printf("c    %c  |  %c  |  %c \n", position[2][0], position[2][1], position[2][2]);
    printf("        |     |    \n");
    printf("\n\n");
    if(jogador == 1)
      printf("Jogador 1, qual casa quer colocar com X?  ");
    else if(jogador == 2)
      printf("Jogador 2, qual casa quer colocar com O?  ");
  }
  void limpa()
  {
    int i, j;
    for(i=0; i<=3; i++)
      for(j=0; j<3; j++)
        position[i][j] = ' ';
  }
  int resultado_do_jogo()
  {
    int i, j;
    if((position[0][0] == position[1][1] && position[1][1] == position[2][2]) || (position[2][0] == position[1][1] && position[1][1] == position[0][2]))
    {
      if(position[1][1] == 'X')
        return 1;
      else if(position[1][1] == 'O')
        return 2;
    }
    for(i = 0; i<3; i++)
    {
      if(position[i][0] == position[i][1] && position[i][1] == position[i][2])
      {
        if(position[i][0] == 'X')
          return 1;
        else if(position[i][0] == 'O')
          return 2;
      }
      if(position[0][i] == position[1][i] && position[1][i] == position[2][i])
      {
        if(position[0][i] == 'X')
          return 1;
        else if(position[0][i] == 'O')
          return 2;
      }
    }
    for(i = 0; i < 3; i++)
      for(j = 0; j < 3 ; j++)
        if(position[i][j] == ' ')
          return 0;
    return 3;
  }
  int escreve_opcao(int jogador)
  {
    int x , y;
    if(resposta[0] == 'a')
      x = 0;
    else if(resposta[0] == 'b')
      x = 1;
    else if(resposta[0] == 'c')
      x = 2;
    else
      return 1;
    if(resposta[1] == '1')
      y = 0;
    else if(resposta[1] == '2')
      y = 1;
    else if(resposta[1] == '3')
      y = 2;
    else
      return 1;
    if(position[x][y] != ' ')
      return 1;
    if(jogador == 1)
      position[x][y] = 'X';
    else
      position[x][y] = 'O';
    return 0;
  }
  limpa();
  imprime(3);
  printf("Qual a quantidade de jogos limite?  ");
  scanf(" %d", &limite);
  while(pont_1 < limite && pont_2 < limite && pont_3 < limite)
  {
    do
    {
      res = resultado_do_jogo();
      if(!res)
      {
        do
        {
          imprime(1);
          scanf(" %s", resposta);
        }while(escreve_opcao(1));
      }
      res = resultado_do_jogo();
      if(!res)
      {
        do
        {
          imprime(2);
          scanf(" %s", resposta);
        }while(escreve_opcao(2));
      }
    }while(!res);
    imprime(3);
    if(res == 1)
    {
      printf("Jogador 1 venceu!\n");
      pont_1++;
    }
    else if(res == 2)
    {
      printf("Jogador 2 venceu!\n");
      pont_2++;
    }
    else
    {
      printf("Deu velha!\n");
      pont_3++;
    }
    aperte_tecla(ENTER);
    limpa();
  }
  imprime(3);
  if(pont_1 == limite)
    printf("Parabéns ao Jogador 1, ganhou %d jogos!", limite);
  else if(pont_2 == limite)
    printf("Parabéns ao Jogador 2, ganhou %d jogos!", limite);
  else
  {
    if(pont_1 > pont_2)
      printf("Parabéns ao Jogador 1, ganhou %d jogos, contra %d do Jogador 2", pont_1, pont_2);
    if(pont_2 > pont_1)
      printf("Parabéns ao Jogador 1, ganhou %d jogos, contra %d do Jogador 2", pont_2, pont_1);
    else
      printf("Empatado! Não há nenhum vencedor!\n");
  }
  aperte_tecla(ENTER);
}
void jogo_da_memoria(completo *record, geral *dado, int *posicao) /* totalmente terminado */
{
  int tamanho = 9, opcao, absi[2], orde[2], contador, final, tentativas;
  int quantidade;
  char gabarito[tamanho][tamanho], usuario[tamanho][tamanho];
  do{
    system("clear");
    printf("Digite o nível de dificuldade(entre 1 e 8):  ");
  } while(scanf("%d", &tamanho) && (tamanho < 1 || tamanho > 8));
  tamanho++;
  quantidade = (tamanho*tamanho)/2;
  int resp() /* essa funcao recolhe a entrada do usuario e retorna um número, por exemplo, resp(*a1*) = 11 */
  {
    int verificao = 0;
    char resposta[5];
    do
    {
      scanf(" %s", resposta);
      if(resposta[0] == 's' || resposta[0] == 'S')
        return 0;
      if(resposta[0] < 97 || resposta[0] > (97+tamanho)) /* Aqui verifica se o primeiro caracter é um caracter válido, a~h */
        verificao = 1;
      else if(resposta[1] < 48 || resposta[1] > (48+tamanho)) /* Aqui verifica se o segundo caracter é um caracter valido, 1~9 */
        verificao = 1;
      else if(tamanho%2 && !(((int)resposta[0]) - 96-tamanho) && !(((int)resposta[1]) - 48-tamanho)) /* Aqui verifica se em caso de tamanho impar, o ultimo termo será pego */
        verificao = 1;
      else if(usuario[(int)resposta[0] - 97][((int)resposta[1]) - 49] != '&')
        verificao = 1;
      else
        verificao = 0;
    }while(verificao);
    return (10*(((int)resposta[0]) - 96) + (((int)resposta[1]) - 48));
  }
  void menu_memoria()
  {
    system("clear");
    printf("\n");
    printf("\n");
    printf("*******************\n");
    printf("* Jogo da Memória *\n");
    printf("*******************\n");
    printf("\n");
    printf("\n");
  }
  void limpa_usuario()
  {
    int i, j;
    for(i=0; i<tamanho; i++)
      for(j = 0; j<tamanho; j++)
        usuario[i][j] = '&';
    if(tamanho%2)
      usuario[tamanho-1][tamanho-1] = ' ';
  }
  void novo_gabarito()
  {
    int i, x, y;
    char caracter = 'A';
    srand((unsigned)time(NULL));
    for(x=0; x<tamanho; x++)
      for(y = 0; y<tamanho; y++)
        gabarito[x][y] = '&';
    if(tamanho%2)
      gabarito[tamanho-1][tamanho-1] = ' ';
    for(i = 0; i < quantidade; i++)
    {
      do
      {
        x = rand()%tamanho;
        y = rand()%tamanho;
      }while(gabarito[x][y] != '&');
      gabarito[x][y] = (char)(((int)caracter) + i);
      do
      {
        x = rand()%tamanho;
        y = rand()%tamanho;
      }while(gabarito[x][y] != '&');
      gabarito[x][y] = (char)(((int)caracter) + i);
    }
  }
  void imprime()
  {
    int i, j;
    printf("\n");
    printf("   ");
    for(i = 0; i<tamanho; i++)
      printf("%d ", i+1);
    printf("\n");
    for(i = 0; i < tamanho; i++)
    {
      printf("%c  ", (char)(i+((int)'a')));
      for(j = 0; j < tamanho; j++)
        printf("%c ", usuario[i][j]);
      printf("\n");
    }
    printf("\n\n");
  }
  menu_memoria();
  printf("1) Jogar\n");
  printf("2) Sair\n");
  while(scanf("%d", &opcao) && opcao != 2)
  {
    if( opcao == 1 )
    {
      printf("Hey\n");
      tentativas = 0;
      final = 0;
      novo_gabarito();
      limpa_usuario();
      contador = 0;
      printf("Hey\n");
      do{
        printf("Hey\n");
        tentativas++;
        menu_memoria();
        imprime();
        printf("Digite o valor da casa a ser aberta: ");
        opcao = resp();
        absi[contador] = (opcao / 10) - 1;
        orde[contador] = (opcao % 10) - 1;
        if(opcao)
        {
          usuario[absi[contador]][orde[contador]] = gabarito[absi[contador]][orde[contador]];
          
        }
        if(contador)
        {
          if(usuario[absi[0]][orde[0]] != usuario[absi[1]][orde[1]])
          {
            menu_memoria();
            imprime();
            aperte_tecla(ENTER);
            usuario[absi[0]][orde[0]] = usuario[absi[1]][orde[1]] = '&';
          }
          else
            final++;
          contador = -1;
        }
        contador++;
      }while(opcao && final < quantidade);
      menu_memoria();
      imprime();
      printf("Parabéns, você conseguiu com %d tentativas!\n", tentativas/2);
      aperte_tecla(ENTER);
    }
    menu_memoria();
    printf("1) Jogar\n");
    printf("2) Sair\n");
  }
}
void xadrez(completo *record, geral *dado, int *posicao)  /* totalmente terminado */
{
  char tab[8][8], pos[8][8];
  int x, y, a, b, opcao=1, ajuste;
  void novo()
  {
    int i, j;
    for(i = 2; i<6; i++)
      for(j = 0; j<8; j++)
        tab[i][j] = ' ';
    for(j = 0; j<8; j++)
    {
      tab[1][j] = 'I';
      tab[6][j] = 'i';
    }
    tab[0][0] = tab[0][7] = 'T';
    tab[0][1] = tab[0][6] = 'C';
    tab[0][2] = tab[0][5] = 'B';
    tab[0][3] = 'Q';
    tab[0][4] = 'K';
    tab[7][0] = tab[7][7] = 't';
    tab[7][1] = tab[7][6] = 'c';
    tab[7][2] = tab[7][5] = 'b';
    tab[7][3] = 'q';
    tab[7][4] = 'k';
  }
  void limpa_pos()
  {
    int i, j;
    for(i=0; i<8; i++)
      for(j=0; j<8; j++)
        pos[i][j] = ' ';
  }
  void menu(int n)
  {
    system("clear");
    printf("\t          \n");
    printf("\t**********\n");
    printf("\t* Xadrez *\n");
    printf("\t**********\n");
    printf("\n");
    if(n == 1)
    {
      printf("K ou k = rei\t\t\t\tQ ou q = rainnha\n");
      printf("B ou b = bispo\t\t\t\tC ou c = cavalo\n");
      printf("T ou t = torre\t\t\t\tI ou i = peao\n");
    }
    else if( n==2)
    {
      printf("1) Novo jogo\n");
      printf("2) Continuar\n");
      printf("3) Sair\n");
      printf("\n");
      printf("Digite sua opcao:  ");
    }
  }
  void imprime()
  {
    int i, j;
    printf("\n       ");
    for(i = 1; i<9; i++)
      printf("   %d    ", i);
    printf("\n      +~~~~~~~+~~~~~~~+~~~~~~~+~~~~~~~+~~~~~~~+~~~~~~~+~~~~~~~+~~~~~~~+\n");
    for(i=0; i<8; i++)
    {
      printf("      |");
      for(j=0; j<8; j++)
      {
        printf(" %c%c%c%c%c |", pos[i][j], pos[i][j], pos[i][j], pos[i][j], pos[i][j]);
        /*if((j+i)%2)
          putchar('&');
        else
          putchar(' ');
        printf("%c%c%c%c%c", pos[i][j], pos[i][j], pos[i][j], pos[i][j], pos[i][j]);
        if((j+i)%2)
          putchar('&');
        else
          putchar(' ');
        putchar('|');*/
      }
      printf("\n  %c   |", (char)(i+97));
      for(j = 0; j < 8; j++)
      {
        printf(" %c %c %c |", pos[i][j], tab[i][j], pos[i][j]);
        /*if((j+i)%2)
          putchar('&');
        else
          putchar(' ');
        printf("%c %c %c", pos[i][j], tab[i][j], pos[i][j]);

        if((j+i)%2)
          putchar('&');
        else
          putchar(' ');
        putchar('|');*/
      }
      printf("\n      |");
      for(j=0; j<8; j++)
      {
        printf(" %c%c%c%c%c |", pos[i][j], pos[i][j], pos[i][j], pos[i][j], pos[i][j]);
        /*if((j+i)%2)
          putchar('&');
        else
          putchar(' ');
        printf("%c%c%c%c%c", pos[i][j], pos[i][j], pos[i][j], pos[i][j], pos[i][j]);
        if((j+i)%2)
          putchar('&');
        else
          putchar(' ');
        putchar('|'); */
      }
      printf("\n      +~~~~~~~+~~~~~~~+~~~~~~~+~~~~~~~+~~~~~~~+~~~~~~~+~~~~~~~+~~~~~~~+\n");
    }
    printf("\n\n");
  }
  void leia_usuario(int *x, int *y)
  {
    char str[10];
    while(scanf(" %s", str) && (!(str[0] >= 'a' && str[0] <= 'h') || !(str[1] >= '1' && str[1] <= '8')) && str[0] != 's' );
    *x = str[0] - 'a';
    *y = str[1] - '1';
    if(str[0] == 's')
      *x = -1;
  }
  int move_peca()
  {
    if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[a][b] <= 'z' && tab[a][b] >= 'a')
      return 0;
    if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[a][b] <= 'Z' && tab[a][b] >= 'A')
      return 0;
    
    tab[a][b] = tab[x][y];
    tab[x][y] = ' ';
    return 1;
  }
  int verifica_movimento()
  {
    int i, j;
    void ve_torre()
    {
      int i = 1, j = y-1;
      while(j >= 0 && i)
      {
        if(tab[x][j] == ' ')
          pos[x][j] = '.';
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x][j] >= 'a' && tab[x][j] <= 'z')
          i = 0;
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x][j] >= 'A' && tab[x][j] <= 'Z')
          i = 0;
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x][j] >= 'A' && tab[x][j] <= 'Z')
        {
          pos[x][j] = '!';
          i = 0;
        }
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x][j] >= 'a' && tab[x][j] <= 'z')
        {
          pos[x][j] = '!';
          i = 0;
        }
        j--;
      }
      i = 1;
      j = y+1;
      while(j < 8 && i)
      {
        if(tab[x][j] == ' ')
          pos[x][j] = '.';
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x][j] >= 'a' && tab[x][j] <= 'z')
          i = 0;
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x][j] >= 'A' && tab[x][j] <= 'Z')
          i = 0;
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x][j] >= 'A' && tab[x][j] <= 'Z')
        {
          pos[x][j] = '!';
          i = 0;
        }
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x][j] >= 'a' && tab[x][j] <= 'z')
        {
          pos[x][j] = '!';
          i = 0;
        }
        j++;
      }
      i = 1;
      j = x-1;
      while(j >= 0 && i)
      {
        if(tab[j][y] == ' ')
          pos[j][y] = '.';
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[j][y] >= 'a' && tab[j][y] <= 'z')
          i = 0;
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[j][y] >= 'A' && tab[j][y] <= 'Z')
          i = 0;
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[j][y] >= 'A' && tab[j][y] <= 'Z')
        {
          pos[j][y] = '!';
          i = 0;
        }
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[j][y] >= 'a' && tab[j][y] <= 'z')
        {
          pos[j][y] = '!';
          i = 0;
        }
        j--;
      }
      i = 1;
      j = x+1;
      while(j < 8 && i)
      {
        if(tab[j][y] == ' ')
          pos[j][y] = '.';
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[j][y] >= 'a' && tab[j][y] <= 'z')
          i = 0;
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[j][y] >= 'A' && tab[j][y] <= 'Z')
          i = 0;
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[j][y] >= 'A' && tab[j][y] <= 'Z')
        {
          pos[j][y] = '!';
          i = 0;
        }
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[j][y] >= 'a' && tab[j][y] <= 'z')
        {
          pos[j][y] = '!';
          i = 0;
        }
        j++;
      }
    }
    void ve_bispo()
    {
      int i = 1, j = -1, k=-1;
      while(x+j >= 0 && y+k >= 0 && i)
      {
        if(tab[x+j][y+k] == ' ')
          pos[x+j][y+k] = '.';
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x+j][y+k] >= 'a' && tab[x+j][y+k] <= 'z')
          i = 0;
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x+j][y+k] >= 'A' && tab[x+j][y+k] <= 'Z')
          i = 0;
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x+j][y+k] >= 'A' && tab[x+j][y+k] <= 'Z')
        {
          pos[x+j][y+k] = '!';
          i = 0;
        }
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x+j][y+k] >= 'a' && tab[x+j][y+k] <= 'z')
        {
          pos[x+j][y+k] = '!';
          i = 0;
        }
        j--;
        k--;
      }
      i = 1;
      j = 1;
      k = -1;
      while(x+j < 8 && y+k >= 0 && i)
      {
        if(tab[x+j][y+k] == ' ')
          pos[x+j][y+k] = '.';
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x+j][y+k] >= 'a' && tab[x+j][y+k] <= 'z')
          i = 0;
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x+j][y+k] >= 'A' && tab[x+j][y+k] <= 'Z')
          i = 0;
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x+j][y+k] >= 'A' && tab[x+j][y+k] <= 'Z')
        {
          pos[x+j][y+k] = '!';
          i = 0;
        }
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x+j][y+k] >= 'a' && tab[x+j][y+k] <= 'z')
        {
          pos[x+j][y+k] = '!';
          i = 0;
        }
        k--;
        j++;
      }
      i = 1;
      j = k = 1;
      while(x+j < 8 && y+k < 8 && i)
      {
        if(tab[x+j][y+k] == ' ')
          pos[x+j][y+k] = '.';
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x+j][y+k] >= 'a' && tab[x+j][y+k] <= 'z')
          i = 0;
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x+j][y+k] >= 'A' && tab[x+j][y+k] <= 'Z')
          i = 0;
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x+j][y+k] >= 'A' && tab[x+j][y+k] <= 'Z')
        {
          pos[x+j][y+k] = '!';
          i = 0;
        }
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x+j][y+k] >= 'a' && tab[x+j][y+k] <= 'z')
        {
          pos[x+j][y+k] = '!';
          i = 0;
        }
        k++;
        j++;
      }
      i = 1;
      j = -1;
      k = 1;
      while(x+j >= 0 && y+k < 8 && i)
      {
        if(tab[x+j][y+k] == ' ')
          pos[x+j][y+k] = '.';
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x+j][y+k] >= 'a' && tab[x+j][y+k] <= 'z'){
          i = 0;
        }
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x+j][y+k] >= 'A' && tab[x+j][y+k] <= 'Z')
          i = 0;
        else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x+j][y+k] >= 'A' && tab[x+j][y+k] <= 'Z')
        {
          pos[x+j][y+k] = '!';
          i = 0;
        }
        else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x+j][y+k] >= 'a' && tab[x+j][y+k] <= 'z')
        {
          pos[x+j][y+k] = '!';
          i = 0;
        }
        k++;
        j--;
      }
    }
    void ve_cavalo()
    {
      int i, j;
      for(i = -2; i<3; i++)
      {
        for(j = -2; j<3; j++)
        {
          if(j && i && abs(i) != abs(j) && (x+i) < 8 && (x+i) >= 0 && (y+j) < 8 && (y+j) >= 0)
          {
            if(tab[x+i][y+j] == ' ')
              pos[x+i][y+j] = '.';
            else if(tab[x][y] <= 'z' && tab[x][y] >= 'a' && tab[x+i][y+j] >= 'A' && tab[x+i][y+j] <= 'Z')
              pos[x+i][y+j] = '!';
            else if(tab[x][y] <= 'Z' && tab[x][y] >= 'A' && tab[x+i][y+j] >= 'a' && tab[x+i][y+j] <= 'z')
              pos[x+i][y+j] = '!';
          }
        }
      }
    }
    void ve_peao()
    {
      if(tab[x][y] == 'i')
      {
        if(tab[x-1][y] == ' ')
          pos[x-1][y]='.';
        if(x > 0 && (tab[x-1][y-1] >= 'A' && tab[x-1][y-1] <= 'Z'))
          pos[x-1][y-1] = '!';
        if(x < 7 && (tab[x-1][y+1] >= 'A' && tab[x-1][y+1] <= 'Z'))
          pos[x-1][y+1] = '!';
      }
      else if(tab[x][y] == 'I')
      {
        if(tab[x+1][y] == ' ')
          pos[x+1][y]='.';
        if(x > 0 && (tab[x+1][y-1] >= 'a' && tab[x+1][y-1] <= 'z'))
          pos[x+1][y-1] = '!';
        if(x < 7 && (tab[x+1][y+1] >= 'a' && tab[x+1][y+1] <= 'z'))
          pos[x+1][y+1] = '!';
      }
    }
    void ve_rei()
    {
      int i, j, a = 0, b = 0;
      if(tab[x][y] == 'k')
      {
        for(i = -1; i<2; i++)
          for(j=-1; j<2; j++)
            if((i || j) && (x-i) >= 0 && (x-i) < 8 && (y-j) >= 0 && (y-j) < 8)
            {
              if(tab[x-i][y-j] == ' ')
                pos[x-i][y-j]='.';
              else if(tab[x-i][y-j] >= 'A' && tab[x-i][y-j] <= 'Z')
                pos[x-i][y-j] = '!';
            }
        for(i = 0; i<8 && tab[a][b] != 'K' ; i++)
          for(j = 0; j<8 && tab[a][b] != 'K'; j++)
            if(tab[i][j] == 'K')
            {
              a = i;
              b = j;
            }
        for(i = -1; i<2; i++)
          for(j=-1; j<2; j++)
            if((a-i) >= 0 && (a-i) < 8 && (b-j) >= 0 && (b-j) < 8)
              pos[a-i][b-j] = ' ';
      }
      else if(tab[x][y] == 'K')
      {
        for(i = -1; i<2; i++)
          for(j=-1; j<2; j++)
            if((i || j) && (x-i) >= 0 && (x-i) < 8 && (y-j) >= 0 && (y-j) < 8)
            {
              if(tab[x-i][y-j] == ' ')
                pos[x-i][y-j]='.';
              else if(tab[x-i][y-j] >= 'a' && tab[x-i][y-j] <= 'z')
                pos[x-i][y-j] = '!';
            }
        for(i = 0; i<8 && tab[a][b] != 'k' ; i++)
          for(j = 0; j<8 && tab[a][b] != 'k'; j++)
            if(tab[i][j] == 'k')
            {
              a = i;
              b = j;
            }
        for(i = -1; i<2; i++)
          for(j=-1; j<2; j++)
            if((a-i) >= 0 && (a-i) < 8 && (b-j) >= 0 && (b-j) < 8)
              pos[a-i][b-j] = ' ';
      }
    }
    if(tab[x][y] == 'i' || tab[x][y] == 'I')
      ve_peao();
    else if(tab[x][y] == 'c' || tab[x][y] == 'C')
      ve_cavalo();
    else if(tab[x][y] == 'b' || tab[x][y] == 'B')
      ve_bispo();
    else if(tab[x][y] == 't' || tab[x][y] == 'T')
      ve_torre();
    else if(tab[x][y] == 'k' || tab[x][y] == 'K')
      ve_rei();
    else if(tab[x][y] == 'q' || tab[x][y] == 'Q')
    {
      ve_torre();
      ve_bispo();
    }
    for(i = 0; i<8; i++)
      for(j = 0; j<8; j++)
        if(pos[i][j] != ' ')
          return 1;
    return 0;
  }
  int rei_morto()
  {
    int i, j, a = 0;
    for(i = 0; i<8; i++)
    {
      for(j = 0; j<8; j++)
      {
        if(tab[i][j] == 'k')
          a = 1;
      }
    }
    if(!a)
      return 2;
    a = 0;
    for(i = 0; i<8; i++)
    {
      for(j = 0; j<8; j++)
      {
        if(tab[i][j] == 'K')
          a = 1;
      }
    }
    if(!a)
      return 1;
    return 0;
  }
  novo();
  menu(2);
  while(scanf("%d", &opcao) && opcao != 3)
  {
    menu(0);
    if(opcao == 1)
      novo();
    if(opcao < 3 && opcao > 0)
    {
      ajuste = 0;
      do
      {
        menu(1);
        limpa_pos();
        imprime();
        printf("Digite as casas a serem acessadas:\n");
        ajuste++;
        ajuste%=2;
        do{
          
          do
          {
            limpa_pos();
              leia_usuario(&x, &y);
              a = verifica_movimento();
          }while(tab[x][y] == ' ' || (!ajuste && !(tab[x][y] <= 'Z' && tab[x][y] >= 'A')) || (ajuste && !(tab[x][y] <= 'z' && tab[x][y] >= 'a')) || !a);
          menu(1);
          imprime();
          printf("Digite as casas a serem acessadas:\n");
          do
          {
             leia_usuario(&a, &b);
             if(pos[a][b] == ' ' && !(a == x && b == y))
               printf("Comando não valido, digite um comando valido:  ");
          }while(pos[a][b] == ' ' && !(a == x && b == y));
          if(x == a && y == b && x != -1)
          {
            opcao = 1;
            menu(1);
            limpa_pos();
            imprime();
            printf("Não foi possivel fazer tal movimento\n");
            printf("Digite as casas a serem acessadas:\n");
          }
          else
            opcao = 0;
        }while(opcao);
        move_peca();
      }while(x >= 0 && a >= 0 && !rei_morto());
      menu(1);
      limpa_pos();
      imprime();
      printf("Jogador %d ganhou!\n", rei_morto());
      aperte_tecla(ENTER);
      menu(2);
    }
    else
    {
      menu(2);
      printf("Comando não possivel, digite um comando válido:  ");
    }
  }
  
  
  
  aperte_tecla(ENTER);
}
int campo_minado(completo *record, geral *dado, int *posicao)
{
	int numero_do_usuario, dificuldade=1, tamanho = 8, bombas = 2, fim, x, y, i, j, pontuacao;
	char gabarito[32][32], campo[32][32];
	void menu_opcoes()
	{
		system("clear");
		printf("****************\n");
		printf("* Campo Minado *\n");
		printf("****************\n");
		printf("\n");
		printf("1) Jogar\n");
		printf("2) Configurações\n");
		printf("3) Instrucoes\n");
		printf("4) Desarmadores de Bombas\n\n");
		printf("0) Sair\n");
		printf("\n");
		printf("Digite uma opcao:  ");
	}
	void jogar()
	{
		int coordenada[2] = {1,1};
		void menu_jogar()
		{
			system("clear");
			printf("****************\n");
			printf("* Campo Minado *\n");
			printf("****************\n");
			printf("\n");
		}
		void novo_campo()
		{
			srand((unsigned)time(NULL));
			for(i = 0; i<32; i++)
				for(j = 0; j<32; j++)
					campo[i][j] = '&';
			for(i = 0; i<32; i++)
				for(j = 0; j<32; j++)
					gabarito[i][j] = '0';
			i = 0;
			while(i < bombas)
			{
				do
				{
					x = rand()%tamanho;
					y = rand()%tamanho;
				}while(gabarito[x][y] == 'X');
				gabarito[x][y] = 'X';
				i++;
			}
			for(i = 0; i < tamanho; i++)
				for(j = 0; j < tamanho; j++)
					if(gabarito[i][j] == 'X')
						for(x = -1; x < 2; x++)
							for(y = -1; y < 2; y++)
								if(i + x >= 0 && i + x < tamanho)
									if(j + y >= 0 && j + y < tamanho)
										if(gabarito[i+x][j+y] != 'X')
											gabarito[i+x][j+y]++;
		}
		void imprimir()
		{
			printf("\n y x");
			for(i = 0; i<tamanho; i++)
			{
				if(i < 9)
					printf("  %d  ", i+1);
				else
					printf("  %d ", i+1);
			}
			printf("\n\n");
			for(i = 0; i < tamanho; i++)
			{
				if(i < 9)
					printf(" %d  ", i+1);
				else
					printf(" %d ", i+1);
				for(j = 0; j < tamanho; j++)
					printf("  %c  ", campo[i][j]);
				printf("\n\n");
			}
			printf("\n");
		}
		void liberar(int a, int b)
		{
			int w, z;
			if(campo[a][b] == '&')
			{
				campo[a][b] = gabarito[a][b];
				if(gabarito[a][b] == '0')
					for(w = -1; w<2; w++)
						for(z = -1; z<2; z++)
							if(a + z >= 0 && a + z < tamanho && b + w >= 0 && b + w < tamanho)
								liberar(a+z, b+w);
			}
		}
		int terminou() /* Retorna 2 caso tenha explodido, retorna 1 caso tenha ganhado, e 0 caso não tenha terminado */
		{
			int i, j, contador_de_bombas = 0;
			for(i = 0; i<tamanho; i++)
				for(j = 0; j < tamanho; j++)
				{
					if(campo[i][j] == 'X')
						return 2;
					if(campo[i][j] == '&')
						contador_de_bombas++;
				}
			if(contador_de_bombas == bombas)
				return 1;
			return 0;
		}
		int valor_pontuacao()
		{
			return 100*dificuldade;
		}
		novo_campo();
		do
		{
			menu_jogar();
			imprimir();
			if(coordenada[0] > 0 && coordenada[0] <= tamanho && coordenada[1] > 0 && coordenada[1] <= tamanho)
				printf("\n");
			else
				printf("As entradas são invalidas, coloque somente coordenadas validas(entre 1 e %d)\n", tamanho);
			printf("Digite a coordenada X:  ");
			coordenada[1] = pega_numero_grande();
			printf("Digite a coordenada Y:  ");
			coordenada[0] = pega_numero_grande();
			if(coordenada[0] > 0 && coordenada[0] <= tamanho && coordenada[1] > 0 && coordenada[1] <= tamanho)
				liberar(coordenada[0]-1, coordenada[1]-1);
			fim = terminou();
		}while(!fim);
		for(i = 0; i < tamanho; i++)
			for(j = 0; j < tamanho; j++)
				campo[i][j] = gabarito[i][j];
		if(fim == 2)
		{
			menu_jogar();
			imprimir();
			printf("Você perdeu! Que pena!");
		}
		else
		{
			menu_jogar();
			imprimir();
			pontuacao = valor_pontuacao();
			if(pontuacao > *(dado->point+7))
			{
				dado->total -= *(dado->point+7);
				*(dado->point+7) = pontuacao;
			    dado->total += pontuacao;
		    }
			printf("Parabéns, você ganhou!");
		}
		aperte_tecla(ENTER);
	}
	int configuracoes()
	{
		void menu_configuracoes()
		{
			system("clear");
			printf("****************\n");
			printf("* Campo Minado *\n");
			printf("****************\n");
			printf("\n");
			printf("1) Alterar a dificuldade(atual: %d)\n\n", dificuldade);
			printf("0) Voltar ao menu alterior\n");
		}
		while(1)
		{
			menu_configuracoes();
			numero_do_usuario = pega_algarismo(0, 1);
			if(numero_do_usuario == 1)
			{
				menu_configuracoes();
				printf("\nDigite a nova dificuldade(entre 1 e 4):  ");
				numero_do_usuario = pega_algarismo(0, 4);
				if(numero_do_usuario)
				{
					dificuldade = numero_do_usuario;
					tamanho = 4 + 4*dificuldade;
					bombas = 5 + 5*dificuldade;
				}
				aperte_tecla(ENTER);
			}
			else
				return 0;
		}
	}
	void instrucoes()
	{
		system("clear");
	    printf("Esse é o jogo campo minado. Dá pra selecionar a dificuldade no menu configuracoes\n");
	    printf("Após colocar pra jogar, é requisitado do usuario dois valores inteiros, consecutivos.\n");
	    printf("Tais valores devem ser validos, pois como indica o mapa, deve ser entre 1 e um determinado\n");
	    printf("valor para cada um. Caso foi algum simbolo ou numero fora do intervalo, ele irá pedir \n");
	    printf("novamente os valores. Para sair do jogo, basta apenas estourar uma bomba.\n");
	    printf("O resto do jogo é tradicional, como um campo minado normal.\n");
	    aperte_tecla(ENTER);
	}
	void rank_jogo()
	{
	  int i;
	  system("clear");
	  printf("***********************************************\n");
	  printf("* Bem vindo à sala dos Desarmadores de Bombas *\n");
	  printf("***********************************************\n");
	  printf("\n");
	  for(i = 0; i < player; i++)
	  {
	    printf("%dº Lugar - ", i+1);
	    imprime_string(((record+7)->w_+i)->nome,espaco);
	    printf(": %d\n", ((record+7)->w_+i)->valor );
	  }
	  aperte_tecla(0);
	}
	while(1)
	{
		menu_opcoes();
		numero_do_usuario = pega_algarismo(0, 4);
		if(numero_do_usuario == 1)
			jogar();
		else if(numero_do_usuario == 2)
			configuracoes();
		else if(numero_do_usuario == 3)
			instrucoes();
		else if(numero_do_usuario == 4)
			rank_jogo();
		else
		{
			limpa_buffer();
			return 0;
		}
		*(posicao+8) = organiza_posicao_completo(record, dado, *(posicao+8), 8);
	}
}
int roda_roda(completo *record, geral *dado, int *posicao)
{
  const int MAX = 10;
  int numero_do_usuario, erros_maximos = 4, erros, pontuacao;
  char palavras[7][100], letra;
  srand((unsigned)time(NULL));
  void menu_opcoes()
  {
    system("clear");
    printf("***************\n");
    printf("* Roda a Roda *\n");
    printf("***************\n");
    printf("\n");
    printf("1) Jogar\n");
    printf("2) Configurações\n");
    printf("3) Instrucoes\n");
    printf("4) Sala das Consultoras\n\n");
    printf("0) Sair\n");
    printf("\n");
    printf("Digite uma opcao:  ");
  }
  void jogar()
  {
    void menu_jogar()
    {
      system("clear");
      printf("***************\n");
      printf("* Roda a Roda *\n");
      printf("***************\n");
      printf("\n");
      printf("Erros: %d\n\n", erros);
    }
    int novas_palavras(int semente)
    {
      int a=-1, b=-2, probabilidade, i;
      if(semente == 1)
      {
        copia("Animais", &palavras[0][0]);
        for(i = 1; i<4; i++)
        {
          do
          {
            probabilidade = rand()%40; 
          }while(probabilidade == a || probabilidade == b);
          if(i == 1)
            a = probabilidade;
          else if(i == 2)
            b = probabilidade;
          switch(probabilidade)
          {
            case 0: copia("ornitorrinco", &palavras[i][0]);break;
            case 1: copia("girafa", &palavras[i][0]);break;
            case 2: copia("onca pintada", &palavras[i][0]);break;
            case 3: copia("leao", &palavras[i][0]);break;
            case 4: copia("avestruz", &palavras[i][0]);break;
            case 5: copia("aranha", &palavras[i][0]);break;
            case 6: copia("burro", &palavras[i][0]);break;
            case 7: copia("coruja", &palavras[i][0]);break;
            case 8: copia("aguia", &palavras[i][0]);break;
            case 9: copia("borboleta", &palavras[i][0]);break;
            case 10: copia("baleia", &palavras[i][0]);break;
            case 11: copia("cavalo", &palavras[i][0]);break;
            case 12: copia("corvo", &palavras[i][0]);break;
            case 13: copia("cachorro", &palavras[i][0]);break;
            case 14: copia("elefante", &palavras[i][0]);break;
            case 15: copia("esquilo", &palavras[i][0]);break;
            case 16: copia("formiga", &palavras[i][0]);break;
            case 17: copia("hipopotamo", &palavras[i][0]);break;
            case 18: copia("jacare", &palavras[i][0]);break;
            case 19: copia("lagarta", &palavras[i][0]);break;
            case 20: copia("leopardo", &palavras[i][0]);break;
            case 21: copia("urso", &palavras[i][0]);break;
            case 22: copia("macaco", &palavras[i][0]);break;
            case 23: copia("mosquito", &palavras[i][0]);break;
            case 24: copia("ovelha", &palavras[i][0]);break;
            case 25: copia("pato", &palavras[i][0]);break;
            case 26: copia("pavao", &palavras[i][0]);break;
            case 27: copia("porco", &palavras[i][0]);break;
            case 28: copia("raposa", &palavras[i][0]);break;
            case 29: copia("rato", &palavras[i][0]);break;
            case 30: copia("peru", &palavras[i][0]);break;
            case 31: copia("pombo", &palavras[i][0]);break;
            case 32: copia("rinoceronte", &palavras[i][0]);break;
            case 33: copia("sapo", &palavras[i][0]);break;
            case 34: copia("tartaruga", &palavras[i][0]);break;
            case 35: copia("texugo", &palavras[i][0]);break;
            case 36: copia("tigre", &palavras[i][0]);break;
            case 37: copia("veado", &palavras[i][0]);break;
            case 38: copia("zebra", &palavras[i][0]);break;
            case 39: copia("tucunare", &palavras[i][0]);break;
          }
        }
      }
      else if(semente == 2)
      {
        copia("Carnes", &palavras[0][0]);
        for(i = 1; i<4; i++)
        {
          do
          {
            probabilidade = rand()%30; 
          }while(probabilidade == a || probabilidade == b);
          if(i == 1)
            a = probabilidade;
          else if(i == 2)
            b = probabilidade;
          switch(probabilidade)
          {
            case 0: copia("fraudinha", &palavras[i][0]);break;
            case 1: copia("musculo", &palavras[i][0]);break;
            case 2: copia("cupim", &palavras[i][0]);break;
            case 3: copia("atum", &palavras[i][0]);break;
            case 4: copia("alcatra", &palavras[i][0]);break;
            case 5: copia("lombo", &palavras[i][0]);break;
            case 6: copia("acem", &palavras[i][0]);break;
            case 7: copia("file mignon", &palavras[i][0]);break;
            case 8: copia("picanha", &palavras[i][0]);break;
            case 9: copia("patinho", &palavras[i][0]);break;
            case 10: copia("figado", &palavras[i][0]);break;
            case 11: copia("coxao mole", &palavras[i][0]);break;
            case 12: copia("coxao duro", &palavras[i][0]);break;
            case 13: copia("figado", &palavras[i][0]);break;
            case 14: copia("mocoto", &palavras[i][0]);break;
            case 15: copia("paleta", &palavras[i][0]);break;
            case 16: copia("salsicha", &palavras[i][0]);break;
            case 17: copia("pernil", &palavras[i][0]);break;
            case 18: copia("bisteca", &palavras[i][0]);break;
            case 19: copia("costela", &palavras[i][0]);break;
            case 20: copia("coxa", &palavras[i][0]);break;
            case 21: copia("asa", &palavras[i][0]);break;
            case 22: copia("peito", &palavras[i][0]);break;
            case 23: copia("sobrecoxa", &palavras[i][0]);break;
            case 24: copia("salmao", &palavras[i][0]);break;
            case 25: copia("sardinha", &palavras[i][0]);break;
            case 26: copia("tilapia", &palavras[i][0]);break;
            case 27: copia("camarao", &palavras[i][0]);break;
            case 28: copia("lagosta", &palavras[i][0]);break;
            case 29: copia("polvo", &palavras[i][0]);break;
          }
        }
      }
      else if(semente == 3)
      {
        copia("Computador", &palavras[0][0]);
        for(i = 1; i<4; i++)
        {
          do
          {
            probabilidade = rand()%26; 
          }while(probabilidade == a || probabilidade == b);
          if(i == 1)
            a = probabilidade;
          else if(i == 2)
            b = probabilidade;
          switch(probabilidade)
          {
            case 0: copia("teclado", &palavras[i][0]);break;
            case 1: copia("impressora", &palavras[i][0]);break;
            case 2: copia("monitor", &palavras[i][0]);break;
            case 3: copia("hardware", &palavras[i][0]);break;
            case 4: copia("software", &palavras[i][0]);break;
            case 5: copia("mouse", &palavras[i][0]);break;
            case 6: copia("transistores", &palavras[i][0]);break;
            case 7: copia("internet", &palavras[i][0]);break;
            case 8: copia("servidor", &palavras[i][0]);break;
            case 9: copia("nuvem", &palavras[i][0]);break;
            case 10: copia("programacao", &palavras[i][0]);break;
            case 11: copia("portas logicas", &palavras[i][0]);break;
            case 12: copia("touchpad", &palavras[i][0]);break;
            case 13: copia("windows", &palavras[i][0]);break;
            case 14: copia("linux", &palavras[i][0]);break;
            case 15: copia("kernel", &palavras[i][0]);break;
            case 16: copia("processador", &palavras[i][0]);break;
            case 17: copia("placa mae", &palavras[i][0]);break;
            case 18: copia("placa de video", &palavras[i][0]);break;
            case 19: copia("leitor de cd", &palavras[i][0]);break;
            case 20: copia("disquete", &palavras[i][0]);break;
            case 21: copia("memoria", &palavras[i][0]);break;
            case 22: copia("armazenamento", &palavras[i][0]);break;
            case 23: copia("calculo", &palavras[i][0]);break;
            case 24: copia("wi fi", &palavras[i][0]);break;
            case 25: copia("email", &palavras[i][0]);break;
          }
        }
      }
      else if(semente == 4)
      {
        copia("Dança", &palavras[0][0]);
        for(i = 1; i<4; i++)
        {
          do
          {
            probabilidade = rand()%25; 
          }while(probabilidade == a || probabilidade == b);
          if(i == 1)
            a = probabilidade;
          else if(i == 2)
            b = probabilidade;
          switch(probabilidade)
          {
            case 0: copia("frevo", &palavras[i][0]);break;
            case 1: copia("forro", &palavras[i][0]);break;
            case 2: copia("tango", &palavras[i][0]);break;
            case 3: copia("baiao", &palavras[i][0]);break;
            case 4: copia("bolero", &palavras[i][0]);break;
            case 5: copia("capoeira", &palavras[i][0]);break;
            case 6: copia("carimbo", &palavras[i][0]);break;
            case 7: copia("danca de rua", &palavras[i][0]);break;
            case 8: copia("fandango", &palavras[i][0]);break;
            case 9: copia("funk", &palavras[i][0]);break;
            case 10: copia("free step", &palavras[i][0]);break;
            case 11: copia("hip hop", &palavras[i][0]);break;
            case 12: copia("jazz", &palavras[i][0]);break;
            case 13: copia("kuduro", &palavras[i][0]);break;
            case 14: copia("lambada", &palavras[i][0]);break;
            case 15: copia("maracatu", &palavras[i][0]);break;
            case 16: copia("merengue", &palavras[i][0]);break;
            case 17: copia("pagode", &palavras[i][0]);break;
            case 18: copia("pole dance", &palavras[i][0]);break;
            case 19: copia("danca do ventre", &palavras[i][0]);break;
            case 20: copia("salsa", &palavras[i][0]);break;
            case 21: copia("samba", &palavras[i][0]);break;
            case 22: copia("sapateado", &palavras[i][0]);break;
            case 23: copia("zouk", &palavras[i][0]);break;
            case 24: copia("zumba", &palavras[i][0]);break;
          }
        }
      }
      else if(semente == 5)
      {
        copia("Casa", &palavras[0][0]);
        for(i = 1; i<4; i++)
        {
          do
          {
            probabilidade = rand()%23; 
          }while(probabilidade == a || probabilidade == b);
          if(i == 1)
            a = probabilidade;
          else if(i == 2)
            b = probabilidade;
          switch(probabilidade)
          {
            case 0: copia("chuveiro", &palavras[i][0]);break;
            case 1: copia("cama", &palavras[i][0]);break;
            case 2: copia("cozinha", &palavras[i][0]);break;
            case 3: copia("jardim", &palavras[i][0]);break;
            case 4: copia("quintal", &palavras[i][0]);break;
            case 5: copia("banheiro", &palavras[i][0]);break;
            case 6: copia("quarto", &palavras[i][0]);break;
            case 7: copia("porao", &palavras[i][0]);break;
            case 8: copia("sotao", &palavras[i][0]);break;
            case 9: copia("chamine", &palavras[i][0]);break;
            case 10: copia("janela", &palavras[i][0]);break;
            case 11: copia("mesa", &palavras[i][0]);break;
            case 12: copia("cadeira", &palavras[i][0]);break;
            case 13: copia("televisao", &palavras[i][0]);break;
            case 14: copia("criado mudo", &palavras[i][0]);break;
            case 15: copia("estante", &palavras[i][0]);break;
            case 16: copia("quadro", &palavras[i][0]);break;
            case 17: copia("guarda roupa", &palavras[i][0]);break;
            case 18: copia("fogao", &palavras[i][0]);break;
            case 19: copia("geladeira", &palavras[i][0]);break;
            case 20: copia("pia", &palavras[i][0]);break;
            case 21: copia("churrasqueira", &palavras[i][0]);break;
            case 22: copia("sofa", &palavras[i][0]);break;
          }
        }
      }
      else if(semente == 6)
      {
        copia("Matematica", &palavras[0][0]);
        for(i = 1; i<4; i++)
        {
          do
          {
            probabilidade = rand()%35; 
          }while(probabilidade == a || probabilidade == b);
          if(i == 1)
            a = probabilidade;
          else if(i == 2)
            b = probabilidade;
          switch(probabilidade)
          {
            case 0: copia("integral", &palavras[i][0]);break;
            case 1: copia("bijetividade", &palavras[i][0]);break;
            case 2: copia("matriz", &palavras[i][0]);break;
            case 3: copia("derivada", &palavras[i][0]);break;
            case 4: copia("injetividade", &palavras[i][0]);break;
            case 5: copia("gradiente", &palavras[i][0]);break;
            case 6: copia("divergente", &palavras[i][0]);break;
            case 7: copia("rotacional", &palavras[i][0]);break;
            case 8: copia("transformada de laplace", &palavras[i][0]);break;
            case 9: copia("series", &palavras[i][0]);break;
            case 10: copia("sequencia", &palavras[i][0]);break;
            case 11: copia("funcao", &palavras[i][0]);break;
            case 12: copia("exponencial", &palavras[i][0]);break;
            case 13: copia("logaritmo", &palavras[i][0]);break;
            case 14: copia("tabuada", &palavras[i][0]);break;
            case 15: copia("multiplicacao", &palavras[i][0]);break;
            case 16: copia("soma", &palavras[i][0]);break;
            case 17: copia("divisao", &palavras[i][0]);break;
            case 18: copia("subtracao", &palavras[i][0]);break;
            case 19: copia("progressao", &palavras[i][0]);break;
            case 20: copia("ponto critico", &palavras[i][0]);break;
            case 21: copia("algebra linear", &palavras[i][0]);break;
            case 22: copia("aritmetica", &palavras[i][0]);break;
            case 23: copia("teoria dos numeros", &palavras[i][0]);break;
            case 24: copia("geometria", &palavras[i][0]);break;
            case 25: copia("numeros complexos", &palavras[i][0]);break;
            case 26: copia("triangulo", &palavras[i][0]);break;
            case 27: copia("quadrado", &palavras[i][0]);break;
            case 28: copia("pentagono", &palavras[i][0]);break;
            case 29: copia("dodecaedro", &palavras[i][0]);break;
            case 30: copia("hexaedro", &palavras[i][0]);break;
            case 31: copia("cubo", &palavras[i][0]);break;
            case 32: copia("paralelepipedo", &palavras[i][0]);break;
            case 33: copia("equacao diferencial", &palavras[i][0]);break;
            case 34: copia("soma de riemann", &palavras[i][0]);break;
          }
        }
      }
      else if(semente == 7)
      {
        copia("Fisica", &palavras[0][0]);
        for(i = 1; i<4; i++)
        {
          do
          {
            probabilidade = rand()%40; 
          }while(probabilidade == a || probabilidade == b);
          if(i == 1)
            a = probabilidade;
          else if(i == 2)
            b = probabilidade;
          switch(probabilidade)
          {
            case 0: copia("eletron", &palavras[i][0]);break;
            case 1: copia("gravitacao", &palavras[i][0]);break;
            case 2: copia("quantico", &palavras[i][0]);break;
            case 3: copia("mecanica", &palavras[i][0]);break;
            case 4: copia("eletrodinamica", &palavras[i][0]);break;
            case 5: copia("magnetismo", &palavras[i][0]);break;
            case 6: copia("inercia", &palavras[i][0]);break;
            case 7: copia("quark", &palavras[i][0]);break;
            case 8: copia("proton", &palavras[i][0]);break;
            case 9: copia("neutron", &palavras[i][0]);break;
            case 10: copia("relatividade", &palavras[i][0]);break;
            case 11: copia("ondas", &palavras[i][0]);break;
            case 12: copia("calor", &palavras[i][0]);break;
            case 13: copia("temperatura", &palavras[i][0]);break;
            case 14: copia("pressao", &palavras[i][0]);break;
            case 15: copia("deslocamento", &palavras[i][0]);break;
            case 16: copia("velocidade", &palavras[i][0]);break;
            case 17: copia("aceleracao", &palavras[i][0]);break;
            case 18: copia("pertubacao", &palavras[i][0]);break;
            case 19: copia("interferencia", &palavras[i][0]);break;
            case 20: copia("ressonancia", &palavras[i][0]);break;
            case 21: copia("difracao", &palavras[i][0]);break;
            case 22: copia("refracao", &palavras[i][0]);break;
            case 23: copia("reflexao", &palavras[i][0]);break;
            case 24: copia("momento angular", &palavras[i][0]);break;
            case 25: copia("momento linear", &palavras[i][0]);break;
            case 26: copia("massa", &palavras[i][0]);break;
            case 27: copia("spin", &palavras[i][0]);break;
            case 28: copia("carga eletrica", &palavras[i][0]);break;
            case 29: copia("foton", &palavras[i][0]);break;
            case 30: copia("buraco negro", &palavras[i][0]);break;
            case 31: copia("big bang", &palavras[i][0]);break;
            case 32: copia("astronomia", &palavras[i][0]);break;
            case 33: copia("super nova", &palavras[i][0]);break;
            case 34: copia("estrela", &palavras[i][0]);break;
            case 35: copia("resistores", &palavras[i][0]);break;
            case 36: copia("indutores", &palavras[i][0]);break;
            case 37: copia("capacitores", &palavras[i][0]);break;
            case 38: copia("leis de newton", &palavras[i][0]);break;
            case 39: copia("estrela de neutrons", &palavras[i][0]);break;
          }
        } 
      }
      else if(semente == 8)
      {
        copia("Quimica", &palavras[0][0]);
        for(i = 1; i<4; i++)
        {
          do
          {
            probabilidade = rand()%3; 
          }while(probabilidade == a || probabilidade == b);
          if(i == 1)
            a = probabilidade;
          else if(i == 2)
            b = probabilidade;
          switch(probabilidade)
          {
            case 0: copia("orbital", &palavras[i][0]);break;
            case 1: copia("equilibrio", &palavras[i][0]);break;
            case 2: copia("radioativo", &palavras[i][0]);break;
          }
        }
      }
      else if(semente == 9)
      {
        copia("Frutas", &palavras[0][0]);
        for(i = 1; i<4; i++)
        {
          do
          {
            probabilidade = rand()%31; 
          }while(probabilidade == a || probabilidade == b);
          if(i == 1)
            a = probabilidade;
          else if(i == 2)
            b = probabilidade;
          switch(probabilidade)
          {
            case 0: copia("manga", &palavras[i][0]);break;
            case 1: copia("pequi", &palavras[i][0]);break;
            case 2: copia("morango", &palavras[i][0]);break;
            case 3: copia("uva", &palavras[i][0]);break;
            case 4: copia("jambo", &palavras[i][0]);break;
            case 5: copia("tomate", &palavras[i][0]);break;
            case 6: copia("jaca", &palavras[i][0]);break;
            case 7: copia("caqui", &palavras[i][0]);break;
            case 8: copia("maca", &palavras[i][0]);break;
            case 9: copia("banana", &palavras[i][0]);break;
            case 10: copia("tamarindo", &palavras[i][0]);break;
            case 11: copia("abacate", &palavras[i][0]);break;
            case 12: copia("jamelao", &palavras[i][0]);break;
            case 13: copia("fruta do conde", &palavras[i][0]);break;
            case 14: copia("buriti", &palavras[i][0]);break;
            case 15: copia("cupuacu", &palavras[i][0]);break;
            case 16: copia("goiaba", &palavras[i][0]);break;
            case 17: copia("pinha", &palavras[i][0]);break;
            case 18: copia("umbu", &palavras[i][0]);break;
            case 19: copia("kiwi", &palavras[i][0]);break;
            case 20: copia("cereja", &palavras[i][0]);break;
            case 21: copia("mamao", &palavras[i][0]);break;
            case 22: copia("jabuticaba", &palavras[i][0]);break;
            case 23: copia("amora", &palavras[i][0]);break;
            case 24: copia("caju", &palavras[i][0]);break;
            case 25: copia("limao", &palavras[i][0]);break;
            case 26: copia("laranja", &palavras[i][0]);break;
            case 27: copia("tangerina", &palavras[i][0]);break;
            case 28: copia("mexerica", &palavras[i][0]);break;
            case 29: copia("camapu", &palavras[i][0]);break;
            case 30: copia("amexa", &palavras[i][0]);break;
          }
        }
      }
      else if(semente == 10)
      {
        copia("Ferramentas", &palavras[0][0]);
        for(i = 1; i<4; i++)
        {
          do
          {
            probabilidade = rand()%21; 
          }while(probabilidade == a || probabilidade == b);
          if(i == 1)
            a = probabilidade;
          else if(i == 2)
            b = probabilidade;
          switch(probabilidade)
          {
            case 0: copia("serrote", &palavras[i][0]);break;
            case 1: copia("martelo", &palavras[i][0]);break;
            case 2: copia("foice", &palavras[i][0]);break;
            case 3: copia("chave de fenda", &palavras[i][0]);break;
            case 4: copia("tesoura", &palavras[i][0]);break;
            case 5: copia("macaco", &palavras[i][0]);break;
            case 6: copia("jacare", &palavras[i][0]);break;
            case 7: copia("pa", &palavras[i][0]);break;
            case 8: copia("chave de boca", &palavras[i][0]);break;
            case 9: copia("ferro de solda", &palavras[i][0]);break;
            case 10: copia("parafuso", &palavras[i][0]);break;
            case 11: copia("alicate", &palavras[i][0]);break;
            case 12: copia("porca", &palavras[i][0]);break;
            case 13: copia("prego", &palavras[i][0]);break;
            case 14: copia("facao", &palavras[i][0]);break;
            case 15: copia("rede", &palavras[i][0]);break;
            case 16: copia("fresa", &palavras[i][0]);break;
            case 17: copia("torno", &palavras[i][0]);break;
            case 18: copia("cavadeira", &palavras[i][0]);break;
            case 19: copia("picareta", &palavras[i][0]);break;
            case 20: copia("machado", &palavras[i][0]);break;
            }
        } 
      }
      return semente;
    }
    void nova_categoria()
    {
      int i, j;
      novas_palavras(rand()%MAX + 1);
      copia(palavras[1], palavras[4]);
      copia(palavras[2], palavras[5]);
      copia(palavras[3], palavras[6]);
      for(i = 4; i<7; i++)
      {
        j = 0;
        do
        {
          if(palavras[i][j] != ' ')
            palavras[i][j] = '_';
          j++;
        }while(palavras[i][j] != '\0');
      }
    }
    int conta_underline()
    {
      int i, j, contador = 0;
      for(i = 4; i < 8; i++)
      {
        j = 0;
        do
        {
          if(palavras[i][j] == '_')
            contador++;
          j++;
        }while(palavras[i][j] != '\0');
      }
      return contador;
    }
    int nao_terminou()
    {
      if(erros == erros_maximos+1)
        return 0;
      return conta_underline();
    }
    void imprime()
    {
      int i, j;
      for(i = 4; i<7; i++)
      {
        j = 0;
        while(palavras[i][j] != '\0')
        {
          printf(" %c", palavras[i][j]);
          j++;
        }
        printf("\n");
      }
      printf("\nDica: %s", palavras[0]);
    }
    int coloca_letra(char qual_letra)
    {
      int i, j, t = 0;
      for(i = 4; i < 8; i++)
      {
        j = 0;
        do
        {
          if(palavras[i-3][j] == qual_letra && palavras[i][j] == '_')
          {
            palavras[i][j] = qual_letra;
            t++;
          }
          j++;
        }while(palavras[i][j] != '\0');
      }
      return t;
    }
    nova_categoria();
    erros = 0;
    numero_do_usuario = nao_terminou();
    while(numero_do_usuario)
    {
      menu_jogar();
      imprime();
      printf("\nDigite uma letra:  ");
      letra = pega_letra('a', 'z', 'A', 'Z');
      numero_do_usuario = coloca_letra(letra);
      erros += (numero_do_usuario ? 0 : 1);
      numero_do_usuario = nao_terminou();
    }
    menu_jogar();
    imprime();
    if(conta_underline())
    {
      printf("\n\nVocê perdeu!\n");
      aperte_tecla(ENTER);
    }
    else
    {
		pontuacao = 20 - erros;
		if(pontuacao > *(dado->point+8))
		{
			dado->total -= *(dado->point+8);
	     	*(dado->point+8) = pontuacao;
	     	dado->total += pontuacao;
	    }
	    printf("\n\nVocê ganhou!\n");
	    aperte_tecla(ENTER);
    }
  }
  int configuracoes()
  {
    void menu_configuracoes()
    {
      system("clear");
      printf("***************\n");
      printf("* Roda a Roda *\n");
      printf("***************\n");
      printf("\n");
      printf("1) Alterar o numero maximo de erros(atual: %d)\n\n", erros_maximos);
      printf("0) Voltar ao menu alterior\n");
    }
    while(1)
    {
      menu_configuracoes();
      numero_do_usuario = pega_algarismo(0, 1);
      if(numero_do_usuario == 1)
      {
        menu_configuracoes();
        printf("\nDigite o novo numero de erros maximos(entre 0 e 9):  ");
        numero_do_usuario = pega_algarismo(0, 9);
        erros_maximos = numero_do_usuario;
        aperte_tecla(ENTER);
      }
      else
        return 0;
    }
  }
  void instrucoes()
  {
    system("clear");
    printf("Esse jogo é o jogo Roda a Roda. Ele consiste em um grupo de palavras\n");
    printf("que devem ser descobertos através de letras. Após pressionar uma letra\n");
    printf("caso a letra esteja em algum lugar das palavras, ela será mostrada. E\n");
    printf("caso contrário, aumentará um erro. O jogo encerra caso o numero de \n");
    printf("erros seja maior que o permitido. O numero de erros é alterado através\n");
    printf("do menu configuracoes.");
    aperte_tecla(ENTER);
  }
  void rank_jogo()
  {
    int i;
    system("clear");
    printf("*********************************************\n");
    printf("* Bem vindo à sala das Melhores Consultoras *\n");
    printf("*********************************************\n");
    printf("\n");
    for(i = 0; i < player; i++)
    {
      printf("%dº Lugar - ", i+1);
      imprime_string(((record+8)->w_+i)->nome,espaco);
      printf(": %d\n", ((record+8)->w_+i)->valor );
    }
    aperte_tecla(0);
  }
  while(1)
  {
    menu_opcoes();
    numero_do_usuario = pega_algarismo(0, 4);
    if(numero_do_usuario == 1)
      jogar();
    else if(numero_do_usuario == 2)
      configuracoes();
    else if(numero_do_usuario == 3)
      instrucoes();
    else if(numero_do_usuario == 4)
      rank_jogo();
    else
    {
      limpa_buffer();
      return 0;
    }
    *(posicao+9) = organiza_posicao_completo(record, dado, *(posicao+9), 9);
  }
}
int cobrinha(completo *record, geral *dado, int *posicao)
{
  int numero_do_usuario, velocidade = 1, numero_mapa=1, dificuldade = 1, tamanhox, tamanhoy, i, j, pontuacao = 0;
  char string[30], mapa[y_m][x_m];
  FILE *arquivo;
  void menu_opcoes()
  {
    system("clear");
    printf("*********\n");
    printf("* Snake *\n");
    printf("*********\n");
    printf("\n");
    printf("1) Jogar\n");
    printf("2) Configurações\n");
    printf("3) Instrucoes\n");
    printf("4) Os encatadores de serpentes\n\n");
    printf("0) Sair\n");
    printf("\n");
    printf("Digite uma opcao:  ");
  }
  void jogar()
  {
    int direcao, tamanho=5, auxiliar, terminou;
    int a[x_m*y_m], b[x_m*y_m]; /* a representa a coordenada x, b representa a coordenada y*/
    int valor_da_pontuacao()
    {
      return velocidade*dificuldade;
    }
    void novo_mapa()
    {
      if(numero_mapa == 0)
      {
        arquivo = fopen(string, "r");
        fscanf(arquivo, "%d %d %d", &tamanhox, &tamanhoy, &dificuldade);
        fgetc(arquivo);
        for(i = 0; i < tamanhox; i++)
        {
          for(j = 0; j < tamanhoy; j++)
            mapa[i][j] = fgetc(arquivo);
          fgetc(arquivo);
        }
        fclose(arquivo);
      }
      else if(numero_mapa == 1)
      {
        tamanhox = y_m;
        tamanhoy = x_m;
        dificuldade = 1;
        for(i = 0; i<tamanhox; i++)
          for(j=0; j<tamanhoy; j++)
            mapa[i][j] = ' ';
      }
      else if(numero_mapa == 2)
      {
        tamanhox = y_m;
        tamanhoy = x_m;
        dificuldade = 2;
        for(j = 0; j<tamanhoy; j++)
          mapa[0][j] = mapa[tamanhox-1][j] = parede;
        for(i = 1; i<tamanhox-1; i++)
        {
          mapa[i][0] = mapa[i][tamanhoy-1] = parede;
          for(j = 1; j<tamanhoy-1; j++)
            mapa[i][j] = ' ';
        }
      }
      else if(numero_mapa == 3)
      {
        tamanhox = y_m;
        tamanhoy = x_m;
        dificuldade = 2;
        for(i = 0; i<tamanhox; i++)
          for(j=0; j<tamanhoy; j++)
            mapa[i][j] = ' ';
        for(j = 10; j < 40; j++)
        {
          mapa[tamanhox/3-1][j] = mapa[2*tamanhox/3][j] = parede;
        }
      }
      else if(numero_mapa == 4)
      {
        tamanhox = y_m;
        tamanhoy = x_m;
        dificuldade = 2;
        for(i = 0; i<tamanhox; i++)
          for(j=0; j<tamanhoy; j++)
            mapa[i][j] = ' ';
        for(i=0; i<4; i++)
        {
          mapa[i+6][12] = mapa[i+6][25] = mapa[i+6][37] = parede;
          mapa[23-i][12] = mapa[23-i][25] = mapa[23-i][37] = parede;
        }
      }
    }
    void menu_jogar()
    {
      system("clear");
      printf("*********\n");
      printf("* Snake *\n");
      printf("*********\n");
      printf("\n");
    }
    void imprime_mapa()
    {
      for(j = 0; j<2*tamanhoy+2; j++)
        putchar(tele1);
      putchar('\n');
      for(i = 0; i<tamanhox; i++)
      {
        putchar(tele2);
        for(j = 0; j<tamanhoy-1; j++)
        {
          putchar(mapa[i][j]);
          putchar(' ');
        }
        putchar(mapa[i][j]);
        putchar(tele2);
        putchar('\n');
      }
      for(j = 0; j<2*tamanhoy+2; j++)
        putchar(tele1);
      printf("\n\n");
    }
    void colocar_comida()
    {
      int i_, j_;
      do
      {
        i_ = rand()%tamanhox;
        j_ = rand()%tamanhoy;
      }while(mapa[i_][j_] == corpo || mapa[i_][j_] == parede);
      mapa[i_][j_] = comida;
    }
    int mover()
    {
      int i_;
      mapa[b[tamanho-2]][a[tamanho-2]] = ' ';
      for(i_=tamanho-1; i_>0; i_--)
      {
        a[i_] = a[i_-1];
        b[i_] = b[i_-1];
      }
      if(direcao == 1)
        b[0]--;
      else if(direcao == 2)
        b[0]++;
      else if(direcao == 3)
        a[0]++;
      else if(direcao == 4)
        a[0]--;
      if(a[0] == -1)
        a[0] = tamanhoy-1;
      else if(a[0] == tamanhoy)
        a[0] = 0;
      if(b[0] == -1)
        b[0] = tamanhox-1;
      else if(b[0] == tamanhox)
        b[0] = 0;
      if(mapa[b[0]][a[0]] == parede || !direcao || mapa[b[0]][a[0]] == corpo)
      {
        mapa[b[tamanho-1]][a[tamanho-1]] = corpo;
        return 1;
      }
      else if(mapa[b[0]][a[0]] == comida)
      {
        tamanho++;
        pontuacao += valor_da_pontuacao();
        colocar_comida();
      }
      mapa[b[0]][a[0]] = corpo;
      return 0;
    }
    novo_mapa();
    do
    {
      b[0] = rand()%tamanhox;
      a[0] = rand()%tamanhoy;
    }while(mapa[b[0]][a[0]] != ' ');
    for(auxiliar = 1; auxiliar < tamanho; auxiliar++)
    {
      b[auxiliar] = b[0];
      a[auxiliar] = a[0];
    }
    pontuacao = 0;
    auxiliar = direcao = 5;
    colocar_comida();
    do
    {
      menu_jogar();
      imprime_mapa();
      terminou = mover();
      usleep(300000/velocidade);
      if(kbhit())
      {
        direcao = pega_quatro_setas();
        if(direcao == 1 && auxiliar == 2)
          direcao = 2;
        else if(direcao == 2 && auxiliar == 1)
          direcao = 1;
        else if(direcao == 3 && auxiliar == 4)
          direcao = 4;
        else if(direcao == 4 && auxiliar == 3)
          direcao = 3;
        else 
          auxiliar = direcao;
      }
    }while(!terminou);
    system("clear");
    if(pontuacao > *(dado->point+9)){
      dado->total -= *(dado->point+9);
      *(dado->point+9) = pontuacao;
      dado->total += pontuacao;
    }
    menu_jogar();
    imprime_mapa();
    printf("Game Over!! Você fez %d pontos!\n", pontuacao);
    aperte_tecla(ENTER);
    system("clear");
  }
  int configuracoes()
  {
    void menu_configuracoes()
    {
      system("clear");
      printf("*********\n");
      printf("* Snake *\n");
      printf("*********\n");
      printf("\n");
      printf("1) Alterar a velocidade(atual: %d)\n", velocidade);
      printf("2) Alterar o numero do mapa(atual: ");
      if(numero_mapa == 0)
        printf("Custom)\n");
      else
        printf("%d)\n", numero_mapa);
      printf("3) Importar mapa\n\n");
      printf("0) Voltar ao menu alterior\n");
    }
    while(1)
    {
      menu_configuracoes();
      numero_do_usuario = pega_algarismo(0, 3);
      if(numero_do_usuario == 1)
      {
        menu_configuracoes();
        printf("Digite a nova velocidade:  ");
        numero_do_usuario = pega_algarismo(1, 9);
        velocidade = numero_do_usuario ? numero_do_usuario : 1;
      }
      else if(numero_do_usuario == 2)
      {
        menu_configuracoes();
        printf("Digite o numero do mapa: ");
        numero_do_usuario = pega_algarismo(1,4);
        numero_mapa = numero_do_usuario;
      }
      else if(numero_do_usuario == 3)
      {
        menu_configuracoes();
        printf("Digite o nome do arquivo que existe o mapa:  ");
        limpa_buffer();
        scanf("%28s", string);
        limpa_buffer();
        arquivo = fopen(string, "r");
        if(arquivo != NULL)
        {
          numero_mapa = 0;
          fclose(arquivo);
        }
        else
        {
          numero_mapa = 1;
          printf("O arquivo não foi carregado, verifique se o arquivo %s realmente existe", string);
          aperte_tecla(ENTER);
        }
      }
      else
        return 0;
    }
  }
  void instrucoes()
  {
    system("clear");
    printf("Esse é o jogo da cobrinha. O jogo consiste em uma cobrinha que deve pegar as comidas que são\n");
    printf("disponibilizadas pelo mapa e é dado por %c. E tem as paredes %c que caso haja colisão, você\n", comida, parede);
    printf("perde o jogo e sua pontuação é em base o tamanho da cobrinha e a velocidade de jogo e o nivel\n");
    printf("de dificuldade do mapa. Os mapas padrões tem dificuldade um, mas os externos podem ter mais.\n");
    printf("O valor da velocidade pode ser alterado no menu configurações, bem como o mapa.\n");
    printf("Veja um exemplo de mapa deixado no github: https://github.com/CarlosAdir/Jogos-em-C\n");
    aperte_tecla(ENTER);
  }
  void rank_jogo()
  {
    int i;
    system("clear");
    printf("*************************************************\n");
    printf("* Bem vindo à sala dos Encatadores de Serpentes *\n");
    printf("*************************************************\n");
    printf("\n");
    for(i = 0; i < player; i++)
    {
      printf("%dº Lugar - ", i+1);
      imprime_string(((record+9)->w_+i)->nome,espaco);
      printf(": %d\n", ((record+9)->w_+i)->valor );
    }
    aperte_tecla(0);
  }
  while(1)
  {
    menu_opcoes();
    numero_do_usuario = pega_algarismo(0, 4);
    if(numero_do_usuario == 1)
      jogar();
    else if(numero_do_usuario == 2)
      configuracoes();
    else if(numero_do_usuario == 3)
      instrucoes();
    else if(numero_do_usuario == 4)
      rank_jogo();
    else
    {
      limpa_buffer();
      return 0;
    }
    *(posicao+10) = organiza_posicao_completo(record, dado, *(posicao+10), 10);
  }
}
int pacman(completo *record, geral *dado, int *posicao)
{
  FILE *arquivo;
  int numero_do_usuario, velocidade = 1, numero_mapa = 1, tamanhox, tamanhoy, x, y, quantidade, contador, direcao=0;
  char string[30]="Map.txt", mapa[32][56];
  void menu_opcoes()
  {
    system("clear");
    printf("**********\n");
    printf("* PacMan *\n");
    printf("**********\n");
    printf("\n");
    printf("1) Jogar\n");
    printf("2) Configurações\n");
    printf("3) Instrucoes\n");
    printf("4) Rank\n\n");
    printf("0) Sair\n");
    printf("\n");
    printf("Digite uma opcao:  ");
  }
  void jogar()
  {
    void menu_jogar()
    {
      system("clear");
      printf("**********\n");
      printf("* PacMan *\n");
      printf("**********\n");
      printf("\n");
    }
    void novo_mapa()
    {
      int i, j;
      if(numero_mapa == 0)
      {
        arquivo = fopen(string, "r");
        if(arquivo == NULL)
          return;
        fscanf(arquivo, "%d %d %d %d %d", &tamanhox, &tamanhoy, &y, &x, &quantidade);
        fgetc(arquivo);
        for(i = 0; i < tamanhox; i++)
        {
          for(j = 0; j < tamanhoy; j++)
            mapa[i][j] = fgetc(arquivo);
          fgetc(arquivo);
        }
        fclose(arquivo);
      }
    }
    int valor_da_pontuacao()
    {
      return velocidade;
    }
    void imprime()
    {
      int i, j;
      for(i = 0; i < tamanhox; i++)
      {
        for(j = 0; j < tamanhoy; j++)
          putchar(mapa[i][j]);
        putchar('\n');
      }
      putchar('\n');
    }
    int nao_terminou()
    {
      return quantidade-1;
    }
    void mover_pacman()
    {
      mapa[y][x] = ' ';
      if(direcao == 1)
        y--;
      else if(direcao == 2)
        y++;
      else if(direcao == 3)
        x+=2;
      else if(direcao == 4)
        x-=2;
      if(mapa[y][x] == '.' || mapa[y][x] == '*')
        quantidade--;
      else if(y < 0)
        y = tamanhox - 1;
      else if(y >= tamanhox)
        y = 0;
      else if(x < 0)
        x = tamanhoy - 1;
      else if(x >= tamanhoy)
        x = 0;
      else if(mapa[y][x] == parede)
      {
        if(direcao == 1)
          y++;
        else if(direcao == 2)
          y--;
        else if(direcao == 3)
          x-=2;
        else if(direcao == 4)
          x+=2;
        limpa_buffer();
      }
      mapa[y][x] = corpo_pacman;
    }
    void mover_monstros()
    {
      contador = 0;
    }
    novo_mapa();
    while(nao_terminou())
    {
      menu_jogar();
      imprime();
      usleep(30000);
      contador++;
      if(kbhit())
      {
        direcao = pega_quatro_setas();
        mover_pacman();
      }
      if(!contador%velocidade)
      {
        mover_monstros();
      }
    }
    aperte_tecla(0);
  }
  int configuracoes()
  {
    void menu_configuracoes()
    {
      system("clear");
      printf("*********\n");
      printf("* Snake *\n");
      printf("*********\n");
      printf("\n");
      printf("1) Alterar a velocidade(atual: %d)\n", velocidade);
      printf("2) Alterar o numero do mapa(atual: ");
      if(numero_mapa == 0)
        printf("Custom)\n");
      else
        printf("%d)\n", numero_mapa);
      printf("3) Importar mapa\n\n");
      printf("0) Voltar ao menu alterior\n");
    }
    while(1)
    {
      menu_configuracoes();
      numero_do_usuario = pega_algarismo(0, 3);
      if(numero_do_usuario == 1)
      {
        menu_configuracoes();
        printf("Digite a nova velocidade:  ");
        numero_do_usuario = pega_algarismo(1, 9);
        velocidade = numero_do_usuario ? numero_do_usuario : 1;
      }
      else if(numero_do_usuario == 2)
      {
        menu_configuracoes();
        printf("Digite o numero do mapa: ");
        numero_do_usuario = pega_algarismo(1,6);
        numero_mapa = numero_do_usuario;
      }
      else if(numero_do_usuario == 3)
      {
        menu_configuracoes();
        printf("Digite o nome do arquivo que existe o mapa:  ");
        limpa_buffer();
        scanf("%28s", string);
        limpa_buffer();
        arquivo = fopen(string, "r");
        if(arquivo != NULL)
        {
          numero_mapa = 0;
          fclose(arquivo);
        }
        else
        {
          numero_mapa = 1;
          printf("O arquivo não foi carregado, verifique se o arquivo %s realmente existe", string);
          aperte_tecla(ENTER);
        }
      }
      else
        return 0;
    }
  }
  void instrucoes()
  {
    system("clear");
    printf("Instrucoes!\n");
  }
  void rank_jogo()
  {
    int i;
    system("clear");
    printf("*************************************************\n");
    printf("* Bem vindo à sala dos Encatadores de Serpentes *\n");
    printf("*************************************************\n");
    printf("\n");
    for(i = 0; i < player; i++)
    {
      printf("%dº Lugar - ", i+1);
      imprime_string(((record+10)->w_+i)->nome,espaco);
      printf(": %d\n", ((record+10)->w_+i)->valor );
    }
    aperte_tecla(0);
  }
  while(1)
  {
    menu_opcoes();
    numero_do_usuario = pega_algarismo(0, 4);
    if(numero_do_usuario == 1)
      jogar();
    else if(numero_do_usuario == 2)
      configuracoes();
    else if(numero_do_usuario == 3)
      instrucoes();
    else if(numero_do_usuario == 4)
      rank_jogo();
    else
    {
      limpa_buffer();
      return 0;
    }
    *(posicao+11) = organiza_posicao_completo(record, dado, *(posicao+11), 11);
  }
}
int labirinto(completo *record, geral *dado, int *posicao)
{
  int numero_do_usuario;
  void menu_opcoes()
  {
    system("clear");
    printf("*************\n");
    printf("* Labirinto *\n");
    printf("*************\n");
    printf("\n");
    printf("1) Jogar\n");
    printf("2) Configurações\n");
    printf("3) Instrucoes\n\n");
    printf("0) Sair\n");
    printf("\n");
    printf("Digite uma opcao:  ");
  }
  void jogar()
  {
    void menu_jogar()
    {
      system("clear");
      printf("*************\n");
      printf("* Labirinto *\n");
      printf("*************\n");
      printf("\n");
    }
  }
  int configuracoes()
  {
    void menu_configuracoes()
    {
      system("clear");
      printf("*************\n");
      printf("* Labirinto *\n");
      printf("*************\n");
      printf("\n");
      printf("1) Alterar o numero maximo(atual: %d)\n", 3);
      printf("2) Voltar ao menu alterior\n");
    }
    return 0;
  }
  void instrucoes()
  {
    system("clear");
    printf("Instrucoes!\n");
  }
  while(1)
  {
    menu_opcoes();
    numero_do_usuario = pega_algarismo(0, 3);
    if(numero_do_usuario == 1)
      jogar();
    else if(numero_do_usuario == 2)
      configuracoes();
    else if(numero_do_usuario == 3)
      instrucoes();
    else
    {
      limpa_buffer();
      return 0;
    }
  }
}
int main()
{
	int opcao, posicao[jogos];
	char string[20];
	geral score[player], dado;
	completo record[jogos];
	void menu_principal()
	{
		system("clear");
		printf("\n");
		printf("\t***********************\n");
		printf("\t* A Taverna de Mangix *\n");
		printf("\t***********************\n");
		printf("\n");
		printf("Menu:\n");
		printf("    a) Senha\n");
		printf("    b) Charadas!\n");
		printf("    c) PTP\n");
		printf("    d) 2048\n");
		printf("    e) Jogo da Velha\n");
		printf("    f) Jogo da Memória\n");
		printf("    g) Xadrez!\n");
		printf("    h) Campo minado\n");
		printf("    i) Roda a roda\n");
		printf("    j) Cobrinha\n");
		printf("    k) Pac-Man\n");
		printf("    l) Labirinto\n");
		printf("    z) Toca dos Feras!\n\n");
		printf("    ESC) Sair\n");
		printf("\n");
		printf(" Digite sua opcao: ");
	}
	void boas_vindas()
	{
		system("clear");
		printf("Olá %s. Seja bem vindo à Taverna de Mangix, o Mestre Cervejeiro!\n\n", dado.nome);
		printf("Vê-se que és novo no jogo. Sente-se comigo e aproveite de uma cerveja!\n\n");
		printf("    wwwww                           _________________________        ____________ \n");
		printf("  </ . . \\>                        /                        /       /          /| \n");
		printf("   \\  v  /              ________  /   .   |_|    |_|       /       /          / / \n");
		printf(" ___|;;;|__            /       / /   / \\                  /       /          / / \n");
		printf("/          \\          /_______/ /    | |   |_|           /|      /          / /\n");
		printf("| /\\    /\\ |            |   |  /     |_|                / |     /          / / \n");
		printf("| ||    || |            |___| /                        /| |    /          / /  \n");
		printf("[_]/ /\\ \\[_]      ________   /________________________/ | |   /          / / \n");
		printf("  | /  \\ |       /       /    | |   | |           | |   | |  /          / / \n");                              
		printf("  / |  | \\      /_______/     | |   | |           | |   | | /          / /   \n");
		printf("  | |  | |        |   |       | |   | |           | |   | |/__________/ /   \n");
		printf(" <__]  [__>       |___|       | |   |_|           | |   |_||__________|/  \n");
		printf("                              | |                 | |         |   | /    \n");
		printf("                              |_|                 |_|         |___|/     \n");
	}
	void imp()
	{
		printf("\n\ncom");
		if(dado.total < 10)
			printf("     ");
		else if(dado.total < 100)
			printf("    ");
		else if(dado.total < 1000)
			printf("   ");
		else if(dado.total < 10000)
			printf("  ");
		else if(dado.total < 100000)
			printf(" ");
		printf("%d pontos: %s\n", dado.total, dado.nome);
		printf("\t              Senha: %d\n", *(dado.point+0));
		printf("\t           Charadas: %d\n", *(dado.point+1));
		printf("\tPedra Papel Tesoura: %d\n", *(dado.point+2));
		printf("\t               2048: %d\n", *(dado.point+3));
		printf("\t      Jogo da Velha: %d\n", *(dado.point+4));
		printf("\t    Jogo da Memoria: %d\n", *(dado.point+5));
		printf("\t             Xadrez: %d\n", *(dado.point+6));
		printf("\t       Campo Minado: %d\n", *(dado.point+7));
		printf("\t        Roda a Roda: %d\n", *(dado.point+8));
		printf("\t           Cobrinha: %d\n", *(dado.point+9));
		printf("\t            Pac Man: %d\n", *(dado.point+10));
		printf("\t          Labirinto: %d\n\n", *(dado.point+11));
		aperte_tecla(ENTER);
	}
	limpar_um(&dado);
	limpa_tudo(score, record);
	system("clear");
	printf("Digite seu nome: ");
	scanf("%19[^\n]", string);
	opcao = -1;
	do
	{
		opcao++;
		dado.nome[opcao] = string[opcao];
	}while(string[opcao] != '\0');
	for(opcao = 0; opcao < jogos; opcao++)
		posicao[opcao] = 0;
	if(!existe_arquivo(bin))
	{
		boas_vindas();
		aperte_tecla(ENTER);
	}
	else
	{
		if(importar_dados(score, record))
			return 1;
		verifica_se_ja_jogou(score, &dado, record, string, posicao);
	}
	while(1)
	{
		menu_principal();
		opcao = pega_letra('a', 'l', 'z', 'z') - 'a' + 1;
		if(opcao == 1)
			senha(record, &dado, posicao);
		else if(opcao == 2)
			charadas(record, &dado, posicao);
		else if(opcao == 3)
			pedra_papel_tesoura(record, &dado, posicao);
		else if(opcao == 4)
			jogo_2048(record, &dado, posicao);
		else if(opcao == 5)
			jogo_da_velha(record, &dado, posicao);
		else if(opcao == 6)
			jogo_da_memoria(record, &dado, posicao);
		else if(opcao == 7)
			xadrez(record, &dado, posicao);
		else if(opcao == 8)
			campo_minado(record, &dado, posicao);
		else if(opcao == 9)
			roda_roda(record, &dado, posicao);
		else if(opcao == 10)
			cobrinha(record, &dado, posicao);
		else if(opcao == 11)
			pacman(record, &dado, posicao);
		else if(opcao == 12)
			labirinto(record, &dado, posicao);
		else if(opcao == 'z' - 'a' + 1)
			ranks(score);
		else if(opcao == 1 - 'a')
		{
			organizar_geral(score);
			escreve_tudo(score, record);
			printf("\n\n");
			return 0;
		}
		posicao[0] = organiza_posicao_geral(score, &dado, posicao[0]);
	}
}