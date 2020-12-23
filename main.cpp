/*
---------------------------
Nome: Matteo
Cognome: Battisti
Classe: 5IB
Progetto di maturità anno 2016/2017
La crittografia nella Grande Guerra
Numero righe di codice: 6340

---------------------------

---------------------------
Codice che gestisce gli eventi dell'interfaccia grafica del progetto
---------------------------
*/

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <windows.h>

#include <tchar.h> //per copy_st
#include <Windowsx.h>
#include "resource.h"
#include <string>
#include <fstream>
#include <time.h> //Per random
#include <conio.h>

#include "Algoritmi/ADFGVX.h"
#include "Algoritmi/ADFMX.h"
#include "Algoritmi/CBD.h"
#include "Algoritmi/Cifr123.h"
#include "Algoritmi/CMT.h"
#include "Algoritmi/PlayfairCipher.h"
#include "Algoritmi/RulloJefferson.h"
#include "Algoritmi/CampAust.h"

//------------------------
//VARIABILI GLOBALI

std::string Algoritmo;
std::string TC; //SIA Testo cifrato che testo criptato
std::string KEY;
std::string KEY_TC;
int const DIM_BUFFER = 1000; //Dimensione serratura, così cifra fino a 500 caratteri con l'ADFMX
HWND FinestraPrecedente;

//Utilizzati nell'123 sia nella cifratura che nella decifratura
const int DIM_Lista_Parole = 100; //Impossibile che vengano inserite più di 100 parole con gli 123
std::string Lista_Parole[DIM_Lista_Parole];
int Pos_Lista_Parole = 0;

//Utilizzato nel Jefferson
bool Modifica_Attiva;
char Vett_Mod_Rullo[DIM_ALF_Jefferson];
int Pos_Mod_Rullo = 0;

int nrRullo;
//-------------------------
//Gestori di procedure

BOOL CALLBACK GestoreProcedure_Home(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GestoreProcedure_ADFMX(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GestoreProcedure_Cif_Std(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GestoreProcedure_Cifr_CampAust(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GestoreProcedure_Reindirizzamento_123(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GestoreProcedure_Cifra_123(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GestoreProcedure_Decifra_123(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GestoreProcedure_Fin_Cifr_Jeff(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GestoreProcedure_Fin_Info_Jeff(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GestoreProcedure_Fin_Mod_Rullo(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

//---------------------------
//Funzioni di cifratura e decifratura

void Cifra_ADFMX(HWND hwndDlg);
void Decifra_ADFMX(HWND hwndDlg);

void Cifra_Std(HWND hwndDlg);
void Decifra_Std(HWND hwndDlg);

void Cifra_ADFGVX(HWND hwndDlg);
void Decifra_ADFGVX(HWND hwndDlg);

void Cifra_CBD(HWND hwndDlg);
void Decifra_CBD(HWND hwndDlg);

void Cifra_Playfair(HWND hwndDlg);
void Decifra_Playfair(HWND hwndDlg);

void Cifra_CMT(HWND hwndDlg);
void Decifra_CMT(HWND hwndDlg);

void Cifra_CampAust(HWND hwndDlg);
void Decifra_CampAust(HWND hwndDlg);

void Ins_Codice_Decifra_123(Cifr123 & Decifra_123,string Codice, string & Parola); //Ritorna vero se la parola è stata trovata, falso altrimenti
void Sottoponi_123(HWND hwndDlg); //Inserisce il codice nel text box nell'array con le parole decifrate

void Cifra_Jefferson(HWND hwndDlg, int K);
void Decifra_Jefferson(HWND hwndDlg);

//-------------------------------
//Altre funzioni
std::string Converte_Vettore_In_Stringa(char Text[], int dim);
void Converte_Stringa_In_Vettore(string Text, char RIS[], int DIMRIS);
void Reimposta_Buffer(char B[], int DIM); //Reimposta il buffer con tutti "!" per utilizzarlo nel gettext
bool Is_Alfabeto(char x);
void Seleziona_Da_ListBox(HWND hwndDlg);
void Scrivi_Testo_Chiaro_123(HWND hwndDlg); //Serve per scrivere il testo in chiaro nello static in 123 Cifra
void Scrivi_Testo_Cifrato_123(HWND hwndDlg, string Vettore[]); //Serve quando si cifra per scrivere il testo nello static
void Visualizza_Animazione_Telegrafo(HWND hwndDlg, string Vettore[]); //Disegna sui quadrati l'animazione
bool Riposa(int Riposo); //Elimina bug animazione
void new_ITOA(int N, char C[]); //Funziona da 0 a 99, poteva essere ampliato ma non era funzionale allo scopo
void Iniz_Mod_Rullo(HWND hwndDlg);
void Imposta_Label_Riga(HWND hwndDlg, int Pos);
void Imposta_Label_NrRullo(HWND hwndDlg, int NrRullo);
//void Scrivi_Testo_Criptato_123(HWND hwndDlg, string Vettore[]); //Serve quando si fa cancella ultimo nel 123 decifra

//-------------------------------

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument,int nCmdShow)
{
    DialogBoxParam(0,FinestraHome,0,GestoreProcedure_Home,0);

    return 0;
}

BOOL CALLBACK GestoreProcedure_Home(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg==WM_CLOSE)
    {
        if(FinestraPrecedente == hwndDlg) return true; //Se è già aperta una finestra reindirizzamento 123
        EndDialog(hwndDlg, 0);
        return TRUE;
    }
    else if(uMsg == WM_INITDIALOG)
    {
        CaricaLogo
        HBITMAP hBmp;

        //Sfondo 1
        HBITMAP hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Sfondo1));
        SendMessage(GetDlgItem(hwndDlg,Static_Sfondo1),STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);


        //Sfondo 2
        hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Sfondo2));
        SendMessage(GetDlgItem(hwndDlg,Static_Sfondo2),STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);


        //Immagine cifrario ADFMX
        hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ADFMX_Image));
        SendMessage(GetDlgItem(hwndDlg,Bn_ADFMX), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);

        //Immagine ADFGVX
        hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ADFGVX_Image));
        SendMessage(GetDlgItem(hwndDlg,Bn_ADFGVX), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);

        //Immagine cifrario 123
        hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_123_Image));
        SendMessage(GetDlgItem(hwndDlg,Bn_123), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);

        //Immagine CMT
        hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CMT_Image));
        SendMessage(GetDlgItem(hwndDlg,Bn_CMT), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);

        //Immagine CBD
        hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CBD_Image));
        SendMessage(GetDlgItem(hwndDlg,Bn_CBD), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);

        //Immagine CampAust
        hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CampAust_Image));
        SendMessage(GetDlgItem(hwndDlg,Bn_CampAust), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);

        //Immagine Playfair
        hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_GB_Image));
        SendMessage(GetDlgItem(hwndDlg,Bn_Playfair), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);

        //Immagine Jefferson
        hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_USA_Image));
        SendMessage(GetDlgItem(hwndDlg,Bn_Jefferson), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);

    }

    else if(uMsg==WM_COMMAND)
    {
        if(FinestraPrecedente == hwndDlg) return true; //Se è già aperta una finestra reindirizzamento 123
        WORD wNotifica = HIWORD(wParam);
        WORD wId = LOWORD(wParam);
        if(wNotifica == BN_CLICKED)
        {
            switch(wId)
            {
            case(Bn_ADFMX):
                //Apri Fin_Cifr_ADFMX (Ha due chiavi)
                Algoritmo = "ADFMX";
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,Fin_Cifr_ADFMX,0,GestoreProcedure_ADFMX,0);
                return true;
                break;
            case(Bn_ADFGVX):
                //Apri Fin_Cifr_Std
                Algoritmo = "ADFGVX";
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,Fin_Cifr_Std,0,GestoreProcedure_Cif_Std,0);
                return true;
                break;
            case(Bn_CBD):
                //Apri Fin_Cifr_Std
                Algoritmo = "CBD";
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,Fin_Cifr_Std,0,GestoreProcedure_Cif_Std,0);
                return true;
                break;
            case(Bn_Playfair):
                //Apri Fin_Cifr_Std
                Algoritmo = "Playfair";
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,Fin_Cifr_Std,0,GestoreProcedure_Cif_Std,0);
                return true;
                break;
            case(Bn_123):
                //Apri Fin_Cifr_123 per il reindirizzamento
                Algoritmo = "123";
                FinestraPrecedente = hwndDlg;
                //EndDialog(hwndDlg, 0);
                DialogBoxParam(0,Fin_Reindirizzamento_123,0,GestoreProcedure_Reindirizzamento_123,0);
                return true;
                break;
            case(Bn_CMT):
                //Apri Fin_Cifr_Std
                Algoritmo = "CMT";
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,Fin_Cifr_Std,0,GestoreProcedure_Cif_Std,0);
                return true;
                break;
            case(Bn_Jefferson):
                Algoritmo = "Jefferson";
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,Fin_Cifr_Jeff,0,GestoreProcedure_Fin_Cifr_Jeff,0);
                return true;
                break;
            case(Bn_CampAust):
                //Apri Fin_Cifr_CampAust (Non ha chiavi)
                Algoritmo = "CampAust";
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,Fin_Cifr_CampAust,0,GestoreProcedure_Cifr_CampAust,0);
                return true;
                break;
            default:
                return false;
            }
        }
    }
    else if(uMsg==WM_CTLCOLORDLG)
    {
        HBRUSH g_hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

        return (LONG)g_hbrBackground;
    }
    return false;
}

BOOL CALLBACK GestoreProcedure_ADFMX(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


    if(uMsg==WM_CLOSE)
    {
        EndDialog(hwndDlg, 0);
        return true;
    }
    else if(uMsg == WM_INITDIALOG)
    {
        HWND hStat = GetDlgItem(hwndDlg, Tb_Cifra_ADFMX); //Ritorna l'HWND dall'id
        Edit_LimitText(hStat, 400); //Imposta dimensione massima testo

        hStat = GetDlgItem(hwndDlg, Tb_Decifra_ADFMX); //Ritorna l'HWND dall'id
        Edit_LimitText(hStat, 800); //Imposta dimensione massima testo

        hStat = GetDlgItem(hwndDlg, Tb_Chiave_TC_ADFMX); //Ritorna l'HWND dall'id
        Edit_LimitText(hStat, 25); //Imposta dimensione massima testo

        //Aumento il carattere della descrizione
        long lfHeight = -MulDiv(12, GetDeviceCaps((HDC)hwndDlg, LOGPIXELSY), 72);
        HFONT font_descrizione = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "MS Sans Serif");
        SendDlgItemMessage(hwndDlg, Id_Static_Descrizione_ADFMX, WM_SETFONT, (WPARAM)font_descrizione, TRUE);

        CaricaLogo
    }
    else if(uMsg==WM_COMMAND)
    {
        WORD wNotifica = HIWORD(wParam);
        WORD wId = LOWORD(wParam);
        if(wNotifica == BN_CLICKED)
        {
            switch(wId)
            {
            case (Bn_Back_ADFMX):
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,FinestraHome,0,GestoreProcedure_Home,0);
                return true;
                break;

            case (Bn_Cifra_ADFMX):
                char buffer_cifra[DIM_BUFFER]; //Dimensione serratura 1000

                Reimposta_Buffer(buffer_cifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Cifra_ADFMX), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_cifra);
                TC = Converte_Vettore_In_Stringa(buffer_cifra, DIM_BUFFER);

                Reimposta_Buffer(buffer_cifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Chiave_ADFMX), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_cifra);
                KEY = Converte_Vettore_In_Stringa(buffer_cifra, DIM_BUFFER);

                Reimposta_Buffer(buffer_cifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Chiave_TC_ADFMX), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_cifra);
                KEY_TC = Converte_Vettore_In_Stringa(buffer_cifra, DIM_BUFFER);

                Cifra_ADFMX(hwndDlg);
                return true;
                break;

            case (Bn_Decifra_ADFMX):
                char buffer_decifra[DIM_BUFFER]; //Dimensione serratura 1000

                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Decifra_ADFMX), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);
                TC = Converte_Vettore_In_Stringa(buffer_decifra, DIM_BUFFER);

                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Chiave_ADFMX), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);
                KEY = Converte_Vettore_In_Stringa(buffer_decifra, DIM_BUFFER);

                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Chiave_TC_ADFMX), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);
                KEY_TC = Converte_Vettore_In_Stringa(buffer_decifra, DIM_BUFFER);

                Decifra_ADFMX(hwndDlg);
                return true;
                break;

            case (Bn_Clear_ADFMX):
                HWND hStat = GetDlgItem(hwndDlg, Tb_Cifra_ADFMX); //Ritorna l'HWND dall'id
                HWND hStat2 = GetDlgItem(hwndDlg, Tb_Decifra_ADFMX); //Ritorna l'HWND dall'id
                HWND hStat3 = GetDlgItem(hwndDlg, Tb_Chiave_ADFMX); //Ritorna l'HWND dall'id
                HWND hStat4 = GetDlgItem(hwndDlg, Tb_Chiave_TC_ADFMX); //Ritorna l'HWND dall'id
                const char* Testo;
                Testo="";
                SendMessage(hStat, WM_SETTEXT,0, (LPARAM)Testo);
                SendMessage(hStat2, WM_SETTEXT,0, (LPARAM)Testo);
                SendMessage(hStat3, WM_SETTEXT,0, (LPARAM)Testo);
                SendMessage(hStat4, WM_SETTEXT,0, (LPARAM)Testo);
                return true;
                break;
            }
        }
    }

  return false;
}

void Cifra_ADFMX(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Decifra_ADFMX); //Ritorna l'HWND dall'id
    HWND hStat_Chiave = GetDlgItem(hwndDlg, Tb_Chiave_ADFMX); //Ritorna l'HWND dall'id
    HWND hStat_Chiave_TC = GetDlgItem(hwndDlg, Tb_Chiave_TC_ADFMX); //Ritorna l'HWND dall'id

    std::string T_Cifrato;

    if(Algoritmo == "ADFMX")
    {
        ADFMX Cifra_ADFMX;
        Cifra_ADFMX.Ins_T_Chiaro(TC);
        //Modifico chiave e chiave_tc solo se le loro caselle non sono vuote.
        if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_ADFMX)) > 0) Cifra_ADFMX.Ins_Key(KEY);
        if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_TC_ADFMX)) > 0) Cifra_ADFMX.Ins_KeyTC(KEY_TC);

        T_Cifrato = Cifra_ADFMX.leggi();

        char Testo[DIM_BUFFER];
        char Chiave[DIM_BUFFER];
        char Chiave_TC[DIM_BUFFER];
        Reimposta_Buffer(Testo, DIM_BUFFER);
        Reimposta_Buffer(Chiave, DIM_BUFFER);
        Reimposta_Buffer(Chiave_TC, DIM_BUFFER);

        Converte_Stringa_In_Vettore(T_Cifrato, Testo, DIM_BUFFER);
        Converte_Stringa_In_Vettore(Cifra_ADFMX.Leggi_Chiave(), Chiave, DIM_BUFFER);
        Converte_Stringa_In_Vettore(Cifra_ADFMX.Leggi_Chiave_TC(), Chiave_TC, DIM_BUFFER);

        /*fstream F;
        F.open("Controllo_Chiavi.txt", ios::out);
        F<<"Chiave: " <<Cifra_ADFMX.Leggi_Chiave() <<"  Chiave tc: " <<Cifra_ADFMX.Leggi_Chiave_TC();
        F.close();*/

        SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);
        SendMessage(hStat_Chiave, WM_SETTEXT,0, (LPARAM)Chiave);
        SendMessage(hStat_Chiave_TC, WM_SETTEXT,0, (LPARAM)Chiave_TC);
    }
    return;
}

void Decifra_ADFMX(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Cifra_ADFMX); //Ritorna l'HWND dall'id
    HWND hStat_Chiave = GetDlgItem(hwndDlg, Tb_Chiave_ADFMX); //Ritorna l'HWND dall'id
    HWND hStat_Chiave_TC = GetDlgItem(hwndDlg, Tb_Chiave_TC_ADFMX); //Ritorna l'HWND dall'id

    std::string T_Decriptato;

    if(Algoritmo == "ADFMX")
    {
        ADFMX Decifra_ADFMX;
        Decifra_ADFMX.Ins_T_Criptato(TC);
        //Modifico chiave e chiave_tc solo se le loro caselle non sono vuote.
        if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_ADFMX)) > 0) Decifra_ADFMX.Ins_Key(KEY);
        if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_TC_ADFMX)) > 0) Decifra_ADFMX.Ins_KeyTC(KEY_TC);

        T_Decriptato = Decifra_ADFMX.Decripta();

        char Testo[DIM_BUFFER];
        char Chiave[DIM_BUFFER];
        char Chiave_TC[DIM_BUFFER];
        Reimposta_Buffer(Testo, DIM_BUFFER);
        Reimposta_Buffer(Chiave, DIM_BUFFER);
        Reimposta_Buffer(Chiave_TC, DIM_BUFFER);

        Converte_Stringa_In_Vettore(T_Decriptato, Testo, DIM_BUFFER);
        Converte_Stringa_In_Vettore(Decifra_ADFMX.Leggi_Chiave(), Chiave, DIM_BUFFER);
        Converte_Stringa_In_Vettore(Decifra_ADFMX.Leggi_Chiave_TC(), Chiave_TC, DIM_BUFFER);

        SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);
        SendMessage(hStat_Chiave, WM_SETTEXT,0, (LPARAM)Chiave);
        SendMessage(hStat_Chiave_TC, WM_SETTEXT,0, (LPARAM)Chiave_TC);
    }
    return;
}

BOOL CALLBACK GestoreProcedure_Cif_Std(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
     if(uMsg==WM_CLOSE)
    {
        EndDialog(hwndDlg, 0);
        return TRUE;
    }
    else if(uMsg == WM_INITDIALOG)
    {
        HWND hStat = GetDlgItem(hwndDlg, Tb_Cifra_Std); //Ritorna l'HWND dall'id
        Edit_LimitText(hStat, 400); //Imposta dimensione massima testo

        hStat = GetDlgItem(hwndDlg, Tb_Decifra_Std); //Ritorna l'HWND dall'id
        Edit_LimitText(hStat, 800); //Imposta dimensione massima testo

        hStat = GetDlgItem(hwndDlg, Tb_Chiave_Std); //Ritorna l'HWND dall'id
        Edit_LimitText(hStat, 25); //Imposta dimensione massima testo

        //Aumento il carattere della descrizione
        long lfHeight = -MulDiv(12, GetDeviceCaps((HDC)hwndDlg, LOGPIXELSY), 72);
        HFONT font_descrizione = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "MS Sans Serif");
        SendDlgItemMessage(hwndDlg, Id_Static_Descrizione_Std, WM_SETFONT, (WPARAM)font_descrizione, TRUE);

        const HANDLE hbicon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_Logo), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);

        if(hbicon)
            SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hbicon);

        HANDLE hsicon = LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_Logo), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
        if(hsicon)
            SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hsicon);

        char* Descrizione;
        if(Algoritmo == "ADFGVX")
        {
            Descrizione = "La cifra ADFGVX fu adottata dai tedeschi in previsione dell'offensiva di primavera del 1918.\n\nQuesto algoritmo con matrice 6x6 è una variante della matrice 5x5 del ADFMX. La matrice 6x6 consente di utilizzare 36 caratteri, i 26 alfabetici più le dieci cifre decimali.\n\nI tedeschi ritenevano che questo cifrario fosse più sicuro dei precedenti, ma l'ufficio cifra francese riuscì a decrittare diversi messaggi tra i quali il Radiogramma della vittoria. Nel giugno 1918 fu infatti decriptato un messaggio tedesco che recitava: 'Affrettare invio munizioni Punto Se non visibile anche di giorno'. Un messaggio che si rivelò fondamentale nel prevenire l'imminente offensiva tedesca ed iniziare la controffensiva che portò alla vittoria francese. Per questo il radiogramma fu soprannominato 'della Vittoria'.\n\nLa cifratura dei messaggi avviene in modo analogo al suo predecessore, con la differenza che in questo caso le lettere della matrice sono disposte in ordine sparso, quindi la chiave di questa matrice sarà proprio la disposizione delle lettere.";
        }
        else if(Algoritmo == "CBD")
        {
            Descrizione = "Il cifrario bifido di Delastelle è un cifrario poligrafico basato su una matrice 5x5, utilizzato per le comunicazioni cifrate francesi dalla seconda metà dei XIX secolo.\n\nIl metodo si articola in quattro passi:\n\n      1)Il messaggio chiaro viene spezzato in blocchi di cinque caratteri ciascuno; se l'ultimo blocco non è esattamente di cinque, gli ultimi posti sono riempiti di X.\n\n      2)Ogni lettera del blocco viene cifrata con due cifre e cioè con l'indice di riga e l'indice di colonna, che vengono scritte in verticale sotto la lettera chiara.\n\n      3)Le cifre vengono ora riscritte in orizzontale riga dopo riga ottenendo un messaggio con un numero di cifre doppio dell'originale.\n\n     4)Ogni coppia di numeri viene ritrasformata in lettera sempre secondo la matrice. Ne risulta il messaggio cifrato da trasmettere.";
        }
        else if(Algoritmo == "Playfair")
        {
            Descrizione = "Il Playfair Chiper è l'algoritmo crittografico utilizzato dagli inglesi dalla fine del XIX secolo.\nIl Playfair Cipher è una forma di cifrario poligrafico nella quale il testo è scomposto da bigrammi.iL Playfair fa uso una matrice 5x5 di 25 caratteri diversi. Un modo semplice di generare la tabella sta nello scegliere una parola chiave che andrà scritta all'inizio riga dopo riga ed eliminando i caratteri già presenti. Dopo la chiave si scrivono le rimanenti lettere in ordine alfabetico.\n\nUna volta generata questa matrice quadrata la cifratura avviene nel seguente modo:\n1)Il testo chiaro viene suddiviso in gruppi di due lettere.\n2)Per ogni gruppo si cercano nella matrice le due lettere e si sostituiscono con altre due secondo le seguenti regole:\n          Se le due lettere sono in colonne e linee diverse, si prendono le due che fanno rettangolo con esse.\n          Se le due lettere si trovano sulla stessa riga, si sostituiscono con le due lettere che le seguono a destra.\n          Se le due lettere sono sulla stessa colonna, si prendono le due lettere sottostanti.\n          Se le due lettere dono uguali se ne elimina una oppure si inserisce una lettera rara (K, W, X, Y).";
        }
        else if(Algoritmo == "CMT")
        {
            Descrizione = "Cifrario militare utilizzato dagli italiani nel corso della prima guerra mondiale. Questo cifrario era ormai da tempo obsoleto, poiché basato sulla tavola di Vigenere del 1586 e di cui era già noto da tempo un metodo di decriptazione. Solo il tragico evento di Caporetto (in cui il cifrario recitò la sua parte drammatica) fece capire agli alti comandi italiani che fosse giunto il momento di cambiare cifrario.\n\nIl cifrario utilizza una matrice 36x26 che contiene i numeri da 10 a 45 ordinati e spostati di un valore ogni riga, come indici delle righe troviamo le lettere dell'alfabeto e come indici delle colonne troviamo le lettere dell'alfabeto e i numeri da 0 a 9.\n\nPer cifrare si prende come indice di colonna la prima lettera del testo in chiaro, come indice di riga la prima lettera della chiave, come risultato si ottiene l'intersezione tra le due e così via per tutte le lettere del testo in chiaro.\nLa chiave viene anche detta verme perchè viene ripetuta finché non raggiunge la lunghezza del testo in chiaro";
        }
        else return true;
        SendMessage(GetDlgItem(hwndDlg, Id_Static_Descrizione_Std), WM_SETTEXT,0, (LPARAM)Descrizione);
    }

    else if(uMsg==WM_COMMAND)
    {
        WORD wNotifica = HIWORD(wParam);
        WORD wId = LOWORD(wParam);
        if(wNotifica == BN_CLICKED)
        {
            switch(wId)
            {
            case (Bn_Back_Std):
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,FinestraHome,0,GestoreProcedure_Home,0);
                return true;
                break;

            case (Bn_Cifra_Std):
                char buffer_cifra[DIM_BUFFER]; //Dimensione serratura 1000

                Reimposta_Buffer(buffer_cifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Cifra_Std), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_cifra);
                TC = Converte_Vettore_In_Stringa(buffer_cifra, DIM_BUFFER);

                Reimposta_Buffer(buffer_cifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Chiave_Std), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_cifra);
                KEY = Converte_Vettore_In_Stringa(buffer_cifra, DIM_BUFFER);

                Cifra_Std(hwndDlg);
                return true;
                break;

            case (Bn_Decifra_Std):
                char buffer_decifra[DIM_BUFFER]; //Dimensione serratura 1000

                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Decifra_Std), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);
                TC = Converte_Vettore_In_Stringa(buffer_decifra, DIM_BUFFER);

                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Chiave_Std), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);
                KEY = Converte_Vettore_In_Stringa(buffer_decifra, DIM_BUFFER);

                Decifra_Std(hwndDlg);
                return true;
                break;

            case (Bn_Clear_Std):
                HWND hStat = GetDlgItem(hwndDlg, Tb_Cifra_Std); //Ritorna l'HWND dall'id
                HWND hStat2 = GetDlgItem(hwndDlg, Tb_Decifra_Std); //Ritorna l'HWND dall'id
                HWND hStat3 = GetDlgItem(hwndDlg, Tb_Chiave_Std); //Ritorna l'HWND dall'id
                const char* Testo;
                Testo="";
                SendMessage(hStat, WM_SETTEXT,0, (LPARAM)Testo);
                SendMessage(hStat2, WM_SETTEXT,0, (LPARAM)Testo);
                SendMessage(hStat3, WM_SETTEXT,0, (LPARAM)Testo);
                return true;
                break;
            }
        }
    }
    return false;
}

void Cifra_Std(HWND hwndDlg)
{
    if(Algoritmo == "ADFGVX") Cifra_ADFGVX(hwndDlg);
    else if (Algoritmo == "CBD") Cifra_CBD(hwndDlg);
    else if (Algoritmo == "Playfair") Cifra_Playfair(hwndDlg);
    else if (Algoritmo == "CMT") Cifra_CMT(hwndDlg);

    return;
}

void Decifra_Std(HWND hwndDlg)
{
    if(Algoritmo == "ADFGVX") Decifra_ADFGVX(hwndDlg);
    else if (Algoritmo == "CBD") Decifra_CBD(hwndDlg);
    else if (Algoritmo == "Playfair") Decifra_Playfair(hwndDlg);
    else if (Algoritmo == "CMT") Decifra_CMT(hwndDlg);

    return;
}

void Cifra_ADFGVX(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Decifra_Std); //Ritorna l'HWND dall'id
    HWND hStat_Chiave = GetDlgItem(hwndDlg, Tb_Chiave_Std); //Ritorna l'HWND dall'id

    std::string T_Decifrato;

    ADFGVX Cifra_ADFGVX;

    Cifra_ADFGVX.Ins_T_Chiaro(TC);

    //Modifico chiave e chiave_tc solo se le loro caselle non sono vuote.
    if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_Std)) > 0) Cifra_ADFGVX.Ins_Key(KEY);


    T_Decifrato = Cifra_ADFGVX.leggi();

    const char* Testo = T_Decifrato.c_str();
    const char* Chiave = Cifra_ADFGVX.Leggi_Chiave().c_str();

    SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);
    SendMessage(hStat_Chiave, WM_SETTEXT,0, (LPARAM)Chiave);

    return;
}

void Decifra_ADFGVX(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Cifra_Std); //Ritorna l'HWND dall'id
    HWND hStat_Chiave = GetDlgItem(hwndDlg, Tb_Chiave_Std); //Ritorna l'HWND dall'id

    std::string T_Decriptato;

    ADFGVX Decifra_ADFGVX;

    Decifra_ADFGVX.Ins_T_Criptato(TC);

    //Modifico chiave e chiave_tc solo se le loro caselle non sono vuote.
    if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_Std)) > 0) Decifra_ADFGVX.Ins_Key(KEY);


    T_Decriptato = Decifra_ADFGVX.Decripta();

    const char* Testo = T_Decriptato.c_str();
    const char* Chiave = Decifra_ADFGVX.Leggi_Chiave().c_str();

    SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);
    SendMessage(hStat_Chiave, WM_SETTEXT,0, (LPARAM)Chiave);

    return;
}

void Cifra_CBD(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Decifra_Std); //Ritorna l'HWND dall'id
    HWND hStat_Chiave = GetDlgItem(hwndDlg, Tb_Chiave_Std); //Ritorna l'HWND dall'id

     std::string T_Decifrato;

     CBD Cifra_CBD;

     Cifra_CBD.Ins_T_Chiaro(TC);

    //Modifico chiave e chiave_tc solo se le loro caselle non sono vuote.
    if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_Std)) > 0) Cifra_CBD.Ins_Key(KEY);


    T_Decifrato = Cifra_CBD.Leggi_Cifra();

    const char* Testo = T_Decifrato.c_str();
    const char* Chiave = Cifra_CBD.Leggi_Chiave().c_str();

    SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);
    SendMessage(hStat_Chiave, WM_SETTEXT,0, (LPARAM)Chiave);

    return;
}

void Decifra_CBD(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Cifra_Std); //Ritorna l'HWND dall'id
    HWND hStat_Chiave = GetDlgItem(hwndDlg, Tb_Chiave_Std); //Ritorna l'HWND dall'id

    std::string T_Decriptato;

    CBD Decifra_CBD;

    Decifra_CBD.Ins_T_Criptato(TC);

    //Modifico chiave e chiave_tc solo se le loro caselle non sono vuote.
    if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_Std)) > 0) Decifra_CBD.Ins_Key(KEY);


    T_Decriptato = Decifra_CBD.Leggi_Decifra();

    const char* Testo = T_Decriptato.c_str();
    const char* Chiave = Decifra_CBD.Leggi_Chiave().c_str();

    SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);
    SendMessage(hStat_Chiave, WM_SETTEXT,0, (LPARAM)Chiave);

    return;
}

void Cifra_Playfair(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Decifra_Std); //Ritorna l'HWND dall'id
    HWND hStat_Chiave = GetDlgItem(hwndDlg, Tb_Chiave_Std); //Ritorna l'HWND dall'id

     std::string T_Decifrato;

     Playfair Cifra_Playfair;

     Cifra_Playfair.Ins_T_Chiaro(TC);

    //Modifico chiave e chiave_tc solo se le loro caselle non sono vuote.
    if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_Std)) > 0) Cifra_Playfair.Ins_Key(KEY);


    T_Decifrato = Cifra_Playfair.Leggi_Cifra();

    const char* Testo = T_Decifrato.c_str();
    const char* Chiave = Cifra_Playfair.Leggi_Chiave().c_str();

    SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);
    SendMessage(hStat_Chiave, WM_SETTEXT,0, (LPARAM)Chiave);

    return;
}

void Decifra_Playfair(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Cifra_Std); //Ritorna l'HWND dall'id
    HWND hStat_Chiave = GetDlgItem(hwndDlg, Tb_Chiave_Std); //Ritorna l'HWND dall'id

    std::string T_Decriptato;

    Playfair Decifra_Playfair;

    Decifra_Playfair.Ins_T_Criptato(TC);

    //Modifico chiave e chiave_tc solo se le loro caselle non sono vuote.
    if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_Std)) > 0) Decifra_Playfair.Ins_Key(KEY);


    T_Decriptato = Decifra_Playfair.Leggi_Decifra();

    const char* Testo = T_Decriptato.c_str();
    const char* Chiave = Decifra_Playfair.Leggi_Chiave().c_str();

    SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);
    SendMessage(hStat_Chiave, WM_SETTEXT,0, (LPARAM)Chiave);

    return;
}

void Cifra_CMT(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Decifra_Std); //Ritorna l'HWND dall'id
    HWND hStat_Chiave = GetDlgItem(hwndDlg, Tb_Chiave_Std); //Ritorna l'HWND dall'id

    std::string T_Decifrato;

    CMT Cifra_CMT;

    Cifra_CMT.Ins_T_Chiaro(TC);

    //Modifico chiave e chiave_tc solo se le loro caselle non sono vuote.
    if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_Std)) > 0) Cifra_CMT.Ins_Key(KEY);


    T_Decifrato = Cifra_CMT.Leggi_Cifra();

    const char* Testo = T_Decifrato.c_str();
    const char* Chiave = Cifra_CMT.Leggi_Chiave().c_str();

    SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);
    SendMessage(hStat_Chiave, WM_SETTEXT,0, (LPARAM)Chiave);

    return;
}

void Decifra_CMT(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Cifra_Std); //Ritorna l'HWND dall'id
    HWND hStat_Chiave = GetDlgItem(hwndDlg, Tb_Chiave_Std); //Ritorna l'HWND dall'id

    std::string T_Decriptato;

    CMT Decifra_CMT;

    Decifra_CMT.Ins_T_Criptato(TC);

    //Modifico chiave e chiave_tc solo se le loro caselle non sono vuote.
    if(Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Chiave_Std)) > 0) Decifra_CMT.Ins_Key(KEY);

    T_Decriptato = Decifra_CMT.Leggi_Decifra();

    const char* Testo = T_Decriptato.c_str();
    const char* Chiave = Decifra_CMT.Leggi_Chiave().c_str();

    SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);
    SendMessage(hStat_Chiave, WM_SETTEXT,0, (LPARAM)Chiave);

    return;
}

BOOL CALLBACK GestoreProcedure_Cifr_CampAust(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg==WM_CLOSE)
    {
        EndDialog(hwndDlg, 0);
        return true;
    }
    else if(uMsg == WM_INITDIALOG)
    {
        HWND hStat = GetDlgItem(hwndDlg, Tb_Cifra_CampAust); //Ritorna l'HWND dall'id
        Edit_LimitText(hStat, 250); //Imposta dimensione massima testo

        hStat = GetDlgItem(hwndDlg, Tb_Decifra_CampAust); //Ritorna l'HWND dall'id
        Edit_LimitText(hStat, 750); //Imposta dimensione massima testo

        //Aumento il carattere della descrizione
        long lfHeight = -MulDiv(12, GetDeviceCaps((HDC)hwndDlg, LOGPIXELSY), 72);
        HFONT font_descrizione = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "MS Sans Serif");
        SendDlgItemMessage(hwndDlg, Id_Static_Descrizione_CampAust, WM_SETFONT, (WPARAM)font_descrizione, TRUE);

        const HANDLE hbicon = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_Logo), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);

        if(hbicon)
            SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)hbicon);

        HANDLE hsicon = LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_Logo), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
        if(hsicon)
            SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hsicon);
    }
    else if(uMsg==WM_COMMAND)
    {
        WORD wNotifica = HIWORD(wParam);
        WORD wId = LOWORD(wParam);
        if(wNotifica == BN_CLICKED)
        {
            switch(wId)
            {
            case (Bn_Back_CampAust):
            {

                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,FinestraHome,0,GestoreProcedure_Home,0);
                return true;
                break;
            }
            case (Bn_Cifra_CampAust):
            {
                char buffer_cifra[DIM_BUFFER]; //Dimensione serratura 1000

                Reimposta_Buffer(buffer_cifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Cifra_CampAust), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_cifra);
                TC = Converte_Vettore_In_Stringa(buffer_cifra, DIM_BUFFER);

                Cifra_CampAust(hwndDlg);
                return true;
                break;
            }
            case(Bn_Decifra_CampAust):
            {
                char buffer_decifra[DIM_BUFFER]; //Dimensione serratura 1000

                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_Decifra_CampAust), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);
                TC = Converte_Vettore_In_Stringa(buffer_decifra, DIM_BUFFER);
                //Fino qui ok

                Decifra_CampAust(hwndDlg);
                return true;
                break;
            }
            case(Bn_Clear_CampAust):
            {
                HWND hStat = GetDlgItem(hwndDlg, Tb_Cifra_CampAust); //Ritorna l'HWND dall'id
                HWND hStat2 = GetDlgItem(hwndDlg, Tb_Decifra_CampAust); //Ritorna l'HWND dall'id
                const char* Testo;
                Testo="";
                SendMessage(hStat, WM_SETTEXT,0, (LPARAM)Testo);
                SendMessage(hStat2, WM_SETTEXT,0, (LPARAM)Testo);
                return true;
                break;
            }
            }
        }
    }

    return false;
}
void Cifra_CampAust(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Decifra_CampAust); //Ritorna l'HWND dall'id

    std::string T_Decifrato;
    std::string App;

    CampAust Cifra_CampAust;

    Cifra_CampAust.Ins_T_Chiaro(TC);

    T_Decifrato = Cifra_CampAust.Leggi_Cifra();

    for(int i=0; i<T_Decifrato.length(); i++)
    {
        App += T_Decifrato[i];
        if ( (i+1) % 5 == 0)
            App.append(" ");
    }
    T_Decifrato.clear();
    T_Decifrato = App;


    //char Testo[DIM_BUFFER * 2];
    //Converte_Stringa_In_Vettore(T_Decifrato, Testo, DIM_BUFFER*2);
    const char* Testo = T_Decifrato.c_str();

    /*char Risultato[DIM_BUFFER * 2];
    for(int i=0; T_Decifrato.length(); i++)
    {
        strcat(Risultato, Testo[i]);
        if (i+1 % 5 == 0)
            strcat(Risultato, " ");
    }*/

    SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);

    return;
}
void Decifra_CampAust(HWND hwndDlg)
{
    HWND hStat_txt = GetDlgItem(hwndDlg, Tb_Cifra_CampAust); //Ritorna l'HWND dall'id

    std::string T_Decriptato;

    CampAust Decifra_CampAust;

    Decifra_CampAust.Ins_T_Criptato(TC);

    T_Decriptato = Decifra_CampAust.Leggi_Decifra();

    const char* Testo = T_Decriptato.c_str();

    SendMessage(hStat_txt, WM_SETTEXT,0, (LPARAM)Testo);

    return;
}

BOOL CALLBACK GestoreProcedure_Reindirizzamento_123(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg==WM_CLOSE)
    {
        FinestraPrecedente = hwndDlg;
        EndDialog(hwndDlg, 0);
        return true;
    }
    else if(uMsg == WM_INITDIALOG)
    {
        CaricaLogo
    }
    else if(uMsg==WM_COMMAND)
    {
        WORD wNotifica = HIWORD(wParam);
        WORD wId = LOWORD(wParam);
        if(wNotifica == BN_CLICKED)
        {
            switch(wId)
            {
            /*case(Bn_Back_123):
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,FinestraHome,0,GestoreProcedure_Home,0);
                return true;
                break;
            */
            case(Bn_Reinderizza_Cifra_123):
                EndDialog(hwndDlg, 0);
                EndDialog(FinestraPrecedente, 0);
                FinestraPrecedente = hwndDlg;
                DialogBoxParam(0,Fin_Cifra_123,0,GestoreProcedure_Cifra_123,0);
                return true;
                break;
            case(Bn_Reinderizza_Decifra_123):
                EndDialog(hwndDlg, 0);
                EndDialog(FinestraPrecedente, 0);
                FinestraPrecedente = hwndDlg;
                DialogBoxParam(0,Fin_Decifra_123,0,GestoreProcedure_Decifra_123,0);
                return true;
                break;
            }
        }
    }
    return false;
}

BOOL CALLBACK GestoreProcedure_Decifra_123(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hStat = GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra); //Ritorna l'HWND dall'id
    Edit_LimitText(hStat, 5); //Imposta dimensione massima testo

    if(uMsg==WM_CLOSE)
    {
        for(int i=0; i<DIM_Lista_Parole; i++)
            Lista_Parole[i] = "";
        Pos_Lista_Parole = 0;

        EndDialog(hwndDlg, 0);
        return true;
    }
    else if(uMsg == WM_INITDIALOG)
    {
        //Aumento il carattere della descrizione
        long lfHeight = -MulDiv(12, GetDeviceCaps((HDC)hwndDlg, LOGPIXELSY), 72);
        HFONT font_descrizione = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "MS Sans Serif");
        SendDlgItemMessage(hwndDlg, Id_Static_Descrizione_123_Decifra, WM_SETFONT, (WPARAM)font_descrizione, TRUE);

        CaricaLogo

        for(int i=0; i<DIM_Lista_Parole; i++)
            Lista_Parole[i] = "";
        Pos_Lista_Parole = 0;
    }
    else if(uMsg==WM_COMMAND)
    {
        WORD wNotifica = HIWORD(wParam);
        WORD wId = LOWORD(wParam);
        int len; //Serve per inserire i numeri nella static text box
        char buffer_decifra[DIM_BUFFER];
        if(wNotifica == BN_CLICKED)
        {
            switch(wId)
            {
            case (Bn_Back_123_Decifra):
            {
                for(int i=0; i<DIM_Lista_Parole; i++)
                    Lista_Parole[i] = "";
                Pos_Lista_Parole = 0;

                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,FinestraHome,0,GestoreProcedure_Home,0);
                return true;
                break;
            }
            case(Bn_1_Decifra):
            {
                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);
                len = GetWindowTextLength(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra));
                if(len<5)
                {
                    SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);

                    buffer_decifra[len] = '1';
                    buffer_decifra[len+1] = '\0';

                    SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_SETTEXT,0, (LPARAM)buffer_decifra);
                }
                return true;
                break;
            }
            case(Bn_2_Decifra):
            {
                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);
                len = GetWindowTextLength(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra));
                if(len<5)
                {
                    SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);

                    buffer_decifra[len] = '2';
                    buffer_decifra[len+1] = '\0';

                    SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_SETTEXT,0, (LPARAM)buffer_decifra);
                }
                return true;
                break;
            }
            case(Bn_3_Decifra):
            {
                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);
                len = GetWindowTextLength(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra));
                if(len<5)
                {
                    SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);

                    buffer_decifra[len] = '3';
                    buffer_decifra[len+1] = '\0';

                    SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_SETTEXT,0, (LPARAM)buffer_decifra);
                }
                return true;
                break;
            }
            case(Bn_Sottoponi_Decifra):
            {
                Sottoponi_123(hwndDlg); //Inserisce il codice nel text box nell'array con le parole decifrate

                return true;
                break;

            }
            case(Bn_Cancella_Decifra):
            {
                const char * Testo;
                Testo="";
                SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_SETTEXT,0, (LPARAM)Testo);
                SendMessage(GetDlgItem(hwndDlg, Id_Static_123_TestoSottoposto_Decifra), WM_SETTEXT,0, (LPARAM)Testo);
                SendMessage(GetDlgItem(hwndDlg, Id_Static_123_Decifra), WM_SETTEXT,0, (LPARAM)Testo);

                for(int i=0; i<DIM_Lista_Parole; i++)
                    Lista_Parole[i] = "";
                Pos_Lista_Parole = 0;

                return true;
                break;
            }
            /*case(Bn_Cancella_Ultimo_Decifra):
            {
                if(Pos_Lista_Parole == 0) return true;
                Pos_Lista_Parole --;
                Lista_Parole[Pos_Lista_Parole] = "";

                Scrivi_Testo_Criptato_123(hwndDlg, Lista_Parole);

                return true;
            }*/
            case(Bn_Undo):
            {
                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);
                SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);
                len = GetWindowTextLength(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra));
                buffer_decifra[len-1] = '\0';

                SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_SETTEXT,0, (LPARAM)buffer_decifra);

                return true;
                break;
            }
            case(Bn_Decifra_Decifra):
            {
                Sottoponi_123(hwndDlg); //Analizza anche l'ultimo codice inserito

                char Risultato[2000]; //Impostiamo il massimo di codici da inserire a 100, quindi 2000 non verrà mai superato in decifratura
                for(int i=0; i<2001; i++)
                    Risultato[i]='\0';
                char App[20];
                /*fstream F;
                F.open("Debug_Decifra123.txt", ios::out);*/

                for(int i=0; i<Pos_Lista_Parole; i++)
                {
                    //F<<"Parola da lista parole: " <<Lista_Parole[i] <<'\n';

                    Converte_Stringa_In_Vettore(Lista_Parole[i], App, 20);

                    //F<<"Parola da App: " <<App <<'\n';

                    strcat(Risultato, App); //Concatena due stringhe (Il primo deve essere un vettore di char[], il secondo di puntatori char*
                    strcat(Risultato," ");
                }

                //F.close();

                SendMessage(GetDlgItem(hwndDlg, Id_Static_123_Decifra), WM_SETTEXT,0, (LPARAM)Risultato);

                return true;
                break;
            }
            } //Chiude lo switch
        }
    }

    return false;
}

void Sottoponi_123(HWND hwndDlg) //Inserisce il codice nel text box nell'array con le parole decifrate
{
    char buff[DIM_BUFFER];
    Reimposta_Buffer(buff, DIM_BUFFER);
    SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_GETTEXT,DIM_BUFFER, (LPARAM)buff);
    std::string Codice = Converte_Vettore_In_Stringa(buff, DIM_BUFFER);

    //GetWindowTextLength -- Restituisce la lunghezza dell'elemento
    if(GetWindowTextLength(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra)) != 0) //Se la Text Box è vuota non inizia neanche la procedura
    {
        Cifr123 Decifra123;
        string Parola; //Parola corrispondente al codice inserito

        Ins_Codice_Decifra_123(Decifra123,Codice, Parola);

        /*fstream F2;
        F2.open("DEBUG_Lista_Parole.txt", ios::app);*/

        if(Parola != "Fail")
        {
            Lista_Parole[Pos_Lista_Parole] = Parola;
            Pos_Lista_Parole++;

            //F2<<Lista_Parole[Pos_Lista_Parole] <<'\n';

            char Testo[DIM_BUFFER];
            SendMessage(GetDlgItem(hwndDlg, Id_Static_123_TestoSottoposto_Decifra), WM_GETTEXT,DIM_BUFFER, (LPARAM)Testo);

            strcat(Testo, buff);
            strcat(Testo, " ");
            SendMessage(GetDlgItem(hwndDlg, Id_Static_123_TestoSottoposto_Decifra), WM_SETTEXT,0, (LPARAM)Testo);
        }
        else
            MessageBox(hwndDlg, "Codice non corrispondente a nessuna parola nel cifrario!", "Errore", MB_OK);

        //F2.close();

        /*fstream F;
        F.open("DEBUG_TAVOLE_123.txt", ios::out);
        F<<"Codice: " <<Codice<<'\n'<<"Buffer_Decifra: " <<buff <<'\n' <<"Parola aggiunta: " <<Parola;
        F.close();*/

        char * Appoggio;
        Appoggio="";
        SendMessage(GetDlgItem(hwndDlg, Tb_TCifrato_123_Decifra), WM_SETTEXT,0, (LPARAM)Appoggio);

    }
    return;
}

BOOL CALLBACK GestoreProcedure_Cifra_123(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Cifr123 Decifra123; //Chiama costruttore e inizializza le tavole
    string V[DIM_123];

    if(uMsg==WM_CLOSE)
    {
        EndDialog(hwndDlg, 0);
        return true;
    }
    else if(uMsg == WM_INITDIALOG) //Quando la finestra viene aperta
    {
        //Aumento il carattere della descrizione
        long lfHeight = -MulDiv(12, GetDeviceCaps((HDC)hwndDlg, LOGPIXELSY), 72);
        HFONT font_descrizione = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "MS Sans Serif");
        SendDlgItemMessage(hwndDlg, Id_Static_Descrizione_123_Cifra, WM_SETFONT, (WPARAM)font_descrizione, TRUE);



        CaricaLogo
        //Inizializza i rettangoli
        //1 Default
        HWND hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led1_Cifra);
        HBITMAP hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Default));
        SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

        //2 Default
        hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led2_Cifra);
        hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Default));
        SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

        //3 Default
        hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led3_Cifra);
        hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Default));
        SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);


        //Vado a inserire la tavola nella listbox
        char buff[50]; //Per inserire nella listbox
        int Dim;
        Dim = Decifra123.Push_Tavole(V);

        for(int i=0; i<Dim; i++)
        {
            Converte_Stringa_In_Vettore(V[i], buff, 50); //Nella ListBox va inserito un vettore di caratteri non una stringa
            SendDlgItemMessage(hwndDlg, List_Tavola_Cifra, LB_ADDSTRING, 0, (LPARAM)buff); //Aggiunge elemento alla listbox
        }

        for(int i=0; i<DIM_Lista_Parole; i++)
            Lista_Parole[i] = "";
        Pos_Lista_Parole = 0;

        return true;
    }
    /* Per ora è congelata, in futuro può essere riprovata
    else if(uMsg == WM_VKEYTOITEM ) //Se viene premuto un tasto sulla tastiera mentre seleziono la listbox
    {
        WORD Tasto = LOWORD(wParam); //Il tasto premuto dall'utente
        if(Is_Alfabeto(Tasto[0]))
        {
            int Pos;
            for(int i=0; i<DIM_123; i++)
                if(V[0] == Tasto[0]) //Ciclo per tutta la tavola fino a trovare una parola che inizia per il tasto premuto
                {
                    Pos = i;
                    break;
                }
            SendMessage(GetDlgItem(hwndDlg, List_Tavola_Cifra), LB_SETCURSEL,0, Pos);
            //LB_SETCURSEL(GetDlgItem(hwndDlg, List_Tavola_Cifra), Pos);
        }

        return true;
    }*/
    else if(uMsg==WM_COMMAND)
    {
        WORD wNotifica = HIWORD(wParam);
        WORD wId = LOWORD(wParam);
        if(wNotifica == BN_CLICKED)
        {
            switch(wId)
            {
            case (Bn_Back_123_Cifra):
            {
                for(int i=0; i<DIM_Lista_Parole; i++)
                    Lista_Parole[i] = "";
                Pos_Lista_Parole = 0;

                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,FinestraHome,0,GestoreProcedure_Home,0);
                return true;
                break;
            }
            case (Bn_Sottoponi_Cifra):
            {
                Seleziona_Da_ListBox(hwndDlg);

                SendMessage(GetDlgItem(hwndDlg, List_Tavola_Cifra), LB_SETCURSEL,0, 0);

                return true;
            }
            case(Bn_CancUltimo_Cifra):
            {
                if(Pos_Lista_Parole == 0) return true;
                Pos_Lista_Parole --;
                Lista_Parole[Pos_Lista_Parole] = "";

                Scrivi_Testo_Chiaro_123(hwndDlg);

                return true;
            }
            case(Bn_CancTutto_Cifra):
            {
                for(int i=0; i<DIM_Lista_Parole; i++)
                    Lista_Parole[i] = "";
                Pos_Lista_Parole = 0;

                Scrivi_Testo_Chiaro_123(hwndDlg);

                char * Cancella;
                Cancella = "";
                SendMessage(GetDlgItem(hwndDlg, Id_Static_123_TCifrato_Cifra), WM_SETTEXT,0, (LPARAM)Cancella);

                return true;
            }
            case(Bn_Cifra_Cifra):
            {
                //fstream F; F.open("DEBUG_PUSHWORD.txt", ios::out);
                string V_Cifrato[100]; //Vettore dove salvare tutti i codici cifrati
                for(int i=0; i<Pos_Lista_Parole; i++)
                {
                    V_Cifrato[i] = Decifra123.Push_Code(Lista_Parole[i]);
                    //F<<i <<": " <<Lista_Parole[i] <<" " <<V_Cifrato[i] <<'\n';
                }
                //F.close();

                Scrivi_Testo_Cifrato_123(hwndDlg, V_Cifrato);

                Visualizza_Animazione_Telegrafo(hwndDlg, V_Cifrato);

                return true;
            }
            }
        }
        else if(wNotifica == LBN_DBLCLK)
        {
            switch(wId)
            {
            case (List_Tavola_Cifra):
            {
                Seleziona_Da_ListBox(hwndDlg);

                return true;
            }
            }
        }
    }

    return false;
}

BOOL CALLBACK GestoreProcedure_Fin_Cifr_Jeff(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg==WM_CLOSE)
    {
        if(FinestraPrecedente == hwndDlg) return true; //Se è già aperta una finestra reindirizzamento 123
        if(!Modifica_Attiva) //Se è aperta la finestra di modifica non chiudere questa finestra
            EndDialog(hwndDlg, 0);
        return true;
    }
    else if(uMsg == WM_INITDIALOG)
    {
        HWND hStat = GetDlgItem(hwndDlg, Tb_Cifra_Jeff); //Ritorna l'HWND dall'id
        Edit_LimitText(hStat, 400); //Imposta dimensione massima testo

        hStat = GetDlgItem(hwndDlg, Tb_Decifra_Jeff); //Ritorna l'HWND dall'id
        Edit_LimitText(hStat, 400); //Imposta dimensione massima testo

        //Aumento il carattere della descrizione
        long lfHeight = -MulDiv(12, GetDeviceCaps((HDC)hwndDlg, LOGPIXELSY), 72);
        HFONT font_descrizione = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "MS Sans Serif");
        SendDlgItemMessage(hwndDlg, Id_Static_Descrizione_Jeff, WM_SETFONT, (WPARAM)font_descrizione, TRUE);

        CaricaLogo

        Modifica_Attiva = false;
        //Inizializzo la listbox per la chiave
        char C[3];
        //fstream F; F.open("DEBUG_ITOA.txt", ios::out);
        for(int i=1; i<NUM_RULLI_Jefferson; i++)
        {
            new_ITOA(i, C);
            //F<<C <<'\n';
            SendDlgItemMessage(hwndDlg, List_Chiave_Jeff, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox
        }
        return true;
    }
    else if(uMsg==WM_COMMAND && !Modifica_Attiva) // Se è aperta la finestra di modifica del rullo non permettere nessuna operazione
    {
        if(FinestraPrecedente == hwndDlg) return true; //Se è già aperta una finestra reindirizzamento 123
        WORD wNotifica = HIWORD(wParam);
        WORD wId = LOWORD(wParam);
        if(wNotifica == BN_CLICKED)
        {
            switch(wId)
            {
            case(Bn_Back_Jeff):
            {
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,FinestraHome,0,GestoreProcedure_Home,0);
                return true;
                break;
            }
            case(Bn_Clear_Jeff):
            {
                const char* Testo;
                Testo="";

                SendMessage(GetDlgItem(hwndDlg, Tb_Cifra_Jeff), WM_SETTEXT,0, (LPARAM)Testo);
                SendMessage(GetDlgItem(hwndDlg, Tb_Decifra_Jeff), WM_SETTEXT,0, (LPARAM)Testo);

                SendMessage(GetDlgItem(hwndDlg, List_Chiave_Jeff), LB_SETCURSEL,0, 0);

                SendDlgItemMessage(hwndDlg, List_Decripta_Jeff, LB_RESETCONTENT, 0, 0); //Scuota la listbox riempita nella precedente decifratura

                return true;
                break;

            }
            case(Bn_Cifra_Jeff):
            {
                if(! (Edit_GetTextLength(GetDlgItem(hwndDlg, Tb_Cifra_Jeff)) > 0 ))
                {
                    return true;
                    break;
                }
                char buffer_cifra[DIM_BUFFER]; //Dimensione serratura 1000
                Reimposta_Buffer(buffer_cifra, DIM_BUFFER);

                SendMessage(GetDlgItem(hwndDlg, Tb_Cifra_Jeff), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_cifra);
                TC = Converte_Vettore_In_Stringa(buffer_cifra, DIM_BUFFER);

                int K = SendMessage(GetDlgItem(hwndDlg, List_Chiave_Jeff), LB_GETCURSEL, 0, 0) +1;

                if(K >= 0 && K <= NUM_RULLI_Jefferson && TC.length() > 0)
                    Cifra_Jefferson(hwndDlg, K);

                return true;
                break;
            }
            case (Bn_Decifra_Jeff):
            {
                char buffer_decifra[DIM_BUFFER]; //Dimensione serratura 1000
                Reimposta_Buffer(buffer_decifra, DIM_BUFFER);

                SendMessage(GetDlgItem(hwndDlg, Tb_Decifra_Jeff), WM_GETTEXT,DIM_BUFFER, (LPARAM)buffer_decifra);
                TC = Converte_Vettore_In_Stringa(buffer_decifra, DIM_BUFFER);

                Decifra_Jefferson(hwndDlg);

                return true;
                break;
            }
            case(Bn_Modifica_Jeff):
            {
                Modifica_Attiva = true;
                DialogBoxParam(0,Fin_Mod_Rullo,0,GestoreProcedure_Fin_Mod_Rullo,0);
                return true;
                break;
            }
            case (Bn_Info_Jeff):
            {
                FinestraPrecedente = hwndDlg;

                DialogBoxParam(0,Fin_Info_Jeff,0,GestoreProcedure_Fin_Info_Jeff,0);
                return true;
                break;
            }
            }
        }
    }

    return false;
}

BOOL CALLBACK GestoreProcedure_Fin_Info_Jeff(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg==WM_CLOSE)
    {
        FinestraPrecedente = hwndDlg;
        EndDialog(hwndDlg, 0);
        return true;
    }
    else if(uMsg == WM_INITDIALOG)
    {
        CaricaLogo

        HBITMAP hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Rullo));
        SendMessage(GetDlgItem(hwndDlg,Id_Imm_Rullo),STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

        return true;
    }
    return false;
}


void Cifra_Jefferson(HWND hwndDlg, int K)
{
    string T_Cifrato;

    RulloJefferson Cifra;
    Cifra.Ins_Key(K);
    Cifra.Ins_T_Chiaro(TC);

    T_Cifrato = Cifra.Leggi_Cifra();
    const char* Testo = T_Cifrato.c_str();

    SendMessage(GetDlgItem(hwndDlg, Tb_Decifra_Jeff), WM_SETTEXT,0, (LPARAM)Testo);

    return;
}

void Decifra_Jefferson(HWND hwndDlg)
{
    string T_Decifrato;
    string App;
    const char* Testo;

    RulloJefferson Decifra;
    Decifra.Ins_T_Criptato(TC);

    T_Decifrato = Decifra.Leggi_Decifra();

    SendDlgItemMessage(hwndDlg, List_Decripta_Jeff, LB_RESETCONTENT, 0, 0); //Scuota la listbox riempita nella precedente decifratura

    for(int a=0; a<T_Decifrato.length(); a+=NUM_RULLI_Jefferson)
    {
        App.clear();
        for(int b=0; b<NUM_RULLI_Jefferson; b++)
            App+=T_Decifrato[a+b];
        Testo = App.c_str();
        SendDlgItemMessage(hwndDlg, List_Decripta_Jeff, LB_ADDSTRING, 0, (LPARAM)Testo); //Aggiunge elemento alla listbox
        Testo = "";
        SendDlgItemMessage(hwndDlg, List_Decripta_Jeff, LB_ADDSTRING, 0, (LPARAM)Testo); //Aggiunge uno spazio bianco per migliorare l'impaginazione
    }
    /*T_Decifrato.clear();
    T_Decifrato = App;

    const char* Testo = T_Decifrato.c_str();

    SendMessage(GetDlgItem(hwndDlg, Tb_Cifra_Jeff), WM_SETTEXT,0, (LPARAM)Testo);
    */
    return;
}

void Visualizza_Animazione_Telegrafo(HWND hwndDlg, string Vettore[]) //Disegna sui quadrati l'animazione
{
    HWND hwndControl;
    HBITMAP hwndBitMap;
    bool Check;

    for(int i=0; i<Pos_Lista_Parole; i++)
    {
        for(int j=0; j<Vettore[i].length(); j++)
        {
            switch(Vettore[i][j])
            {
            case '1': //Nel caso in cui debba essere accesa una luce
            {
                //1 Acceso
                hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led1_Cifra);
                hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_On));
                SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

                //2 Spento
                hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led2_Cifra);
                hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Off));
                SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

                //3 Spento
                hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led3_Cifra);
                hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Off));
                SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

                break;
            }
            case '2': //Nel caso in cui debbano essere accese due luci
            {
                //1 Acceso
                hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led1_Cifra);
                hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_On));
                SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

                //2 Acceso
                hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led2_Cifra);
                hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_On));
                SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

                //3 Spento
                hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led3_Cifra);
                hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Off));
                SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

                break;
            }
            case '3': //Nel caso in cui debbano essee accese tre luci
            {
                //1 Acceso
                hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led1_Cifra);
                hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_On));
                SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

                //2 Acceso
                hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led2_Cifra);
                hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_On));
                SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

                //3 Acceso
                hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led3_Cifra);
                hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_On));
                SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

                break;
            }
            }
            if(!Riposa(600)) break; //Lascio le luci accese 0.6s

            //1 Default
            hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led1_Cifra);
            hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Default));
            SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

            //2 Default
            hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led2_Cifra);
            hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Default));
            SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

            //3 Default
            hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led3_Cifra);
            hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Default));
            SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

            //Se ci sono errori esco
            if(!Riposa(200)) break; //Spengo tutte le luci per 0.2 secondi per far capire che ne sto per segnalare un'altra

        }
        if(i != Pos_Lista_Parole-1) //Se è l'ultimo ciclo evita di fare la pausa di 2 secondi, così permette subito nuove operazioni
            if(!Riposa(1000)) break; //Pausa di 1 secondo fra una parola e l'altra
    }

    //1 Default
    hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led1_Cifra);
    hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Default));
    SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

    //2 Default
    hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led2_Cifra);
    hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Default));
    SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

    //3 Default
    hwndControl=GetDlgItem(hwndDlg,Id_Static_123_Led3_Cifra);
    hwndBitMap=LoadBitmap(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_Led_Default));
    SendMessage(hwndControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hwndBitMap);

    return;
}

bool Riposa(int Riposo) //Elimina bug animazione -- Ritorna true se non ci sono stati errori, falso altrimenti
{
    for(int i=0; i<Riposo; i+=10)
        if(kbhit() || GetAsyncKeyState(1)<0 || GetAsyncKeyState(2)<0) return false;
            else
                Sleep(10);

    return true;
}

void Scrivi_Testo_Cifrato_123(HWND hwndDlg, string Vettore[]) //Serve quando si cifra per scrivere il testo nello static
{
    char Risultato[500];  //Abbiamo impostato il numero di parole da inserire massimo a 100, quindi non possono uscire più di 500 simboli (MAX 5)
    for(int i=0; i<501; i++)
        Risultato[i]='\0';
    char App[20];

    for(int i=0; i<Pos_Lista_Parole; i++)
    {
        Converte_Stringa_In_Vettore(Vettore[i], App, 20);

        if(App[0] != 'F' && App[1] != 'a' && App[2] != 'i' && App[3] != 'l')
        {
            strcat(Risultato, App); //Concatena due stringhe (Il primo deve essere un vettore di char[], il secondo di puntatori char*
            strcat(Risultato, " ");
        }
    }
    SendMessage(GetDlgItem(hwndDlg, Id_Static_123_TCifrato_Cifra), WM_SETTEXT, 0, (LPARAM)Risultato);

    return;
}

/*void Scrivi_Testo_Criptato_123(HWND hwndDlg, string Vettore[]) //Serve quando si fa cancella ultimo nel 123 decifra
{
    char Risultato[500];  //Dimensione serratura
    for(int i=0; i<501; i++)
        Risultato[i]='\0';
    char App[5];

    for(int i=0; i<Pos_Lista_Parole; i++)
    {
        Converte_Stringa_In_Vettore(Vettore[i], App, 5);

        strcat(Risultato, App);
        strcat(Risultato, " ");
    }
    SendMessage(GetDlgItem(hwndDlg, Id_Static_123_TestoSottoposto_Decifra), WM_SETTEXT, 0, (LPARAM)Risultato);

    return;
}*/

void Seleziona_Da_ListBox(HWND hwndDlg)
{
    char Temp[50];
    Reimposta_Buffer(Temp, 50);
    int SelIndex = SendMessage(GetDlgItem(hwndDlg, List_Tavola_Cifra), LB_GETCURSEL, 0, 0);
    if (SelIndex != LB_ERR)
        SendMessage(GetDlgItem(hwndDlg, List_Tavola_Cifra), LB_GETTEXT, (int)SelIndex, (LPARAM)(LPSTR)Temp);

    //DEBUG
    /*fstream F;
    F.open("DEBUG_Selezione_ListBox.txt", ios::app);
    F<<"POS: " <<SelIndex <<'\n' <<"Testo: " <<Temp[0];*/

    Lista_Parole[Pos_Lista_Parole] = Converte_Vettore_In_Stringa (Temp, 50);
    Pos_Lista_Parole ++;

    /*F<<'\n' <<"Lista parole di: " <<Pos_Lista_Parole-1 <<" Car: " <<Lista_Parole[Pos_Lista_Parole-1] <<'\n';
    F.close();*/

    SendMessage(GetDlgItem(hwndDlg, List_Tavola_Cifra), LB_SETTOPINDEX, 0, 0); //Riporta la lista all'inizio dopo aver inserito
    //Qui sotto annullo la selezione dopo avero sottoposto la parola
    SendMessage(GetDlgItem(hwndDlg, List_Tavola_Cifra), LB_SETCURSEL, 0, 0);

    Scrivi_Testo_Chiaro_123(hwndDlg);

    return;
}

void Scrivi_Testo_Chiaro_123(HWND hwndDlg) //Serve per scrivere il testo in chiaro nello static in 123 Cifra
{
    char Risultato[2000];  //Abbiamo impostato il numero di parole da inserire massimo a 100, quindi non possono uscire più di 2000 caratteri
    for(int i=0; i<2001; i++)
        Risultato[i]='\0';
    char App[20];

    /*fstream F;
    F.open("Debug_Cifra123.txt", ios::out);*/
    for(int i=0; i<Pos_Lista_Parole; i++)
    {
        //F<<"Parola da lista parole: " <<Lista_Parole[i] <<'\n';

        Converte_Stringa_In_Vettore(Lista_Parole[i], App, 20);
        //F<<"Parola da App: " <<App <<'\n';

        strcat(Risultato, App); //Concatena due stringhe (Il primo deve essere un vettore di char[], il secondo di puntatori char*
        strcat(Risultato, " ");
        //F<<"Risultato: " <<Risultato <<'\n';
    }
    /*F<<'\n' <<"FINE";
    F.close();*/
    SendMessage(GetDlgItem(hwndDlg, Id_Static_123_TChiaro_Cifra), WM_SETTEXT, 0, (LPARAM)Risultato);

    return;
}

BOOL CALLBACK GestoreProcedure_Fin_Mod_Rullo(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //int nrRullo;

    if(uMsg==WM_CLOSE)
    {
        Modifica_Attiva = false;
        EndDialog(hwndDlg, 0);
        return true;
    }
    else if(uMsg == WM_INITDIALOG)
    {
        CaricaLogo

        nrRullo = 1;
        Pos_Mod_Rullo = 0;
        Iniz_Mod_Rullo(hwndDlg);
        Imposta_Label_Riga(hwndDlg, Pos_Mod_Rullo);
        Imposta_Label_NrRullo(hwndDlg, nrRullo);
        return true;
    }
    else if(uMsg==WM_COMMAND) // Se è aperta la finestra di modifica del rullo non permettere nessuna operazione
    {
        WORD wNotifica = HIWORD(wParam);
        WORD wId = LOWORD(wParam);
        if(wNotifica == BN_CLICKED)
        {
            switch(wId)
            {
            case (Bn_Reset_Mod_Rullo):
            {
                //Reimposta il rullo iniziale
                RulloJefferson Reimposta;
                Reimposta.Reimposta_Rullo_STD();

                //Chiude e riapre la finestra per reimpostare le impostazioni della finestra
                EndDialog(hwndDlg, 0);
                DialogBoxParam(0,Fin_Mod_Rullo,0,GestoreProcedure_Fin_Mod_Rullo,0);

                return true;
            }
            case (Bn_CancUltimo_Mod_Rullo):
            {
                if(Pos_Mod_Rullo == 0) return true;
                Pos_Mod_Rullo --;
                char C[2];
                C[0] = Vett_Mod_Rullo[Pos_Mod_Rullo];
                C[1] = '\0';

                SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;

                int ultimo = ListBox_GetCount(GetDlgItem(hwndDlg, List_Visualizz_Mod_Rullo)) - 1;
                ListBox_DeleteString (GetDlgItem(hwndDlg, List_Visualizz_Mod_Rullo), ultimo);

                Imposta_Label_Riga(hwndDlg, Pos_Mod_Rullo);

                return true;
            }
            case (Bn_CancTutto_Mod_Rullo):
            {
                ListBox_ResetContent(GetDlgItem(hwndDlg, List_Scelta_Mod_Rullo));
                ListBox_ResetContent(GetDlgItem(hwndDlg, List_Visualizz_Mod_Rullo));
                Pos_Mod_Rullo = 0;
                Iniz_Mod_Rullo(hwndDlg);

                Imposta_Label_Riga(hwndDlg, Pos_Mod_Rullo);

                return true;
            }
            case (Bn_Back_Mod_Rullo):
            {
                if(nrRullo <= 1) nrRullo = NUM_RULLI_Jefferson;
                else nrRullo--;

                Imposta_Label_NrRullo(hwndDlg, nrRullo);

                return true;
            }
            case (Bn_Next_Mod_Rullo):
            {
                if(nrRullo >= NUM_RULLI_Jefferson) nrRullo = 1;
                else nrRullo++;

                Imposta_Label_NrRullo(hwndDlg, nrRullo);

                return true;
            }
            case (Bn_Applica_Mod_Rullo):
            {
                if(ListBox_GetCount(GetDlgItem(hwndDlg, List_Visualizz_Mod_Rullo)) < DIM_ALF_Jefferson)
                {
                    MessageBox(hwndDlg, "Devi finire di compilare il rullo!", "Errore", MB_OK);
                    return true;
                }

                RulloJefferson ModificaRullo;

                ModificaRullo.Mod_Rullo(Vett_Mod_Rullo, nrRullo);

                MessageBox(hwndDlg, "Rullo modificato con successo!", "Modifica", MB_OK);
                return true;
            }
            case(Bn_Casuale_Mod_Rullo):
            {
                //Svuota momentaneamente le listbox
                ListBox_ResetContent(GetDlgItem(hwndDlg, List_Scelta_Mod_Rullo));
                ListBox_ResetContent(GetDlgItem(hwndDlg, List_Visualizz_Mod_Rullo));
                Pos_Mod_Rullo = 0;
                Iniz_Mod_Rullo(hwndDlg); //Reimposta la listbox di scelta

                int Pos;
                char Appoggio[2];
                Reimposta_Buffer(Appoggio, 2);
                for(int i=0; i<DIM_ALF_Jefferson; i++)
                {
                    Appoggio[0]='\0'; Appoggio[1]='\0';
                    Pos = rand()%(DIM_ALF_Jefferson-i);
                    //Prendo il carattere casuale dalla listbox di sinistra
                    SendMessage(GetDlgItem(hwndDlg, List_Scelta_Mod_Rullo), LB_GETTEXT, (int)Pos, (LPARAM)(LPSTR)Appoggio);
                    //Lo inserisco nella LB di destra
                    SendDlgItemMessage(hwndDlg, List_Visualizz_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)Appoggio);
                    //E lo cancello da dove l'ho preso
                    ListBox_DeleteString (GetDlgItem(hwndDlg, List_Scelta_Mod_Rullo), Pos);
                    //Infine aggiungo il carattere nel vettore di modifica
                    Vett_Mod_Rullo[Pos_Mod_Rullo] = Converte_Vettore_In_Stringa(Appoggio, 2)[0]; //Prende la posizione 0 della stringa (cioè la lettera interessata)
                    Pos_Mod_Rullo ++;

                }
                Imposta_Label_Riga(hwndDlg, Pos_Mod_Rullo);
                return true;
            }
            }
        }

        else if(wNotifica == LBN_DBLCLK)
        {
            switch(wId)
            {
            case (List_Scelta_Mod_Rullo):
            {
                char Testo[2];
                Reimposta_Buffer(Testo, 2);

                int SelIndex = SendMessage(GetDlgItem(hwndDlg, List_Scelta_Mod_Rullo), LB_GETCURSEL, 0, 0);
                if (SelIndex != LB_ERR)
                    SendMessage(GetDlgItem(hwndDlg, List_Scelta_Mod_Rullo), LB_GETTEXT, (int)SelIndex, (LPARAM)(LPSTR)Testo);

                Vett_Mod_Rullo[Pos_Mod_Rullo] = Converte_Vettore_In_Stringa(Testo, 2)[0]; //Prende la posizione 0 della stringa (cioè la lettera interessata)
                Pos_Mod_Rullo ++;

                ListBox_DeleteString (GetDlgItem(hwndDlg, List_Scelta_Mod_Rullo), SelIndex);
                SendDlgItemMessage(hwndDlg, List_Visualizz_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)Testo);

                /*fstream F;
                F.open("DEBUG_MODIFICA.txt", ios::out);
                for(int i=0; i<DIM_ALF_Jefferson; i++)
                    F<<Vett_Mod_Rullo[i] <<'\n';
                F.close();*/

                Imposta_Label_Riga(hwndDlg, Pos_Mod_Rullo);

                return true;
            }
            }
        }
    }

    return false;
}

void Imposta_Label_Riga(HWND hwndDlg, int Pos)
{
    char C[3];
    new_ITOA(Pos, C);

    string Ris_String = "Riga nr. " + Converte_Vettore_In_Stringa(C, 3);

    char Ris_Char[15];
    Converte_Stringa_In_Vettore(Ris_String, Ris_Char, 15);

    SendMessage(GetDlgItem(hwndDlg, Lb_NrRiga_Mod_Rullo), WM_SETTEXT,0, (LPARAM)Ris_Char);

    return;
}

void Imposta_Label_NrRullo(HWND hwndDlg, int NrRullo)
{
    char C[3];
    new_ITOA(NrRullo, C);

    string Ris_String = "Rotore nr. " + Converte_Vettore_In_Stringa(C, 3);

    char Ris_Char[15];
    Converte_Stringa_In_Vettore(Ris_String, Ris_Char, 15);

    SendMessage(GetDlgItem(hwndDlg, Lb_NrRullo_Mod_Rullo), WM_SETTEXT,0, (LPARAM)Ris_Char);

    return;
}

void Iniz_Mod_Rullo(HWND hwndDlg)
{
    char * C;

    C = "A";
    SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox

	C =  "B" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "C" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "D" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "E" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "F" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "G" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "H" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "I" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "J" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "K" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "L" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "M" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "N" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "O" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "P" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "Q" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C = "R" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C = "S"; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "T" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "U" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "V" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "W" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "X" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "Y" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "Z" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "0" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "1" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "2" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "3" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "4" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "5" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "6" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "7" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "8" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;
	C =  "9" ; SendDlgItemMessage(hwndDlg, List_Scelta_Mod_Rullo, LB_ADDSTRING, 0, (LPARAM)C); //Aggiunge elemento alla listbox;

	return;
}

std::string Converte_Vettore_In_Stringa(char Text[], int dim)
{
    int C=0;
    char A;
    std::string RIS;

    while(C<dim)
    {
        A = Text[C];
        if(A == '!') break;
        else RIS += A;
        C++;
    }
    return RIS;
}

void Converte_Stringa_In_Vettore(string Text, char RIS[], int DIMRIS)
{
    char A;
    for(int i=0; i<DIMRIS; i++)
        RIS[i] = '\0';

    for(int i=0; i<Text.length(); i++)
    {
        A = Text[i];
        RIS[i] = A;
    }

    return;
}

void Reimposta_Buffer(char B[], int DIM)
{
    for(int i=0; i<DIM; i++)
        B[i] = '!';
    return;
}

void Ins_Codice_Decifra_123(Cifr123 & Decifra_123,string Codice, string & Parola) //Ritorna vero se la parola è stata trovata, falso altrimenti
{
    string Ris, App;
    int i=0;

    for(i=0; i<Codice.length()-1; i++) //Elimina lo spazio bianco alla fine che non si sa perché veniva aggiunto
        App+=Codice[i];

    Ris = Decifra_123.Push_Word(App);

    /*fstream F;
    F.open("DEBUG_TAVOLE_123_Funz.txt", ios::out);
    F<<"Codice: " <<App <<'\n' <<"Risultato: " <<Ris<<'\n';
    F.close();*/

    Parola = Ris;

    return;
}

bool Is_Alfabeto(char x)
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

void new_ITOA(int N, char C[]) //Funziona da 0 a 36, poteva essere fatto parametrico ma ci avrei solo perso tempo per niente
{
    C[0] = '\0';
    C[1] = '\0';
    C[2] = '\0';
    switch(N)
    {
    case 1:
        C[0] = '1';
        break;
    case 2:
        C[0] =  '2';
        break;
    case 3:
        C[0] =  '3';
        break;
    case 4:
        C[0] =  '4';
        break;
    case 5:
        C[0] =  '5';
        break;
    case 6:
        C[0] =  '6';
        break;
    case 7:
        C[0] =  '7';
        break;
    case 8:
        C[0] =  '8';
        break;
    case 9:
        C[0] =  '9';
        break;
    case 10:
        C[0] =  '1';
        C[1] = '0';
        break;
    case 11:
        C[0] =  '1';
        C[1] = '1';
        break;
    case 12:
        C[0] =  '1';
        C[1] = '2';
        break;
    case 13:
        C[0] =  '1';
        C[1] = '3';
        break;
    case 14:
        C[0] =  '1';
        C[1] = '4';
        break;
    case 15:
        C[0] =  '1';
        C[1] = '5';
        break;
    case 16:
        C[0] =  '1';
        C[1] = '6';
        break;
    case 17:
        C[0] =  '1';
        C[1] = '7';
        break;
    case 18:
        C[0] =  '1';
        C[1] = '8';
        break;
    case 19:
        C[0] =  '1';
        C[1] = '9';
        break;
    case 20:
        C[0] =  '2';
        C[1] = '0';
        break;
    case 21:
        C[0] =  '2';
        C[1] = '1';
        break;
    case 22:
        C[0] =  '2';
        C[1] = '2';
        break;
    case 23:
        C[0] =  '2';
        C[1] = '3';
        break;
    case 24:
        C[0] =  '2';
        C[1] = '4';
        break;
    case 25:
        C[0] =  '2';
        C[1] = '5';
        break;
    case 26:
        C[0] =  '2';
        C[1] = '6';
        break;
    case 27:
        C[0] =  '2';
        C[1] = '7';
        break;
    case 28:
        C[0] =  '2';
        C[1] = '8';
        break;
    case 29:
        C[0] =  '2';
        C[1] = '9';
        break;
    case 30:
        C[0] =  '3';
        C[1] = '0';
        break;
    case 31:
        C[0] =  '3';
        C[1] = '1';
        break;
    case 32:
        C[0] =  '3';
        C[1] = '2';
        break;
    case 33:
        C[0] =  '3';
        C[1] = '3';
        break;
    case 34:
        C[0] =  '3';
        C[1] = '4';
        break;
    case 35:
        C[0] =  '3';
        C[1] = '5';
        break;
    case 36:
        C[0] =  '3';
        C[1] = '6';
        break;
    default:
        C[0] = '0';
        break;
    }

    return;
}
