/*
---------------------------
Codice dell'algoritmo ADFGVX
---------------------------
*/

#include<iostream>
#include<string>
#include <algorithm> //Per tolower, toupper
#include<conio.h>

using namespace std;

//Costanti globali
const int DIM_ADFGVX = 6;
const int DIM_ALF_ADFGVX = 36;
const int DIM_X_TC_ADFGVX=200;
const int DIM_Y_TC_ADFGVX=1000;

class ADFGVX
{
	public:
		ADFGVX();

		void Ins_Key(string K);
		void Ins_T_Chiaro(string TC);
		void Ins_T_Criptato(string TC); //Eliminare spazi

		string Decripta();
		string leggi();
		string Leggi_Chiave();

	private:
		struct Alf_Flag//Per controllare se una lettera è già stata inserita nella matrice
		{
			char Lettera;
			bool Flag;
		};

		//Matrici
		char Sgn[DIM_ADFGVX]; //ADFGVX
		char Matrice[DIM_ADFGVX][DIM_ADFGVX]; //Matrice 6X6
		Alf_Flag Alfabeto[DIM_ALF_ADFGVX]; //Per controllare se una lettera è già stata inserita nella matrice

		//Variabili private
		string T_Chiaro; //TUTTO MAIUSCOLO
		string T_Cifrato; //TUTTO MAIUSCOLO -----	Per Cifratura
		string Key; //TUTTO MAIUSCOLO -------	Diventa chiave per traspozione a chiave
		string T_Criptato; //TUTTO MAIUSCOLO ----- Per decifrare
        string T_Decriptato;


		//Funzioni private
		//X Criptare
		void Iniz_ADFGVX();
		void Iniz_Matrice();
		void Iniz_Alfabeto();
		void Cifratura();
		void Trasposizione_Chiave();
		bool Is_ADFGVX(char x);
		bool Is_Alfabeto(char x);

		//X Decriptare
		void Ins_Trasposizione_Chiave_Decr();
        void Lett_Trasposizione_Chiave_Decr(char Mat_TC[][DIM_Y_TC_ADFGVX], int A, int B);
        void Decifrazione_Scacchiera();

};

ADFGVX::ADFGVX()
{
	Iniz_ADFGVX();
	Iniz_Alfabeto();

	T_Chiaro = "DEFAULT";
	Key = "PROVA";
	T_Criptato="ADFGVX";
}

string ADFGVX::Decripta()
{
	Ins_Trasposizione_Chiave_Decr();

	Decifrazione_Scacchiera();

	return T_Decriptato;
}

string ADFGVX::Leggi_Chiave()
{
    return Key;
}

string ADFGVX::leggi()
{
	Iniz_ADFGVX();
	Iniz_Alfabeto();
	Iniz_Matrice();

	Cifratura();

	Trasposizione_Chiave();

	string App;

    for(int i=0; i<T_Cifrato.length(); i++) //Elimino tutti gli spazi bianchi che derivano dalla trasposizione
    {
        if(T_Cifrato[i] != ' ') App += T_Cifrato[i];
    }
    T_Cifrato.clear();
    T_Cifrato = App;

    App.clear();

    for(int i=0; i<T_Cifrato.length(); i++) //Aggiungo gli spazi da comunicare
    {
        App += T_Cifrato[i];

        if((i+1) % 5 == 0) //In questo cifrario va uno spazio ogni 5 lettere
            App+=' ';
    }
    T_Cifrato.clear();
    T_Cifrato = App;

	return T_Cifrato;
}

void ADFGVX::Ins_Key(string K)
{
	transform( K.begin(),K.end(), K.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < K.length())
	{
		if((!(Is_Alfabeto(K[i]))) ||
                K[i] == '0' ||
                K[i] == '1' ||
                K[i] == '2' ||
                K[i] == '3' ||
                K[i] == '4' ||
                K[i] == '5' ||
                K[i] == '6' ||
                K[i] == '7' ||
                K[i] == '8' ||
                K[i] == '9' )
                    K.erase(i,1);
		else i++;
	}

	Key=K;

	return;
}

void ADFGVX::Ins_T_Chiaro(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())
	{
		if(!(Is_Alfabeto(TC[i])))
			TC.erase(i,1);
		else i++;
	}

	T_Chiaro = TC;

	return;
}

void ADFGVX::Ins_T_Criptato(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())  //rimuovo spazi bianchi
	{
		if(!(Is_ADFGVX(TC[i])))
			TC.erase(i,1);

		else i++;

	}
	T_Criptato=TC;

	return;
}

void ADFGVX::Iniz_ADFGVX()
{
	Sgn[0] = 'A';
	Sgn[1] = 'D';
	Sgn[2] = 'F';
	Sgn[3] = 'G';
	Sgn[4] = 'V';
	Sgn[5] = 'X';

	return;
}

void ADFGVX::Iniz_Matrice()
{
	Matrice[0][0] = 'C';
	Matrice[1][0] = 'O';
	Matrice[2][0] = '8';
	Matrice[3][0] = 'X';
	Matrice[4][0] = 'F';
	Matrice[5][0] = '4';
	Matrice[0][1] = 'M';
	Matrice[1][1] = 'K';
	Matrice[2][1] = '3';
	Matrice[3][1] = 'A';
	Matrice[4][1] = 'Z';
	Matrice[5][1] = '9';
	Matrice[0][2] = 'N';
	Matrice[1][2] = 'W';
	Matrice[2][2] = 'L';
	Matrice[3][2] = '0';
	Matrice[4][2] = 'J';
	Matrice[5][2] = 'D';
	Matrice[0][3] = '5';
	Matrice[1][3] = 'S';
	Matrice[2][3] = 'I';
	Matrice[3][3] = 'Y';
	Matrice[4][3] = 'H';
	Matrice[5][3] = 'U';
	Matrice[0][4] = 'P';
	Matrice[1][4] = '1';
	Matrice[2][4] = 'V';
	Matrice[3][4] = 'B';
	Matrice[4][4] = '6';
	Matrice[5][4] = 'R';
	Matrice[0][5] = 'E';
	Matrice[1][5] = 'Q';
	Matrice[2][5] = '7';
	Matrice[3][5] = 'T';
	Matrice[4][5] = '2';
	Matrice[5][5] = 'G';
}

void ADFGVX::Iniz_Alfabeto()
{
	for(int i = 0; i<DIM_ALF_ADFGVX ; i++)
	{
		Alfabeto[i].Flag = false;
	}

	Alfabeto[0].Lettera = 'A';
	Alfabeto[1].Lettera = 'B';
	Alfabeto[2].Lettera = 'C';
	Alfabeto[3].Lettera = 'D';
	Alfabeto[4].Lettera = 'E';
	Alfabeto[5].Lettera = 'F';
	Alfabeto[6].Lettera = 'G';
	Alfabeto[7].Lettera = 'H';
	Alfabeto[8].Lettera = 'I';
	Alfabeto[9].Lettera = 'J';
	Alfabeto[10].Lettera = 'K';
	Alfabeto[11].Lettera = 'L';
	Alfabeto[12].Lettera = 'M';
	Alfabeto[13].Lettera = 'N';
	Alfabeto[14].Lettera = 'O';
	Alfabeto[15].Lettera = 'P';
	Alfabeto[16].Lettera = 'Q';
	Alfabeto[17].Lettera = 'R';
	Alfabeto[18].Lettera = 'S';
	Alfabeto[19].Lettera = 'T';
	Alfabeto[20].Lettera = 'U';
	Alfabeto[21].Lettera = 'V';
	Alfabeto[22].Lettera = 'W';
	Alfabeto[23].Lettera = 'X';
	Alfabeto[24].Lettera = 'Y';
	Alfabeto[25].Lettera = 'Z';
	Alfabeto[26].Lettera = '0';
	Alfabeto[27].Lettera = '1';
	Alfabeto[28].Lettera = '2';
	Alfabeto[29].Lettera = '3';
	Alfabeto[30].Lettera = '4';
	Alfabeto[31].Lettera = '5';
	Alfabeto[32].Lettera = '6';
	Alfabeto[33].Lettera = '7';
	Alfabeto[34].Lettera = '8';
	Alfabeto[35].Lettera = '9';

	return;
}

void ADFGVX::Cifratura()
{
	int Cont = T_Chiaro.length();

	for(int C=0; C<Cont; C++)
	{
		for(int j=0; j<DIM_ADFGVX; j++)
			for(int i=0; i<DIM_ADFGVX; i++)
				if(Matrice[i][j] == T_Chiaro[C])
				{
					T_Cifrato+= Sgn[j];
					T_Cifrato+= Sgn[i];
				}
	}

	return;
}

void ADFGVX::Trasposizione_Chiave()
{
	char Mat_TC[DIM_X_TC_ADFGVX][DIM_Y_TC_ADFGVX]; //Matrice per la trasposizione a chiave

	for(int i=0; i<DIM_X_TC_ADFGVX; i++)
		for(int j=0; j<DIM_Y_TC_ADFGVX; j++)
			Mat_TC[i][j] == '!';

	string App = Key;
	int L_Key = Key.length();
	int L_Testo = T_Cifrato.length();
	int C=0;
	int Cont_righe = 0;


	int Ordine[DIM_X_TC_ADFGVX];
	for(int i=0; i<DIM_X_TC_ADFGVX; i++)
		Ordine[i]='!';

	for(int i = 0; i<L_Key-1; i++) //Nella stringa di appoggio ordino la chiave in ordine alfabetico
           for(int k = 0; k<L_Key-1-i; k++)
                 if(App[k] > App[k+1])
				 {
				 	int Scambio=App[k];
     				App[k]=App[k+1];
    				App[k+1]=Scambio;
				}

	for(int i=0; i<L_Key; i++)
		for(int j=0; j<L_Key; j++)
			if(App[i] == Key[j])
			{
				if(Ordine[j] == '!')
				{
					Ordine[j]=i;  //Imposto l'ordine per la trasposizione a chiave
					break;
				}
			}


	for(int j=0; j<DIM_Y_TC_ADFGVX; j++) //Inserisco il testo cifrato nella matrice per la trasposizione a chiave
	{
		if(L_Testo>C) //Se la stringa non è finita
		{
			Cont_righe++;
			for(int i=0; i<L_Key; i++)
			{
					if(L_Testo>C) //Se la stringa non è finita
					{
						Mat_TC[i][j] = T_Cifrato[C];
						C++;
					}
			}
		}
	}

	T_Cifrato.clear();
	int Pos;


	for(int i=0; i<L_Key; i++)  //Cripto il testo con la trasposizione a chiave
	{
		for(int k=0; k<L_Key; k++)
				if(Ordine[k] == i)
				{
					Pos=k;
					break;
				}

		for(int j=0; j<Cont_righe; j++)
		{
			if(Is_ADFGVX(Mat_TC[Pos][j]))
				T_Cifrato += Mat_TC[Pos][j];
		}

		T_Cifrato += " ";
	}

	T_Cifrato.erase(T_Cifrato.length() - 1 , 1);

	return;

}

bool ADFGVX::Is_ADFGVX(char x)
{
	if(x == 'A') return true;
	if(x == 'D') return true;
	if(x == 'F') return true;
	if(x == 'G') return true;
	if(x == 'V') return true;
	if(x == 'X') return true;

	return false;
}

bool ADFGVX::Is_Alfabeto(char x)
{
	if(x == 'A') return true;
	if(x == 'B') return true;
	if(x == 'C') return true;
	if(x == 'D') return true;
	if(x == 'E') return true;
	if(x == 'F') return true;
	if(x == 'G') return true;
	if(x == 'H') return true;
	if(x == 'I') return true;
	if(x == 'J') return true;
	if(x == 'K') return true;
	if(x == 'L') return true;
	if(x == 'M') return true;
	if(x == 'N') return true;
	if(x == 'O') return true;
	if(x == 'P') return true;
	if(x == 'Q') return true;
	if(x == 'R') return true;
	if(x == 'S') return true;
	if(x == 'T') return true;
	if(x == 'U') return true;
	if(x == 'V') return true;
	if(x == 'W') return true;
	if(x == 'X') return true;
	if(x == 'Y') return true;
	if(x == 'Z') return true;
	if(x == '0') return true;
	if(x == '1') return true;
	if(x == '2') return true;
	if(x == '3') return true;
	if(x == '4') return true;
	if(x == '5') return true;
	if(x == '6') return true;
	if(x == '7') return true;
	if(x == '8') return true;
	if(x == '9') return true;

	return false;
}

void ADFGVX::Ins_Trasposizione_Chiave_Decr()
{
	char Mat_TC[DIM_X_TC_ADFGVX][DIM_Y_TC_ADFGVX]; //Matrice per la trasposizione a chiave

	for(int i=0; i<DIM_X_TC_ADFGVX; i++) //Inizializzo la matrice
		for(int j=0; j<DIM_Y_TC_ADFGVX; j++)
			Mat_TC[i][j] == '!';

	int Ordine[DIM_X_TC_ADFGVX];
	for(int i=0; i<DIM_X_TC_ADFGVX; i++)
		Ordine[i]='!';

	int L_Key = Key.length();
	int L_Testo = T_Criptato.length();
	int C=0;
	int Cont_righe = 0;
	string App = Key;

	for(int i = 0; i<L_Key-1; i++) //Nella stringa di appoggio ordino la chiave in ordine alfabetico
           for(int k = 0; k<L_Key-1-i; k++)
                 if(App[k] > App[k+1])
				 {
				 	int Scambio=App[k];
     				App[k]=App[k+1];
    				App[k+1]=Scambio;
				}

	for(int i=0; i<L_Key; i++)
		for(int j=0; j<L_Key; j++)
			if(App[i] == Key[j])
			{
				if(Ordine[j] == '!')
				{
					Ordine[j]=i;  //Imposto l'ordine per la trasposizione a chiave
					break;
				}
			}

	int Pos;
	int Resto = L_Testo % L_Key;

	for(int i=0; i<L_Key; i++) //Inserisco il testo criptato nella matrice per la trasposizione a chiave
	{
		for(int k=0; k<L_Key; k++)
			if(i == Ordine[k]) Pos=k; //Pos diventa la posizione dell'indice nell'ordine
		if(L_Testo>C) //Se la stringa non è finita
		{
			for(int j=0; j< L_Testo / L_Key +1; j++) //Lunghezza del testo diviso la lunghezza della chiave +1 da il numero di righe della matrice
			{
					if(j ==( L_Testo / L_Key ) && (Pos + 1) > Resto )break; //Salta l'ultima riga se dovrebbe essere vuota
					if(L_Testo>C) //Se la stringa non è finita
					{
						Mat_TC[Pos][j] = T_Criptato[C];
						C++;
					}

					if(j == L_Testo / L_Key && Resto == 0 ) break; //Non fa il ciclo in più se l'ultima riga è completamente piena (evita ciclo a vuoto che darebb errore)

			}
		}
	}

	Lett_Trasposizione_Chiave_Decr(Mat_TC, L_Key, L_Testo / L_Key +1);

	return;
}

void ADFGVX::Lett_Trasposizione_Chiave_Decr(char Mat_TC[][DIM_Y_TC_ADFGVX], int A, int B)
{
	int L_Testo = T_Criptato.length();
	T_Criptato.clear();
	int C=0;

	for(int j=0; j<B; j++)
    	for(int i=0; i<A; i++) // Leggo la matrice per righe, per ricomporre il testo cifrato
        {
        	if(C<L_Testo)
			{
				T_Criptato+=Mat_TC[i][j];
				C++;
			}
		}
    return;
}

void ADFGVX::Decifrazione_Scacchiera()
{
    Iniz_Matrice(); //inizializzo la scacchiera su cui andare a decifrare
    Iniz_ADFGVX();
    int i,j;

    int L_Testo = T_Criptato.length();

    int Pos = 0;
    for(int k=0; k<L_Testo; k+=2)
    {
        for(i=0; i<DIM_ADFGVX; i++)
            if(T_Criptato[k+1] == Sgn[i]) break; //Si cifra prima riga e poi colonna, quindi prima k+1, poi k
        for(j=0; j<DIM_ADFGVX; j++)
            if(T_Criptato[k] == Sgn[j]) break;

		Pos++;

        T_Decriptato+= Matrice[i][j];
    }
    return;
}
