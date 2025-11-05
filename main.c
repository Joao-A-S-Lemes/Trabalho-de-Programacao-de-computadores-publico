#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

void ajustar(char n[]){
   n[strcspn(n, "\r\n")] = '\0';
}

void tirarLixo(char *str) {
	int len = strlen(str);
	while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r' || str[len-1] == ' ')){
		str[--len] = '\0';
    }
}

void removerAcentos(char *str) {
    char buffer[256];
    int j = 0;
    for (int i = 0; str[i]; i++) {
        unsigned char c = str[i];
        if ((c & 0xE0) == 0xC0 && str[i+1]) {
            unsigned char c2 = str[i+1];
            if ((c == 0xC3 && c2 >= 0x80 && c2 <= 0x87) || (c == 0xC3 && c2 >= 0xA0 && c2 <= 0xA7)) { 
                buffer[j++] = 'a';
            }
            else if ((c == 0xC3 && c2 >= 0x88 && c2 <= 0x8B) || (c == 0xC3 && c2 >= 0xA8 && c2 <= 0xAB)) {
                buffer[j++] = 'e';
            }
            else if ((c == 0xC3 && c2 >= 0x8C && c2 <= 0x8F) || (c == 0xC3 && c2 >= 0xAC && c2 <= 0xAF)) {
                buffer[j++] = 'i';
            }
            else if ((c == 0xC3 && c2 >= 0x92 && c2 <= 0x96) || (c == 0xC3 && c2 >= 0xB2 && c2 <= 0xB6)) {
                buffer[j++] = 'o';
            }
            else if ((c == 0xC3 && c2 >= 0x99 && c2 <= 0x9C) || (c == 0xC3 && c2 >= 0xB9 && c2 <= 0xBC)) {
                buffer[j++] = 'u';
            }
            else if ((c == 0xC3 && c2 == 0x87) || (c == 0xC3 && c2 == 0xA7)) {
                buffer[j++] = 'c';
            }
            else {
                buffer[j++] = c, buffer[j++] = c2;}
            i++;
        } else {
            buffer[j++] = c;
        }
    }
    buffer[j] = '\0';
    strcpy(str, buffer);
}

void pedirEnter(){
    getchar();
    printf("\nPressione ENTER para prosseguir.\n");
    getchar();
}

typedef struct {
	int entrada, hp, attack, defense, spatk, spdef, speed;
	char nome[15], tipo1[15], tipo2[15];
	int maxHP;
} Pokemon;

void mostrarPokemon(Pokemon p) {
    printf("-------------------------------------\n");
    printf("Entrada: %d\nNome: %s\nTipo 1: %s\nTipo 2: %s\n", 
           p.entrada, p.nome, p.tipo1, p.tipo2);
}

void mostrarTime(Pokemon time[], int tam, char *nomeTime) {
    printf("\n%s:\n", nomeTime);
    for(int i = 0; i < tam; i++) {
        if(strcmp(time[i].tipo2, "Nenhum") == 0){
            printf("%d - %s (%s) HP: %d\n", time[i].entrada, time[i].nome, time[i].tipo1, time[i].hp);
        }
        else{
            printf("%d - %s (%s / %s) HP: %d\n", time[i].entrada, time[i].nome, time[i].tipo1, time[i].tipo2, time[i].hp);
        }
    }
}

int pokemonVivo(Pokemon time[], int tam) {
    for(int i = 0; i < tam; i++){
        if(time[i].hp > 0){
            return 1;
        }
    }
    return 0;
}

int escolherProximoPokemon(Pokemon time[], int tam) {
    for(int i = 0; i < tam; i++) {
        if(time[i].hp > 0) {
            return i;
        }
    }
    return -1;
}

int calcularDano(Pokemon atacante, Pokemon defensor) {
    int ataque = atacante.attack;
    int defesa = defensor.defense;
    float stab = 1.5;
    float random = 0.85 + ((rand() % 16) / 100.0);
    int dano = (int)(((float)ataque * 25 / defesa) * stab * random);
    return dano;
}

void atacar(Pokemon *atacante, Pokemon *defensor){
    int dano = calcularDano(*atacante, *defensor);
    printf("-------------------------------------\nO ataque de %s deu %d de dano!\n", atacante->nome, dano);
    defensor->hp -= dano;
    if(defensor->hp <= 0){
        defensor->hp = 0;
    }
    printf("-------------------------------------\n%s HP: %d\n%s HP: %d\n", atacante->nome, atacante->hp, defensor->nome, defensor->hp);
    if(defensor->hp <= 0){
        printf("-------------------------------------\n%s morreu!\n", defensor->nome);
    }
}

void curar(Pokemon *p, int *pocoes) {
    int cura = 30;
    if(*pocoes > 0) {
        (*pocoes)--;
        p->hp += cura;
        if(p->hp > p->maxHP) {
            p->hp = p->maxHP;
        }
        printf("-------------------------------------\n%s usou uma poção! HP atual: %d | Poções restantes: %d\n", p->nome, p->hp, *pocoes);
    } else {
        printf("-------------------------------------\nVocê não tem mais poções!\n-------------------------------------\n");
    }
}

void trocar(Pokemon time[], int tam, int *ativo) {
    int vivos = 0;
    for (int i = 0; i < tam; i++) {
        if (time[i].hp > 0) {
            vivos = 1;
        }
    }
    if (!vivos) {
        printf("-------------------------------------\nNenhum Pokémon disponível para troca!\n");
        return;
    }
    printf("-------------------------------------\nEscolha um Pokémon para entrar em campo:\n");
    for (int i = 0; i < tam; i++) {
        printf("%d - %s (HP: %d)\n", i, time[i].nome, time[i].hp);
    }
    int escolha;
    scanf("%d", &escolha);
    while (1) {
        if (escolha >= 0 && escolha < tam && time[escolha].hp > 0) {
            *ativo = escolha;
            printf("-------------------------------------\n%s entrou em campo!\n", time[*ativo].nome);
            break;
        } 
        else {
            printf("-------------------------------------\nEscolha inválida ou Pokémon desmaiado!\n-------------------------------------\nTente novamente: ");
            scanf("%d", &escolha);
        }
    }
}

void turno(Pokemon timeAtacante[], int tamAtacante, Pokemon timeDefensor[], int tamDefensor, int *pocoes, int *pocoesDefensor) {
    int ativoAtacante = 0, ativoDefensor = 0, escolha;
    while(1) {
        printf("\n------------- STATUS --------------\n");
        mostrarTime(timeAtacante, tamAtacante, "Seu time");
        mostrarTime(timeDefensor, tamDefensor, "Time adversário");
        printf("-------------------------------------\n");
        printf("O Pokémon atual é: %s (HP %d)\n", timeAtacante[ativoAtacante].nome, timeAtacante[ativoAtacante].hp);
        printf("O que gostaria de fazer?\n1 - Atacar\n2 - Usar pocao\n3 - Trocar de Pokémon\n");
        printf("-------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);
        if(escolha == 1) {
            atacar(&timeAtacante[ativoAtacante], &timeDefensor[ativoDefensor]);
            if(timeDefensor[ativoDefensor].hp <= 0) {
                printf("-------------------------------------\nO adversário perdeu %s!\n", timeDefensor[ativoDefensor].nome);
                int prox = escolherProximoPokemon(timeDefensor, tamDefensor);
                if(prox != -1) {
                    ativoDefensor = prox;
                    printf("-------------------------------------\n%s entrou em campo!\n", timeDefensor[ativoDefensor].nome);
                } 
                else {
                    printf("-------------------------------------\nO adversário não tem mais Pokémon!\n");
                }
            }
        }
        else if(escolha == 2) {
            curar(&timeAtacante[ativoAtacante], pocoes);
        }
        else if(escolha == 3) {
            trocar(timeAtacante, tamAtacante, &ativoAtacante);
        }
        else {
            printf("-------------------------------------\nEscolha inválida! Tente novamente:");
        }
        int acaoOponente = rand() % 10;
        if (acaoOponente < 9){ 
            pedirEnter();
            atacar(&timeDefensor[ativoDefensor], &timeAtacante[ativoAtacante]);
        }
        else {
            pedirEnter();
            curar(&timeDefensor[ativoDefensor], pocoesDefensor);
        }
        if(timeAtacante[ativoAtacante].hp <= 0) {
        printf("-------------------------------------\nVocê perdeu %s!\n", timeAtacante[ativoAtacante].nome);
        int vivos = 0;
            for(int i = 0; i < tamAtacante; i++) {
                if(timeAtacante[i].hp > 0) {
                    vivos = 1;
                    break;
                }
            }
            if(vivos) {
                trocar(timeAtacante, tamAtacante, &ativoAtacante);
            }
        }
        if(!pokemonVivo(timeAtacante, tamAtacante)) {
            printf("-------------------------------------\nVocê perdeu a batalha!\n-------------------------------------\n");
            break;
        }
        if(!pokemonVivo(timeDefensor, tamDefensor)) {
            printf("-------------------------------------\nVocê venceu a batalha!\n-------------------------------------\n");
            break;
        }
    }
    printf("---------- Fim da batalha -----------\n");
    printf("-------------------------------------\n");
}

void pesquisarPorNome(Pokemon lista[], int qtd, char nome[], int i, int *encontrado) {
    if(i >= qtd) return;
    char nomePokemon[15];
    strcpy(nomePokemon, lista[i].nome);
    removerAcentos(nomePokemon);
    if(strcasecmp(nome, nomePokemon) == 0) {
        mostrarPokemon(lista[i]);
        *encontrado = 1;
    }
    pesquisarPorNome(lista, qtd, nome, i+1, encontrado);
}

void pesquisa(Pokemon lista[], int qtd) {
	int encontrado = 0;
	printf("-------------------------------------\n          PESQUISA POKÉMON\n-------------------------------------\nComo você gostaria de fazer sua pesquisa?\n-------------------------------------\n1 - Por nome\n2 - Por entrada na Pokédex\n3 - Por tipo\n-------------------------------------\nEscolha uma opção: ");
	int p = 0;
	while(1) {
		scanf("%d",&p);
		getchar();
		if(p == 1 || p == 2 || p == 3) {
			break;
		} else {
			printf("-------------------------------------\nErro! Valor inválido!\n-------------------------------------\nTente novamente: ");
		}
	}
	if(p == 1) {
		char nome[15];
		printf("-------------------------------------\nDigite o nome do Pokémon que deseja pesquisar: ");
		fgets(nome, 15, stdin);
        removerAcentos(nome);
        ajustar(nome);
        int encontrado = 0;
        pesquisarPorNome(lista, qtd, nome, 0, &encontrado);
        if(!encontrado) {
            printf("-------------------------------------\nPokémon não encontrado.\n");
	    }
	} 
	else if(p == 2) {
		int numero;
		printf("-------------------------------------\nDigite a entrada na Pokédex do Pokémon que deseja pesquisar: ");
		scanf("%d",&numero);
		for(int i = 0; i < qtd; i++) {
			if(numero == lista[i].entrada) {
				encontrado = 1;
				mostrarPokemon(lista[i]);
			}
		}
	} else if(p == 3) {
		char type[15];
		printf("-------------------------------------\nVocê gostaria de pesquisar de acordo com 1 tipo ou 2?\n-------------------------------------\n1 - 1 tipo\n2 - 2 tipos\n3 - Lista de tipos\n-------------------------------------\nEscolha uma opção: ");
		int t = 0;
		while(1) {
			scanf("%d",&t);
			getchar();
		    if(t == 3){
			    printf("-------------------------------------\nTipos disponíveis:\n\nÁgua\n\nDragão\n\nElétrico\n\nFada\n\nFantasma\n\nFogo\n\nGelo\n\nGrama\n\nInseto\n\nLutador\n\nNormal\n\nPedra\n\nPsíquico\n\nTerrestre\n\nVeneno\n\nVoador\n\n-------------------------------------\n");
			    printf("Você gostaria de pesquisar de acordo com 1 tipo ou 2?\n-------------------------------------\n1 - 1 tipo\n2 - 2 tipos\n3 - Lista de tipos\n-------------------------------------\nEscolha uma opção: ");
			} else if(t == 1 || t == 2){
			    break;
			} else {
		        printf("-------------------------------------\nErro! Valor inválido!\n-------------------------------------\nTente novamente:");
			}
		}
		if(t == 1) {
			printf("-------------------------------------\nDigite o tipo do Pokémon que deseja pesquisar: ");
			fgets(type, 15, stdin);
			removerAcentos(type);
			ajustar(type);
			for(int i = 0; i < qtd; i++) {
			    char tipoPokemon1[15], tipoPokemon2[15];
                strcpy(tipoPokemon1, lista[i].tipo1);
                strcpy(tipoPokemon2, lista[i].tipo2);
			    removerAcentos(tipoPokemon1);
			    removerAcentos(tipoPokemon2);
				if(strcasecmp(type, tipoPokemon1) == 0 || strcasecmp(type, tipoPokemon2) == 0) {
					if(encontrado == 0) {
						printf("-------------------------------------\nOs Pokémons que apresentam esse tipo são:\n");
					}
					encontrado = 1;
					mostrarPokemon(lista[i]);
				}
			}
		}
		if(t == 2) {
		    char type2[15];
			printf("-------------------------------------\nDigite o primeiro tipo do Pokémon que deseja pesquisar: ");
			fgets(type, 15, stdin);
			removerAcentos(type);
			ajustar(type);
			printf("-------------------------------------\nDigite o segundo tipo do Pokémon que deseja pesquisar: ");
			fgets(type2, 15, stdin);
			removerAcentos(type2);
			ajustar(type2);
			for(int i = 0; i < qtd; i++) {
			    char tipoPokemon1[15], tipoPokemon2[15];
			    strcpy(tipoPokemon1, lista[i].tipo1);
                strcpy(tipoPokemon2, lista[i].tipo2);
			    removerAcentos(tipoPokemon1);
			    removerAcentos(tipoPokemon2);
				if(((strcasecmp(type, tipoPokemon1) == 0) && (strcasecmp(type2, tipoPokemon2) == 0)) || ((strcasecmp(type, tipoPokemon2) == 0) && (strcasecmp(type2, tipoPokemon1) == 0))) {
					if(encontrado == 0) {
						printf("-------------------------------------\nOs Pokémons que apresentam esses tipos são:\n");
					}
					encontrado = 1;
					mostrarPokemon(lista[i]);
				}
			}
			if(encontrado == 0){
			    printf("-------------------------------------\nNão existem Pokemóns com essa combinação de tipos!\n");
			}
		}
	}
	printf("-------------------------------------\n");
    printf("Fim da pesquisa.");
}

void batalha(Pokemon lista[], int qtd) {
    int tam1, tam2, escolha;
	printf("-------------------------------------\nQuantos Pokémons você quer no seu time? Insira um valor de 1 a 6: ");
    while(1) {
		scanf("%d",&tam1);
		if(tam1 >=1 && tam1 <= 6) {
			break;
		} else {
			printf("-------------------------------------\nErro! Valor inválido!\n-------------------------------------\nTente novamente:");
		}
	}
	printf("-------------------------------------\nQuantos Pokémons você quer no time do seu oponente? Insira um valor de 1 a 6: ");
    while(1) {
		scanf("%d",&tam2);
		if(tam2 >=1 && tam2 <= 6) {
			break;
		} else {
			printf("-------------------------------------\nErro! Valor inválido!\n-------------------------------------\nTente novamente:");
		}
	}
	printf("-------------------------------------\nBeleza, você gostaria de escolher seus Pokémons ou de receber Pokemóns aleatórios?\n-------------------------------------\n1 - Escolher\n2 - Aleatório\n-------------------------------------\nEscolha uma opção: ");
    while(1) {
		scanf("%d",&escolha);
		if(escolha == 1 || escolha == 2) {
			break;
		} else {
			printf("-------------------------------------\nErro! Valor inválido!\n-------------------------------------\nTente novamente:");
		}
	}
	int escolhaadv;
	printf("-------------------------------------\nBeleza, você gostaria de escolher os Pokémons de seu oponente ou quer que ele receba Pokemóns aleatórios?\n-------------------------------------\n1 - Escolher\n2 - Aleatório\n-------------------------------------\nEscolha uma opção: ");
    while(1) {
		scanf("%d",&escolhaadv);
		if(escolhaadv == 1 || escolhaadv == 2) {
			break;
		} else {
			printf("-------------------------------------\nErro! Valor inválido!\n-------------------------------------\nTente novamente:");
		}
	}
	Pokemon *time1 = malloc(sizeof(Pokemon) * tam1);
    Pokemon *time2 = malloc(sizeof(Pokemon) * tam2);
    if (!time1 || !time2) {
        printf("-------------------------------------\nErro de alocaçãoo para times.\n-------------------------------------\n");
        free(time1); free(time2);
        return;
    }
	if(escolha == 1){
	    int e;
	    printf("-------------------------------------\nBeleza, vamos montar seu time então! Já tem seus Pokémon em mente, ou gostaria de pesquisar?\n-------------------------------------\n1 - Já sei quais vou usar\n2 - Quero pesquisar\n-------------------------------------\nEscolha uma opção:");
	    while(1) {
		    scanf("%d",&e);
		    if(e == 2){
		        pesquisa(lista, qtd);
		        printf("-------------------------------------\nBeleza, vamos montar seu time então! Já decidiu seus Pokémon, ou gostaria de pesquisar novamente?\n-------------------------------------\n1 - Já sei quais vou usar\n2 - Quero pesquisar\n-------------------------------------\nEscolha uma opção: ");
		    }
		    else if(e == 1) {
			    break;
		    } else {
			    printf("-------------------------------------\nErro! Valor inválido!\n-------------------------------------\nTente novamente:");
		    }
	    }
	    if(e == 1) {
	        getchar();
            for(int i = 0; i < tam1; i++) {
                char nome[15];
                printf("-------------------------------------\nDigite o nome do Pokémon %d do seu time: ", i+1);
                fgets(nome, 15, stdin);
                removerAcentos(nome);
                ajustar(nome);
                int achou = 0;
                for(int j = 0; j < qtd; j++) {
                    char nomeP[15];
                    strcpy(nomeP, lista[j].nome);
                    removerAcentos(nomeP);
                    if(strcasecmp(nome, nomeP) == 0) {
                        time1[i] = lista[j];
                        achou = 1;
                        break;
                    }
                }
                if(!achou) {
                    printf("-------------------------------------\nPokémon não encontrado! Tente novamente.\n");
                    i--; 
                }
            }
        }
	}
	if(escolhaadv == 1){
	    int eadv;
	    printf("-------------------------------------\nBeleza, vamos montar o time do seu oponente! Já tem os Pokémon em mente, ou gostaria de pesquisar?\n-------------------------------------\n1 - Já sei quais ele vai usar\n2 - Quero pesquisar\n-------------------------------------\nEscolha uma opção: ");
	    while(1) {
		    scanf("%d",&eadv);
		    if(eadv == 2){
		        pesquisa(lista, qtd);
		        printf("-------------------------------------\nBeleza, vamos montar o time do seu oponente! Já decidiu tem os Pokémon em mente, ou gostaria de pesquisar novamente?\n-------------------------------------\n1 - Já sei quais ele vai usar\n2 - Quero pesquisar novamente\n-------------------------------------\nEscolha uma opção: ");
		    }
		    else if( eadv== 1) {
			    break;
		    } else {
			    printf("-------------------------------------\nErro! Valor inválido!\n-------------------------------------\nTente novamente: ");
		    }
	    }
	    if(eadv == 1) {
	        getchar();
            for(int i = 0; i < tam2; i++) {
                char nome[15];
                printf("-------------------------------------\nDigite o nome do Pokémon %d do time dele: ", i+1);
                fgets(nome, 15, stdin);
                removerAcentos(nome);
                ajustar(nome);
                int achou = 0;
                for(int j = 0; j < qtd; j++) {
                    char nomeP[15];
                    strcpy(nomeP, lista[j].nome);
                    removerAcentos(nomeP);
                    if(strcasecmp(nome, nomeP) == 0) {
                        time2[i] = lista[j];
                        achou = 1;
                        break;
                    }
                }
                if(!achou) {
                    printf("-------------------------------------\nPokémon não encontrado! Tente novamente.\n-------------------------------------\n");
                    i--; 
                }
            }
        }
	    if(eadv == 2){
	        pesquisa(lista, qtd);
	    }
	}
	if(escolha == 2){
	    srand(time(NULL));
        for(int i = 0; i < tam1; i++) {
            int r = rand() % qtd;
            time1[i] = lista[r];
        }
        mostrarTime(time1, tam1, "Seu time");
	}
	if(escolhaadv == 2){
	    if(escolha == 1){
	        srand(time(NULL));
	    }
        for(int i = 0; i < tam2; i++) {
            int r = rand() % qtd;
            time2[i] = lista[r];
        }
        mostrarTime(time2, tam2, "Time adversario");
	}
	int pocao = 3, pocaoDefensor = 3;
	turno(time1, tam1, time2, tam2, &pocao, &pocaoDefensor);
	free(time1);
	free(time2);
}

void swap(Pokemon *a, Pokemon *b) {
    Pokemon temp = *a;
    *a = *b;
    *b = temp;
}

int partitionNumero(Pokemon lista[], int low, int high) {
    int pivot = lista[high].entrada;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (lista[j].entrada < pivot) {
            i++;
            swap(&lista[i], &lista[j]);
        }
    }
    swap(&lista[i + 1], &lista[high]);
    return i + 1;
}

void quicksortNumero(Pokemon lista[], int low, int high) {
    if (low < high) {
        int pi = partitionNumero(lista, low, high);
        quicksortNumero(lista, low, pi - 1);
        quicksortNumero(lista, pi + 1, high);
    }
}

int partitionNome(Pokemon lista[], int low, int high) {
    char pivot[15];
    strcpy(pivot, lista[high].nome);
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (strcasecmp(lista[j].nome, pivot) < 0) {
            i++;
            swap(&lista[i], &lista[j]);
        }
    }
    swap(&lista[i + 1], &lista[high]);
    return i + 1;
}

void quicksortNome(Pokemon lista[], int low, int high) {
    if (low < high) {
        int pi = partitionNome(lista, low, high);
        quicksortNome(lista, low, pi - 1);
        quicksortNome(lista, pi + 1, high);
    }
}

void opcoesExtras(Pokemon lista[], int qtd) {
    int opc;
    while (1) {
        printf("-------------------------------------\nOpções de reordenação da Pokédex:\n-------------------------------------\n");
        printf("1 - Ordenar por número(Padrão)\n2 - Ordenar por nome\n3 - Voltar\n-------------------------------------\nEscolha uma opção: ");
        scanf("%d", &opc);
        if (opc == 1) {
            quicksortNumero(lista, 0, qtd - 1);
            printf("Pokédex ordenada por número!\n");
        } 
        else if (opc == 2) {
            quicksortNome(lista, 0, qtd - 1);
            printf("Pokédex ordenada por nome!\n");
        } 
        else if (opc == 3) {
            break;
        } else {
            printf("Opção inválida!\n");
        }
        mostrarTime(lista, qtd, "Pokédex Atualizada");
    }
}

int carregarPokemons(Pokemon **lista, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("-------------------------------------\nErro ao abrir o arquivo.\n-------------------------------------\n");
        *lista = NULL;
        return 0;
    }
    char linha[512];
    int capacidade= 50, i = 0;
    *lista = (Pokemon*) malloc(sizeof(Pokemon) * capacidade);
    if (*lista == NULL) {
        printf("-------------------------------------\nErro de alocação.\n-------------------------------------\n");
        fclose(arquivo);
        return 0;
    }
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (i >= capacidade) {
            capacidade *= 2;
            Pokemon *tmp = (Pokemon*) realloc(*lista, sizeof(Pokemon) * capacidade);
            if (tmp == NULL) {
                printf("-------------------------------------\nErro de realocação.\n-------------------------------------\n");
                free(*lista);
                fclose(arquivo);
                return i;
            }
            *lista = tmp;
        }
        linha[strcspn(linha, "\n")] = '\0';
        char *token = strtok(linha, ";");
        if (token) (*lista)[i].entrada = atoi(token);
        token = strtok(NULL, ";");
        if (token) { strncpy((*lista)[i].nome, token, sizeof((*lista)[i].nome)-1); tirarLixo((*lista)[i].nome); }
        token = strtok(NULL, ";");
        if (token) (*lista)[i].hp = atoi(token);
        (*lista)[i].maxHP = (*lista)[i].hp;
        token = strtok(NULL, ";");
        if (token) (*lista)[i].attack = atoi(token);
        token = strtok(NULL, ";");
        if (token) (*lista)[i].defense = atoi(token);
        token = strtok(NULL, ";");
        if (token) (*lista)[i].spatk = atoi(token);
        token = strtok(NULL, ";");
        if (token) (*lista)[i].spdef = atoi(token);
        token = strtok(NULL, ";");
        if (token) (*lista)[i].speed = atoi(token);
        token = strtok(NULL, ";");
        if (token) { strncpy((*lista)[i].tipo1, token, sizeof((*lista)[i].tipo1)-1); tirarLixo((*lista)[i].tipo1); }
        token = strtok(NULL, ";");
        if (token) { strncpy((*lista)[i].tipo2, token, sizeof((*lista)[i].tipo2)-1); tirarLixo((*lista)[i].tipo2); }
        else { strcpy((*lista)[i].tipo2, "Nenhum"); }
        i++;
    }
    fclose(arquivo);
    Pokemon *final = (Pokemon*) realloc(*lista, sizeof(Pokemon) * i);
    if (final) *lista = final;
    return i;
}

int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    Pokemon *pokedex = NULL;
    int qtd = carregarPokemons(&pokedex, "Pokemon.txt");

    if (qtd == 0) {
        printf("-------------------------------------\nNenhum Pokémon carregado. Encerrando programa.\n-------------------------------------\n");
        return 1;
    }

    int n = 0;
    while (1) {
        printf("\n=====================================\n");
        printf("     BATALHA E PESQUISA POKÉMON\n");
        printf("=====================================\n");
        printf("Pokédex carregada com sucesso! \n(%d Pokémon disponíveis)\n", qtd);
        printf("-------------------------------------\n");
        printf("1 - Pesquisar Pokémon\n");
        printf("2 - Montar times e iniciar batalha\n");
        printf("3 - Opções extras\n");
        printf("4 - Sair\n");
        printf("=====================================\n");
        printf("Escolha uma opção: ");
        scanf("%d", &n);

        if (n == 1) {
            pesquisa(pokedex, qtd);
        } 
        else if (n == 2) {
            batalha(pokedex, qtd);
        } 
        else if (n == 3) {
            opcoesExtras(pokedex, qtd);
        } 
        else if (n == 4){
            printf("-------------------------------------\nSaindo do programa...\n");
            printf("-------------------------------------\nPrograma encerrado.\n-------------------------------------\n");
            break;
        }
        else {
            printf("-------------------------------------\nOpção inválida! Tente novamente.");
        }
    }
    free(pokedex);
    return 0;
}
