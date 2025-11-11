#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <conio.h>

// Desafio Super Trunfo - Países
// Tema 1 - Cadastro das cartas
// Objetivo: No nível novato você deve criar as cartas representando as cidades utilizando scanf para entrada de dados e printf para exibir as informações.
// Objetivo: No nível aventureiro você deve calcular e exibir a densidade populacional (população/área) e o PIB per capita (PIB/população) de cada cidade.

// Área para definição das variáveis GLOBAIS para armazenar as propriedades das cidades
char estado1;
char estado2;
char codigoCarta1[3]; // aumento para 3 (2 dígitos + '\0')
char codigoCarta2[3]; // ...
char nomeCidade1[50];
char nomeCidade2[50];
int populacao1;
int populacao2;
float area1;
float area2;
float pib1;
float pib2;
int pontosTurismo1;
int pontosTurismo2;

void clearScreen()
{
    printf("\033[2J\033[H"); // Limpa e move o cursor para o topo
}

void coletarDadosdaCarta(char *estado, char *codigoCarta, char *nomeCidade, int *populacao, float *area, float *pib, int *pontosTurismo)
{
    printf("Digite o estado da cidade (uma letra A-H): ");
    scanf(" %c", estado);
    printf("Digite o código da carta (2 números ex: 01 a 04): ");
    scanf("%2s", codigoCarta);
    printf("Digite o nome da cidade: ");
    scanf(" %[^\n]", nomeCidade);
    printf("Digite a população da cidade: ");
    scanf("%d", populacao);
    printf("Digite a área da cidade (em km²): ");
    scanf("%f", area);
    printf("Digite o PIB da cidade (em bilhões): ");
    scanf("%f", pib);
    printf("Digite o número de pontos turísticos da cidade: ");
    scanf("%d", pontosTurismo);
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

void exibirDadosDaCarta(int indiceCarta, char estado, char *codigoCarta, char *nomeCidade, int populacao, float area, float pib, int pontosTurismo, float densidadePopulacional, float pibPerCapita)
{
    printf("\n--- Carta0%d ---\n", indiceCarta);
    printf("Estado: %c\n", estado);
    printf("Código: %c%s\n", estado, codigoCarta);
    printf("Nome da Cidade: %s\n", nomeCidade);
    printf("População: %d\n", populacao);
    printf("Área: %.2f km²\n", area);
    printf("PIB: %.2f bilhões\n", pib);
    printf("Pontos Turísticos: %d\n", pontosTurismo);
    printf("Densidade Populacional: %.2f hab/km²\n", densidadePopulacional);
    printf("PIB per Capita: %.2f reais\n", pibPerCapita);
}

void jogar()
{
    int opcao = 0;

    do
    {
        printf("\n=== Desafio Super Trunfo - Países ===\n");
        printf("1) Cadastrar Carta 01.\n");
        printf("2) Cadastrar Carta 02.\n");
        printf("3) Exibir Regras do Jogo.\n");
        printf("4) Imprimir cartas.\n");
        printf("5) Sair.\n");
        printf("=====================================\n\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao)
        {
            case 1:
                printf("Opção 1 selecionada: Cadastrar Carta 01.\n");
                coletarDadosdaCarta(&estado1, codigoCarta1, nomeCidade1, &populacao1, &area1, &pib1, &pontosTurismo1);

                // Calcular Densidade Populacional
                float densidadePopulacional1 = calculaDensidadePopulacional(populacao1, area1);

                // Calcular PIB per capita
                float pibPerCapita1 = calculaPibPerCapita(pib1, populacao1);

                break;
            
            case 2:
                printf("Opção 2 selecionada: Cadastrar Carta 02.\n");
                coletarDadosdaCarta(&estado2, codigoCarta2, nomeCidade2, &populacao2, &area2, &pib2, &pontosTurismo2);

                // Calcular Densidade Populacional
                float densidadePopulacional2 = calculaDensidadePopulacional(populacao2, area2);
                
                // Calcular PIB per capita
                float pibPerCapita2 = calculaPibPerCapita(pib2, populacao2);

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
                getch();

                break;

            case 4:
                printf("Opção 4 selecionada: Imprimir cartas.\n");
                
                // Exibir dados da Carta 01
                exibirDadosDaCarta(1, estado1, codigoCarta1, nomeCidade1, populacao1, area1, pib1, pontosTurismo1, calculaDensidadePopulacional(populacao1, area1), calculaPibPerCapita(pib1, populacao1));
                
                // Exibir dados da Carta 02
                exibirDadosDaCarta(2, estado2, codigoCarta2, nomeCidade2, populacao2, area2, pib2, pontosTurismo2, calculaDensidadePopulacional(populacao2, area2), calculaPibPerCapita(pib2, populacao2));

                printf("\nPressione qualquer tecla para continuar...\n");
                getch();

                break;
            
            case 5:
                printf("Saindo do jogo. Obrigado por jogar!\n");
                printf("\nPressione qualquer tecla para continuar...\n");
                getch();

                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                printf("\nPressione qualquer tecla para continuar...\n");
                getch();
                
                break;
        }
            
        clearScreen();

    } while (opcao != 5);
}

int main()
{
    clearScreen();
    setlocale(LC_ALL, "Portuguese_Brazil.1252");
    jogar();

    return 0;
}
