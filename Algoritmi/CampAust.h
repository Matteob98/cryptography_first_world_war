/*
---------------------------
Codice dell'algoritmo del cifrario campale austriaco
---------------------------
*/
#include<iostream>
#include<string>
#include <stdlib.h>
#include <algorithm> //Per tolower, toupper

using namespace std;

int const DIM_CAMPAUST = 26;

class CampAust //Campale Austriaca
{
	public:
		CampAust();

		void Ins_T_Chiaro(string TC);
		void Ins_T_Criptato(string TC);

		string Leggi_Cifra();
		string Leggi_Decifra();

	private:
		struct Cifrario
		{
			char Lettera;
			string Cod;
		};

		void Iniz_Alfabeto();
		void Iniz_Codici();
		void Cifratura();
		void Decifratura();
		char Intero_To_Carattere_CAMPAUST(int x); //C::B non riconosce itoa ma riconosce atoi
		bool Is_Testo_Chiaro(char x);
		bool Is_Testo_Criptato(string x);

		Cifrario Matrice[DIM_CAMPAUST];

		string T_Chiaro; //TUTTO MAIUSCOLO
		string T_Cifrato; //Per Cifratura
		string T_Criptato; //Per decifratura
		string T_Decriptato;

};

CampAust::CampAust()
{
	T_Chiaro="PROVA";
	T_Criptato="747492729028073"; //PROVA

	Iniz_Alfabeto();
	Iniz_Codici();
}

void CampAust::Ins_T_Chiaro(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())
	{
		if(!(Is_Testo_Chiaro(TC[i])))
			TC.erase(i,1);
		else i++;
	}

	T_Chiaro = TC;

	return;
}

void CampAust::Ins_T_Criptato(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );
    int i=0;
	string App;

	while(i< TC.length()) //Elimino gli spazi bianchi
	{
	    if(TC[i] == ' ')
            TC.erase(i,1);

        else i++;
	}

	while(i < TC.length())
	{
		//IL campaust viene cifrato con gruppi di 3 cifre!!
		App += TC[i];
		i++;
		App += TC[i];
		i++;
		App += TC[i];
		i++;
		if(!(Is_Testo_Criptato(App))) //IL campaust viene cifrato con gruppi di 3 cifre!!
		{
			i-=3;
			TC.erase(i,1);
			TC.erase(i,1);
			TC.erase(i,1);
		}
		App.clear();
	}
	T_Criptato=TC;

	return;
}

string CampAust::Leggi_Cifra()
{
	Cifratura();
	return T_Cifrato;
}

string CampAust::Leggi_Decifra()
{
	Decifratura();
	return T_Decriptato;
}

void CampAust::Iniz_Alfabeto()
{
	Matrice[0].Lettera = 'A';
	Matrice[1].Lettera = 'B';
	Matrice[2].Lettera = 'C';
	Matrice[3].Lettera = 'D';
	Matrice[4].Lettera = 'E';
	Matrice[5].Lettera = 'F';
	Matrice[6].Lettera = 'G';
	Matrice[7].Lettera = 'H';
	Matrice[8].Lettera = 'I';
	Matrice[9].Lettera = 'J';
	Matrice[10].Lettera = 'K';
	Matrice[11].Lettera = 'L';
	Matrice[12].Lettera = 'M';
	Matrice[13].Lettera = 'N';
	Matrice[14].Lettera = 'O';
	Matrice[15].Lettera = 'P';
	Matrice[16].Lettera = 'Q';
	Matrice[17].Lettera = 'R';
	Matrice[18].Lettera = 'S';
	Matrice[19].Lettera = 'T';
	Matrice[20].Lettera = 'U';
	Matrice[21].Lettera = 'V';
	Matrice[22].Lettera = 'W';
	Matrice[23].Lettera = 'X';
	Matrice[24].Lettera = 'Y';
	Matrice[25].Lettera = 'Z';

	return;
}

void CampAust::Iniz_Codici()
{
	Matrice[0].Cod = "073";
	Matrice[1].Cod = "349";
	Matrice[2].Cod = "177";
	Matrice[3].Cod = "065";
	Matrice[4].Cod = "629";
	Matrice[5].Cod = "164";
	Matrice[6].Cod = "489";
	Matrice[7].Cod = "504";
	Matrice[8].Cod = "834";
	Matrice[9].Cod = "250";
	Matrice[10].Cod = "907";
	Matrice[11].Cod = "108";
	Matrice[12].Cod = "652";
	Matrice[13].Cod = "264";
	Matrice[14].Cod = "729";
	Matrice[15].Cod = "747";
	Matrice[16].Cod = "361";
	Matrice[17].Cod = "492";
	Matrice[18].Cod = "589";
	Matrice[19].Cod = "255";
	Matrice[20].Cod = "197";
	Matrice[21].Cod = "028";
	Matrice[22].Cod = "557";
	Matrice[23].Cod = "639";
	Matrice[24].Cod = "741";
	Matrice[25].Cod = "941";

	return;
}

void CampAust::Cifratura()
{
	for(int i=0; i<T_Chiaro.length(); i++)
		for(int j=0; j<DIM_CAMPAUST; j++)
			if(T_Chiaro[i] == Matrice[j].Lettera)
				T_Cifrato = T_Cifrato + Matrice[j].Cod;


	int N;
	char C;
	while (!(T_Cifrato.length() % 5 == 0)) //Considerato che il testo va inviato cifrato a gruppi di 5, dobbiamo aggiungere caratteri casuali per farlo arrivare ad essere divisibile per 5
	{
		N = rand() % 10;
		C = Intero_To_Carattere_CAMPAUST(N);
		T_Cifrato += C; //Numero randomico da 0 e 9
	}

	return;
}

void CampAust::Decifratura()
{
	while(!(T_Criptato.length() % 3 == 0)) //in cifratura sono aggiunti caratteri per renderlo divisibile per 5, adesso si tolgono per farlo tornare divisibile per 3
		T_Criptato.erase(T_Criptato.length() -1); //Elimina ultimo carattere finchè il testo non torna divisibile per 3

	string APP;

	for(int i=0; i<T_Criptato.length(); i+=3)
	{
		APP = T_Criptato[i];
		APP = APP + T_Criptato[i+1];
		APP = APP + T_Criptato[i+2];
		for(int j=0; j<DIM_CAMPAUST; j++)
			if(APP == Matrice[j].Cod)
				T_Decriptato = T_Decriptato + Matrice[j].Lettera;
	}

	return;
}

char CampAust::Intero_To_Carattere_CAMPAUST(int x) //C::B non riconosce itoa ma riconosce atoi
{
    switch(x)
    {
    case 0:
		return '0';
    case 1:
        return '1';
    case 2:
        return '2';
    case 3:
        return '3';
    case 4:
        return '4';
    case 5:
        return '5';
    case 6:
        return '6';
    case 7:
        return '7';
    case 8:
        return '8';
    case 9:
        return '9';
	}

	return -1;
}

bool CampAust::Is_Testo_Chiaro(char x)
{
	for(int i=0; i<DIM_CAMPAUST; i++)
		if(x == Matrice[i].Lettera) return true;

	return false;
}

bool CampAust::Is_Testo_Criptato(string x)
{
	for(int i=0; i<DIM_CAMPAUST; i++)
		if(x == Matrice[i].Cod) return true;

	return false;
}
