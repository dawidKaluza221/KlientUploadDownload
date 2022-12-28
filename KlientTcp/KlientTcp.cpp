// KlientTcp.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//



#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <windows.h>

#include <winsock2.h>

#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")



using namespace std;

int main(void)
{
	WSADATA wsaData;
	// inicjalizacja żądanej wersja biblioteki WinSock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	
	int conSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//zmienna do łączenia do klienta
	if (conSocket < 0) //błąd
	{
		perror("Error at socket:"); return 1;
	}
	struct sockaddr_in clientService;
	memset(&clientService, '\0', sizeof clientService);
	clientService.sin_family = AF_INET; //IP4
	inet_pton(AF_INET, "127.0.0.1", &(clientService.sin_addr));
	clientService.sin_port = htons(2000);//port
	if (connect(conSocket, (struct sockaddr*)&clientService, sizeof(clientService)) < 0)//błąd połączenia
	{
		perror("Error at connect:"); return 1;
		closesocket(conSocket);
		WSACleanup();
		return 1;
	}

	FILE* file;
	errno_t err;
	
	int reading_size;
	int const buffer_size = 512;//wielkośc bufora do odczytu z pliku
	char buf[buffer_size];//tablica char do odczytu z pliku
	string buffer1;
	int sendsize = 0;
	int part = 0;
	int wielkosc=0 ;
	int odczytane = 0;
	int zmienna;
	int RecvSize=1;
	int FSize=0;
	string NazwaPliku;
	do 
	{
		cin.sync();
		cout << "Dla wysłania pliku kliknij 1 dla odebrania pliku nacisnij 2" << endl;
		cin >> zmienna;
	} while (!(zmienna == 1 || zmienna == 2));
	cout << "wybierz nazwe pliku" << endl;
	cin >> NazwaPliku;

	char tab1[sizeof(NazwaPliku)];
	char tablica1[buffer_size];
	switch (zmienna) 
	{
	case 1:
		buffer1 = "UPLOAD\n";
		strcpy_s(tab1, NazwaPliku.c_str());
		if ((err = fopen_s(&file,tab1, "rb")) != 0)
		{
			std::cout << "Couldn't open the file."<<endl;
			buffer1 += "plik tekstowy nie istnieje u klienta";
			if ((sendsize = send(conSocket, buffer1.c_str(), sizeof(buffer1), 0)) < 0)//wysyłanie poprzez send tablicy buf o wielkosci reading_size(czyli tyle ile odczytaliśmy z tablicy)
			{//send zwraca liczbe odczytanych wilekosc 

				cout << ("Error with sending data");
			}
		}//otwarcie folderu 
		else 
		{
			do {
				if ((reading_size = fread(tablica1, sizeof(char), buffer_size, file)) < 0) {//odczyt z pliku tekstu do tablicy char o wielkosci buffer_size 
					cout << ("Error with reading data") << endl;

				}
				buffer1 += string(tablica1);
				if ((sendsize = send(conSocket, buffer1.c_str(), reading_size + 7, 0)) < 0)//wysyłanie poprzez send tablicy buf o wielkosci reading_size(czyli tyle ile odczytaliśmy z tablicy)
				{//send zwraca liczbe odczytanych wilekosc 

					cout << ("Error with sending data");
				}
			} while (reading_size > 0);
		}
			break;
		
	case 2:
		
		buffer1 = "DOWNLOAD:" + NazwaPliku + '\n';
		char tab[sizeof(buffer1)];
		strcpy_s(tab, buffer1.c_str());
		strcpy_s(tab1, NazwaPliku.c_str());
		send(conSocket, tab, sizeof(buffer1), 0);
		err = fopen_s(&file, tab1, "wb");
		if (err != 0) {
			cout << ("Error opening file!") << endl;
		}
		RecvSize = recv(conSocket, buf, buffer_size, 0);
		if (RecvSize>0) 
		{

			FSize = fwrite(buf, sizeof(char), RecvSize, file);
		}

		break;
	}
		

	closesocket(conSocket);
	WSACleanup(); //na koniec programu zwalniamy interfejs
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
