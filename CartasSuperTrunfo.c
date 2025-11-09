#include <stdio.h>

// Desafio Super Trunfo - Países
// Tema 1 - Cadastro das cartas
// Objetivo: No nível novato você deve criar as cartas representando as cidades utilizando scanf para entrada de dados e printf para exibir as informações.
// Objetivo: No nível aventureiro você deve calcular e exibir a densidade populacional (população/área) e o PIB per capita (PIB/população) de cada cidade.

void coletarDadosdaCarta(char *estado, char *codigoCarta, char *nomeCidade, int *populacao, float *area, float *pib, int *pontosTurismo) {
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

float calculaDensidadePopulacional(int populacao, float area) {
    return populacao / area;
}

float calculaPibPerCapita(float pib, int populacao) {
    float pibEmReais = pib * 1000000000; // Convertendo bilhões para reais
    return pibEmReais / populacao;
}

void exibirDadosDaCarta(int indiceCarta, char estado, char *codigoCarta, char *nomeCidade, int populacao, float area, float pib, int pontosTurismo, float densidadePopulacional, float pibPerCapita) {
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

int main() {
  // Área para definição das variáveis para armazenar as propriedades das cidades
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

  // Área para entrada de dados
  // Cadastro da Carta 01
  printf("Preencha os dados das 2 cartas:\n");
  printf("Cadastro da Carta 01:\n");
  coletarDadosdaCarta(&estado1, codigoCarta1, nomeCidade1, &populacao1, &area1, &pib1, &pontosTurismo1);

  // Calcular Densidade Populacional
  float densidadePopulacional1 = calculaDensidadePopulacional(populacao1, area1);

  // Calcular PIB per capita
  float pibPerCapita1 = calculaPibPerCapita(pib1, populacao1);

  // Cadastro da Carta 02
  printf("\n");
  printf("Cadastro da Carta 02:\n");
  coletarDadosdaCarta(&estado2, codigoCarta2, nomeCidade2, &populacao2, &area2, &pib2, &pontosTurismo2);

  // Calcular Densidade Populacional
  float densidadePopulacional2 = calculaDensidadePopulacional(populacao2, area2);

  // Calcular PIB per capita
  float pibPerCapita2 = calculaPibPerCapita(pib2, populacao2);

  // Exibir dados da Carta 01
  exibirDadosDaCarta(1, estado1, codigoCarta1, nomeCidade1, populacao1, area1, pib1, pontosTurismo1, densidadePopulacional1, pibPerCapita1);
  
  // Exibir dados da Carta 02
  exibirDadosDaCarta(2, estado2, codigoCarta2, nomeCidade2, populacao2, area2, pib2, pontosTurismo2, densidadePopulacional2, pibPerCapita2);

  return 0;
}
