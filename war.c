// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_TERRITORIOS 5
#define MAX_MISSOES 2 // Reduzido para 2 para simplificar o exemplo
#define MAX_NOME 30
#define MAX_COR 10

// Constantes para os tipos de missão
#define MISSAO_DESTRUIR_EXERCITO 0

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Estrutura para a missão do jogador
typedef struct {
    int id;
    char descricao[100];
    char alvo_cor[MAX_COR]; // Usado para missões de destruir exército
} Missao;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa(int num_territorios);
void inicializarTerritorios(Territorio* mapa, int num_territorios);
void liberarMemoria(Territorio* mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa, int num_territorios);
void exibirMissao(const Missao* missao);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa, int num_territorios, const char* cor_jogador);
void simularAtaque(Territorio* atacante, Territorio* defensor, const char* cor_jogador);
int sortearMissao(Missao* missao, const char* cor_jogador);
int verificarVitoria(const Territorio* mapa, int num_territorios, const Missao* missao, const char* cor_jogador);

// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // Declaração de variáveis
    Territorio* mapa = NULL;
    char cor_jogador[MAX_COR];
    Missao missao_jogador;
    int escolha_menu;
    int jogo_ativo = 1;

    // 1. Configuração Inicial (Setup):
    srand(time(NULL)); // Inicializa a semente para números aleatórios

    mapa = alocarMapa(MAX_TERRITORIOS);
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa. Encerrando o jogo.\n");
        return 1;
    }
    inicializarTerritorios(mapa, MAX_TERRITORIOS);

    printf("Escolha a cor do seu exercito (ex: Vermelho, Azul, Verde): ");
    scanf("%s", cor_jogador);
    limparBufferEntrada();

    sortearMissao(&missao_jogador, cor_jogador);

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        exibirMapa(mapa, MAX_TERRITORIOS);
        exibirMissao(&missao_jogador);
        exibirMenuPrincipal();

        printf("Escolha sua acao: ");
        scanf("%d", &escolha_menu);
        limparBufferEntrada();

        switch (escolha_menu) {
            case 1:
                faseDeAtaque(mapa, MAX_TERRITORIOS, cor_jogador);
                break;
            case 2:
                if (verificarVitoria(mapa, MAX_TERRITORIOS, &missao_jogador, cor_jogador)) {
                    printf("\nPARABENS! VOCE COMPRIU SUA MISSAO E VENCEU O JOGO!\n");
                    jogo_ativo = 0;
                } else {
                    printf("\nSua missao ainda nao foi cumprida. Continue lutando!\n");
                }
                break;
            case 0:
                printf("\nEncerrando o jogo. Ate a proxima!\n");
                jogo_ativo = 0;
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
        printf("\nPressione ENTER para continuar...");
        getchar(); // Pausa a execução

    } while (jogo_ativo);

    // 3. Limpeza:
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
Territorio* alocarMapa(int num_territorios) {
    Territorio* mapa = (Territorio*)calloc(num_territorios, sizeof(Territorio));
    if (mapa == NULL) {
        perror("Falha ao alocar memória para o mapa");
    }
    return mapa;
}

// inicializarTerritorios():
void inicializarTerritorios(Territorio* mapa, int num_territorios) {
    // Exemplo de inicialização. Em um jogo real, isso viria de um arquivo ou configuração.
    // Para simplificar, vamos preencher com dados fixos e pedir a cor do jogador.
    // Certifique-se de que não haja dois territórios com a mesma cor inicial, exceto se for a cor do jogador.

    // Nomes de exemplo para os territórios
    const char* nomes_territorios[] = {"Brasil", "Argentina", "Canada", "EUA", "Mexico"};
    const char* cores_iniciais[] = {"Vermelho", "Azul", "Verde", "Amarelo", "Preto"};

    for (int i = 0; i < num_territorios; i++) {
        strncpy(mapa[i].nome, nomes_territorios[i], MAX_NOME - 1);
        mapa[i].nome[MAX_NOME - 1] = '\0';

        strncpy(mapa[i].cor, cores_iniciais[i], MAX_COR - 1);
        mapa[i].cor[MAX_COR - 1] = '\0';

        mapa[i].tropas = (rand() % 5) + 3; // Tropas iniciais entre 3 e 7
    }
}

// liberarMemoria():
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// exibirMenuPrincipal():
void exibirMenuPrincipal() {
    printf("\n--- MENU PRINCIPAL ---\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missao\n");
    printf("0. Sair do Jogo\n");
    printf("----------------------\n");
}

// exibirMapa():
void exibirMapa(const Territorio* mapa, int num_territorios) {
    printf("\n--- MAPA ATUAL ---\n");
    printf("%-5s %-20s %-10s %-7s\n", "ID", "Territorio", "Dono", "Tropas");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < num_territorios; i++) {
        printf("%-5d %-20s %-10s %-7d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("-------------------------------------------------\n");
}

// exibirMissao():
void exibirMissao(const Missao* missao) {
    printf("\n--- SUA MISSAO ---\n");
    printf("Objetivo: %s\n", missao->descricao);
    printf("------------------\n");
}

// faseDeAtaque():
void faseDeAtaque(Territorio* mapa, int num_territorios, const char* cor_jogador) {
    int id_origem, id_destino;

    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Digite o ID do territorio de origem (seu): ");
    scanf("%d", &id_origem);
    limparBufferEntrada();

    printf("Digite o ID do territorio de destino (inimigo): ");
    scanf("%d", &id_destino);
    limparBufferEntrada();

    // Validações básicas
    if (id_origem < 0 || id_origem >= num_territorios ||
        id_destino < 0 || id_destino >= num_territorios) {
        printf("IDs de territorio invalidos.\n");
        return;
    }

    Territorio* atacante = &mapa[id_origem];
    Territorio* defensor = &mapa[id_destino];

    if (strcmp(atacante->cor, cor_jogador) != 0) {
        printf("Voce so pode atacar com seus proprios territorios.\n");
        return;
    }

    if (strcmp(defensor->cor, cor_jogador) == 0) {
        printf("Voce nao pode atacar seus proprios territorios.\n");
        return;
    }

    if (atacante->tropas <= 1) {
        printf("Territorio de origem precisa ter mais de 1 tropa para atacar.\n");
        return;
    }

    simularAtaque(atacante, defensor, cor_jogador);
}

// simularAtaque():
void simularAtaque(Territorio* atacante, Territorio* defensor, const char* cor_jogador) {
    printf("\n--- SIMULANDO ATAQUE ---\n");
    printf("%s (%s, %d tropas) ataca %s (%s, %d tropas)!\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    int dados_atacante = (rand() % 6) + 1; // Rola um dado de 6 lados
    int dados_defensor = (rand() % 6) + 1;

    printf("Dados do atacante: %d\n", dados_atacante);
    printf("Dados do defensor: %d\n", dados_defensor);

    if (dados_atacante > dados_defensor) {
        printf("Ataque bem-sucedido! O defensor perde uma tropa.\n");
        defensor->tropas--;
        if (defensor->tropas == 0) {
            printf("%s conquistou %s!\n", atacante->nome, defensor->nome);
            strncpy(defensor->cor, atacante->cor, MAX_COR - 1);
            defensor->cor[MAX_COR - 1] = '\0';
            defensor->tropas = 1; // Move uma tropa para o território conquistado
            atacante->tropas--; // A tropa é movida, não perdida
        }
    } else {
        printf("Ataque falhou! O atacante perde uma tropa.\n");
        atacante->tropas--;
    }

    printf("Resultado: %s (%s, %d tropas) vs %s (%s, %d tropas)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);
}

// sortearMissao():
int sortearMissao(Missao* missao, const char* cor_jogador) {
    int tipo_missao = rand() % MAX_MISSOES; // Sorteia um tipo de missão

    missao->id = tipo_missao;

    switch (tipo_missao) {
        case MISSAO_DESTRUIR_EXERCITO: {
            // Sorteia uma cor inimiga para destruir
            const char* cores_inimigas[] = {"Azul", "Verde", "Amarelo", "Preto", "Branco"};
            int idx_cor = rand() % (sizeof(cores_inimigas) / sizeof(cores_inimigas[0]));
            // Garante que a cor a ser destruída não seja a do próprio jogador
            while (strcmp(cores_inimigas[idx_cor], cor_jogador) == 0) {
                idx_cor = rand() % (sizeof(cores_inimigas) / sizeof(cores_inimigas[0]));
            }
            strncpy(missao->alvo_cor, cores_inimigas[idx_cor], MAX_COR - 1);
            missao->alvo_cor[MAX_COR - 1] = '\0';
            snprintf(missao->descricao, sizeof(missao->descricao), "Destruir completamente o exercito %s.", missao->alvo_cor);
            break;
        }
        // Adicionar outros tipos de missão aqui
        default:
            strncpy(missao->descricao, "Conquistar 10 territorios.", sizeof(missao->descricao) - 1);
            missao->descricao[sizeof(missao->descricao) - 1] = '\0';
            break;
    }
    return missao->id;
}

// verificarVitoria():
int verificarVitoria(const Territorio* mapa, int num_territorios, const Missao* missao, const char* cor_jogador) {
    switch (missao->id) {
        case MISSAO_DESTRUIR_EXERCITO: {
            // Verifica se a cor alvo ainda existe no mapa
            for (int i = 0; i < num_territorios; i++) {
                if (strcmp(mapa[i].cor, missao->alvo_cor) == 0) {
                    return 0; // Exército alvo ainda existe
                }
            }
            return 1; // Exército alvo foi destruído
        }
        // Adicionar lógica para outros tipos de missão
        default:
            // Exemplo: Conquistar um certo número de territórios
            int territorios_jogador = 0;
            for (int i = 0; i < num_territorios; i++) {
                if (strcmp(mapa[i].cor, cor_jogador) == 0) {
                    territorios_jogador++;
                }
            }
            // Para a missão padrão, vamos dizer que precisa de 3 territórios
            if (territorios_jogador >= 3) {
                return 1;
            }
            return 0;
    }
}

// limparBufferEntrada():
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
