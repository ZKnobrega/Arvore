#include <iostream>
#include <stdlib.h>

using namespace std;
struct NO{
    int info;
    int altura;
    struct NO *esq;
    struct NO *dir;
}NO;

typedef struct NO* ArvBAL;

ArvBAL* cria_ArvBAL(){
    ArvBAL* raiz = new ArvBAL;
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

void libera_NO(struct NO* no){
    if(no == NULL)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
    no = NULL;
}

void libera_ArvBAL(ArvBAL* raiz){
    if(raiz == NULL)
        return;
    libera_NO(*raiz);
    free(raiz);
}

int altura_NO(struct NO* no){
    if(no == NULL)
        return -1;
    else
    return no->altura;
}

int FatBalanc_NO(struct NO* no){
    return labs(altura_NO(no->esq) - altura_NO(no->dir));
}

int maior(int x, int y){
    if(x > y)
        return x;
    else
        return y;
}

int estaVazia_ArvBAL(ArvBAL *raiz){
    if(raiz == NULL)
        return 1;
    if(*raiz == NULL)
        return 1;
    return 0;
}

int totalNO_ArvBAL(ArvBAL *raiz){
    if (raiz == NULL)
        return 0;
    if (*raiz == NULL)
        return 0;
    int alt_esq = totalNO_ArvBAL(&((*raiz)->esq));
    int alt_dir = totalNO_ArvBAL(&((*raiz)->dir));
    return(alt_esq + alt_dir + 1);
}

int altura_ArvBAL(ArvBAL *raiz){
    if (raiz == NULL)
        return 0;
    if (*raiz == NULL)
        return 0;
    int alt_esq = altura_ArvBAL(&((*raiz)->esq));
    int alt_dir = altura_ArvBAL(&((*raiz)->dir));
    if (alt_esq > alt_dir)
        return (alt_esq + 1);
    else
        return(alt_dir + 1);
}

void preOrdem_ArvBAL(ArvBAL *raiz){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        cout << "No\n" << (*raiz)->info;
        preOrdem_ArvBAL(&((*raiz)->esq));
        preOrdem_ArvBAL(&((*raiz)->dir));
    }
}

void emOrdem_ArvBAL(ArvBAL *raiz){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        emOrdem_ArvBAL(&((*raiz)->esq));
        cout << "No\n" <<(*raiz)->info;
        emOrdem_ArvBAL(&((*raiz)->dir));
    }
}

void posOrdem_ArvBAL(ArvBAL *raiz){
    if(raiz == NULL)
        return;
    if(*raiz != NULL){
        posOrdem_ArvBAL(&((*raiz)->esq));
        posOrdem_ArvBAL(&((*raiz)->dir));
        cout << "No\n" << (*raiz)->info;
    }
}

int consulta_ArvBAL(ArvBAL *raiz, int valor){
    if(raiz == NULL)
        return 0;
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(valor == atual->info){
            return 1;
        }
        if(valor > atual->info)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
    return 0;
}

void RotacaoLL(ArvBAL *A){
    cout << "RotacaoLL\n";
    struct NO *B;
    B = (*A)->esq;
    (*A)->esq = B->dir;
    B->dir = *A;
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->esq),(*A)->altura) + 1;
    *A = B;
}

void RotacaoRR(ArvBAL *A){
    cout << "RotacaoRR\n";
    struct NO *B;
    B = (*A)->dir;
    (*A)->dir = B->esq;
    B->esq = (*A);
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->dir),(*A)->altura) + 1;
    (*A) = B;
}

void RotacaoLR(ArvBAL *A){
    RotacaoRR(&(*A)->esq);
    RotacaoLL(A);
}

void RotacaoRL(ArvBAL *A){
    RotacaoLL(&(*A)->dir);
    RotacaoRR(A);
}

int insere_ArvBAL(ArvBAL *raiz, int valor){
    int res;
    if(*raiz == NULL){
        struct NO *novo;
        novo = (struct NO*)malloc(sizeof(struct NO));
        if(novo == NULL)
            return 0;

        novo->info = valor;
        novo->altura = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }

    struct NO *atual = *raiz;
    if(valor < atual->info){
        if((res = insere_ArvBAL(&(atual->esq), valor)) == 1){
            if(FatBalanc_NO(atual) >= 2){
                if(valor < (*raiz)->esq->info ){
                    RotacaoLL(raiz);
                }else{
                    RotacaoLR(raiz);
                }
            }
        }
    }else{
        if(valor > atual->info){
            if((res = insere_ArvBAL(&(atual->dir), valor)) == 1){
                if(FatBalanc_NO(atual) >= 2){
                    if((*raiz)->dir->info < valor){
                        RotacaoRR(raiz);
                    }else{
                        RotacaoRL(raiz);
                    }
                }
            }
        }else{
            cout << "Valor duplicado!!\n";
            return 0;
        }
    }

    atual->altura = maior(altura_NO(atual->esq),altura_NO(atual->dir)) + 1;

    return res;
}

struct NO* procuraMenor(struct NO* atual){
    struct NO *no1 = atual;
    struct NO *no2 = atual->esq;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

int remove_ArvBAL(ArvBAL *raiz, int valor){
	if(*raiz == NULL){
	    cout << "valor não existe!!\n";
	    return 0;
	}

    int res;
	if(valor < (*raiz)->info){
	    if((res = remove_ArvBAL(&(*raiz)->esq,valor)) == 1){
            if(FatBalanc_NO(*raiz) >= 2){
                if(altura_NO((*raiz)->dir->esq) <= altura_NO((*raiz)->dir->dir))
                    RotacaoRR(raiz);
                else
                    RotacaoRL(raiz);
            }
	    }
	}

	if((*raiz)->info < valor){
	    if((res = remove_ArvBAL(&(*raiz)->dir, valor)) == 1){
            if(FatBalanc_NO(*raiz) >= 2){
                if(altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq) )
                    RotacaoLL(raiz);
                else
                    RotacaoLR(raiz);
            }
	    }
	}

	if((*raiz)->info == valor){
	    if(((*raiz)->esq == NULL || (*raiz)->dir == NULL)){
			struct NO *oldNode = (*raiz);
			if((*raiz)->esq != NULL)
                *raiz = (*raiz)->esq;
            else
                *raiz = (*raiz)->dir;
			free(oldNode);
		}else {
			struct NO* temp = procuraMenor((*raiz)->dir);
			(*raiz)->info = temp->info;
			remove_ArvBAL(&(*raiz)->dir, (*raiz)->info);
            if(FatBalanc_NO(*raiz) >= 2){
				if(altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq))
					RotacaoLL(raiz);
				else
					RotacaoLR(raiz);
			}
		}
		if (*raiz != NULL)
            (*raiz)->altura = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;
		return 1;
	}

	(*raiz)->altura = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;

	return res;
}

int main ()
{   int op,op2,valor,inserir,remover,total,altura;
    ArvBAL* raiz=cria_ArvBAL();
    do{
    cout << "1 - Inserir no na arvore\n2 - Listar arvore\n3 - Remover no da arvore\n4 - Altura da arvore\n5 - Quantidade de nos na arvore\n6 - Sair:\n";
    cout << "Opcao:";
    cin >> op;
    switch(op){

        case(1):
            cout << "Insira um numero:\n";
            cin >> valor;
            inserir= insere_ArvBAL(raiz,valor);
        break;

        case(2):
                cout << "listar arvore:\n1. pre-ordem\n2. em-ordem\n3. pos-ordem\n";
                cin >> op2;
                switch(op2){
                    case(1):
                        preOrdem_ArvBAL(raiz);
                    break;
                    case(2):
                        emOrdem_ArvBAL(raiz);
                    break;
                    case(3):
                        posOrdem_ArvBAL(raiz);
                    break;
                }
        break;


        case(3):
            cout << "entre com um numero:\n";
            cin >> valor;
            remover= remove_ArvBAL(raiz,valor);
        break;

        case(4):
            altura= altura_ArvBAL(raiz);
            cout << "altura da arvore:%i \n" << altura << endl;
        break;

        case(5):
            total= totalNO_ArvBAL(raiz);
            cout << "total de Nos da arvore:%i \n" << total << endl;
        break;

        default:
            cout << "opicao invalida!\n";
        break;

        case(6):
        break;
    }
    }while(op!=6);
    return 0;
}
