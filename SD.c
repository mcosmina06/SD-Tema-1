//***NUME Mandru, PRENUME Cosmina***
//****GRUPA 313CB, SERIA CB**********

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct celulaHS {
	char *info;
	struct celulaHS *urm;
} TCelulaHS, *TListaHS, **AListaHS;

typedef struct {
	char card_number[17];
	char pin[10];
	char expiry_date[10];
	char cvv[4];
	int balance;
	char status[10];
	TListaHS history;
	int nb_change_pin;	
	int nb_fails;
} info_card;

typedef struct celulaLSC {
	info_card info;
	struct celulaLSC *urm;
} TCelulaLSC, *TListaLSC, **AListaLSC;

typedef struct celulaLC {
	TListaLSC info;
	struct celulaLC *urm;
} TCelulaLC, *TListaLC, **AListaLC;

void FreeHS(AListaHS l) {
	TListaHS aux;
	
	while(*l) {
		aux = *l;
		*l = aux->urm;
		free(aux->info);	
		free(aux);
	}
}

void FreeLSC(AListaLSC l) {
	TListaLSC aux;
	
	while(*l) {
		aux = *l;
		*l = aux->urm;	
		FreeHS(&aux->info.history);
		free(aux);
	}
}

void FreeLC(AListaLC l) {
	TListaLC aux;
	
	while(*l) {
		aux = *l;
		*l = aux->urm;
		FreeLSC(&aux->info);
		free(aux);
	}
}

TListaLSC AlocCelulaLSC(info_card x) {
	TListaLSC auxLSC;
	
	auxLSC = (TListaLSC)malloc(sizeof(TCelulaLSC));
	if (!auxLSC) {
		return NULL;
	}
	
	auxLSC->info = x;
	auxLSC->urm = NULL;
	return auxLSC;
}

TListaLC AlocCelulaLC(info_card x) {
	TListaLC auxLC;
	
	auxLC = (TListaLC)malloc(sizeof(TCelulaLC));
	if (!auxLC) {
		return NULL;
	}
	
	auxLC->info = AlocCelulaLSC(x);
	if (!auxLC->info) {
		FreeLC(&auxLC);
		return NULL;
	}
	
	auxLC->urm = NULL;
	return auxLC;
}

TListaLC AlocCelulaLCNULL() {
	TListaLC auxLC;
	
	auxLC = (TListaLC)malloc(sizeof(TCelulaLC));
	if (!auxLC) {
		return NULL;
	}
	
	auxLC->info = NULL;
	auxLC->urm = NULL;
	return auxLC;
}

TListaHS AlocCelulaHS(char *x) {
	TListaHS aux;
	
	aux = (TListaHS)malloc(sizeof(TCelulaHS));
	if (!aux) {
		return NULL;
	}
	
	aux->info = strdup(x);
	if(!aux->info){
		FreeHS(&aux);
		return NULL;
	}
	
	aux->urm = NULL;
	return aux;
}


int InsInc(AListaLSC l, info_card x) {
	TListaLSC aux;
	
	aux = AlocCelulaLSC(x);
	if (!aux) {
		return 0;
	}
	
	aux->urm = *l;
	*l = aux;
	return 1;
}

int InsSf(AListaLC l, TListaLC x) {
	TListaLC u = NULL;
	
	if (*l != NULL) {
		u = *l;
		while(u->urm != NULL) {
			u = u->urm;
		}
	}
	
	if (u == NULL) {
		*l = x;
	}
	
	else {
		u->urm = x;
	}
	
	return 1;
}

int InsIncHS(AListaHS l, char *x) {
	TListaHS aux;
	
	aux = AlocCelulaHS(x);
	if (!aux) {
		return 0;
	}
	
	aux->urm = *l;
	*l = aux;
	return 1;
}

int Eliminare(AListaLSC l, info_card x){ 
 	TListaLSC ant, p;
 	
 	for(p = *l, ant =NULL; p != NULL; ant = p, p = p->urm) {
 		if(strcmp(p->info.card_number, x.card_number) == 0) {
 			break;
 		}
 	}
 	
 	if(p == NULL) {
 		return 0;
 	}
 	
 	if(ant == NULL) {
 		*l = p->urm;
 	}
 	
 	else {
 		ant->urm = p->urm;
 	}
 	
 	FreeLSC(&p);
 	return 1;
}

int EliminareHS(AListaHS lH, char* x) {
	TListaHS h, ant;
	
	for(h = *lH, ant = NULL; h != NULL; ant = h, h = h->urm) {
		if(strcmp(h->info, x) == 0) {
			break;
		}
	}

	if (h == NULL) {
		return 0;
	}

	if(ant == NULL) {
		*lH = h->urm;
	}
	else {
		ant->urm = h->urm;
	}

	free(h->info);
	return 1;
}

//functie ce verifica daca exista deja cardul in lista in functie
//de CARD_NUMBER
int check_double(TListaLC l, char CARD_NUMBER[17], FILE *f) {
	TListaLC p;
	TListaLSC q;
	
	for(p = l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strcmp(q->info.card_number, CARD_NUMBER) == 0) {
				fprintf(f, "The card already exists\n");
				return 0;
				break;
			}
		}
	}
	
	return 1;
}

//functie ce verifica daca cardul este blocat sau nu pentru a se
//sti daca poate fi folosit
int VerifCard(AListaLC L, char CARD_NUMBER[17], char *FAIL) {
 	TListaLC p;
 	TListaLSC q;
 	
 	for(p = *L; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strncmp(q->info.card_number, CARD_NUMBER, 16) == 0) {
				if(strcmp(q->info.status, "LOCKED") == 0) {
					InsIncHS(&q->info.history, FAIL);
					return 1;
				}
			}
		}
	}
	
	return 0;
}

//functia adauga cardul in lista data, existand 3 cazuri posibile
int add_card(char CARD_NUMBER[17], char PIN[10], char EXPIRY_DATE[10], char CVV[3], int nr_max_card, AListaLC l, FILE *f) {
	TListaLC p, aux = NULL, r;
	info_card IF;
	int poz, suma = 0, k = -1, j = 0, i;
	long int x = atol(CARD_NUMBER);

	while(x != 0) {
		suma += x % 10;
		x = x / 10;
	}

	poz = suma % nr_max_card;
	
	//se numara cate intrari/pozitii sunt in listsa LC
	for (r = *l; r != NULL; r = r->urm) {
		j++;
	}

	//verific daca cardul exista deja sau nu
	if(check_double(*l, CARD_NUMBER, f) == 0) {
		return 0;
	}

	//copiez informatiile cardului in structura creata in acest scop
	strcpy(IF.card_number, CARD_NUMBER);
	strcpy(IF.cvv, CVV);
	strcpy(IF.pin, PIN);
	strcpy(IF.expiry_date, EXPIRY_DATE);
	strcpy(IF.status, "NEW");
	IF.history = NULL;
	IF.nb_change_pin = 0;
	IF.nb_fails = 0;
	IF.balance = 0;

	//daca pozitia cardului ce trbuie inserat este egala cu numarul de 
	//pozitii din card atunci este inserat la sfarsit
	if (poz == j) {
		p = AlocCelulaLC(IF);
		InsSf(l, p);
		return 1;
	}

	//daca sunt mai putine pozitii in lista LC adaug pozitii nule
	//pana la pozitia necesara cardului, iar acesta este inserat la sfarsit
	else if (poz > j) {
		for (i = j ; i < poz; i++) {
			p = AlocCelulaLCNULL();
			InsSf(l, p);
		}
		p = AlocCelulaLC(IF);
		InsSf(l, p);
		return 1;
	}
	//daca nu a fost parcursa niciuna din situatiile anterioare se cauta 
	//pozitia cardului printre cele curente si se insereaza in locul potrivit
	for (p = *l; p != NULL; p = p->urm) {
		k++;
		if(k == poz) {
			aux = p;
			break;
		}
	}
	
	if (aux == NULL) {
		return 0;
	}
	
	if (aux->info != NULL) {
		InsInc(&aux->info, IF);
	}
	else {
		aux->info = AlocCelulaLSC(IF);
	}
	return 1;
}

int delete_card(char CARD_NUMBER[16], AListaLC l, FILE *f) {
	TListaLC p;
	TListaLSC q;
	
	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strncmp(q->info.card_number, CARD_NUMBER, strlen(CARD_NUMBER) - 1) == 0) {
				Eliminare(&p->info, q->info);
				return 1;
			}
		}
	}
	
	return 0;
}

int cancel(char CARD_NUMBER[17], AListaLC l, FILE *f) {
 	TListaLC p;
 	TListaLSC q;
	char *SUCCESS;
	
	SUCCESS = malloc(100 * sizeof(char));
	if(!SUCCESS) {
		return 0;
	}
	sprintf(SUCCESS, "%s %s%s", "(SUCCESS, cancel", CARD_NUMBER, ")");
	SUCCESS[strlen(SUCCESS) ] = '\0';

 	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strncmp(q->info.card_number, CARD_NUMBER, strlen(CARD_NUMBER) - 1) == 0) {
				InsIncHS(&q->info.history, SUCCESS);
				free(SUCCESS);
				return 1;
			}
		}
	}

	free(SUCCESS);
	return 0;
}

int insert_card(char CARD_NUMBER[16], char PIN[10], AListaLC l, FILE *f) {
	TListaLC p;
	TListaLSC q;
	char *SUCCESS;
	char *FAIL;
	
	SUCCESS = malloc(100 * sizeof(char));
	if(!SUCCESS) {
		return 0;
	}
	FAIL = malloc(100 * sizeof(char));
	if(!FAIL) {
		free(SUCCESS);
		return 0;
	}

	sprintf(SUCCESS, "%s %s %s%s", "(SUCCESS, insert_card", CARD_NUMBER, PIN, ")");
	SUCCESS[strlen(SUCCESS) ] = '\0';
	sprintf(FAIL, "%s %s %s%s", "(FAIL, insert_card", CARD_NUMBER, PIN, ")");
	FAIL[strlen(FAIL)] = '\0';
	
	if(VerifCard(l, CARD_NUMBER, FAIL) == 1) {
		fprintf(f, "The card is blocked. Please contact the administrator.\n");
		free(SUCCESS);
		free(FAIL);
		return 0;
	}
	
	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strcmp(q->info.card_number, CARD_NUMBER)== 0) {
				
				//se verifica daca pinul este unul valid
				if(strcmp(q->info.pin, PIN) != 0) {
					fprintf(f, "Invalid PIN\n");
					q->info.nb_fails++;

					if(q->info.nb_fails == 3) {
						strcpy(q->info.status, "LOCKED");					
						fprintf(f, "The card is blocked. Please contact the administrator.\n");
					}
					
					InsIncHS(&q->info.history, FAIL);
					free(SUCCESS);
					free(FAIL);
					return 0;
				}
				
				//se verifica daca cardul are pinul initial
				if(q->info.nb_change_pin == 0) {
					fprintf(f, "You must change your PIN.\n");
				}
				
				if(q->info.nb_change_pin > 0) {
					strcpy(q->info.status, "ACTIVE");
				}
				
				q->info.nb_fails = 0;
				InsIncHS(&q->info.history, SUCCESS);
				free(SUCCESS);
				free(FAIL);
				return 1;
			}
		}
	}

	InsIncHS(&q->info.history, FAIL);
	free(SUCCESS);
	free(FAIL);
	return 0;
}

int pin_change(char CARD_NUMBER[17], char NEW_PIN[10], AListaLC l, FILE *f) {
	TListaLC p;
	TListaLSC q;
	int x = atoi(NEW_PIN), k = 0;
	char *SUCCESS;
	char *FAIL;
	
	SUCCESS = malloc(100 * sizeof(char));
	if(!SUCCESS) {
		return 0;
	}
	
	FAIL = malloc(100 * sizeof(char));
	if(!FAIL) {
		free(SUCCESS);
		return 0;
	}

	sprintf(SUCCESS, "%s %s %s%s", "(SUCCESS, pin_change", CARD_NUMBER, NEW_PIN, ")");
	SUCCESS[strlen(SUCCESS) ] = '\0';
	sprintf(FAIL, "%s %s %s%s", "(FAIL, pin_change", CARD_NUMBER, NEW_PIN, ")");
	FAIL[strlen(FAIL)] = '\0';
	
	while(x != 0) {
		k++;
		x = x / 10;
	}

	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strcmp(q->info.card_number, CARD_NUMBER) == 0) {
				
				//se verifica daca pinul are 4 cifre
				if(k != 4) {
					fprintf(f, "Invalid PIN\n");
					InsIncHS(&q->info.history, FAIL);
					free(SUCCESS);
					free(FAIL);
					return 0;
				}
				
				strcpy(q->info.pin, NEW_PIN);
				q->info.nb_change_pin++;
				strcpy(q->info.status, "ACTIVE");
				InsIncHS(&q->info.history, SUCCESS);
				free(SUCCESS);
				free(FAIL);		
				return 1;
			}
		}
	}
	
	free(SUCCESS);
	free(FAIL);
	return 0;
}

int unblock_card(char CARD_NUMBER[17], AListaLC l) {
	TListaLC p;
	TListaLSC q;	
	
	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strncmp(q->info.card_number, CARD_NUMBER, strlen(CARD_NUMBER) - 1) == 0) {
				//se reseteaza numarul de introduceri gresite ale pinului
				q->info.nb_fails = 0;
				
				if(strcmp(q->info.status, "LOCKED") == 0) {
					strcpy(q->info.status, "ACTIVE");
				}
				
				if(strcmp(q->info.status, "NEW") == 0) {
					strcpy(q->info.status, "ACTIVE");
				}
				
				return 1;
			}
		}
	}
	
	return 0;	
}

int balance_inquiry(char CARD_NUMBER[17], AListaLC l, FILE *f) {
	TListaLC p;
	TListaLSC q;
	char *SUCCESS;
	char *FAIL;
	
	SUCCESS = malloc(100 * sizeof(char));
	if(!SUCCESS) {
		return 0;
	}
	
	FAIL = malloc(100 * sizeof(char));
	if(!FAIL) {
		free(SUCCESS);
		return 0;
	}

	sprintf(SUCCESS, "%s %s%s", "(SUCCESS, balance_inquiry", CARD_NUMBER, ")");
	SUCCESS[strlen(SUCCESS) ] = '\0';
	sprintf(FAIL, "%s %s%s", "(FAIL, balance_inquiry", CARD_NUMBER, ")");
	FAIL[strlen(FAIL)] = '\0';

	if(VerifCard(l, CARD_NUMBER, FAIL) == 1) {
		fprintf(f, "The card is blocked. Please contact the administrator\n");
		free(SUCCESS);
		free(FAIL);
		return 0;
	}
	
	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strncmp(q->info.card_number, CARD_NUMBER, strlen(CARD_NUMBER)) == 0) {
				InsIncHS(&q->info.history, SUCCESS);
				fprintf(f, "%d\n", q->info.balance);
				free(SUCCESS);
				free(FAIL);
				
				return 1;
			}
		}
	}
	
	free(SUCCESS);
	free(FAIL);
	return 0;
}

int recharge(char CARD_NUMBER[16], int SUM, AListaLC l, FILE *f) {
	TListaLC p;
	TListaLSC q;
	char *SUCCESS;
	char *FAIL;
	
	SUCCESS = malloc(100 * sizeof(char));
	if(!SUCCESS) {
		return 0;
	}
	
	FAIL = malloc(100 * sizeof(char));
	if(!FAIL){
		free(SUCCESS);
		return 0;
	}
	
	sprintf(SUCCESS, "%s %s %d%s", "(SUCCESS, recharge", CARD_NUMBER, SUM, ")");
	SUCCESS[strlen(SUCCESS) ] = '\0';
	sprintf(FAIL, "%s %s %d%s", "(FAIL, recharge", CARD_NUMBER, SUM, ")");
	FAIL[strlen(FAIL)] = '\0';
	
	if(VerifCard(l, CARD_NUMBER, FAIL) == 1) {
		fprintf(f, "The card is blocked. Please contact the administrator\n");
		free(SUCCESS);
		free(FAIL);
		return 0;
	}

	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strcmp(q->info.card_number, CARD_NUMBER) == 0) {
			
				if(SUM % 10 != 0) {
					fprintf(f, "The added amount must be multiple of 10\n");
					InsIncHS(&q->info.history, FAIL);
					free(SUCCESS);
					free(FAIL);
					return 0;
				}
				
				q->info.balance += SUM;
				fprintf(f, "%d\n", q->info.balance);
				InsIncHS(&q->info.history, SUCCESS);
				free(SUCCESS);
				free(FAIL);
				
				return 1;
			}
		}
	}
	
	InsIncHS(&q->info.history, FAIL);
	free(SUCCESS);
	free(FAIL);
	return 0;
}

int cash_withdrawal(char CARD_NUMBER[16], int SUM, AListaLC l, FILE *f) {
	TListaLC p;
	TListaLSC q;
	char *SUCCESS;
	char *FAIL;
	
	SUCCESS = malloc(100 * sizeof(char));
	if(!SUCCESS) {
		return 0;
	}
	FAIL = malloc(100 * sizeof(char));
	if(!FAIL) {
		free(SUCCESS);
		return 0;
	}
	
	sprintf(SUCCESS, "%s %s %d%s", "(SUCCESS, cash_withdrawal", CARD_NUMBER, SUM, ")");
	SUCCESS[strlen(SUCCESS) ] = '\0';
	sprintf(FAIL, "%s %s %d%s", "(FAIL, cash_withdrawal", CARD_NUMBER, SUM, ")");
	FAIL[strlen(FAIL)] = '\0';
	
	if(VerifCard(l, CARD_NUMBER, FAIL) == 1) {
		fprintf(f, "The card is blocked. Please contact the administrator\n");
		free(SUCCESS);
		free(FAIL);
		return 0;
	}
	
	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strcmp(q->info.card_number, CARD_NUMBER) == 0) {
				
				if(SUM % 10 != 0) {
					fprintf(f, "The requested amount must be multiple of 10\n");
					InsIncHS(&q->info.history, FAIL);
					free(SUCCESS);
					free(FAIL);
					return 0;
				}
				
				if(q->info.balance < SUM) {
					fprintf(f, "Insufficient funds\n");
					InsIncHS(&q->info.history, FAIL);
					free(SUCCESS);
					free(FAIL);
					return 0;
				}

				q->info.balance -= SUM;
				fprintf(f, "%d\n", q->info.balance);
				InsIncHS(&q->info.history, SUCCESS);
				free(SUCCESS);
				free(FAIL);
				return 1;
			}
		}
	}
	InsIncHS(&q->info.history, FAIL);
	free(SUCCESS);
	free(FAIL);
	
	return 0;
}

int transfer_funds(char CARD_NUMBER_SOURCE[17], char CARD_NUMBER_DEST[17], int SUM, AListaLC l, FILE *f){
	TListaLC p;
	TListaLSC q, aux;
	char *SUCCESS;
	char *FAIL;
	
	SUCCESS = malloc(200 * sizeof(char));
	if(!SUCCESS) {
		return 0;
	}
	
	FAIL = malloc(100 * sizeof(char));
	if(!FAIL) {
		free(SUCCESS);
		return 0;
	}
	
	sprintf(SUCCESS, "%s %s %s %d%s", "(SUCCESS, transfer_funds", CARD_NUMBER_SOURCE, CARD_NUMBER_DEST, SUM, ")");
	SUCCESS[strlen(SUCCESS) ] = '\0';
	sprintf(FAIL, "%s %s %s %d%s", "(FAIL, transfer_funds", CARD_NUMBER_SOURCE, CARD_NUMBER_DEST, SUM, ")");
	FAIL[strlen(FAIL)] = '\0';
	
	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strcmp(q->info.card_number, CARD_NUMBER_SOURCE) == 0) {
				aux = q;
				
				if(VerifCard(l, CARD_NUMBER_SOURCE, FAIL) == 1) {
					fprintf(f, "The card is blocked. Please contact the administrator\n");
					InsIncHS(&q->info.history, FAIL);
					free(SUCCESS);
					free(FAIL);
					return 0;
				}
				
				if(SUM % 10 != 0) {
					fprintf(f, "The transferred amount must be multiple of 10\n");
					InsIncHS(&q->info.history, FAIL);
					free(SUCCESS);
					free(FAIL);
					return 0;
				}
				
				if(q->info.balance < SUM) {
					fprintf(f, "Insufficient funds\n");
					InsIncHS(&q->info.history, FAIL);
					free(SUCCESS);
					free(FAIL);
					return 0; 
				}
			}
		}
	}

	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strcmp(q->info.card_number, CARD_NUMBER_DEST) == 0) {
				q->info.balance += SUM;
				aux->info.balance -= SUM;
				fprintf(f, "%d\n", aux->info.balance);
				InsIncHS(&q->info.history, SUCCESS);
				InsIncHS(&aux->info.history,SUCCESS);
				free(SUCCESS);
				free(FAIL);
				return 1;
			}
		}
	}
	
	InsIncHS(&q->info.history, FAIL);
	free(SUCCESS);
	free(FAIL);
	return 0;
}

int reverse_transaction(char CARD_NUMBER_SOURCE[17], char CARD_NUMBER_DEST[17], int SUM, AListaLC l, FILE *f) {
	TListaLC p;
	TListaLSC q, aux;
	char *SUCCESS;
	char *FAIL;
	char *REVERSE;
	
	SUCCESS = malloc(100 * sizeof(char));
	if(!SUCCESS) {
		free(SUCCESS);
		return 0;
	}
	FAIL = malloc(100 * sizeof(char));
	if(!FAIL) {
		free(SUCCESS);
		return 0;
	}
	
	sprintf(SUCCESS, "%s %s %s %d%s", "(SUCCESS, reverse_transaction", CARD_NUMBER_SOURCE, CARD_NUMBER_DEST, SUM, ")");
	SUCCESS[strlen(SUCCESS) ] = '\0';
	sprintf(FAIL, "%s %s %s %d%s", "(FAIL, reverse_transaction", CARD_NUMBER_SOURCE, CARD_NUMBER_DEST, SUM, ")");
	FAIL[strlen(FAIL)] = '\0';

	if(VerifCard(l, CARD_NUMBER_SOURCE, FAIL) == 1) {
		fprintf(f, "The card is blocked. Please contact the administrator\n");
		free(SUCCESS);
		free(FAIL);
		return 0;
	}
	
	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strcmp(q->info.card_number, CARD_NUMBER_SOURCE) == 0) {
				aux = q;
				break;
			}
		}
	}
	
	for(p = *l; p != NULL; p = p->urm) {
		for(q = p->info; q != NULL; q = q->urm) {
			if(strcmp(q->info.card_number, CARD_NUMBER_DEST) == 0) {
				
				if(q->info.balance < SUM) {
					fprintf(f, "The transaction cannot be reversed\n");
					free(SUCCESS);
					free(FAIL);
					return 0; 
				}
				
				q->info.balance -= SUM;
				aux->info.balance += SUM;
				InsIncHS(&aux->info.history, SUCCESS);
				
				REVERSE = malloc(100 * sizeof(char));
				if(!REVERSE) {
					free(SUCCESS);
					free(FAIL);
					return 0;
				}
				
				//creez REVERSE pentru a sti ce trebuie eliminat din istoricul
				//cardului destinatie
				sprintf(REVERSE, "%s %s %s %d%s", "(SUCCESS, transfer_funds", CARD_NUMBER_SOURCE, CARD_NUMBER_DEST, SUM, ")");
				REVERSE[strlen(REVERSE)] = '\0';
				EliminareHS(&q->info.history, REVERSE);
				
				free(SUCCESS);
				free(FAIL);
				free(REVERSE);
				return 1;
			}
		}
	}
	
	free(SUCCESS);
	free(FAIL);
	return 0;
}

int show(TListaLC L, FILE *f) {
	TListaLC p;
	TListaLSC q;
	TListaHS h;
	int k = 0;               
	
	for (p = L; p != NULL; p = p->urm) {
    	if(p->info == NULL) {
    		fprintf(f, "pos%d: []\n", k);
    	}
    	else {
	    	fprintf(f, "pos%d: [\n", k);
	    	
	    	for(q = p->info; q != NULL; q = q->urm) {
	    		fprintf(f, "(card number: %s, ", q->info.card_number);
	    		fprintf(f, "PIN: %s, ", q->info.pin);
	    		fprintf(f, "expiry date: %s, ", q->info.expiry_date);
	    		fprintf(f, "CVV: %s, ", q->info.cvv);
	    		fprintf(f, "balance: %d, ", q->info.balance);
	    		fprintf(f, "status: %s, ", q->info.status);
	    		
	    		if(q->info.history == NULL) {
	    			fprintf(f, "history: [])\n");   			
	    		}
	    		else {
		    		fprintf(f, "history: [");
		    		for(h = q->info.history; h != NULL; h = h->urm) {
		    			if(h->urm == NULL) {
		    				fprintf(f, "%s])\n", h->info);
		    			}
		    			else {
		    				fprintf(f, "%s, ", h->info);
		    			}
		    		}
		    	}
	    	}                            
	    	fprintf(f, "]\n");
	    }
   		k++; 
   }  
   
   return 1;
}

void showcard(TListaLC L, FILE *f, char CARD_NUMBER[17]) {
	TListaLC p;
	TListaLSC q;
	TListaHS h;            
	
	for (p = L; p != NULL; p = p->urm) {
    	for(q = p->info; q != NULL; q = q->urm) {
    		if(strcmp(q->info.card_number, CARD_NUMBER) == 0) {
	    		fprintf(f, "(card number: %s, ", q->info.card_number);
	    		fprintf(f, "PIN: %s, ", q->info.pin);
	    		fprintf(f, "expiry date: %s, ", q->info.expiry_date);
	    		fprintf(f, "CVV: %s, ", q->info.cvv);
	    		fprintf(f, "balance: %d, ", q->info.balance);
	    		fprintf(f, "status: %s, ", q->info.status);
	    		
	    		if(q->info.history == NULL) {
	    			fprintf(f, "history: [])\n");   			
	    		}
	    		else {
		    		fprintf(f, "history: [");
		    		for(h = q->info.history; h != NULL; h = h->urm) {
		    			if(h->urm == NULL) {
		    				fprintf(f, "%s])\n", h->info);
		    			}
		    			else {
		    				fprintf(f, "%s, ", h->info);
		    			}
		    		}
		    	}
		    }
    	}                            
    }  
}

int main() {
	TListaLC L = NULL;
	int nr_max_card = 0, sum = 0;
	char line[256], *p, pin[10], date[10], cvv[4], card_number[17];
	char card_number_dest[17], card_number_source[17];

	FILE *fin = fopen("input.in", "r");

	if (fin == NULL) {
        fprintf(stderr, "ERROR: Can't open file %s", "input.in");
        return -1;
    }

	FILE *fout = fopen("output.out", "a");

	if (fout == NULL) {
        fprintf(stderr, "ERROR: Can't open file %s", "output.out");
        return -1;
    }

	fgets(line, sizeof(line), fin);
	nr_max_card = atoi(line);
	while(fgets(line, sizeof(line), fin)) {
		p = strtok(line, " ");
		
		if(strcmp(p, "add_card") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number, 17, "%s", p);
			p = strtok(NULL, " ");
			snprintf(pin, strlen(p) + 1, "%s", p);
			p = strtok(NULL, " ");
			snprintf(date, 10, "%s", p);
			p = strtok(NULL, " ");
			snprintf(cvv, 4, "%s", p);
			add_card(card_number, pin, date, cvv, nr_max_card, &L, fout);
		}
		
		if(strcmp(p, "delete_card") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number, 20, "%s", p);
			delete_card(card_number, &L, fout);
		}
		
		if(strcmp(p, "insert_card") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number, 17, "%s", p);
			p = strtok(NULL, " ");
			snprintf(pin, strlen(p), "%s", p);
			insert_card(card_number, pin, &L, fout);
		}
		
		if(strcmp(p, "pin_change") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number, 17, "%s", p);
			p = strtok(NULL, " ");
			snprintf(pin, strlen(p), "%s", p);
			pin_change(card_number, pin, &L, fout);
		}
		
		if(strcmp(p, "cancel") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number, 17, "%s", p);
			cancel(card_number, &L, fout);
		}
		
		if(strcmp(p, "unblock_card") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number, 20, "%s", p);
			unblock_card(card_number, &L);
		}
		
		if(strcmp(p, "balance_inquiry") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number, 17, "%s", p);
			balance_inquiry(card_number, &L, fout);
		}
		
		if(strcmp(p, "recharge") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number, 17, "%s", p);
			p = strtok(NULL, " ");
			sum = atoi(p);
			recharge(card_number, sum, &L, fout);
		}
		
		if(strcmp(p, "cash_withdrawal") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number, 17, "%s", p);
			p = strtok(NULL, " ");
			sum = atoi(p);
			cash_withdrawal(card_number, sum, &L, fout);
		}

		if(strcmp(p, "transfer_funds") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number_source, 17, "%s", p);
			p = strtok(NULL, " ");
			snprintf(card_number_dest, 17, "%s", p);
			p = strtok(NULL, " ");
			sum = atoi(p);
			transfer_funds(card_number_source, card_number_dest, sum, &L, fout);
		}

		if(strcmp(p, "reverse_transaction") == 0) {
			p = strtok(NULL, " ");
			snprintf(card_number_source, 17, "%s", p);
			p = strtok(NULL, " ");
			snprintf(card_number_dest, 17, "%s", p);
			p = strtok(NULL, " ");
			sum = atoi(p);
			reverse_transaction(card_number_source, card_number_dest, sum, &L, fout);
		}

		if(strncmp(p, "show", 4) == 0) {
			p = strtok(NULL, " ");
			if(p != NULL) {
				snprintf(card_number, 17, "%s", p);
				showcard(L, fout, card_number);			
			}
			else {
				show(L, fout);
			}
		}
	}

	fclose(fin);
	fclose(fout);
	FreeLC(&L);

	return 0;
}