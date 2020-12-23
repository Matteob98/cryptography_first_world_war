/*
---------------------------
Codice dell'algoritmo del cifrario per telegrafo ottico 1 2 3
---------------------------
*/

#include<iostream>
#include<fstream>
#include<string.h>
#include <algorithm> //Per tolower, toupper

using namespace std;

int const DIM_123 = 363;


class Cifr123
{
	public:
		Cifr123();
		string Push_Code(string Text); // Ritorna "Fail" in caso di codice non trovato. Inserire sempre questo controllo
		string Push_Word(string Code); // Ritorna "Fail" in caso di parola non trovata. Inserire sempre questo controllo
		int Push_Tavole(string V[]);

	private:
		void Importa();

		string Matrice[DIM_123][2]; //Prima colonna codice, seconda testo
		bool Is_Alfabeto(char x);
		bool Is_Number(char x);
};

Cifr123::Cifr123()
{
	Importa();
}

string Cifr123::Push_Code(string Text) // Ritorna "Fail" in caso di parola non trovata. Inserire sempre questo controllo
{
	transform(Text.begin(),Text.end(), Text.begin(),(int(*)(int)) toupper );

	string App;
	for(int i=0; i<Text.length(); i++)
        if(Is_Alfabeto(Text[i]))
            App += Text[i];
    Text.clear();
    Text = App;

	if(Text == "0") Text = 'O'; //0 e O cifrano con lo stesso simbolo
	for(int i=0; i<DIM_123; i++)
		if(Text == Matrice[i][1])
			return Matrice[i][0];

	return "Fail";
}

string Cifr123::Push_Word(string Code) // Ritorna "Fail" in caso di parola non trovata. Inserire sempre questo controllo
{
	string App;
	for(int i=0; i<Code.length(); i++)
        if(Is_Number(Code[i]))
            App += Code[i];
    Code.clear();
    Code = App;

	for(int i=0; i<DIM_123; i++)
		if(Code == Matrice[i][0])
			return Matrice[i][1];

	return "Fail";
}

int Cifr123::Push_Tavole(string V[])
{
	for(int i=0; i<DIM_123; i++)
		V[i] = Matrice[i][1];
	return DIM_123;
}

void Cifr123::Importa()
{
	string Matrice_App[DIM_123+1][2]; //Perche altrimenti salva sempre due volte l'ultima parola. così la salvo due volte e poi alla fine, tornando alla matrice iniziale evito di compiare l'ultima
	ifstream F;
	F.open("Tavola.txt", ios::in);
	if(!(F.is_open())) //Se non lo trovo in questa cartella lo cerco su tutto il computer
	{
	    F.close();
	    F.open("*\Tavola.txt", ios::in);
	}

	string Cod, Testo;
	int i=0;

	while(!F.eof())
	{
		F>>Cod;
		F>>Testo;

		Matrice_App[i][0] = Cod;
		Matrice_App[i][1] = Testo;
		i++;
	}
	F.close();

	for(int i=0; i<DIM_123; i++)
	{
		Matrice[i][0] = Matrice_App[i][0];
		Matrice[i][1] = Matrice_App[i][1];
	}

	return;
}

bool Cifr123::Is_Alfabeto(char x)
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

bool Cifr123::Is_Number(char x)
{
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
