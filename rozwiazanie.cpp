#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <wininet.h>

#pragma comment(lib,"Wininet.lib")

using namespace std;

string CzyWolny(string numer) {
	if (numer == "X") return "Wolny";
	else return "Pracy";
}

string Czas(string numer) {
	if (numer == "1") return "06:00";
	if (numer == "2") return "14:00";
	if (numer == "3" || numer == "3.") return "22:00";
	return "Brak informacji";
}

int main()
{
	vector<vector<string>> zawartoscPliku;
	vector<string> rzad;
	string calyrzad, komorka, czywolne;

	fstream plik("Praca.csv", ios::in);
	if (plik.is_open())
	{
		while (getline(plik, calyrzad))
		{
			rzad.clear();

			stringstream doStringa(calyrzad);

			while (getline(doStringa, komorka, ';'))
				if (komorka != "") rzad.push_back(komorka);
			zawartoscPliku.push_back(rzad);
		}
	}

	ofstream wynikXML;
	wynikXML.open("PracaWynik.xml");
	wynikXML << "<?xml version=\"1.0\" encoding=\"Unicode\" ?>" << "\n";
	wynikXML << "<Root xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">" << "\n";
	wynikXML << "\t" << "<DniPlanu>" << "\n";

	for (int i = 3; i < zawartoscPliku.size(); i++)
	{
		for (int j = 1; j < zawartoscPliku[i].size(); j++)
		{
			czywolne = CzyWolny(zawartoscPliku[i][j]);
			wynikXML << "\t" << "\t" << "<DzienPlanu>" << "\n";

			wynikXML << "\t" << "\t" << "\t" << "<Pracownik>" << zawartoscPliku[i][0] << "</Pracownik>" << "\n";

			replace(zawartoscPliku[2][j].begin(), zawartoscPliku[2][j].end(), '-', '/');
			wynikXML << "\t" << "\t" << "\t" << "<Data>" << zawartoscPliku[2][j] << "</Data>" << "\n";

			wynikXML << "\t" << "\t" << "\t" << "<Definicja>" << czywolne << "</Definicja>" << "\n";

			if (czywolne == "Pracy") {

				wynikXML << "\t" << "\t" << "\t" << "<OdGodziny>" << Czas(zawartoscPliku[i][j]) << "</OdGodziny>" << "\n";

				wynikXML << "\t" << "\t" << "\t" << "<Czas>08:00</Czas>" << "\n";

			}

			wynikXML << "\t" << "\t" << "</DzienPlanu>" << "\n";
		}
		cout << "\n";
	}

	wynikXML << "\t" << "</DniPlanu>" << "\n";
	wynikXML << "</Root>" << "\n";
	wynikXML.close();


	HINTERNET polInternet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	//HINTERNET sesjaFTP = InternetConnect(polInternet, "adresHosta", INTERNET_DEFAULT_FTP_PORT, "haslo", "login", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0); //do uzupelnienia danymi
	HINTERNET sesjaFTP = InternetConnectA(polInternet, "192.168.11.39", 2121, "test", "test", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0); //testowanie na moim lokalnym kliencie FTP
	FtpSetCurrentDirectoryA(sesjaFTP, "F:\\222Programy\\VisualStudio\\Aplikacje\\Test\\");


	FtpPutFileA(sesjaFTP, "PracaWynik.xml", "PracaWynik.xml", FTP_TRANSFER_TYPE_BINARY, 0);

	InternetCloseHandle(sesjaFTP);
	InternetCloseHandle(polInternet);

	return 0;
}