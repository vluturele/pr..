#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "resource1.h"

const char g_szClassName[] = "proiect tp";
#define ID_button1 1000
#define ID_button2 1001
#define ID_button3 1002
#define ID_button_ex 2000



// aici definim un produs, care e nod pentru heapul de produse
typedef struct produs
{
	char* nume;
	int pret;
} produs;

// functie care returneaza un pointer catre un nou produs
produs* nou_produs(char* nume, int pret)
{
	produs* p = (produs*)malloc(sizeof(produs));
	p->nume = (char*)malloc(strlen(nume) + 1);
	strcpy(p->nume, nume);
	p->pret = pret;
	return p;
}

// o functie care interschimba pointerii a 2 produse;
void interschimba(produs** a, produs** b)
{
	produs* temp = *a;
	*a = *b;
	*b = temp;
}

// aici definim heapul care contine produse
typedef struct heap
{
	produs* curent;
	heap* stanga;
	heap* dreapta;
} heap;

//functie de creare heap
heap* nou_heap(produs* p)
{
	heap* h = (heap*)malloc(sizeof(heap));
	h->curent = p;
	h->stanga = NULL;
	h->dreapta = NULL;
	return h;
}

// inserarea unui produs in heap
// e o formula: iei numarul de noduri, adaugi 1, il scrii in baza 2 (numa 0 si 1), elimini prima cifra si parcurgi cifra cu cifra ce ai obtinut;
// daca dai de 0, mergi in jos la stanga pe heap; daca dai de 1, la dreapta; cand nu mai ai cifre, ai ajuns in locul in care trebuie inserat un nou nod;
void insereaza(heap* h, produs* p, int numar, int putere)
{
	if (p->pret < h->curent->pret)
	{
		interschimba(&p, &h->curent);
	}

	if (numar - putere >= 0)
	{
		if (putere / 2 > 0)
		{
			insereaza(h->dreapta, p, numar - putere, putere / 2);
		}
		else
		{
			h->dreapta = nou_heap(p);
		}
	}
	else
	{
		if (putere / 2 > 0)
		{
			insereaza(h->stanga, p, numar, putere / 2);
		}
		else
		{
			h->stanga = nou_heap(p);
		}
	}
}

// functie de afisare a heapului
void afiseazaproduse(heap* h,char *infsite)
{
	if (h->curent != NULL)
	{
		strcat(infsite, h->curent->nume);
		strcat(infsite, " ");
	}
	if (h->stanga != NULL)
	{
		afiseazaproduse(h->stanga,infsite);
	}
	if (h->dreapta != NULL)
	{
		afiseazaproduse(h->dreapta,infsite);
	}
}

// functie care returneaza produsul cu pret minim din heap
produs* minim(heap* h)
{
	return h->curent;
}

// aici definim categoria, care e nod pentru lista de categorii
typedef struct
{
	char* nume;
	heap* produse;
	int numar;
} categorie;

categorie* noua_categorie(char* nume)
{
	categorie* c = (categorie*)malloc(sizeof(categorie));
	c->nume = (char*)malloc(strlen(nume) + 1);
	strcpy(c->nume, nume);
	c->produse = nou_heap(NULL);
	c->numar = 0;
	return c;
}

// functie care baga produsul p in heapul categoriei c
void insereaza_pc(categorie* c, produs* p)
{
	if (c->numar == 0)
	{
		c->produse->curent = p;
	}
	else
	{
		// vrem cea mai mare putere a lui 2 mai mica decat numarul de noduri + 1
		int putere = 1;
		while (putere <= c->numar + 1)
		{
			putere *= 2;
		}
		putere /= 2;
		insereaza(c->produse, p, c->numar + 1 - putere, putere / 2);
	}

	c->numar++;
}


// functie care returneaza produsul cu pret minim din categorie
produs* minim(categorie* c)
{
	return minim(c->produse);
}

// aici definim lista care contine categorii
typedef struct categorii
{
	categorie* curenta;
	categorii* urmatoarea;
} categorii;

categorii* noua_lista_categorii(categorie* i)
{
	categorii* c = (categorii*)malloc(sizeof(categorii));
	c->curenta = i;
	c->urmatoarea = NULL;
	return c;
}


// functie care insereaza o noua categorie in lista de categorii
void insereaza_cc(categorii* c, categorie* i)
{
	if (c->curenta == NULL)
	{
		c->curenta = i;
	}
	else
	{
		if (c->urmatoarea == NULL)
		{
			c->urmatoarea = noua_lista_categorii(i);
		}
		else
		{
			insereaza_cc(c->urmatoarea, i);
		}
	}
}


// functie care returneaza produsul cu pret minim dintr-o categorie din lista de categorii
produs* minim(categorii* c, char* nume)
{
	if (c->curenta != NULL && strcmp(c->curenta->nume, nume) == 0)
	{
		return minim(c->curenta);
	}
	if (c->urmatoarea != NULL)
	{
		return minim(c->urmatoarea, nume);
	}
	return NULL;
}

// aici definim structura unui site, care e nod pentru lista de siteuri
typedef struct
{
	char* nume;
	categorii* lista_categorii;
} site;

site* nou_site(char* nume)
{
	site* s = (site*)malloc(sizeof(site));
	s->nume = (char*)malloc(strlen(nume) + 1);
	strcpy(s->nume, nume);
	s->lista_categorii = noua_lista_categorii(NULL);
	return s;
}

// aici definim lista de site-uri
typedef struct siteuri
{
	site* curent;
	struct siteuri* urmatorul;
} siteuri;

siteuri* noua_lista_siteuri(site* i)
{
	siteuri* s = (siteuri*)malloc(sizeof(siteuri));
	s->curent = i;
	s->urmatorul = NULL;
	return s;
}

// functie care insereaza un nou site in lista
void insereaza_site(siteuri* s, site* i)
{
	if (s->curent == NULL)
	{
		s->curent = i;
	}
	else
	{
		if (s->urmatorul == NULL)
		{
			s->urmatorul = noua_lista_siteuri(i);
		}
		else
		{
			insereaza_site(s->urmatorul, i);
		}
	}
}

// functie care incarca un site
void incarca(siteuri* s, char* nume)
{
	site* i = nou_site(nume);
	insereaza_site(s, i);

	FILE* fisier = fopen(nume, "r");
	char linie[2048];

	const char* delimitatori = " -\n";
	while (fgets(linie, 2048, fisier) != NULL)
	{
		char* token = strtok(linie, delimitatori);

		categorie* j = noua_categorie(token);
		insereaza_cc(i->lista_categorii, j);

		token = strtok(NULL, delimitatori);
		while (token != NULL)
		{
			insereaza_pc(j, nou_produs(token, strtol(strtok(NULL, delimitatori), NULL, 10)));

			token = strtok(NULL, delimitatori);
		}
	}
}

// site-ul produsului minim global;
site* siteul_cerut = NULL;

// functie care returneaza minimul dinre 2 produse;
produs* minim(produs* p1, produs* p2, site* s1, site*s2)
{
	if (p2 == NULL)
	{
		siteul_cerut = s1;
		return p1;
	}
	if (p1 == NULL)
	{
		siteul_cerut = s2;
		return p2;
	}

	if (p1->pret <= p2->pret)
	{
		siteul_cerut = s1;
		return p1;
	}
	else
	{
		siteul_cerut = s2;
		return p2;
	}
}

// functie care afla minimul global dintr-o categorie si site-ul in care se gaseste
produs* minim(siteuri* s, char* nume)
{
	if (s != NULL && s->curent != NULL)
	{
		if (s->urmatorul != NULL)
		{
			return minim(minim(s->curent->lista_categorii, nume), minim(s->urmatorul, nume), s->curent, s->urmatorul->curent);
		}
		else
		{
			siteul_cerut = s->curent;
			return minim(s->curent->lista_categorii, nume);
		}
	}
	else
	{
		return NULL;
	}
}

//afisarea minimului
void afiseaza_minimul(siteuri* s, char* nume,char *pr_st)
{
	produs* p = minim(s, nume);
	if (p != NULL)
	{
		strcpy(pr_st, "Site: ");
		strcat(pr_st, siteul_cerut->nume);
		strcat(pr_st, "\r\nProdus: ");
		strcat(pr_st, p->nume);
	}
}

siteuri *siteslist = noua_lista_siteuri (NULL);
int nrsiteuri = 0;
char infsite[200];
//functie de afisare continut site
void afiscontinutsite(char *numesite)
{
	siteuri *p,*gasit=NULL;
	p = siteslist;
	while (p != NULL)
	{
		if (strcmp(p->curent->nume, numesite) == 0) 
		{
			gasit = p;
			break;
		}
		p = p->urmatorul;
	}
	if (gasit != NULL)
	{
		categorii *c = gasit->curent->lista_categorii;
		strcpy(infsite, c->curenta->nume);
		strcat(infsite, "\r\n");
		afiseazaproduse(c->curenta->produse, infsite);
		strcat(infsite, "\r\n");
		c = c->urmatoarea;
		while (c!= NULL)
		{
			strcat(infsite, c->curenta->nume);
			strcat(infsite, "\r\n");
			afiseazaproduse(c->curenta->produse, infsite);
			strcat(infsite, "\r\n");
			c = c->urmatoarea;
		}
	}
}
//verific daca un site a fost sau nu introdus
int sitedejainserat(char *numesite)
{
	int exista = 0;
	siteuri *p;
	p = siteslist;
	if (p->curent == NULL) return 0;
	while (p!= NULL)
	{
		if (strcmp(p->curent->nume, numesite) == 0)
		{
			exista = 1;
			break;
		}
		p = p->urmatorul;
	}
	return exista;
}
//primul dialog 
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:
	{
							  return TRUE; 
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
		{
							int len;
							char *numesite,*numefissite;
							len = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT1));
							if (len == 0) MessageBox(hwnd, "Nu ai introdus nimic!", "Atentionare", MB_OK);
							else
							{
								numesite = (char*)malloc(len + 1);
								GetDlgItemText(hwnd, IDC_EDIT1, numesite, len + 1);
								numefissite = (char*)malloc(len + 5);
								strcpy(numefissite, numesite);
								strcat(numefissite, ".txt");
								FILE *f = fopen(numefissite, "r");
								if (f == NULL) MessageBox(hwnd, "Nu exista acest site!", "Atentionare", MB_OK);
								else if (sitedejainserat(numefissite)==0 )
								{
									incarca(siteslist, numefissite);
									nrsiteuri++;
									fclose(f);
									MessageBox(hwnd, "Site adaugat cu succes!", "Atentionare", MB_OK);
									EndDialog(hwnd, IDD_DIALOG1);
								}
								else if (sitedejainserat(numefissite) == 1) 
								{
									MessageBox(hwnd, "Site-ul exista deja in lista de siteuri!", "Atentionare", MB_OK);
									fclose(f);
									EndDialog(hwnd, IDD_DIALOG1);
								}
							}
							break;

		}
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, IDD_DIALOG1);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
//dialog 2
BOOL CALLBACK dlg(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{//apre imediat dupa deschiderea dialogului
	case WM_INITDIALOG:
	{
						  char ss[100];
						  siteuri *p = siteslist;
						  strcpy(ss, p->curent->nume);
						  p = p->urmatorul;
						  while (p != NULL)
						  {
							  strcat(ss, "\r\n");
							  strcat(ss, p->curent->nume);
							  p = p->urmatorul;
						  }
						  SetDlgItemText(hwnd, ID_EDIT3, ss);
	}
	case WM_COMMAND:
	{
	    switch (LOWORD(wParam))
		{
		case ID_BUTTON1:
		{
						   int len;
						   char *sitecaut;
						   len = GetWindowTextLength(GetDlgItem(hwnd, ID_EDIT2));
						   if (len == 0) MessageBox(hwnd, "Nu ai introdus nimic!", "Atentionare", MB_OK);
						   else
						   {
								   sitecaut = (char*)malloc(len + 1);
								   GetDlgItemText(hwnd, ID_EDIT2, sitecaut, len + 1);
								   if (sitedejainserat(sitecaut) != 0)
								   {
									   afiscontinutsite(sitecaut);
									   SetDlgItemText(hwnd, ID_EDIT1, infsite);
								   }
								   else MessageBox(hwnd, "Nu exista acest site in lista!", "Atentionare", MB_OK|MB_ICONEXCLAMATION);
						   }
						   break;
		}
		
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, IDD_DIALOG2);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
//dialog 3
BOOL CALLBACK dlg2(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_INITDIALOG:
	{
						  return TRUE;
	}
	case WM_COMMAND:
	{
					   switch (LOWORD(wParam))
					   {
					   case IDC_BUTTON1:
					   {
									   int len;
									   char *categ;
									   len = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT1));
									   if (len == 0) MessageBox(hwnd, "Nu ai introdus denumirea categoriei!", "Atentionare", MB_OK | MB_ICONEXCLAMATION);
									   else
									   {
										   categ = (char*)malloc(len + 1);
										   char min[50];
										   GetDlgItemText(hwnd, IDC_EDIT1, categ, len + 1);
										   afiseaza_minimul(siteslist, categ, min);
										   SetDlgItemText(hwnd, IDC_EDIT2, min);
									   }
									   break;
					   }
					   }
					   break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, IDD_DIALOG3);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND h;
	static int aps = 0;
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		{
		case ID_button1:
		{
						   int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutDlgProc);
						   if (ret == -1) MessageBox(hwnd, "Eroare creare dialog", "Eroare", MB_OK | MB_ICONERROR);
						   break;
		}
		case ID_button2:
		{
						   if (siteslist->curent != NULL)
						   {
							   int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwnd, dlg);
							   if (ret == -1) MessageBox(hwnd, "Eroare creare dialog", "Eroare", MB_OK | MB_ICONERROR);
						   }
						   else MessageBox(hwnd, "Nu exista niciun site in lista!", "Atentionare", MB_OK|MB_ICONEXCLAMATION);
						   break;
		}
		case ID_button3:
		{
						   if (siteslist->curent != NULL)
						   {
							   int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hwnd, dlg2);
							   if (ret == -1) MessageBox(hwnd, "Eroare creare dialog", "Eroare", MB_OK | MB_ICONERROR);
						   }
						   else MessageBox(hwnd, "Nu exista niciun site in lista!", "Atentionare", MB_OK | MB_ICONEXCLAMATION);
						   break;
		}
		case ID_button_ex:
		{
							 DestroyWindow(hwnd);
							 break;
		}
		}
		default:
			break;
		}
	case WM_CREATE:
	{
					  ////////Buton adaugare site
					  h = CreateWindowEx(WS_EX_WINDOWEDGE, "BUTTON", "Adauga site", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 10, 90, 40, hwnd, (HMENU)ID_button1, NULL, NULL);
					  if (h == NULL) MessageBox(hwnd, "Butonul1 nu a fost creat", "Eroare", MB_OK | MB_ICONERROR);

					  /////////////Buton afisare site
					  h = CreateWindowEx(WS_EX_WINDOWEDGE, "BUTTON", "Afiseaza site", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 60, 90, 40, hwnd, (HMENU)ID_button2, NULL, NULL);
					  if (h == NULL) MessageBox(hwnd, "Butonul2 nu a fost creat", "Eroare", MB_OK | MB_ICONERROR);

					  /////////////Buton cautare produs
					  h = CreateWindowEx(WS_EX_WINDOWEDGE, "BUTTON", "Cauta produs", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 110, 90, 40, hwnd, (HMENU)ID_button3, NULL, NULL);
					  if (h == NULL) MessageBox(hwnd, "Butonul3 nu a fost creat", "Eroare", MB_OK | MB_ICONERROR);

					  ///////Buton exit
					  h = CreateWindowEx(WS_EX_WINDOWEDGE, "BUTTON", "EXIT", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 160, 90, 40, hwnd, (HMENU)ID_button_ex, NULL, NULL);
					  if (h == NULL) MessageBox(hwnd, "Butonul exit nu a fost creat", "Eroare", MB_OK | MB_ICONERROR);
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	// Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Find the cheapest",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 250, 300,
		NULL, NULL, hInstance, NULL);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	// The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
