#include <iostream>
#include <regex>
#include <fstream>
using namespace std;

class Uzytkownik
{
	string email;
	string haslo;
	regex emailR;
	regex hasloR;
public:
	Uzytkownik()
		:emailR("^(?=.{5,254})(?:[a-z]+[a-zA-Z\\d_!#$%&'*+-/=?^_`{|}~]*){1,63}@(?:[a-z]+[a-z\\d_-]*\.)+[a-z]+$"),
		hasloR("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?=.*[!_$?-])[A-Za-z\\d!_$?-]{6,20}$")
	{
		pob_email(emailR);
		pob_haslo(hasloR);
	}

	void pob_email(regex r)
	{
		cout << "Podaj mail: ";
		string mailik;
		cin >> mailik;
		while (!regex_match(mailik, r))
		{
			cout << "Podany email nie jest poprawny, sprobuj ponownie" << endl;
			cin >> mailik;
		}
		email = mailik;
	}

	void pob_haslo(regex r)
	{
		cout << "Podaj haslo: ";
		string haselko;
		cin >> haselko;
		while (!regex_match(haselko, r))
		{
			cout << "Podane haslo nie jest poprawne, sprobuj ponownie" << endl;
			cin >> haselko;
		}
		haslo = haselko;
	}

	void zapisz(std::string file_name)
	{
		fstream plik;
		plik.open(file_name);
		plik << email << endl << haslo;
		plik.close();
	}
};