/*
---------------------------
Codice dell'algoritmo del cifrario militare tascabile
---------------------------
*/

#include<iostream>
#include<string>
#include <algorithm> //Per tolower, toupper
#include <time.h> //Per random

using namespace std;

//Costanti globali
const int DIM_CMT = 36;
const int DIM2_CMT = 26;

class CMT
{
	public:
		CMT();

		void Ins_Key(string K); //Inserimento chiave da fuori
		void Ins_T_Chiaro(string TC);
		void Ins_T_Criptato(string TC);

		string Leggi_Cifra();
		string Leggi_Decifra();
		string Leggi_Chiave();

	private:

		//Matrici privatestring Legg_Decifra()
		char Sgn_Testo[DIM_CMT]; //ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890
		char Sgn_Chiave[DIM2_CMT]; //ABCDEFGHIJKLMNOPQRSTUVWXYZ
		int Mat_Cifrario[DIM_CMT][DIM2_CMT]; //Intero perchè altrimenti da errore su Iniz_Mat_Cifrario quando vai a fare j+10

		//Variabili private
		string Key; //INIZIALIZZARE NEL COSTRUTTORE
		string T_Chiaro; //INIZIALIZZARE NEL COSTRUTTORE
		string T_Cifrato;
		string T_Criptato; //INIZIALIZZARE NEL COSTRUTTORE ---	Per decifrare
		string T_Decriptato;

		//Funzioni private
		void Iniz_Sgn_Testo();
		void Iniz_Sgn_Chiave();
		void Iniz_Mat_Cifrario();
		void Iniz_Key_Cript(); //Viene inizializata in Cifratura() -----	Crea la chiave a partire dalla chiave inserita prima (ripete la chiave fino ad arrivare alla lunghezza del testo in chiaro)
		void Iniz_Key_Decript();
		bool Is_Testo(char x);
		bool Is_Chiave(char x);
		bool Mag_9_Min_46(char A, char B);
		string Intero_To_Carattere_CMT(int x); //C::B non riconosce itoa ma riconosce atoi
		int Posizione(char x, char M[], int D); //carattere da ricercare, matrice in cui ricercare, dimensione della matrice	-----	segnale di errore = -1
		void Cifra();
		void Decifra();
};

CMT::CMT()
{
	Key="ABC";
	T_Chiaro="ABC";
	T_Criptato="111111";

	Iniz_Sgn_Testo();
	Iniz_Sgn_Chiave();
	Iniz_Mat_Cifrario();

}

string CMT::Leggi_Cifra()
{
	Cifra();

	string App;

	while(T_Cifrato.length() % 5 != 0) //Completo l'ultima cinquina
    {
        T_Cifrato += '3';// Numero standard de facto
    }
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

string CMT::Leggi_Decifra()
{
	Decifra();
	return T_Decriptato;
}

string CMT::Leggi_Chiave()
{
    return Key;
}

void CMT::Cifra()
{
	Iniz_Key_Cript();
	T_Cifrato.clear();

	int A,B,i;
	string Car;

	for(i=0; i<T_Chiaro.length(); i++)
	{
		A = Posizione(T_Chiaro[i], Sgn_Testo, DIM_CMT);
		if(A== -1) cout<<endl <<"ERRORE A -> Carattere non trovato" <<endl;

		B = Posizione(Key[i], Sgn_Chiave, DIM2_CMT);
		if(B== -1) cout<<endl <<"ERRORE B -> Carattere non trovato" <<endl;

		Car = Intero_To_Carattere_CMT(Mat_Cifrario[A][B]);
		//itoa(Mat_Cifrario[A][B], Car, 10);

		T_Cifrato+= Car;
	}

	return;
}

void CMT::Decifra()
{
	Iniz_Key_Decript();
	T_Decriptato.clear();

	int POS_X, POS_Y, Txt;
	char c[2];

	for(int i=0; i<T_Criptato.length(); i+=2) //Cicla tutto il testo criptato
	{
		POS_Y = Posizione(Key[i/2], Sgn_Chiave, DIM2_CMT);

		c[0] = T_Criptato[i];
		c[1] = T_Criptato[i+1];

		Txt = atoi(c);

		for(int x=0; x<DIM_CMT; x++)
			if(Mat_Cifrario[x][POS_Y] == Txt)
				T_Decriptato+=Sgn_Testo[x];

	}

	return;
}

void CMT::Ins_Key(string K)
{
	transform( K.begin(),K.end(), K.begin(),(int(*)(int)) toupper );
	int i=0;
	while(i < K.length())
	{
		if(!(Is_Chiave(K[i])))
            K.erase(i,1);
		else i++;
	}

	if(K.length()>0) Key=K;
	else Key="ABC";

	return;
}

void CMT::Ins_T_Chiaro(string TC)
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

void CMT::Ins_T_Criptato(string TC)
{
    //fstream F;
    //F.open("DEBUG Ins_T_Criptato.txt", ios::out);
    //F<<TC <<endl;
	int N;
	char C [2];
	T_Criptato.clear();
	int i=0;

    while(i< TC.length()) //Elimino gli spazi bianchi
	{
	    if(TC[i] == ' ')
            TC.erase(i,1);

        else i++;
	}

	if(TC.length() % 2 == 1) TC.erase(TC.length() - 1, 1);
    //F<<TC <<endl;
	for(int i=0; i<TC.length(); i+=2)
	{
		C[0] = TC[i];
		C[1] = TC[i+1];
		if(Mag_9_Min_46(C[0], C[1]))
		{
			//itoa (N , C, 10 );
			T_Criptato +=  C[0];
			T_Criptato += C[1];
			//F<<"C[0]: " <<C[0] <<" C[1]: " <<C[1] <<"TC: " <<T_Criptato <<endl;
		}
	}
	//F<<T_Criptato <<endl;

	return;
}

void CMT::Iniz_Sgn_Testo()
{
	Sgn_Testo[0]= 'A';
	Sgn_Testo[1]= 'B';
	Sgn_Testo[2]= 'C';
	Sgn_Testo[3]= 'D';
	Sgn_Testo[4]= 'E';
	Sgn_Testo[5]= 'F';
	Sgn_Testo[6]= 'G';
	Sgn_Testo[7]= 'H';
	Sgn_Testo[8]= 'I';
	Sgn_Testo[9]= 'J';
	Sgn_Testo[10]= 'K';
	Sgn_Testo[11]= 'L';
	Sgn_Testo[12]= 'M';
	Sgn_Testo[13]= 'N';
	Sgn_Testo[14]= 'O';
	Sgn_Testo[15]= 'P';
	Sgn_Testo[16]= 'Q';
	Sgn_Testo[17]= 'R';
	Sgn_Testo[18]= 'S';
	Sgn_Testo[19]= 'T';
	Sgn_Testo[20]= 'U';
	Sgn_Testo[21]= 'V';
	Sgn_Testo[22]= 'W';
	Sgn_Testo[23]= 'X';
	Sgn_Testo[24]= 'Y';
	Sgn_Testo[25]= 'Z';
	Sgn_Testo[26]= '1';
	Sgn_Testo[27]= '2';
	Sgn_Testo[28]= '3';
	Sgn_Testo[29]= '4';
	Sgn_Testo[30]= '5';
	Sgn_Testo[31]= '6';
	Sgn_Testo[32]= '7';
	Sgn_Testo[33]= '8';
	Sgn_Testo[34]= '9';
	Sgn_Testo[35]= '0';

	return;
}

void CMT::Iniz_Sgn_Chiave()
{
	Sgn_Chiave[0]= 'A';
	Sgn_Chiave[1]= 'B';
	Sgn_Chiave[2]= 'C';
	Sgn_Chiave[3]= 'D';
	Sgn_Chiave[4]= 'E';
	Sgn_Chiave[5]= 'F';
	Sgn_Chiave[6]= 'G';
	Sgn_Chiave[7]= 'H';
	Sgn_Chiave[8]= 'I';
	Sgn_Chiave[9]= 'J';
	Sgn_Chiave[10]= 'K';
	Sgn_Chiave[11]= 'L';
	Sgn_Chiave[12]= 'M';
	Sgn_Chiave[13]= 'N';
	Sgn_Chiave[14]= 'O';
	Sgn_Chiave[15]= 'P';
	Sgn_Chiave[16]= 'Q';
	Sgn_Chiave[17]= 'R';
	Sgn_Chiave[18]= 'S';
	Sgn_Chiave[19]= 'T';
	Sgn_Chiave[20]= 'U';
	Sgn_Chiave[21]= 'V';
	Sgn_Chiave[22]= 'W';
	Sgn_Chiave[23]= 'X';
	Sgn_Chiave[24]= 'Y';
	Sgn_Chiave[25]= 'Z';

	return;
}

void CMT::Iniz_Mat_Cifrario()
{
	for(int i=0; i<DIM_CMT; i++)
		for(int j=0; j<DIM2_CMT; j++)
			if((i+j+10) <= 45 ) Mat_Cifrario[i][j] = i+j+10;
			else Mat_Cifrario[i][j] =  i+j+10-DIM_CMT;  //DIM_CMT=36 -> Lettere + numeri(0-9)

	return;
}

void CMT::Iniz_Key_Cript()
{
	string App;

	int Pos=0;
	if(Key.length() < T_Chiaro.length())
	{
	    for(int i=0; i< T_Chiaro.length(); i++)
        {
            Pos = i - (Key.length() * (i / Key.length())); //Può essere sostituti con ciclo for da 0 a Key.lenght()	-------	Questo è più ottimizzato

            App.insert(i, 1,  Key[Pos]); //(Pos, DIM_CMT, char)
        }
	}
	else if (Key.length() > T_Chiaro.length())
        for(int i=0; i<T_Chiaro.length(); i++)
            App.insert(i, 1, Key[i]);
    else
        App=Key;

	Key=App;

	return;
}

void CMT::Iniz_Key_Decript()
{
	string App;

	//La chiave(verme) deve essere uguale alla lunghezza del testo in chiaro
	//Lunghezza testo in chiaro uguale alla metà di quello cifrato
	int Lunghezza_TChiaro = T_Criptato.length() / 2;
	int Pos=0;
	if(Key.length() < Lunghezza_TChiaro)
	{
	    for(int i=0; i< Lunghezza_TChiaro; i++)
        {
            Pos = i - (Key.length() * (i / Key.length())); //Può essere sostituti con ciclo for da 0 a Key.lenght()	-------	Questo è più ottimizzato

            App.insert(i, 1,  Key[Pos]); //(Pos, DIM_CMT, char)
        }
	}
	else if (Key.length() > Lunghezza_TChiaro)
        for(int i=0; i<Lunghezza_TChiaro; i++)
            App.insert(i, 1, Key[i]);

    else
        App=Key;

	Key=App;

	return;
}

bool CMT::Is_Testo(char x)
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
	if(x == '1') return true;
	if(x == '2') return true;
	if(x == '3') return true;
	if(x == '4') return true;
	if(x == '5') return true;
	if(x == '6') return true;
	if(x == '7') return true;
	if(x == '8') return true;
	if(x == '9') return true;
	if(x == '0') return true;

	return false;
}

bool CMT::Is_Chiave(char x)
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

	return false;
}

int CMT::Posizione(char x, char M[], int D)
{
	for(int i=0; i<D; i++)
		if(M[i] == x) return i;

	return -1;
}

string CMT::Intero_To_Carattere_CMT(int x) //C::B non riconosce itoa ma riconosce atoi
{
    switch(x)
    {
    case 1:
        return "1";
    case 2:
        return "2";
    case 3:
        return "3";
    case 4:
        return "4";
    case 5:
        return "5";
    case 6:
        return "6";
    case 7:
        return "7";
    case 8:
        return "8";
    case 9:
        return "9";
    case 10:
        return "10";
    case 11:
        return "11";
    case 12:
        return "12";
    case 13:
        return "13";
    case 14:
        return "14";
    case 15:
        return "15";
    case 16:
        return "16";
    case 17:
        return "17";
    case 18:
        return "18";
    case 19:
        return "19";
    case 20:
        return "20";
    case 21:
        return "21";
    case 22:
        return "22";
    case 23:
        return "23";
    case 24:
        return "24";
    case 25:
        return "25";
    case 26:
        return "26";
    case 27:
        return "27";
    case 28:
        return "28";
    case 29:
        return "29";
    case 30:
        return "30";
    case 31:
        return "31";
    case 32:
        return "32";
    case 33:
        return "33";
    case 34:
        return "34";
    case 35:
        return "35";
    case 36:
        return "36";
    case 37:
        return "37";
    case 38:
        return "38";
    case 39:
        return "39";
    case 40:
        return "40";
    case 41:
        return "41";
    case 42:
        return "42";
    case 43:
        return "43";
    case 44:
        return "44";
    case 45:
        return "45";
    case 46:
        return "46";

    default:
        return 0;
    }

    return 0;
}

bool CMT::Mag_9_Min_46(char A, char B)
{
    if(A!='1' && A!='2' && A!='3' && A!='4') return false;
    if(A=='4')
        switch(B)
        {
        case '6':
            return false;
        case '7':
            return false;
        case '8':
            return false;
        case '9':
            return false;
        default:
            break;
        }
    return true;
}
