/*
---------------------------
Codice dell'algoritmo ADFMX
---------------------------
*/

#include<iostream>
#include<string>
#include <algorithm> //Per tolower, toupper
#include<conio.h>

using namespace std;

//Costanti globali
const int DIM_ADFMX = 5;
const int DIM_ALF_ADFMX = 25;
const int DIM_X_TC=200;
const int DIM_Y_TC=1000;

class ADFMX
{
	public:
		ADFMX();

		void Ins_Key(string K);
		void Ins_T_Chiaro(string TC);
		void Ins_KeyTC(string K);
		void Ins_T_Criptato(string TC); //Eliminare spazi

		string Decripta();
		string leggi();
		string Leggi_Chiave();
		string Leggi_Chiave_TC();

	private:
		struct Alf_Flag//Per controllare se una lettera è già stata inserita nella matrice
		{
			char Lettera;
			bool Flag;
		};

		//Matrici
		char Sgn[DIM_ADFMX]; //ADFMX
		char Matrice[DIM_ADFMX][DIM_ADFMX]; //Matrice 5x5
		Alf_Flag Alfabeto[DIM_ALF_ADFMX]; //Per controllare se una lettera è già stata inserita nella matrice

		//Variabili private
		string T_Chiaro; //TUTTO MAIUSCOLO
		string T_Cifrato; //TUTTO MAIUSCOLO -----	Per Cifratura
		string Key; //TUTTO MAIUSCOLO
		string Key_TC; //TUTTO MAIUSCOLO
		string T_Criptato; //TUTTO MAIUSCOLO ----- Per decifrare
        string T_Decriptato;


		//Funzioni private
		//X Criptare
		void Iniz_ADFMX();
		void Iniz_Matrice();
		void Iniz_Alfabeto();
		void Cifratura();
		void Trasposizione_Chiave();
		bool Is_ADFMX(char x);
		bool Is_Alfabeto(char x);

		//X Decriptare
		void Ins_Trasposizione_Chiave_Decr();
        void Lett_Trasposizione_Chiave_Decr(char Mat_TC[][DIM_Y_TC], int A, int B);
        void Decifrazione_Scacchiera();
};

ADFMX::ADFMX()
{
	Iniz_ADFMX();
	Iniz_Alfabeto();

	T_Chiaro = "DEFAULT";
	Key = "PROVA";
	Key_TC = "PROVA";
	T_Criptato="ADFMX";

}

string ADFMX::leggi()
{
	Iniz_ADFMX();
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

        if((i+1) % 5 == 0) //In questo cifrario va uno spazio dopo ogni n lettere. n lettere = lunghezza della chiave per la trasposizione
            App+=' ';
    }
    T_Cifrato.clear();
    T_Cifrato = App;

	return T_Cifrato;
}

string ADFMX::Decripta()
{
	Ins_Trasposizione_Chiave_Decr();

	Decifrazione_Scacchiera();

	return T_Decriptato;

}

string ADFMX::Leggi_Chiave()
{
    return Key;
}

string ADFMX::Leggi_Chiave_TC()
{
    return Key_TC;
}

void ADFMX::Ins_Key(string K)
{
	transform( K.begin(),K.end(), K.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < K.length())
	{
		if(K[i] == 'J') K[i] = 'I';
		if(!(Is_Alfabeto(K[i])))
			K.erase(i,1);
		else i++;
	}

	Key=K;

	return;
}

void ADFMX::Ins_T_Chiaro(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())
	{
		if(TC[i] == 'J') TC[i] = 'I';
		if(!(Is_Alfabeto(TC[i])))
			TC.erase(i,1);
		else i++;
	}

	T_Chiaro = TC;

	return;
}

void ADFMX::Ins_KeyTC(string K)
{
	transform( K.begin(),K.end(), K.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < K.length())
	{
		if(K[i] == 'J') K[i] = 'I';
		if(!(Is_Alfabeto(K[i])))
			K.erase(i,1);
		else i++;
	}

	Key_TC = K;

	return;
}

void ADFMX::Ins_T_Criptato(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())  //rimuovo spazi bianchi
	{
		if(TC[i] == 'J') TC[i] = 'I';
		if(!(Is_ADFMX(TC[i])))
			TC.erase(i,1);

		else i++;

	}
	T_Criptato=TC;

	return;
}

void ADFMX::Iniz_ADFMX()
{
	Sgn[0] = 'A';
	Sgn[1] = 'D';
	Sgn[2] = 'F';
	Sgn[3] = 'M';
	Sgn[4] = 'X';

	return;
}

void ADFMX::Iniz_Matrice()
{
	//Inizializzazione a vuoto della matrice
	for(int j=0; j<DIM_ADFMX; j++)
	for(int i=0; i<DIM_ADFMX; i++)
		Matrice[i][j]= '!';

	int Lungh = Key.length();

	int i,j, Cont_i=0, Cont_j=0;
	char L;
	bool Check;

	for(i = 0; i<Lungh; i++)
	{
		L=Key.at(i);

		for(j=0; j<DIM_ALF_ADFMX; j++)
		{
			if(Alfabeto[j].Lettera == L)
			{
				Check=Alfabeto[j].Flag;
				break;
			}
		}

		if(!(Check))
		{
			Alfabeto[j].Flag = true;
			Matrice[Cont_i][Cont_j] = L;
			Cont_i++;
			if(Cont_i == DIM_ADFMX)
			{
				Cont_i=0;
				Cont_j++;
			}
		}
	}

	for(j= 0; j<DIM_ADFMX; j++)
		for(i=0; i<DIM_ADFMX; i++)
		{
			if(Matrice[i][j] == '!')
			{

				for(int C=0; C<DIM_ALF_ADFMX; C++)
					if(Alfabeto[C].Flag == false)
					{
						Alfabeto[C].Flag =true;
						Matrice[i][j]=Alfabeto[C].Lettera;
						break;
					}
			}
		}

	return;

}

void ADFMX::Iniz_Alfabeto()
{
	for(int i = 0; i<DIM_ALF_ADFMX ; i++)
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
	Alfabeto[8].Lettera = 'I'; //I e J cifrate con lo stesso simbolo
	Alfabeto[9].Lettera = 'K';
	Alfabeto[10].Lettera = 'L';
	Alfabeto[11].Lettera = 'M';
	Alfabeto[12].Lettera = 'N';
	Alfabeto[13].Lettera = 'O';
	Alfabeto[14].Lettera = 'P';
	Alfabeto[15].Lettera = 'Q';
	Alfabeto[16].Lettera = 'R';
	Alfabeto[17].Lettera = 'S';
	Alfabeto[18].Lettera = 'T';
	Alfabeto[19].Lettera = 'U';
	Alfabeto[20].Lettera = 'V';
	Alfabeto[21].Lettera = 'W';
	Alfabeto[22].Lettera = 'X';
	Alfabeto[23].Lettera = 'Y';
	Alfabeto[24].Lettera = 'Z';

	return;
}

void ADFMX::Cifratura()
{
	int Cont = T_Chiaro.length();

	for(int C=0; C<Cont; C++)
	{
		for(int j=0; j<DIM_ADFMX; j++)
			for(int i=0; i<DIM_ADFMX; i++)
				if(Matrice[i][j] == T_Chiaro[C])
				{
					T_Cifrato+= Sgn[j];
					T_Cifrato+= Sgn[i];
				}
	}

	return;
}

void ADFMX::Trasposizione_Chiave()
{
	char Mat_TC[DIM_X_TC][DIM_Y_TC]; //Matrice per la trasposizione a chiave

	for(int i=0; i<DIM_X_TC; i++)
		for(int j=0; j<DIM_Y_TC; j++)
			Mat_TC[i][j] == '!';

	string App = Key_TC;
	int L_Key = Key_TC.length();
	int L_Testo = T_Cifrato.length();
	int C=0;
	int Cont_righe = 0;


	int Ordine[DIM_X_TC];
	for(int i=0; i<DIM_X_TC; i++)
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
			if(App[i] == Key_TC[j])
			{
				if(Ordine[j] == '!')
				{
					Ordine[j]=i;  //Imposto l'ordine per la trasposizione a chiave
					break;
				}
			}


	for(int j=0; j<DIM_Y_TC; j++) //Inserisco il testo cifrato nella matrice per la trasposizione a chiave
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
			if(Is_ADFMX(Mat_TC[Pos][j]))
				T_Cifrato += Mat_TC[Pos][j];
		}

		T_Cifrato += " ";
	}

	T_Cifrato.erase(T_Cifrato.length() - 1 , 1);

	return;

}

bool ADFMX::Is_ADFMX(char x)
{
	if(x == 'A') return true;
	if(x == 'D') return true;
	if(x == 'F') return true;
	if(x == 'M') return true;
	if(x == 'X') return true;

	return false;
}

bool ADFMX::Is_Alfabeto(char x)
{
	if(x == 'A') return true;
	if(x == 'B') return true;
	if(x == 'C') return true;
	if(x == 'D') return true;
	if(x == 'E') return true;
	if(x == 'F') return true;
	if(x == 'G') return true;
	if(x == 'H') return true;
	if(x == 'I') return true; //I e J cifrate con lo stesso simbolo
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

	return false;
}

void ADFMX::Ins_Trasposizione_Chiave_Decr()
{
	char Mat_TC[DIM_X_TC][DIM_Y_TC]; //Matrice per la trasposizione a chiave

	for(int i=0; i<DIM_X_TC; i++) //Inizializzo la matrice
		for(int j=0; j<DIM_Y_TC; j++)
			Mat_TC[i][j] == '!';

	int Ordine[DIM_X_TC];
	for(int i=0; i<DIM_X_TC; i++)
		Ordine[i]='!';

	int L_Key = Key_TC.length();
	int L_Testo = T_Criptato.length();
	int C=0;
	int Cont_righe = 0;
	string App = Key_TC;

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
			if(App[i] == Key_TC[j])
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

					if(j == L_Testo / L_Key && Resto == 0 )break; //Non fa il ciclo in più se l'ultima riga è completamente piena (evita ciclo a vuoto che darebb errore)

			}
		}
	}

	Lett_Trasposizione_Chiave_Decr(Mat_TC, L_Key, L_Testo / L_Key +1);

	return;
}

void ADFMX::Lett_Trasposizione_Chiave_Decr(char Mat_TC[][DIM_Y_TC], int A, int B)
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

void ADFMX::Decifrazione_Scacchiera()
{
    Iniz_Matrice(); //inizializzo la scacchiera su cui andare a decifrare
    Iniz_ADFMX();
    int i,j;

    int L_Testo = T_Criptato.length();

    int Pos = 0;
    for(int k=0; k<L_Testo; k+=2)
    {
        for(i=0; i<DIM_ADFMX; i++)
            if(T_Criptato[k+1] == Sgn[i]) break; //Si cifra prima riga e poi colonna, quindi prima k+1, poi k
        for(j=0; j<DIM_ADFMX; j++)
            if(T_Criptato[k] == Sgn[j]) break;

		Pos++;

        T_Decriptato+= Matrice[i][j];

    }
    return;
}
