#include <stdio.h>
#include <windows.h> //biblioteca para o sleep

#define MAX_DISCOS 9 //define que o numero máximo de discos é 8

//declaracao de variaveis globais (elas serao usadas em diferentes funcoes)
int discos; 
char disco_jogado; //disco que sera movido na jogada atual, escolhido pelo jogador, em letra
int numero_de_jogadas;
int linha, coluna, coluna_nova, coluna_antiga, coluna_auxiliar;
int resposta = 0; //determina se o usuário irá jogar ou apenas assistir o jogo. declarada como global pois o resultado influencia em outra funcao (imprimir tabuleiro). inicializa em 0 para evitar erro
char *temp;
char *tabuleiro [MAX_DISCOS][3]; //nao posso declarar a matriz como [discos][3], porque uma matriz nao pode ter um tamanho variavel globalmente. 
                                    //Entao definindo o numero maximo de discos conseguimos contornar isso. o +1 eh para ter uma casa alem dos discos, para nao bugar a memoria.

char *vazio = "       |       "; //cria variável vazio para armazenar o espaçamento correto
int disco_superior [3]; //cria um vetor para armazenar qual é a linha do disco mais alto em cada coluna, que sera atualizado a cada jogada
char *disco; //sera usado para descobrir a letra 


//retorna a string do disco jogado para imprimir no tabuleiro (funcao)
char *disco_string (char disco)
{
     if (disco == 'A') return "       A       ";
     if (disco == 'B') return "      =B=      "; 
     if (disco == 'C') return "     ==C==     ";
     if (disco == 'D') return "    ===D===    ";
     if (disco == 'E') return "   ====E====   ";
     if (disco == 'F') return "  =====F=====  ";
     if (disco == 'G') return " ======G====== ";
     if (disco == 'H') return "=======H=======";
     return vazio;
}
 
 
 
//descobre a letra do disco (funcao)
char disco_letra(char *disco) //aqui disco eh usado como um ponteiro, apontando para a string dos discos
{
   int numero;
   for (numero = 0; disco[numero] != '\0'; numero++)  //percorre toda a string do disco ate o ultimo caractere (nulo "\0")
         {
           if (disco[numero] >= 'A' && disco[numero] <= 'H')  //encontra a letra na string 
              {
               return disco[numero];
              }
         }
   return '\0'; // caso não ache uma letra na string, retorna nulo
}

 
 
//cria o tabuleiro inicial (funcao)
void inicializar_tabuleiro() 
{
    for (linha = 0; linha < discos; linha++) 
        {
            tabuleiro[linha][0] = disco_string ('A' + (discos - 1 - linha)); //salva os discos na coluna 0. 
            // A formula define qual disco vai em cada linha, de forma que comece pelo maior na linha zero
            tabuleiro [linha] [1] = vazio; //salva o valor vazio em toda a coluna 1 e 2
            tabuleiro [linha] [2] = vazio; 
        }
    
     //define qual eh o disco mais alto em cada coluna  no momento
     disco_superior[0] = discos;  //a linha do disco superior comeca como o proprio numero de discos (estao todos aqui)
     disco_superior[1] = 0; //a segunda e terceira colunas nao terao nenhum disco superior, entao o valor salvo é zero
     disco_superior[2] = 0;
}




//imprime o tabuleiro (funcao)
void imprimir_tabuleiro (int discos)
{
        printf ("\n");
        for (linha = 0; linha<50; linha++) //limpar a tela
            printf ("\n");
        
          // laco para imprimir o tabuleiro 
        for (linha = discos-1; linha >=0; linha--)  //a linha decrementa para que visualmente o disco maior fique embaixo 
            {
              for (coluna = 0; coluna < 3; coluna++) 
                  {
                   if (linha < disco_superior [coluna]) //se a linha analisada estiver abaixo do disco superior, imprime o disco
                       {
                        printf("%s", tabuleiro [linha] [coluna]);
                       }
                   else //se a linha analisada estiver acima do disco superior, imprime vazio
                        {
                         printf("%s", vazio); 
                         }
                   }
            printf("\n"); //pula para a próxima linha quando termina de imprimir a linha atual do tabuleiro
            }
        printf("    Torre 1        Torre 2        Torre 3       \n");
        printf ("\nNumero de jogadas: %d\n", numero_de_jogadas); 
        printf ("\n");
        
         if (resposta==2) 
            Sleep(900); // se estiver no jogo automatico, faz pausa para ver as jogadas       
}





//move o disco (funcao)
int mover_disco(int coluna_antiga, int coluna_nova, int erro) //erro define se aparecera msg de erro ou nao (jogador x auto)
{     
     temp = tabuleiro[disco_superior[coluna_antiga] - 1][coluna_antiga]; //salva valor do disco em variavel auxiliar
     
     //tratamento de erros no modo jogador 
     if (erro==1) 
         {  
           if (coluna_antiga < 0 || coluna_antiga >= 3)
                {
                  printf ("\nJogada invalida. Voce so pode mover discos que estejam no topo!\n"); 
                  return 0;
                }                   
           if (disco_superior[coluna_nova] > 0 && disco_letra(tabuleiro[disco_superior[coluna_antiga] - 1][coluna_antiga]) > disco_letra(tabuleiro[disco_superior[coluna_nova] - 1][coluna_nova])) 
              {//se tiver algum disco na coluna e se esse disco for maior que o inferior, da erro
               printf("\nJogada invalida. Nao pode colocar um disco maior sobre um menor!\n");
               return 0;
               }
          
            if (coluna_antiga == -1 || disco_jogado != disco_letra(tabuleiro[disco_superior[coluna_antiga] - 1][coluna_antiga])) 
               {//se a coluna antiga for desconhecida ou se o disco for diferente do disco superior, da erro
                 printf("\nJogada invalida. Voce so pode mover discos que estejam no topo!\n"); 
                 return 0;
               }
          
           if (coluna_antiga == coluna_nova) 
              {//se tentar colocar o disco na propria coluna, da erro
               printf("\nJogada invalida. O disco ja esta nessa coluna!\n");
               return 0;
              } 
         }

    //move o disco se ele passou pela verificacao
    tabuleiro[disco_superior[coluna_nova]][coluna_nova] = temp; //salva disco na nova posicao 
    tabuleiro[disco_superior[coluna_antiga] - 1][coluna_antiga] = vazio; //'apaga' o disco da posicao antiga
    
    disco_superior[coluna_antiga]--; //decrementa o disco superior daquela coluna antiga
    disco_superior[coluna_nova]++; //incrementa o disco superior da coluna nova
    
    // incrementa o numero de jogadas se estiver no modo manual
    if (erro == 1)
       numero_de_jogadas++;
    
    return 1;//indica jogada bem sucedida   
}




//verifica qual a melhor jogada no modo automatico quando houver mais de uma opcao (funcao)
void mover_entre_torres(int torre_antiga, int torre_nova) 
{  
      // Se a torre antiga estiver vazia e a nova não, vai mover da nova para a antiga
    if (disco_superior[torre_antiga] == 0 && disco_superior[torre_nova] > 0) 
       {
          mover_disco(torre_nova, torre_antiga, 0);
          return;  
       }
       
      // Se torre nova estiver vazia e torre antiga não, vai mover da torre antiga para torre nova
    if (disco_superior[torre_nova] == 0 && disco_superior[torre_antiga] > 0)  
        {
           mover_disco(torre_antiga, torre_nova, 0);
           return;
        }
        
    // Se nenhuma torre estiver vazia, vai comparar os discos do topo 
    
      //salva qual o disco superior da coluna antiga e da nova em outras variaveis
    char disco_superior1 = disco_letra(tabuleiro[disco_superior[torre_antiga] - 1][torre_antiga]); 
    char disco_superior2 = disco_letra(tabuleiro[disco_superior[torre_nova] - 1][torre_nova]); 
    
    //verifica qual eh a unica jogada disponivel
    
      //se o disco da torre antiga for menor que o da nova, vai mover o disco para a torre nova  
    if (disco_superior1 < disco_superior2) 
        {  
          mover_disco(torre_antiga, torre_nova, 0);
        } 
    else //se o disco da torre antiga for maior que o da nova, vai mover da nova para a antiga  
        {
          mover_disco(torre_nova, torre_antiga, 0);
        }
}
  
  
 
//jogo automático (funcao)
void hanoi_automatico(int coluna_antiga, int coluna_nova, int coluna_auxiliar) 
{
      //se o numero de discos for par, inverte as torres auxiliar e nova
    if (discos % 2 == 0) 
        { 
           int temp_2 = coluna_nova; 
           coluna_nova = coluna_auxiliar;
           coluna_auxiliar = temp_2;
         }
         
      //fórmula do total de movimentos que serão feitos
    int total_movimentos = (1 << discos) - 1; 
    
      //enquanto o numero de jogadas for menor que o total de movimentos, o jogo continua
    for (numero_de_jogadas = 1; numero_de_jogadas <= total_movimentos; numero_de_jogadas++) 
         {   //se for a primeira jogada do loop (jogadas 1, 4, 7...), move o disco da coluna antiga para a nova
           if (numero_de_jogadas % 3 == 1) 
               mover_entre_torres(coluna_antiga, coluna_nova);
        
           else 
                {  //se for a segunda jogada do loop (jogadas 2, 5, 8...), move o disco da coluna antiga para a auxiliar   
                  if (numero_de_jogadas % 3 == 2)   
                      mover_entre_torres(coluna_antiga, coluna_auxiliar);
            
                   else 
                        {   //se for a terceira jogada do loop (jogadas 3, 6, 9...), move o disco da coluna auxiliar para a nova
                           if (numero_de_jogadas % 3 == 0)
                               mover_entre_torres(coluna_auxiliar, coluna_nova);
                        }
                }
            imprimir_tabuleiro(discos);
         }   
}
      
//obs: a lógica da torre de hanoi é esta: 
//Na primeira jogada, movemos o menor disco disponível para a coluna auxiliar. 
//Na segunda jogada, fazemos a única jogada disponível envolvendo outro disco. 
//Na terceira jogada, movemos o menor disco disponível para a coluna final. 
//para definir se estamos na primeira, segunda ou terceira jogada do looping, usamos o resto da divisao por 3
//Essas três jogadas ocorrem em looping até terminar o jogo. 
//Agora, se o numero de discos for par, invertemos a coluna auxiliar com a coluna final (invertendo o ciclo do disco menor)       



//funcao principal
int main ()
{
    
   int reiniciar_jogo; 
   char nome [25];   

do
{  //esse do while engloba quase toda a funcao main, para reiniciar o jogo se o usuario quiser        
        
   if (reiniciar_jogo == 1) //limpar tela
       {   printf ("\n");
           for (linha = 0; linha<50; linha++) 
                { 
                  printf ("\n");
                }
       }  
       
   numero_de_jogadas = 0;
     
   printf ("\nVamos jogar Torre de Hanoi!\n\n");  
   printf ( "Regras do Jogo: \n\n"); 
   printf ("1. Eh permitido mover apenas um disco por vez.\n");
   printf ("2. Um disco maior nunca pode ser colocado sobre um disco menor.\n");
   printf ("3. O objetivo eh transferir todos os discos da torre inicial para a final, utilizando a segunda torre como auxiliar.\n");
 
   //jogador escolhe quem ira jogar  
   do {
          printf ("\nDigite 1 se voce quiser jogar. Digite 2 se quiser ver o computador jogando: ");
          if (scanf ("%d", &resposta)!=1); //ao mesmo tempo que o scanf eh verificado se eh numero inteiro mesmo, ele eh executado
             getchar ();
          if (resposta != 1 && resposta !=2) //se o jogador digitar uma opcao diferente de 1 ou 2, da erro
              printf ("\nEntrada invalida, tente novamente\n");
      }
   while (resposta != 1 && resposta !=2); //repete a pergunta enquanto o jogador errar a resposta
      
      
   //jogo automatico     
   if (resposta == 2) //se o jogador digitou 2 na main, entra no jogo automatico
   { 
      do // laco para tratamento de erros
          {
            printf ("\nQuantos discos voce quer? (escolha entre 2 e 8) >> "); 
            if (scanf ("%d",&discos)!=1); //ao mesmo tempo que o scanf eh verificado, ele eh executado
                getchar (); //limpa o buffer
          
            if (discos>8 || discos<2)
                printf ("\nEntrada invalida, tente novamente\n");
            }    
       while (discos>8 || discos<2); 

       inicializar_tabuleiro ();
       
       imprimir_tabuleiro (discos); 
       
       hanoi_automatico (0, 2, 1); //numeros das torres como argumentos da funcao
       
         //imprime um numero de jogadas a menos, pois ela comeca como zero
       printf ("Fim do Jogo. O computador venceu em %d jogadas.\n\n", numero_de_jogadas-1); 
      
       
       system ("pause"); 
       
       return 0; //finaliza o programa 
    } 
 
 
 
 
    //jogo com o jogador 
    printf ("\nDigite o nome do jogador >> ");
    
    scanf(" %[^\n]", nome); //salva o nome do jogador como string incluindo espaços 
    
    do
       {
          printf ("\n %s, quantos discos voce quer? (escolha entre 2 e 8) >> ", nome); 
          if (scanf ("%d",&discos)!=1); 
             getchar (); 
             
          if (discos>8 || discos<2) 
             printf ("\nEntrada invalida, tente novamente\n");
       } 
       
   while (discos>8 || discos<2);


   inicializar_tabuleiro (); 
   
   imprimir_tabuleiro (discos); 
 

   do  //repete laco ate o jogador ganhar
      {  
        do //repete a pergunta se o jogador digitar algo invalido
           {     
             printf ("\nEscreva sua jogada (letra do disco e o numero da torre destino. ex: A2, use maiuscula), %s >> ", nome);
             if (scanf (" %c%d", &disco_jogado, &coluna_nova)!=1)
                getchar ();
             
             coluna_nova--; //decrementa a coluna nova, para igualar torre com coluna
             
             if (disco_jogado<'A' || disco_jogado> ('A' + discos - 1) || coluna_nova <0 || coluna_nova>2) //tratamento de erros
                {
                     printf ("\nEntrada Invalida.\n\n"); 
                }            
             }
         while (disco_jogado<'A' || disco_jogado> ('A' + discos - 1) || coluna_nova <0 || coluna_nova>2); 
        
       //detecta pela letra do disco qual eh a sua coluna
       coluna_antiga = -1;  //comeca como -1 para mostrar que ainda nao se sabe qual a coluna
       for (coluna = 0; coluna < 3; coluna ++) 
           {
                  {    //se a coluna tiver mais de um disco sera verificada
                      if (disco_superior [coluna] > 0) 
                        {   //confere se o disco esta na coluna verificada
                         if (disco_letra(tabuleiro[disco_superior[coluna] - 1][coluna]) == disco_jogado)   
                        {  coluna_antiga = coluna; //se for igual, encontrou a coluna antiga e salva o valor na variavel
                          break;
                        }
                        }
                   } //se o valor da coluna foi atualizado, sai do laco
               if (coluna_antiga != -1) 
                   break; 
            }                                  
               
       if (mover_disco (coluna_antiga, coluna_nova, 1)) //tipo de erro 1 para verificar se a jogada eh valida
          {
            imprimir_tabuleiro (discos); //so imprime o tabuleiro se o movimento der certo
            printf ("\nJogadas: %d\n", numero_de_jogadas); 
          }       
        
      }  //quando o disco superior da ultima torre for igual ao numero de discos, vai sair do loop
   while (disco_superior [2] <  discos);    
 
 
   
   printf ("\nParabens!! Voce conseguiu em %d jogadas!\n", numero_de_jogadas);
  
   
   do //laco para reiniciar o jogo ou finalizar, o jogador escolhe
     {
       printf ("\n Quer jogar de novo? Digite 1 para sim e 2 para sair: ");
       
       if (scanf ("%d", &reiniciar_jogo) != 1); //verifica e executa o scanf para evitar erros
           getchar (); 
           
       if (reiniciar_jogo != 1 && reiniciar_jogo != 2)
             printf ("\nEntrada invalida, tente novamente\n");
      } 
   while (reiniciar_jogo != 1 && reiniciar_jogo != 2);  //repete a pergunta enquanto a resposta for diferente de 1 ou 2
   
   if (reiniciar_jogo == 2) //encerra o programa
       return 0;

}
while (reiniciar_jogo == 1); //recomeça o jogo, voltando para o 'do' logo no inicio da main

                 
   system ("pause");
 
   return 0;
 
}


