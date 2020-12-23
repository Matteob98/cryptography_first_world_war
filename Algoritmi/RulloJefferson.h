/*
---------------------------
Codice dell'algoritmo del rullo di jefferson
---------------------------
*/

#include<iostream>
#include<string>
#include <algorithm> //Per tolower, toupper
#include <fstream>
#include <time.h>

using namespace std;

const int DIM_ALF_Jefferson = 36;
const int NUM_RULLI_Jefferson = 25;

class RulloJefferson
{
	public:
		RulloJefferson();
		void Mod_Rullo(char C[], int N_Rullo);
		void Ins_Key(int K); //Inserimento chiave da fuori
		void Ins_T_Chiaro(string TC);
		void Ins_T_Criptato(string TC);
		void Reimposta_Rullo_STD(); //Annulla ogni modifica effettuata sul rullo

		void Leggi_Rullo(char R[], int & DIM_Rulli, int & N_Rulli); //I vettori sono sempre passati per riferimento mai per valore
		string Leggi_Cifra();
		string Leggi_Decifra();
	private:
		char Rullo[NUM_RULLI_Jefferson][DIM_ALF_Jefferson];
		char Rullo_Decriptazione[NUM_RULLI_Jefferson][DIM_ALF_Jefferson]; //Per decriptazione step 3
		string T_Chiaro;
		string T_Cifrato;
		string T_Criptato;
		string T_Decriptato;
		int Key;

		struct Alf_Flag//Per controllare se una lettera è già stata inserita nella matrice
		{
			char Lettera;
			bool Flag;
		};
		Alf_Flag Alfabeto[DIM_ALF_Jefferson]; //Per controllare se una lettera è già stata inserita nella matrice

		void Iniz_Alfabeto();
		void Carica_Rullo();
		bool Check_Validita_Rullo(char C[]);
		bool Is_Testo(char x);
		int Posizione_In_Matrice_Riga_Fissa(char X, char Mat[][DIM_ALF_Jefferson], int N_Rullo);
		void Salva_Rullo();

		void Cifratura_Stp1(); //Controllo se il testo è più corto di 25, in quel caso aggiungo X fino a 25, in caso contrario divido il testo di 25 in 25
		void Cifratura_Stp2(); //Ruoto i rotori per formare la parola con il testo in chiaro, e mi segno sul verrore Posizioni di quanti step ho rotato i singoli rotori
		void Cifratura_Stp3(int V[]);

		void Decifratura_Stp1();
		void Decifratura_Stp2();
		void Decifratura_Stp3(int v[]);
};

RulloJefferson::RulloJefferson()
{
	Key=0;
	T_Chiaro = "ABC";
	T_Criptato="XXX";
	Iniz_Alfabeto();
	Carica_Rullo();
}

void RulloJefferson::Ins_Key(int K)
{
	//if(K<1) K=1;
	//if(K>NUM_RULLI_Jefferson-1) K=NUM_RULLI_Jefferson-1;

	Key=K;

	return;
}

void RulloJefferson::Ins_T_Chiaro(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())
	{
		if(!(Is_Testo(TC[i])))
			TC.erase(i,1);
		else i++;
	}

	T_Chiaro = TC;

	return;
}

void RulloJefferson::Ins_T_Criptato(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())
	{
		if(!(Is_Testo(TC[i])))
			TC.erase(i,1);
		else i++;
	}

	T_Criptato = TC;

	return;
}

void RulloJefferson::Leggi_Rullo(char R[], int & DIM_Rulli, int & N_Rulli)
{
	for(int i=0; i<NUM_RULLI_Jefferson; i++)
		for(int j=0; j<DIM_ALF_Jefferson; j++)
			R[j + i*j] = Rullo[i][j];

	DIM_Rulli = DIM_ALF_Jefferson;
	N_Rulli = NUM_RULLI_Jefferson;

	return;
}

string RulloJefferson::Leggi_Cifra()
{
	Cifratura_Stp1();

	return T_Cifrato;
}

string RulloJefferson::Leggi_Decifra()
{
		Decifratura_Stp1();

		return T_Decriptato;
}

void RulloJefferson::Iniz_Alfabeto()
{
	for(int i = 0; i<DIM_ALF_Jefferson ; i++)
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

void RulloJefferson::Carica_Rullo()
{
    fstream Carica;
    Carica.open("Rullo_Jefferson.txt", ios::in);

    if(Carica.is_open())
        for(int i=0; i<NUM_RULLI_Jefferson; i++)
            for(int j=0; j<DIM_ALF_Jefferson; j++)
                Carica >> Rullo[i][j];

    else Reimposta_Rullo_STD(); //Se non riesco ad aprire il file .txt

    return;
}

void RulloJefferson::Reimposta_Rullo_STD() //Annulla ogni modifica fatta sul rullo
{
	for(int i=0; i<NUM_RULLI_Jefferson; i++)
		for(int j=0; j<DIM_ALF_Jefferson; j++)
			if(i+j < DIM_ALF_Jefferson) Rullo[i][j] = Alfabeto[i+j].Lettera;
			else Rullo[i][j] = Alfabeto[i+j - DIM_ALF_Jefferson].Lettera;

    Salva_Rullo();

	return;
}

void RulloJefferson::Mod_Rullo(char C[], int N_Rullo)
{
	//if(sizeof(C) != DIM_ALF_Jefferson) return;
	//if(N_Rullo > NUM_RULLI_Jefferson || N_Rullo < 1) return;
	//if(!Check_Validita_Rullo(C)) return;

	N_Rullo--; //Numero rullo adesso diventa indice

	for(int i=0; i<DIM_ALF_Jefferson; i++)
		Rullo[N_Rullo][i] = C[i];

	//Salvo il rullo
    Salva_Rullo();

	return;
}

bool RulloJefferson::Check_Validita_Rullo(char C[])
{
	for(int i=0; i <DIM_ALF_Jefferson; i++)
		for(int j=0; j<DIM_ALF_Jefferson; j++)
		{
			if(C[j] == Alfabeto[i].Lettera)
				break;
			if(j == DIM_ALF_Jefferson - 1) return false;
		}
	return true;
}

bool RulloJefferson::Is_Testo(char x)
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

int RulloJefferson::Posizione_In_Matrice_Riga_Fissa(char X, char Mat[][DIM_ALF_Jefferson], int N_Rullo)
{
	for(int i=0; i<DIM_ALF_Jefferson; i++)
		if(Mat[N_Rullo][i] == X) return i;

	return -1;  //Errore, carattere non trovato
}

void RulloJefferson::Cifratura_Stp1() //Controllo se il testo è più corto di 25, in quel caso aggiungo X fino a 25, in caso contrario divido il testo di 25 in 25
{
	int Numero_Di_Cicli;
	string Cifratura_Multipla, App;

	while(!(T_Chiaro.length() % NUM_RULLI_Jefferson == 0))
		T_Chiaro += Rullo[0][rand() % DIM_ALF_Jefferson];
		//T_Chiaro += 'X';


	if(T_Chiaro.length() <= NUM_RULLI_Jefferson) //Testo più corto di 25 caratteri
		Cifratura_Stp2();

	else //Testo più lungo di 25 caratteri
	{
		if (T_Chiaro.length() % NUM_RULLI_Jefferson == 0) Numero_Di_Cicli = T_Chiaro.length() / NUM_RULLI_Jefferson;
		else Numero_Di_Cicli = T_Chiaro.length() / NUM_RULLI_Jefferson + 1;
		App=T_Chiaro;
		for(int i=0; i<Numero_Di_Cicli; i++)
		{
			T_Chiaro.clear();
			for(int j=0; j<NUM_RULLI_Jefferson; j++)
				T_Chiaro += App[j + NUM_RULLI_Jefferson * i];

			Cifratura_Stp2();
			Cifratura_Multipla += T_Cifrato;
		}
		T_Cifrato = Cifratura_Multipla;
	}

	return;
}

void RulloJefferson::Cifratura_Stp2() //Ruoto i rotori per formare la parola con il testo in chiaro, e mi segno sul verrore Posizioni di quanti step ho rotato i singoli rotori
{
	int Posizioni[NUM_RULLI_Jefferson]; //Contiene di quante posizioni ruoto il rullo per formare il testo per ogni lettera

	for(int i=0; i<T_Chiaro.length(); i++)
		Posizioni[i] = Posizione_In_Matrice_Riga_Fissa(T_Chiaro[i], Rullo, i);

	Cifratura_Stp3(Posizioni);

	return;
}

void RulloJefferson::Cifratura_Stp3(int V[])
{
	int Pos;
	T_Cifrato.clear();
	for(int i=0; i<NUM_RULLI_Jefferson; i++)
	{
		Pos = V[i] + Key;
		if(Pos >= DIM_ALF_Jefferson) Pos -= DIM_ALF_Jefferson;
		T_Cifrato += Rullo[i][Pos];
	}

	return;
}

void RulloJefferson::Decifratura_Stp1() //Controllo se il testo è più corto di 25, in quel caso aggiungo X fino a 25, in caso contrario divido il testo di 25 in 25
{
	int Numero_Di_Cicli;
	string Decifratura_Multipla, App;
	string Vett_APP[DIM_ALF_Jefferson];

	while(!(T_Criptato.length() % NUM_RULLI_Jefferson == 0)) //Anche se in teoria in testo criptato dovrebbe essere sempre divisibile per 25 perchè ha passato la cifratura, meglio aggiungere ulteriore controllo
		T_Criptato += 'X';

	if(T_Criptato.length() <= NUM_RULLI_Jefferson) //Testo più corto di 25 caratteri
	{
		Decifratura_Stp2();

		//Il risultato è salvato in Rullo_Decriptazione. Per trasformarlo in stringa inserisco ogni riga della matrice separata da uno /n
		for(int j=0; j<DIM_ALF_Jefferson; j++)
		{
			for(int i=0; i<NUM_RULLI_Jefferson; i++)
				T_Decriptato+=Rullo_Decriptazione[i][j];
			//T_Decriptato += '\n';
		}
	}
	else //Testo più lungo di 25 caratteri
	{
		if (T_Criptato.length() % NUM_RULLI_Jefferson == 0) Numero_Di_Cicli = T_Criptato.length() / NUM_RULLI_Jefferson;
		else Numero_Di_Cicli = T_Criptato.length() / NUM_RULLI_Jefferson + 1;
		App=T_Criptato;
		for(int i=0; i<Numero_Di_Cicli; i++)
		{
			T_Criptato.clear();
			for(int j=0; j<NUM_RULLI_Jefferson; j++)
				T_Criptato += App[j + NUM_RULLI_Jefferson * i];

			Decifratura_Stp2();

			//Inserisco quello che ho appena cifrato in un vettore di appoggio e passo al prossimo ciclo
			for(int k=0; k<DIM_ALF_Jefferson; k++)
			{
				for(int w=0; w<NUM_RULLI_Jefferson; w++)
					Vett_APP[k] += Rullo_Decriptazione[w][k];
			}
		}

		//Prendo il vettore di appoggio che ho usato in maniera graduale per tutti i cicli e ricompongo il testo decriptato
		for(int y=0; y<DIM_ALF_Jefferson; y++)
		{
			for(int z=0; z<Vett_APP[y].length(); z++)
				T_Decriptato += Vett_APP[y].at(z);
			//T_Decriptato += '\n';
		}
	}

	return;
}

void RulloJefferson::Decifratura_Stp2() //Ruoto i rotori per formare la parola con il testo in chiaro, e mi segno sul verrore Posizioni di quanti step ho rotato i singoli rotori
{
	int Posizioni[NUM_RULLI_Jefferson]; //Contiene di quante posizioni ruoto il rullo per formare il testo per ogni lettera

	for(int i=0; i<T_Criptato.length(); i++)
		Posizioni[i] = Posizione_In_Matrice_Riga_Fissa(T_Criptato[i], Rullo, i);

	//DEBUG OK FINO A QUI
	Decifratura_Stp3(Posizioni);

	return;
}

void RulloJefferson::Decifratura_Stp3(int v[])
{
	int Pos;

	for(int i=0; i<NUM_RULLI_Jefferson; i++) //Per ogni elemento nel vettore
		for(int j=0; j<DIM_ALF_Jefferson; j++)
		{
			Pos = v[i] + j;
			if(Pos >= DIM_ALF_Jefferson) Pos -= DIM_ALF_Jefferson;
			Rullo_Decriptazione[i][j] = Rullo[i][Pos];
		}

	//OK FINO A QUI

	return;
}

void RulloJefferson::Salva_Rullo()
{
    fstream Salva;
    Salva.open("Rullo_Jefferson.txt", ios::out);

    for(int i=0; i<NUM_RULLI_Jefferson; i++)
    {
        for(int j=0; j<DIM_ALF_Jefferson; j++)
            Salva<<Rullo[i][j];
        Salva<<'\n';
    }

    Salva.close();

    return;
}
