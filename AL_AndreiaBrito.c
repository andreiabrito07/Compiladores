#include "token.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define ENTER 13


typedef struct{
	char Lexema[50];
	char Token[60];
}PAR;

typedef struct{
	char Lexema[50];
	char Token[60];
	int linha;
	char tipo[10];
	int endereco;
	int tamanho;
	char escopo[20];
	char dimensao[20];
	char categoria[20];
	
}table;

//Global VARs
FILE *ARQ;
char str[60];
PAR reservadas[32];
table *tokenTable;
int indexTable = 0, indexErros = 0, cont = 0, linha = 1;
int *tableErros;

void iniciarVet(){
	int i = 0;
	strcpy(reservadas[0].Lexema, "asm");
	strcpy(reservadas[0].Token, "Palavara_Res_Asm");
	strcpy(reservadas[1].Lexema, "auto");
	strcpy(reservadas[1].Token, "Palavara_Res_Auto");
	strcpy(reservadas[2].Lexema, "break");
	strcpy(reservadas[2].Token, "Palavara_Res_Break");
	strcpy(reservadas[3].Lexema, "case");
	strcpy(reservadas[3].Token, "Palavara_Res_Case");
	strcpy(reservadas[4].Lexema, "char");
	strcpy(reservadas[4].Token, "Palavara_Res_Char");
	strcpy(reservadas[5].Lexema, "const");
	strcpy(reservadas[5].Token, "Palavara_Res_Const");
	strcpy(reservadas[6].Lexema, "continue");
	strcpy(reservadas[6].Token, "Palavara_Res_Contninue");
	strcpy(reservadas[7].Lexema, "default");
	strcpy(reservadas[7].Token, "Palavara_Res_Default");
	strcpy(reservadas[8].Lexema, "do");
	strcpy(reservadas[8].Token, "Palavara_Res_Do");
	strcpy(reservadas[9].Lexema, "double");
	strcpy(reservadas[9].Token, "Palavara_Res_Double");
	strcpy(reservadas[10].Lexema, "else");
	strcpy(reservadas[10].Token, "Palavara_Res_Else");
	strcpy(reservadas[11].Lexema, "enum");
	strcpy(reservadas[11].Token, "Palavara_Res_Enum");
	strcpy(reservadas[12].Lexema, "extern");
	strcpy(reservadas[12].Token, "Palavara_Res_Extern");
	strcpy(reservadas[13].Lexema, "float");
	strcpy(reservadas[13].Token, "Palavara_Res_Float");
	strcpy(reservadas[14].Lexema, "for");
	strcpy(reservadas[14].Token, "Palavara_Res_For");
	strcpy(reservadas[15].Lexema, "goto");
	strcpy(reservadas[15].Token, "Palavara_Res_Goto");
	strcpy(reservadas[16].Lexema, "if");
	strcpy(reservadas[16].Token, "Palavara_Res_If");
	strcpy(reservadas[17].Lexema, "int");
	strcpy(reservadas[17].Token, "Palavara_Res_Int");
	strcpy(reservadas[18].Lexema, "long");
	strcpy(reservadas[18].Token, "Palavara_Res_Long");
	strcpy(reservadas[19].Lexema, "register");
	strcpy(reservadas[19].Token, "Palavara_Res_Register");
	strcpy(reservadas[20].Lexema, "return");
	strcpy(reservadas[20].Token, "Palavara_Res_Return");
	strcpy(reservadas[21].Lexema, "short");
	strcpy(reservadas[21].Token, "Palavara_Res_Short");
	strcpy(reservadas[22].Lexema, "signed");
	strcpy(reservadas[22].Token, "Palavara_Res_Signed");
	strcpy(reservadas[23].Lexema, "sizeof");
	strcpy(reservadas[23].Token, "Palavara_Res_Sizeof");
	strcpy(reservadas[24].Lexema, "static");
	strcpy(reservadas[24].Token, "Palavara_Res_Static");
	strcpy(reservadas[25].Lexema, "struct");
	strcpy(reservadas[25].Token, "Palavara_Res_Struct");
	strcpy(reservadas[26].Lexema, "switch");
	strcpy(reservadas[26].Token, "Palavara_Res_Switch");
	strcpy(reservadas[27].Lexema, "typedef");
	strcpy(reservadas[27].Token, "Palavara_Res_Typedef");
	strcpy(reservadas[28].Lexema, "union");
	strcpy(reservadas[28].Token, "Palavara_Res_Union");
	strcpy(reservadas[29].Lexema, "unsigned");
	strcpy(reservadas[29].Token, "Palavara_Res_Unsigned");
	strcpy(reservadas[30].Lexema, "void");
	strcpy(reservadas[30].Token, "Palavara_Res_Void");
	strcpy(reservadas[31].Lexema, "volatile");
	strcpy(reservadas[31].Token, "Palavara_Res_Volatile");
	strcpy(reservadas[32].Lexema, "while");
	strcpy(reservadas[32].Token, "Palavara_Res_While");
	
}


void iniciarTable(){
	tokenTable = malloc(sizeof(table));
	
}

void iniciarTableErros(){
	tableErros = malloc(sizeof(int));
}
void inseriNaTabela(table  e){
	tokenTable = (table *)realloc(tokenTable,(indexTable+1)*sizeof(table));
	tokenTable[indexTable] = e;
	//printf("Inseri %s\n", tokenTable[indexTable].Lexema);
	indexTable++;
	
}

void inserirErro(char *erro){
	tableErros = (int *)realloc(tableErros, (indexErros+1)*sizeof(int));
}
char lerChar(){
	char ler;
	fread(&ler, 1, 1, ARQ);
	return ler;
}

void zerarVetor(){
	int i;
	for(i = 0; i <= cont; i++){
		str[i] = '\0';
	}
	
}

void zeraCont(){
	cont = 0;
}


int comparaString(char str[30], char str2[30]){
	int i, flag = 0;
	if(strlen(str) != strlen(str2)){
		return 1;
	}
	for (i = 0; str[i] != '\0'; i++){
		if(str[i]  != str2[i]){
			flag = 1;
		}
		
	}
	return flag;
}

PAR verificarReservadas(){
	int i = 0;
	for(i = 0; i <= 32; i++){
		if(comparaString(str, reservadas[i].Lexema) == 0){
			return reservadas[i];
		}
	}
	PAR aux;
	strcpy(aux.Lexema, str);
	strcpy(aux.Token, TOK_ID);
	return aux;
}
void  analex(){
	table elem;
	table par;
	char car;
	int estado = 0;
	char str2[50];
	while(!feof(ARQ)){
		
		car = lerChar();
		
		switch(estado){
			case 0:
				{	
					voltar:
					if((car >= 'A' && car <= 'Z') || (car >= 'a' && car <='z')|| (car == '_')){
						
						str[cont] = car;
						cont ++;
						estado = 1;
						
					} else if(car == '0' || car == '1' || car == '2' || car == '3' || car == '4' || car == '5'
					|| car == '6' || car == '7' || car == '8'|| car == '9'){
					
						str[cont] = car;
						cont ++;
						estado = 3;
						
					} else if (car == '>'){
						str[cont] = car;
						cont ++;
						estado = 8;
					
					}else if(car == '<'){
						str[cont] = car;
						cont ++;
						estado = 16;
					
					}else if (car == '='){
						str[cont] = car;
						cont ++;
						estado = 24;
					
					}else if(car == '!'){
						str[cont] = car;
						cont ++;
						estado = 28;
						
					}else if (car == '*'){
						str[cont] = car;
						cont ++;
						estado = 32;
						
					}else if(car == '+'){
						str[cont] = car;
						cont ++;
						estado = 44;
					
					}else if(car == '-'){
						str[cont] = car;
						cont ++;
						estado = 50;
					
					}else if(car == '&'){
						str[cont] = car;
						cont ++;
						estado = 82;
						
					}else if(car == '|'){
						str[cont] = car;
						cont ++;
						estado = 88;
						
					}else if (car == '^'){
						str[cont] = car;
						cont ++;
						estado = 94;
					
					}else if(car == '~'){
						str[cont] = car;
						cont ++;
						estado = 98;
					
					}else if(car == ','){
						str[cont] = car;
						cont ++;
						estado = 100;
						
					}else if(car == '?'){
						str[cont] = car;
						cont ++;
						estado = 102;
						
					}else if(car == '('){
						strcpy(par.Lexema, "(");
						strcpy(par.Token, TOK_AP);
						par.linha = linha;
						inseriNaTabela(par);
					
					}else if(car == ')'){
						str[cont] = car;
						cont ++;
						estado = 106;
					;	
					}else if(car == '{'){
						str[cont] = car;
						cont ++;
						estado = 108;
					
					}else if(car == '}'){
						str[cont] = car;
						cont ++;
						estado = 110;
					
					}else if(car == '['){
						str[cont] = car;
						cont ++;
						estado = 112;
						
					}else if(car == ']'){
						str[cont] = car;
						cont ++;
						estado = 114;
					
					}else if(car == ':'){
						str[cont] = car;
						cont ++;
						estado = 116;
					
					}else if(car == ';'){
						str[cont] = car;
						cont ++;
						estado = 118;
					
					}else if(car == '.'){
						str[cont] = car;
						cont ++;
						estado = 120;
					
					}else if(car == '"'){
						str[cont] = car;
						cont ++;
						estado = 122;
					
					}else if(car == '#'){
						str[cont] = car;
						cont ++;
						estado = 126;
		
					}else if(car == '\t'){
						estado = 0;
					}else if(car == '\''){
						str[cont] = car;
						cont ++;
						estado = 124;
					
					}else if(car == '%'){
						str[cont] = car;
						cont ++;
						estado = 56;
						
					}else if(car == '/'){
						str[cont] = car;
						cont ++;
						estado = 36;
					}else if(car == ' '){
						break;
					}else if(car == '\''){
						str[cont] = car;
						cont ++;
						estado = 124;
						
					}else if (car == '\n'){
						linha++;
						estado = 0;
					}
					break;
					
				}
			case 1:
				{
				
					if((car >= 'A' && car <= 'Z') || (car >= 'a' && car <='z')|| (car == '_')||(car >= '0' && car <= '9')){
						str[cont] = car;
						cont ++;
						estado = 1;
						
					}else{
						if(car == '\''){
							strcpy(elem.Lexema, str);
							strcpy(elem.Token, "Caractere");
							elem.linha = linha;
							inseriNaTabela(elem);
							zerarVetor();
							zeraCont();
							goto voltar;
						}else{
							PAR aux = verificarReservadas();
							strcpy(par.Lexema, aux.Lexema);
							strcpy(par.Token, aux.Token);
							par.linha = linha;
							inseriNaTabela(par);
							zerarVetor();
							zeraCont();
							estado = 0;
							goto voltar;
						}	
					
					}
					break;
				}
			case 3:
				{
					if(car == '0' || car == '1' || car == '2' || car == '3' || car == '4' || car == '5'
					|| car == '6' || car == '7' || car == '8'|| car == '9'){
						str[cont] = car;
						cont ++;
						estado = 3;
						
					}else if (car == '.'){
						str[cont] = car;
						cont ++;
						estado = 4;
						
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_NUMINT);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
					break;
					
				}
			case 4:
				{	
					if(car == '0' || car == '1' || car == '2' || car == '3' || car == '4' || car == '5'
					|| car == '6' || car == '7' || car == '8'|| car == '9'){
					str[cont] = car;
					cont ++;
					estado = 6;	
					}
					break;
					
				}
			case 6:
				{
					if(car == '0' || car == '1' || car == '2' || car == '3' || car == '4' || car == '5'
					|| car == '6' || car == '7' || car == '8'|| car == '9'){
						
						str[cont] = car;
						cont ++;
						estado = 6;	
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_NUMREAL);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					
						
					}
					break;
				}
			case 8:
				{
					if(car == '='){
						str[cont] = car;
						cont ++;
						estado = 10;
						
					}else if (car == '>'){
						str[cont] = car;
						cont ++;
						estado = 12;
					
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_MAIOR);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
				
					}
					break;
				}
			case 10:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_MOUI);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;
					goto voltar;
				
					break;
				}
			case 12:
				{
					if(car == '='){
						str[cont] = car;
						cont ++;
						strcpy(par.Lexema, str);
						strcpy(par.Token,TOK_MAIORMIG);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_MAIORMAIOR);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
				break;
				}
			case 16:
				{
					if (car == '='){
						str[cont] = car;
						cont ++;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_MENORIG);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
					
					}else if(car == '<'){
						str[cont] = car;
						cont ++;
						estado = 20;
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_MENOR);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
					break;
				}
			case 20:
				{
					if(car == '='){
						str[cont] = car;
						cont ++;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_MENORMIG);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						
						
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_MENORMENOR);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
					break;
				}
			case 24:
				{
					if(car == '='){
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_IGUAL);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_ATRIBUICAO);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
					break;
				}
			case 28:
				{
					if(car == '='){
						str[cont] =  car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_DIFERENTE);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						break;
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_FALSE);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
					
				}
			case 32:
				{
					if(car == '='){
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_ATRIBUIMULTIPLICA);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						break;
					}else{
						
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_MULTIPLICACAO);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
					
				}
			case 36:
				 {
				 	if(car == '='){
				 		str[cont] = car;
				 		strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_ATRIBUIDIVIDE);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
					
						
					 }else if (car == '/'){
					 	str[cont] = car;
					 	cont ++;
					 	estado = 130;
					 
					 }else if( car == '*'){
					 	str[cont] = car;
					 	cont ++;
					 	estado = 40;
					 
					 }else{
					 	strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_DIVISAO);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					 }
					 break;
					
				 }
			case 40:
				{
					if(car == '*'){
						str[cont] = car;
						cont++;
						estado = 41;
					
					}else{
						str[cont] = car;
						cont ++;
						if(car == '\n'){
							linha++;
						}
						estado = 40;
					
					}
					break;
				}
			case 41:
				{
					
					if(car == '*'){
						str[cont] = car;
						cont ++;
						estado = 41;
					
					}else if(car == '/'){
						str[cont] =  car;
						strcpy(par.Lexema, "Comentario");
						strcpy(par.Token, TOK_COMENTARIO);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
					
					}else{
						str[cont] = car;
						cont ++;
						estado = 40;
						if(car == '\n'){
							linha++;
						}
						
					}
					break;
				}
			case 44:
				{
					if(car == '+'){
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_INCREMENTO);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						break;
						
					}else if(car == '='){
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_ATRIBUIADICIONA);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						break;
						
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_MAIS);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
				
				}
			case 50:
				{
					if(car == '-'){
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_DECREMENTO);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						
					}else if(car == '='){
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_ATRIBUISUBTRAI);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
					
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_MENOS);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
					break;
					
				}
			case 56:
				{
					if(car == '='){
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_ATRIBUIMODULO);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						break;
					
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_MODULO);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
				
					
				}
			case 82:
				{
					if(car =='&'){ 
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_ELOG);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
					}else if( car == '='){
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_ATRIBUIAND);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_E);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
					break;
					
				}
			case 88:
				{
					if(car == '|'){
						str[cont] = car;
						
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_OULOG);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
					}else if(car == '='){
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_ATRIBUIOU);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_OU);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
				break;
					
				}
			case 94:
				{
					if(car == '='){
						str[cont] = car;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_CHAPEUIGUAL);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						break;
					}else{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_CHAPEU);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}
					
				}
			case 98:
				{
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_TIL);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						break;
				}
			case 100:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_VG);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;
					goto voltar;
					break;
				}
			case 102:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_PI);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;	
					goto voltar;
					
					break;
				}
			case 104:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_AP);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;	
					break;
				}
			case 106:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_FP);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;
					goto voltar;
					break;
				}
			case 108:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_ACHV);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;
					break;
				}	
			case 110:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_FCHV);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;
					goto voltar;	
					break;
					
				}
			case 112:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_APR);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;
					goto voltar;
					break;
				}
			case 114:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_FPR);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;	
					goto voltar;
					break;
				}
			case 116:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_DPT);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;	
					goto voltar;
					break;	
				}
			case 118:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_PVG);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;	
					goto voltar;
					break;	
				}
			case 120:
				{
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_PT);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;	
					goto voltar;	
					break;	
				}
			case 122:
				{	
					
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_ASPAS);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;	
					goto voltar;
					break;	
				}
				case 124:
				{	
					
					strcpy(par.Lexema, str);
					strcpy(par.Token, TOK_APOSTROFE);
					par.linha = linha;
					inseriNaTabela(par);
					zerarVetor();
					zeraCont();
					estado = 0;	
					goto voltar;
					
					
					break;	
				}
			case 126:
				{
					if(car != ENTER){
						str[cont] = car;
						cont++;
						estado = 126;
					}else{
						str[cont--]= 0;
						strcpy(par.Lexema, str);
						strcpy(par.Token, TOK_CARDINAL);
						par.linha = linha;
						inseriNaTabela(par);
						
						zerarVetor();
						zeraCont();
						estado = 0;
					}
					break;	
				}
			case 130:
				{
					if(car == '\n'){
						strcpy(par.Lexema, "Comentario");
						strcpy(par.Token, TOK_COMENTARIO);
						par.linha = linha;
						inseriNaTabela(par);
						zerarVetor();
						zeraCont();
						estado = 0;
						goto voltar;
					}else{
						str[cont] = car;
						cont++;
						estado = 130;
						
					}
					break;
					
				}		
			}
		}
		strcpy(par.Lexema, "EOF");
		strcpy(par.Token, TOK_END);
		par.linha = linha;
		inseriNaTabela(par);
	
}

int main(){
	iniciarTable();
	iniciarVet();
	int i = 0, tamanho;
	char filename[20];
	printf("Digite o nome do file com a extensao .txt: ");
	gets(filename);
	ARQ = fopen(filename, "rb");
	analex();
	printf("LINHA\tLEXEMA\tTOKEN\n");
	for (i = 0; i<= indexTable -1 ; i++){
		printf("%d\t%s\t%s\n",tokenTable[i].linha, tokenTable[i].Lexema, tokenTable[i].Token);
			
	}

	return 0;
}
