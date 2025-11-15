#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#if defined(_WIN32) || defined(_WIN64) // Implementação de getch compatível com conio.h para Windows
  #include <conio.h>
#else // Implementação de getch compatível com conio.h para Unix/Linux/macOS
  #include <termios.h>
  #include <unistd.h>
  
  int getch(void) {
      struct termios oldt, newt;
      int ch;
      int ret;
      
      // Obter configurações atuais do terminal
      ret = tcgetattr(STDIN_FILENO, &oldt);
      if (ret < 0) {
          // Se tcgetattr falhar (ex: stdin não é TTY), ler um caractere normalmente
          return getchar();
      }
      
      // Copiar configurações
      newt = oldt;
      
      // Desabilitar modo canônico e echo:
      // ICANON: desabilita a leitura canonizada (permite caractere por caractere)
      // ECHO:   desabilita o eco (não exibe o caractere digitado)
      newt.c_lflag &= ~(ICANON | ECHO);
      
      // Configurar leitura bloqueante:
      // VMIN=1:  aguarda até 1 byte antes de retornar
      // VTIME=0: sem timeout (espera indefinidamente)
      newt.c_cc[VMIN] = 1;
      newt.c_cc[VTIME] = 0;
      
      // Aplicar novas configurações imediatamente e descarregar output
      ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, &newt);
      if (ret < 0) {
          perror("tcsetattr");
          return EOF;
      }
      
      // Ler um caractere
      ch = getchar();
      
      // Restaurar configurações originais imediatamente
      ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldt);
      if (ret < 0) {
          perror("tcsetattr restore");
      }
      
      return ch;
  }
#endif

// Área para definição das variáveis GLOBAIS para armazenar as propriedades das cidades
typedef struct {
  char estado;
  char codigoCarta[3];
  char nomeCidade[50];
  int populacao;
  float area;
  float pib;
  int pontosTurismo;
  float densidadePopulacional;
  float pibPerCapita;
} Carta;

Carta carta[2];

typedef struct {
  int id;
  char descricao[50];
} Menu;

Menu menuPrincipal[6];
Menu menuAtributos[6];

typedef struct {
  int idCartaCadastrada;
  int idAtributo1Selecionado;
  int idAtributo2Selecionado;
  float valorAtributo1Selecionado;
  float valorAtributo2Selecionado;
  float somaAtributosCarta;
} DadosJogada;

void clearScreen()
{
    printf("\033[2J\033[H"); // Limpa e move o cursor para o topo
}

void clearInputBuffer()
{
    // Limpar o buffer de entrada, removendo caracteres residuais do scanf
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void coletarDadosdaCarta(char *estado, char *codigoCarta, char *nomeCidade, int *populacao, float *area, float *pib, int *pontosTurismo)
{
    char buffer[128];
    // Garantir buffer limpo antes de começar
    clearInputBuffer();

    // Estado: esperar por um caractere válido
    while (1) {
        printf("Digite o estado da cidade (uma letra A-H): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) return;
        // Pegar primeiro caractere não-espaço
        int i = 0;
        while (buffer[i] && isspace((unsigned char)buffer[i])) i++;
        if (buffer[i] && isalpha((unsigned char)buffer[i])) {
            *estado = toupper((unsigned char)buffer[i]);
            break;
        }
        printf("Entrada inválida. Informe uma letra (A-H).\n");
    }

    // Código da carta: até 2 dígitos
    while (1) {
        printf("Digite o código da carta (2 números ex: 01 a 04): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) return;
        // remover newline
        buffer[strcspn(buffer, "\r\n")] = '\0';
        size_t len = strlen(buffer);
        if (len >= 1 && len <= 2) {
            int allDigits = 1;
            for (size_t k = 0; k < len; k++) if (!isdigit((unsigned char)buffer[k])) allDigits = 0;
            if (allDigits) {
                // copiar e garantir terminação
                strncpy(codigoCarta, buffer, 3);
                codigoCarta[2] = '\0';
                break;
            }
        }
        printf("Código inválido. Informe 1 ou 2 dígitos numéricos.\n");
    }

    // Nome da cidade: não vazio
    while (1) {
        printf("Digite o nome da cidade: ");
        if (!fgets(nomeCidade, 50, stdin)) return;
        nomeCidade[strcspn(nomeCidade, "\r\n")] = '\0';
        if (strlen(nomeCidade) > 0) break;
        printf("Nome inválido. Informe um nome não vazio.\n");
    }

    // População: inteiro positivo
    while (1) {
        printf("Digite a população da cidade: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) return;
        if (sscanf(buffer, "%d", populacao) == 1 && *populacao > 0) break;
        printf("População inválida. Informe um número inteiro positivo.\n");
    }

    // Área: float positivo
    while (1) {
        printf("Digite a área da cidade (em km²): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) return;
        if (sscanf(buffer, "%f", area) == 1 && *area > 0.0f) break;
        printf("Área inválida. Informe um número maior que zero.\n");
    }

    // PIB: float não-negativo (em bilhões)
    while (1) {
        printf("Digite o PIB da cidade (em bilhões): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) return;
        if (sscanf(buffer, "%f", pib) == 1 && *pib >= 0.0f) break;
        printf("PIB inválido. Informe um número (pode ser 0).\n");
    }

    // Pontos turísticos: inteiro não-negativo
    while (1) {
        printf("Digite o número de pontos turísticos da cidade: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) return;
        if (sscanf(buffer, "%d", pontosTurismo) == 1 && *pontosTurismo >= 0) break;
        printf("Número inválido. Informe um inteiro não-negativo.\n");
    }
}

float calculaDensidadePopulacional(int populacao, float area)
{
    return populacao / area;
}

float calculaPibPerCapita(float pib, int populacao)
{
    float pibEmReais = pib * 1000000000; // Convertendo bilhões para reais
    return pibEmReais / populacao;
}

void exibirDadosDaCarta(int indiceCarta)
{
    printf("\n--- Carta0%d ---\n", indiceCarta + 1);
    if (carta[indiceCarta].estado == '\0') {
        printf("Estado: [Não fornecido]\n");
    } else {
        printf("Estado: %c\n", carta[indiceCarta].estado);
    }
    printf("Código: %c%s\n", carta[indiceCarta].estado == '\0' ? ' ' : carta[indiceCarta].estado, carta[indiceCarta].codigoCarta);
    printf("Nome da Cidade: %s\n", carta[indiceCarta].nomeCidade);
    printf("População: %d\n", carta[indiceCarta].populacao);
    printf("Área: %.2f km²\n", carta[indiceCarta].area);
    printf("PIB: %.2f bilhões\n", carta[indiceCarta].pib);
    printf("Pontos Turísticos: %d\n", carta[indiceCarta].pontosTurismo);
    printf("Densidade Populacional: %.2f hab/km²\n", carta[indiceCarta].densidadePopulacional);
    printf("PIB per Capita: %.2f reais\n", carta[indiceCarta].pibPerCapita);
}

void exibirMenuAtributos(int atributo1Selecionado, int atributo2Selecionado)
{
    printf("\n=== Desafio Super Trunfo - Cidades ===\n");
    printf("Escolha um atributo para comparação:\n");

    for (int ma = 0; ma < 6; ma++)
    {
      int indiceSelecionado = ma + 1;

      if (atributo1Selecionado == indiceSelecionado || atributo2Selecionado == indiceSelecionado)
        printf("| %d) %s (já selecionado)\n", menuAtributos[ma].id, menuAtributos[ma].descricao);
      else
        printf("| %d) %s \n", menuAtributos[ma].id, menuAtributos[ma].descricao);
    }
    
    printf("======================================\n\n");
    printf("Digite o número do atributo escolhido: ");
}

void jogar()
{
    int opcao = 0;

    menuPrincipal[0].id = 1;
    snprintf(menuPrincipal[0].descricao, sizeof(menuPrincipal[0].descricao), "Cadastrar Carta 01");
    menuPrincipal[1].id = 2;
    snprintf(menuPrincipal[1].descricao, sizeof(menuPrincipal[1].descricao), "Cadastrar Carta 02");
    menuPrincipal[2].id = 3;
    snprintf(menuPrincipal[2].descricao, sizeof(menuPrincipal[2].descricao), "Exibir Regras do Jogo");
    menuPrincipal[3].id = 4;
    snprintf(menuPrincipal[3].descricao, sizeof(menuPrincipal[3].descricao), "Imprimir cartas");
    menuPrincipal[4].id = 5;
    snprintf(menuPrincipal[4].descricao, sizeof(menuPrincipal[4].descricao), "Selecionar Atributos");
    menuPrincipal[5].id = 6;
    snprintf(menuPrincipal[5].descricao, sizeof(menuPrincipal[5].descricao), "Sair");

    menuAtributos[0].id = 1;
    snprintf(menuAtributos[0].descricao, sizeof(menuAtributos[0].descricao), "População");
    menuAtributos[1].id = 2;
    snprintf(menuAtributos[1].descricao, sizeof(menuAtributos[1].descricao), "Área");
    menuAtributos[2].id = 3;
    snprintf(menuAtributos[2].descricao, sizeof(menuAtributos[2].descricao), "PIB");
    menuAtributos[3].id = 4;
    snprintf(menuAtributos[3].descricao, sizeof(menuAtributos[3].descricao), "Pontos Turísticos");
    menuAtributos[4].id = 5;
    snprintf(menuAtributos[4].descricao, sizeof(menuAtributos[4].descricao), "Densidade Populacional");
    menuAtributos[5].id = 6;
    snprintf(menuAtributos[5].descricao, sizeof(menuAtributos[5].descricao), "Voltar ao menu principal");

    do
    {
        printf("\n=== Desafio Super Trunfo - Cidades ===\n");

        for (int mp = 0; mp < 6; mp++)
        {
          printf("| %d) %s\n", menuPrincipal[mp].id, menuPrincipal[mp].descricao);
        }
        
        printf("======================================\n\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao)
        {
            case 1:
                printf("Opção 1 selecionada: Cadastrar Carta 01.\n");
                coletarDadosdaCarta(&carta[0].estado, carta[0].codigoCarta, carta[0].nomeCidade, &carta[0].populacao, &carta[0].area, &carta[0].pib, &carta[0].pontosTurismo);

                // Calcular Densidade Populacional
                carta[0].densidadePopulacional = calculaDensidadePopulacional(carta[0].populacao, carta[0].area);

                // Calcular PIB per capita
                carta[0].pibPerCapita = calculaPibPerCapita(carta[0].pib, carta[0].populacao);

                break;
            
            case 2:
                printf("Opção 2 selecionada: Cadastrar Carta 02.\n");
                coletarDadosdaCarta(&carta[1].estado, carta[1].codigoCarta, carta[1].nomeCidade, &carta[1].populacao, &carta[1].area, &carta[1].pib, &carta[1].pontosTurismo);

                // Calcular Densidade Populacional
                carta[1].densidadePopulacional = calculaDensidadePopulacional(carta[1].populacao, carta[1].area);
                
                // Calcular PIB per capita
                carta[1].pibPerCapita = calculaPibPerCapita(carta[1].pib, carta[1].populacao);

                break;

            case 3:
                printf("Opção 3 selecionada: Exibir Regras do Jogo.\n");
                printf("Regras do Jogo:\n");
                printf("1. Cada jogador escolhe uma carta.\n");
                printf("2. Os jogadores comparam atributos das cartas.\n");
                printf("3. O jogador com o melhor atributo vence a rodada.\n");
                printf("4. O jogo continua até que todas as cartas sejam jogadas.\n");
                printf("5. O jogador com mais vitórias no final é o vencedor!\n");

                printf("\nPressione qualquer tecla para continuar...\n");
                clearInputBuffer();
                getch();

                break;

            case 4:
                printf("Opção 4 selecionada: Imprimir cartas.\n");
                
                // Exibir dados da Carta 01
                if (carta[0].estado == '\0') {
                  printf("\nCarta 01 não cadastrada.\n");
                } else {
                  exibirDadosDaCarta(0);
                }
                
                // Exibir dados da Carta 02
                if (carta[1].estado == '\0') {
                  printf("\nCarta 02 não cadastrada.\n");
                } else {
                  exibirDadosDaCarta(1);
                }

                printf("\nPressione qualquer tecla para continuar...\n");
                clearInputBuffer();
                getch();

                break;
            
            case 5:

                SELECAO_ATRIBUTO:

                // Exibir dados da Carta 01
                if (carta[0].estado == '\0') {
                  printf("\nCarta 01 não cadastrada.\n");
                  printf("\nPressione qualquer tecla para continuar...\n");
                  clearInputBuffer();
                  getch();
                  break;
                }
                
                // Exibir dados da Carta 02
                if (carta[1].estado == '\0') {
                  printf("\nCarta 02 não cadastrada.\n");
                  printf("\nPressione qualquer tecla para continuar...\n");
                  clearInputBuffer();
                  getch();
                  break;
                }

                printf("Opção 5 selecionada: Seleção de atributos.\n");

                int atributo1Selecionado = 0;
                int atributo2Selecionado = 0;

                DadosJogada cartaJogada[2];

                cartaJogada[0].idCartaCadastrada = 1; // Carta 01
                cartaJogada[1].idCartaCadastrada = 2; // Carta 02

                exibirMenuAtributos(atributo1Selecionado, atributo2Selecionado);
                scanf("%d", &atributo1Selecionado);
                
                if (atributo1Selecionado == 6) {
                    // Voltar ao menu principal
                    break;
                }

                clearScreen();
                printf("\n");

                exibirMenuAtributos(atributo1Selecionado, atributo2Selecionado);
                scanf("%d", &atributo2Selecionado);
                
                if (atributo2Selecionado == 6) {
                    // Voltar ao menu principal
                    break;
                }

                if (atributo2Selecionado == atributo1Selecionado) {
                    printf("Atributos iguais selecionados. Por favor, escolha atributos diferentes.\n");
                    getch();
                    goto SELECAO_ATRIBUTO;
                }

                if (atributo1Selecionado < 1 || atributo1Selecionado > 5 || atributo2Selecionado < 1 || atributo2Selecionado > 5) {
                    printf("Atributo inválido selecionado. Por favor, escolha entre 1 e 5.\n");
                    getch();
                    goto SELECAO_ATRIBUTO;
                }

                cartaJogada[0].idAtributo1Selecionado = atributo1Selecionado;
                cartaJogada[0].idAtributo2Selecionado = atributo2Selecionado;

                cartaJogada[1].idAtributo1Selecionado = atributo1Selecionado;
                cartaJogada[1].idAtributo2Selecionado = atributo2Selecionado;

                clearScreen();

                printf("Comparando atributos selecionados em:\n");
                
                // Contagem regressiva de 3 segundos para pausa dramática
                for (int i = 3; i > 0; i--)
                {
                  printf("%d\n", i);
                  sleep(1); 
                }

                for (int l = 0; l < 2; l++)
                {
                  // Define o valor do atributo 1 da jogada para cada carta
                  switch (cartaJogada[l].idAtributo1Selecionado)
                  {
                    case 1:
                      cartaJogada[l].valorAtributo1Selecionado = carta[l].populacao;
                      break;
                    case 2:
                      cartaJogada[l].valorAtributo1Selecionado = carta[l].area;
                      break;
                    case 3:
                      cartaJogada[l].valorAtributo1Selecionado = carta[l].pib;
                      break;
                    case 4:
                      cartaJogada[l].valorAtributo1Selecionado = carta[l].pontosTurismo;
                      break;
                    case 5:
                      cartaJogada[l].valorAtributo1Selecionado = carta[l].densidadePopulacional;
                      break;
                  }

                  // Define o valor do atributo 2 da jogada para cada carta
                  switch (cartaJogada[l].idAtributo2Selecionado)
                  {
                    case 1:
                      cartaJogada[l].valorAtributo2Selecionado = carta[l].populacao;
                      break;
                    case 2:
                      cartaJogada[l].valorAtributo2Selecionado = carta[l].area;
                      break;
                    case 3:
                      cartaJogada[l].valorAtributo2Selecionado = carta[l].pib;
                      break;
                    case 4:
                      cartaJogada[l].valorAtributo2Selecionado = carta[l].pontosTurismo;
                      break;
                    case 5:
                      cartaJogada[l].valorAtributo2Selecionado = carta[l].densidadePopulacional;
                      break;
                  }

                  cartaJogada[l].somaAtributosCarta = cartaJogada[l].valorAtributo1Selecionado + cartaJogada[l].valorAtributo2Selecionado;
                }
                
                clearScreen();

                printf("Resultado da jogada por atributo: \n");

                printf("Carta 01 - 1º Atributo: %.2f\n", cartaJogada[0].valorAtributo1Selecionado);
                printf("Carta 02 - 1º Atributo: %.2f\n", cartaJogada[1].valorAtributo1Selecionado);

                int cartaVencedoraAtributo1 = 0;

                switch (atributo1Selecionado)
                {
                  case 1: // População
                  case 2: // Área
                  case 3: // PIB
                  case 4: // Pontos Turísticos
                    if (cartaJogada[0].valorAtributo1Selecionado > cartaJogada[1].valorAtributo1Selecionado)
                      cartaVencedoraAtributo1 = 1;
                    else if (cartaJogada[0].valorAtributo1Selecionado < cartaJogada[1].valorAtributo1Selecionado)
                      cartaVencedoraAtributo1 = 2;
                    else
                      cartaVencedoraAtributo1 = 0; /* empate */
                    break;
                  case 5: // Densidade Populacional
                    if (cartaJogada[0].valorAtributo1Selecionado < cartaJogada[1].valorAtributo1Selecionado)
                      cartaVencedoraAtributo1 = 1;
                    else if (cartaJogada[0].valorAtributo1Selecionado > cartaJogada[1].valorAtributo1Selecionado)
                      cartaVencedoraAtributo1 = 2;
                    else
                      cartaVencedoraAtributo1 = 0; /* empate */
                    break;
                }

                if (cartaVencedoraAtributo1 == 1)
                  printf("Carta 01 vence no 1º atributo!\n");
                else if (cartaVencedoraAtributo1 == 2)
                  printf("Carta 02 vence no 1º atributo!\n");
                else
                  printf("Empate no 1º atributo!\n");

                printf("Carta 01 - 2º Atributo: %.2f\n", cartaJogada[0].valorAtributo2Selecionado);
                printf("Carta 02 - 2º Atributo: %.2f\n", cartaJogada[1].valorAtributo2Selecionado);

                int cartaVencedoraAtributo2 = 0;

                switch (atributo2Selecionado)
                {
                  case 1: // População
                  case 2: // Área
                  case 3: // PIB
                  case 4: // Pontos Turísticos
                    if (cartaJogada[0].valorAtributo2Selecionado > cartaJogada[1].valorAtributo2Selecionado)
                      cartaVencedoraAtributo2 = 1;
                    else if (cartaJogada[0].valorAtributo2Selecionado < cartaJogada[1].valorAtributo2Selecionado)
                      cartaVencedoraAtributo2 = 2;
                    else
                      cartaVencedoraAtributo2 = 0; /* empate */
                    break;
                  case 5: // Densidade Populacional
                    if (cartaJogada[0].valorAtributo2Selecionado < cartaJogada[1].valorAtributo2Selecionado)
                      cartaVencedoraAtributo2 = 1;
                    else if (cartaJogada[0].valorAtributo2Selecionado > cartaJogada[1].valorAtributo2Selecionado)
                      cartaVencedoraAtributo2 = 2;
                    else
                      cartaVencedoraAtributo2 = 0; /* empate */
                    break;
                }

                if (cartaVencedoraAtributo2 == 1)
                  printf("Carta 01 vence no 2º atributo!\n");
                else if (cartaVencedoraAtributo2 == 2)
                  printf("Carta 02 vence no 2º atributo!\n");
                else
                  printf("Empate no 2º atributo!\n");
                
                printf("Soma dos Atributos da Carta 01: %.2f\n\n", cartaJogada[0].somaAtributosCarta);
                printf("Soma dos Atributos da Carta 02: %.2f\n\n", cartaJogada[1].somaAtributosCarta);
                
                if (cartaJogada[0].somaAtributosCarta > cartaJogada[1].somaAtributosCarta)
                  printf("Carta 01 é a vencedora da jogada!\n");
                else if (cartaJogada[0].somaAtributosCarta < cartaJogada[1].somaAtributosCarta)
                  printf("Carta 02 é a vencedora da jogada!\n");
                else
                  printf("A jogada terminou em empate!\n");

                printf("\nPressione qualquer tecla para continuar...\n");
                clearInputBuffer();
                getch();
                
                break;

            case 6:
                printf("Saindo do jogo. Obrigado por jogar!\n");

                printf("\nPressione qualquer tecla para continuar...\n");
                clearInputBuffer();
                getch();

                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                printf("\nPressione qualquer tecla para continuar...\n");
                clearInputBuffer();
                getch();
                
                break;
        }
            
        clearScreen();

    } while (opcao != 6);
}

int main()
{
    clearScreen();
    setlocale(LC_ALL, "Portuguese_Brazil.1252");
    jogar();

    return 0;
}
