/*
---------------------------
Codice dell'algoritmo del cifrario bifido di dalastelle
---------------------------
*/

#include<iostream>
#include<string>
#include <algorithm> //Per tolower, toupper
#include <stdlib.h>

using namespace std;

int const DIM = 5;
int const DIM_ALF = 25;
int const DIM_M = 800; //Dimensione di sicurezza per cifratura_step2

class CBD
{
	public:
		CBD();
		void Ins_Key(string K); //Inserimento chiave da fuori
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

		Alf_Flag Alfabeto[DIM_ALF]; //Per controllare se una lettera è già stata inserita nella matrice
		char Matrice[DIM][DIM]; //Matrice 5x5
		string Key; //TUTTO MAIUSCOLO
		string T_Chiaro; //TUTTO MAIUSCOLO
		string T_Cifrato; //Per Cifratura
		string T_Criptato; //Per decifratura
		string T_Decriptato;

		void Cifra();
		void Decifra();
		void Iniz_Matrice();
		void Iniz_Alfabeto();
		void Posizione_In_Matrice(char x, char M[][DIM], int DIM_Y, int & Pos_X, int & Pos_Y);
		bool Is_Testo(char x);
		char Intero_To_Carattere_CBD(int x); //C::B non riconosce itoa ma riconosce atoi

		void Cifratura_Stp1();
		void Cifratura_Stp2(string X, string Y);
		void Cifratura_Stp3(char Mat[][2], int Mat_Dim);

		void Decifratura_Stp1(); //Da lettera a coordinate in matrice
		void Decifratura_Stp2(int Coordinate[], int D);
		void Decifratura_Stp3(int Coo_X[], int Coo_Y[], int D);
};

CBD::CBD()
{
	T_Chiaro = "DEFAULT";
	Key = "PROVA";
	T_Criptato="XXX";

	Iniz_Alfabeto();
}

void CBD::Cifra()
{
	Iniz_Matrice();

	bool Verifica=false; //Controlla se la chiave è valida
	for(int i=0; i<Key.length(); i++)
		if(Is_Testo(T_Chiaro[i]))
		{
			Verifica=true;
			break;
		}

	if(!Verifica)
	{
		cout<<endl <<"ATTENZIONE! Errore nella chiave" <<endl;
		return;
	}
	else Verifica=false;

	for(int i=0; i<T_Chiaro.length(); i++) //Controlla se il testo in chiaro è valido
		if(Is_Testo(T_Chiaro[i]))
		{
			Verifica=true;
			break;
		}

	if(!Verifica)
	{
		cout<<endl <<"ATTENZIONE! Errore nel testo in chiaro" <<endl;
		return;
	}
	else Verifica=false;


	Cifratura_Stp1();

	return;
}

void CBD::Decifra()
{
	Iniz_Matrice();
	bool Verifica=false;
	for(int i=0; i<Key.length(); i++) //Controlla se la chiave è valida
		if(Is_Testo(T_Chiaro[i]))
		{
			Verifica=true;
			break;
		}

	if(!Verifica)
	{
		cout<<endl <<"ATTENZIONE! Errore nella chiave" <<endl;
		return;
	}
	else Verifica=false;

	for(int i=0; i<T_Criptato.length(); i++) //Controlla se il testo criptato è valido
		if(Is_Testo(T_Criptato[i]))
		{
			Verifica=true;
			break;
		}

	if(!Verifica)
	{
		cout<<endl <<"ATTENZIONE! Errore nel testo criptato" <<endl;
		return;
	}
	else Verifica=false;

	Decifratura_Stp1();

	return;
}

string CBD::Leggi_Chiave()
{
    return Key;
}

void CBD::Ins_Key(string K)
{
	transform( K.begin(),K.end(), K.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < K.length())
	{
		if(K[i] == 'W') K[i] = 'V';
		if(!(Is_Testo(K[i])))
			K.erase(i,1);

		else i++;
	}

	Key=K;

	return;
}

void CBD::Ins_T_Chiaro(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())
	{
		if(TC[i] == 'W') TC[i] = 'V';
		if(!(Is_Testo(TC[i])))
			TC.erase(i,1);
		else i++;
	}

	T_Chiaro = TC;

	return;
}

void CBD::Ins_T_Criptato(string TC)
{
	transform( TC.begin(),TC.end(), TC.begin(),(int(*)(int)) toupper );

	int i=0;
	while(i < TC.length())  //rimuovo spazi bianchi
	{
		if(TC[i] == 'W') TC[i] = 'V';
		if(!(Is_Testo(TC[i])))
			TC.erase(i,1);

		else i++;

	}
	T_Criptato=TC;

	return;
}

string CBD::Leggi_Cifra()
{
	Cifra();

	string App;

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

string CBD::Leggi_Decifra()
{
	Decifra();
	return T_Decriptato;
}

bool CBD::Is_Testo(char x)
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

void CBD::Iniz_Matrice()
{
	//Inizializzazione a vuoto della matrice
	for(int j=0; j<DIM; j++)
	for(int i=0; i<DIM; i++)
		Matrice[i][j]= '!';

	int Lungh = Key.length();

	int i,j, Cont_i=0, Cont_j=0;
	char L;
	bool Check;

	for(i = 0; i<Lungh; i++)
	{
		L=Key.at(i);

		for(j=0; j<DIM_ALF; j++)
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
			if(Cont_i == DIM)
			{
				Cont_i=0;
				Cont_j++;
			}
		}
	}

	for(j= 0; j<DIM; j++)
		for(i=0; i<DIM; i++)
		{
			if(Matrice[i][j] == '!')
			{

				for(int C=0; C<DIM_ALF; C++)
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

void CBD::Iniz_Alfabeto()
{
	for(int i = 0; i<DIM_ALF ; i++)
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
	Alfabeto[21].Lettera = 'V';//V e W cifrate con lo stesso simbolo
	Alfabeto[22].Lettera = 'X';
	Alfabeto[23].Lettera = 'Y';
	Alfabeto[24].Lettera = 'Z';

	return;
}

void CBD::Posizione_In_Matrice(char x, char M[][DIM], int DIM_Y, int & Pos_X, int & Pos_Y)
{
	for(int i=0; i<DIM; i++)
		for(int j=0; j<DIM_Y; j++)
			if(M[i][j] == x)
			{
				Pos_X = i;
				Pos_Y = j;
				break;
			}

	return;
}

void CBD::Cifratura_Stp1() //Il testo in chiaro viene cifrato per righe e colonne. ES. A(1,1) viene inizialmente cifrato come 1,1
{
	int x=0, y=0;
	char Car_x, Car_y;
	string Cifratura_X, Cifratura_Y;

	for(int i=0; i<T_Chiaro.length(); i++)
	{
		Posizione_In_Matrice(T_Chiaro[i], Matrice, DIM, x, y);
		x++;
		y++;

        Car_x = Intero_To_Carattere_CBD(x);
        Car_y = Intero_To_Carattere_CBD(y);
		//itoa(x, Car_x, 10);
		//itoa(y, Car_y, 10);

		Cifratura_X += Car_y; //Cifra al contrario -> Prima colonne, poi righe
		Cifratura_Y += Car_x;
	}

	//Il testo va diviso in 5, se ci sono lettere eccedenti vanno aggiunte tante X. In questo modo controllo quante X vanno aggiunte
	if(T_Chiaro.length() % 5 != 0)
	{
	    int Resto =5 - (T_Chiaro.length() % 5);
        for(int i=0; i<Resto; i++)
        {
            Posizione_In_Matrice('X', Matrice, DIM, x, y);
            x++;
            y++;

            Car_x = Intero_To_Carattere_CBD(y);//Cifra al contrario -> prima colonna, poi riga
            Car_y = Intero_To_Carattere_CBD(x);

            //itoa(x, Car_y, 10); //Cifra al contrario -> prima colonna, poi riga
            //itoa(y, Car_x, 10);

            Cifratura_X += Car_x;
            Cifratura_Y += Car_y;
        }
	}

	Cifratura_Stp2(Cifratura_X, Cifratura_Y);

	return;
}

void CBD::Cifratura_Stp2(string X, string Y) //Le coordinate vengono divise. Da una parte le righe, dall'altra le colonne
{
	string Z = X+Y;
	char Mat[DIM_M][2];
	int i=0,C=0;

	for(i=0; i<Z.length(); i+=2)
	{
		Mat[C][0] = Z[i];
		Mat[C][1] = Z[i+1];

		C++;
	}

	Cifratura_Stp3(Mat, C);

	return;
}

void CBD::Cifratura_Stp3(char Mat[][2], int Mat_Dim)
{
	char C;
	int X,Y;
	char car[2];

	for(int i=0; i<Mat_Dim; i++)
	{

		car[0] = Mat[i][0];
		X = atoi(car) -1; //Perché le matrici iniziano da 0,0

		car[0] = Mat[i][1];
		Y = atoi(car) -1; //Perché le matrici iniziano da 0.0

		C = Matrice[Y][X]; //Prima Y, poi X -> Cifra al contrario -> Prima colonna poi riga
		T_Cifrato += C;
	}

	return;
}

void CBD::Decifratura_Stp1()
{
	int X=0, Y=0, D=0;
	int Coordinate[DIM_M];
	for(int i=0; i<T_Criptato.length(); i++)
	{
		Posizione_In_Matrice(T_Criptato[i], Matrice, DIM, X, Y);
		Coordinate[D] = Y;
		D++;
		Coordinate[D] = X;
		D++;
	}

	Decifratura_Stp2(Coordinate, D);

	return;

}

void CBD::Decifratura_Stp2(int Coordinate[], int D)
{
	int D_Mezzo = D/2;
	int Coo_X[DIM_M], Coo_Y[DIM_M];

	for(int i=0; i<D_Mezzo; i++)
		Coo_X[i] = Coordinate[i];
	for(int i=D_Mezzo; i<D; i++)
		Coo_Y[i-D_Mezzo] = Coordinate[i];

	Decifratura_Stp3(Coo_Y, Coo_X, D_Mezzo); //LO PASSO AL CONTRAIO PERCHE' QUESTO CIFRARIO CIFRA AL CONTRARIO RIGHE E COLONNE

	return;
}

void CBD::Decifratura_Stp3(int Coo_X[], int Coo_Y[], int D) //Le variabili sono state passate al contrario. --Vedi passaggio variabili su stp2--
{
	char C;
	int X,Y;
	for(int i=0; i<D; i++)
	{
		X=Coo_X[i];
		Y=Coo_Y[i];
		C=Matrice[X][Y];
		T_Decriptato+=C;
	}

	return;
}

char CBD::Intero_To_Carattere_CBD(int x) //C::B non riconosce itoa ma riconosce atoi
{
    switch(x)
    {
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
    default:
        return 0;
    }
}
