#include <stdio.h>

// Desafio Super Trunfo - Países
// Tema 1 - Cadastro das cartas
// Objetivo: No nível novato você deve criar as cartas representando as cidades utilizando scanf para entrada de dados e printf para exibir as informações.

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
  printf("Digite o estado da cidade (uma letra A-H): ");
  scanf(" %c", &estado1);                  // espaço para consumir newline
  printf("Digite o código da carta (2 números ex: 01 a 04): ");
  scanf("%2s", codigoCarta1);               // limite para 2 chars
  printf("Digite o nome da cidade: ");
  scanf(" %[^\n]", nomeCidade1);            // lê até newline (com espaço inicial)
  printf("Digite a população da cidade: ");
  scanf("%d", &populacao1);
  printf("Digite a área da cidade (em km²): ");
  scanf("%f", &area1);
  printf("Digite o PIB da cidade (em bilhões): ");
  scanf("%f", &pib1);
  printf("Digite o número de pontos turísticos da cidade: ");
  scanf("%d", &pontosTurismo1);

  // Cadastro da Carta 02
  printf("\n");
  printf("Cadastro da Carta 02:\n");
  printf("Digite o estado da cidade (uma letra A-H): ");
  scanf(" %c", &estado2);                  // espaço para consumir newline
  printf("Digite o código da carta (2 números ex: 01 a 04): ");
  scanf("%2s", codigoCarta2);               // limite para 2 chars
  printf("Digite o nome da cidade: ");
  scanf(" %[^\n]", nomeCidade2);            // lê até newline (com espaço inicial)
  printf("Digite a população da cidade: ");
  scanf("%d", &populacao2);
  printf("Digite a área da cidade (em km²): ");
  scanf("%f", &area2);
  printf("Digite o PIB da cidade (em bilhões): ");
  scanf("%f", &pib2);
  printf("Digite o número de pontos turísticos da cidade: ");
  scanf("%d", &pontosTurismo2);

  // Área para exibição dos dados da cidade
  printf("\n--- Carta01 ---\n");
  printf("Estado: %c\n", estado1);
  printf("Código: %c%s\n", estado1, codigoCarta1);
  printf("Nome da Cidade: %s\n", nomeCidade1);
  printf("População: %d\n", populacao1);
  printf("Área: %.2f km²\n", area1);
  printf("PIB: %.2f bilhões\n", pib1);
  printf("Pontos Turísticos: %d\n", pontosTurismo1);

  printf("\n--- Carta02 ---\n");
  printf("Estado: %c\n", estado2);
  printf("Código: %c%s\n", estado2, codigoCarta2);
  printf("Nome da Cidade: %s\n", nomeCidade2);
  printf("População: %d\n", populacao2);
  printf("Área: %.2f km²\n", area2);
  printf("PIB: %.2f bilhões\n", pib2);
  printf("Pontos Turísticos: %d\n", pontosTurismo2);
  
  return 0;
}
