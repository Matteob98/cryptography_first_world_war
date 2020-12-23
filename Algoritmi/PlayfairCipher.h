/*
---------------------------
Codice dell'algoritmo del Playfair Cipher
---------------------------
*/

#include<iostream>
#include<string>
#include <algorithm> //Per tolower, toupper

using namespace std;

//Costanti globali
const int DIM_Playfair = 5;
const int DIM_ALF_Playfair = 25;
const int DIM_M_Playfair = 500; //Dimensione di sicurezza per cifratura_step1

class Playfair
{
	public:
		Playfair();
		void Ins_Key(string K);
		void Ins_T_Chiaro(string TC);
		void Ins_T_Criptato(string TC);

		string Leggi_Cifra();
		string Leggi_Decifra();
		string Leggi_Chiave();


	private:
		struct Alf_Flag//Per controllare se una lettera è già stata inserita nella matrice
		{
			char Lettera;
			bool Flag;
		};

		char Matrice[DIM_Playfair][DIM_Playfair]; //Matrice 5x5
		Alf_Flag Alfabeto[DIM_ALF_Playfair]; //Per controllare se una lettera è già stata inserita nella matrice

		string T_Chiaro; //TUTTO MAIUSCOLO
		string T_Cifrato; //TUTTO MAIUSCOLO -----	Per Cifratura
		string T_Criptato; //Per decifratura
		string T_Decriptato;
		string Key; //TUTTO MAIUSCOLO

		void Iniz_Matrice();
		void Iniz_Alfabeto();
		bool Is_Alfabeto(char x);
		void Posizione_In_Matrice(char x, char M[][DIM_Playfair], int DIM_Y, int & Pos_X, int & Pos_Y);

		void Cifratura_Stp1(); //Divido in gruppi di due il testo in chiaro
		void Cifratura_Stp2(char Mat[][2], int D);

		void Decifratura_Stp1();
		void Decifratura_Stp2(char Mat[][2], int D);

		void Cfr_Colonna(int X, int Y1,int Y2, char & R1, char & R2); //Se sulla stessa colonna prendo la lettera subito sotto ogni lettera
		void Cfr_Riga(int Y, int X1, int X2, char & R1, char & R2); //Se sulla stessa riga prendo la letttera subito a destra di ogni riga
		void Cfr_Rettangolo(int X1, int X2, int Y1, int Y2, char & R1, char & R2); //Prendo le 2 lettere che fanno da rettangolo, partendo dalla riga della prima lettera (colonna della seconda), poi dalla riga della seconda (colonna della prima)

		void Dcfr_Colonna(int X, int Y1,int Y2, char & R1, char & R2); //Se sulla stessa colonna prendo la lettera subito sopra ogni lettera
		void Dcfr_Riga(int Y, int X1, int X2, char & R1, char & R2); //Se sulla stessa riga prendo la letttera subito a destra di ogni riga
		void Dcfr_Rettangolo(int X1, int X2, int Y1, int Y2, char & R1, char & R2);  //Prendo le 2 lettere che fanno da rettangolo, partendo dalla riga della prima lettera (colonna della seconda), poi dalla riga della seconda (colonna della prima)
};

Playfair::Playfair()
{
	T_Chiaro = "DEFAULT";
	Key = "PROVA";
	T_Criptato = "XXX";

	Iniz_Alfabeto();
}

void Playfair::Ins_Key(string K)
{
	transform( K.begin(),K.end(), K.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < K.length())
	{
		if(K[i] == 'W') K[i] = 'V';
		if(!(Is_Alfabeto(K[i])))
			K.erase(i,1);
		else i++;
	}

	Key=K;

	return;
}

void Playfair::Ins_T_Chiaro(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())
	{
		if(TC[i] == 'W') TC[i] = 'V';
		if(!(Is_Alfabeto(TC[i])))
			TC.erase(i,1);
		else i++;
	}

	T_Chiaro = TC;

	return;
}

void Playfair::Ins_T_Criptato(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())  //rimuovo spazi bianchi
	{
		if(TC[i] == 'W') TC[i] = 'V';
		if(!(Is_Alfabeto(TC[i])))
			TC.erase(i,1);

		else i++;

	}
	if(TC.length() % 2 == 1) TC.erase(TC.length()-1,1); //Se il testo non è divisibile per 2

	T_Criptato=TC;

	return;
}

string Playfair::Leggi_Cifra()
{
	Iniz_Matrice();
	Cifratura_Stp1();

	string App;
	while(T_Cifrato.length() % 5 != 0)
        T_Cifrato += 'P'; //Standard
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

string Playfair::Leggi_Decifra()
{
	Iniz_Matrice();
	Decifratura_Stp1();
	return T_Decriptato;
}

string Playfair::Leggi_Chiave()
{
    return Key;
}

void Playfair::Iniz_Matrice()
{
	//Inizializzazione a vuoto della matrice
	for(int j=0; j<DIM_Playfair; j++)
	for(int i=0; i<DIM_Playfair; i++)
		Matrice[i][j]= '!';

	int Lungh = Key.length();

	int i,j, Cont_i=0, Cont_j=0;
	char L;
	bool Check;

	for(i = 0; i<Lungh; i++)
	{
		L=Key.at(i);

		for(j=0; j<DIM_ALF_Playfair; j++)
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
			if(Cont_i == DIM_Playfair)
			{
				Cont_i=0;
				Cont_j++;
			}
		}
	}

	for(j= 0; j<DIM_Playfair; j++)
		for(i=0; i<DIM_Playfair; i++)
		{
			if(Matrice[i][j] == '!')
			{

				for(int C=0; C<DIM_ALF_Playfair; C++)
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

void Playfair::Iniz_Alfabeto()
{
	for(int i = 0; i<DIM_ALF_Playfair ; i++)
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
	Alfabeto[21].Lettera = 'V'; //V e W cifrate con lo stesso simbolo
	Alfabeto[22].Lettera = 'X';
	Alfabeto[23].Lettera = 'Y';
	Alfabeto[24].Lettera = 'Z';

	return;
}

bool Playfair::Is_Alfabeto(char x)
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
	if(x == 'X') return true;
	if(x == 'Y') return true;
	if(x == 'Z') return true;

	return false;
}

void Playfair::Posizione_In_Matrice(char x, char M[][DIM_Playfair], int DIM_Y, int & Pos_X, int & Pos_Y)
{
	for(int i=0; i<DIM_Playfair; i++)
		for(int j=0; j<DIM_Y; j++)
			if(M[i][j] == x)
			{
				Pos_X = i;
				Pos_Y = j;
				break;
			}

	return;
}

void Playfair::Cifratura_Stp1()
{
	char Mat_T_Chiaro[DIM_M_Playfair][2];
	int index = 0;

	if(T_Chiaro.length() % 2 == 1) T_Chiaro += 'X';

	for(int i=0; i<T_Chiaro.length(); i++)
	{
		Mat_T_Chiaro[index][0] = T_Chiaro[i];
		i++;
		if(T_Chiaro[i] != T_Chiaro[i-1])
			Mat_T_Chiaro[index][1] = T_Chiaro[i];
		else Mat_T_Chiaro[index][1] = 'X'; //Se ci sono due lettere consecutive si sostituisce con una lettera rara per eliminare appiglio a crittoanalisti
		index ++;
	}

	//DEBUG OK FINO QUI

	Cifratura_Stp2(Mat_T_Chiaro, index);
}

void Playfair::Cifratura_Stp2(char Mat[][2], int D)
{
	int X1,Y1,X2,Y2;

	char R1,R2; //I risultati della prima cifratura (prima lettera) e della seconda

	for(int i=0; i<D; i++)
	{
		Posizione_In_Matrice(Mat[i][0], Matrice, DIM_Playfair, X1, Y1);
		Posizione_In_Matrice(Mat[i][1], Matrice, DIM_Playfair, X2, Y2);

		if(X1 == X2) //Se si trovano sulla stessa colonna
			Cfr_Colonna(X1, Y1, Y2, R1, R2);
		else if (Y1 == Y2) //Se si trovano sulla stessa riga
			Cfr_Riga(Y1, X1, X2, R1, R2);
		else //Se si trovano in diagonale, cifro a rettangolo
			Cfr_Rettangolo(X1,X2,Y1,Y2,R1,R2);

		T_Cifrato += R1;
		T_Cifrato += R2;
	}

	return;
}

void Playfair::Cfr_Colonna(int X, int Y1,int Y2, char & R1, char & R2) //Se sulla stessa colonna prendo la lettera subito sotto ogni lettera
{
	Y1++;
	Y2++;

	//controllare in fase di debug se devono essere invertiti X e Y in assegnazione
	if(Y1 < DIM_Playfair) R1 = Matrice[X][Y1];
	else R1 = Matrice[X][0];

	if(Y2 < DIM_Playfair) R2 = Matrice[X][Y2];
	else R2 = Matrice[X][0];

	return;
}

void Playfair::Cfr_Riga(int Y, int X1, int X2, char & R1, char & R2) //Se sulla stessa riga prendo la letttera subito a destra di ogni riga
{
	X1++;
	X2++;

	if(X1 < DIM_Playfair) R1 = Matrice[X1][Y];
	else R1 = Matrice[0][Y];

	if(X2 <DIM_Playfair) R2 = Matrice[X2][Y];
	else R2 = Matrice[0][Y];

	return;
}

void Playfair::Cfr_Rettangolo(int X1, int X2, int Y1, int Y2, char & R1, char & R2)
{
	R1 = Matrice[X2][Y1];
	R2 = Matrice[X1][Y2];

	return;
}

void Playfair::Decifratura_Stp1()
{
	char Mat_T_Criptato[DIM_M_Playfair][2];
	int index = 0;

	if(T_Criptato.length() % 2 == 1) T_Criptato += 'X'; //Se ha passato la cifratura dovrebbe già essere pari ma si preferisce inserire un controllo aggiuntivo

	for(int i=0; i<T_Criptato.length(); i++)
	{
		Mat_T_Criptato[index][0] = T_Criptato[i];
		i++;
			Mat_T_Criptato[index][1] = T_Criptato[i];
		index ++;
	}

	Decifratura_Stp2(Mat_T_Criptato, index);
	return;
}

void Playfair::Decifratura_Stp2(char Mat[][2], int D)
{
	int X1,Y1,X2,Y2;

	char R1,R2; //I risultati della prima decifratura (prima lettera) e della seconda

	for(int i=0; i<D; i++)
	{
		Posizione_In_Matrice(Mat[i][0], Matrice, DIM_Playfair, X1, Y1);
		Posizione_In_Matrice(Mat[i][1], Matrice, DIM_Playfair, X2, Y2);

		if(X1 == X2) //Se si trovano sulla stessa colonna
			Dcfr_Colonna(X1, Y1, Y2, R1, R2);
		else if (Y1 == Y2) //Se si trovano sulla stessa riga
			Dcfr_Riga(Y1, X1, X2, R1, R2);
		else //Se si trovano in diagonale, decifro a rettangolo
			Dcfr_Rettangolo(X1,X2,Y1,Y2,R1,R2);

		T_Decriptato += R1;
		T_Decriptato += R2;
	}

	return;
}

void Playfair::Dcfr_Colonna(int X, int Y1,int Y2, char & R1, char & R2) //Se sulla stessa colonna prendo la lettera subito sopra ogni lettera
{
	Y1--;
	Y2--;

	if(Y1 >= 0) R1 = Matrice[X][Y1];
	else R1 = Matrice[X][DIM_Playfair -1]; //Se DIM_Playfair = 5 l'ultima posizione della matrice è 4

	if(Y2 >= 0) R2 = Matrice[X][Y2];
	else R2 = Matrice[X][DIM_Playfair - 1];

	return;
}

void Playfair::Dcfr_Riga(int Y, int X1, int X2, char & R1, char & R2) //Se sulla stessa riga prendo la letttera subito a destra di ogni riga
{
	X1--;
	X2--;

	if(X1 >= 0) R1 = Matrice[X1][Y];
	else R1 = Matrice[DIM_Playfair-1][Y];

	if(X2 >= 0) R2 = Matrice[X2][Y];
	else R2 = Matrice[DIM_Playfair-1][Y];

	return;
}

void Playfair::Dcfr_Rettangolo(int X1, int X2, int Y1, int Y2, char & R1, char & R2)  //Prendo le 2 lettere che fanno da rettangolo, partendo dalla riga della prima lettera (colonna della seconda), poi dalla riga della seconda (colonna della prima)
{
	R1 = Matrice[X2][Y1];
	R2 = Matrice[X1][Y2];

	return;
}
