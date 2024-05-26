#include <iostream>
#include "Machine.h"
using namespace std;


string generateRandomAlphabet() {
	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	int randomIndex = rand() % alphabet.size();
	return alphabet.substr(randomIndex, 1);
}

class DHT {
public:
	Machine machine;
	DHT() {};
	void start() {
		srand(time(0));
		takeInput();//5 machines for testing//
		if (choice == 1) {
			for (int i = 0; i < numMachines; i++) {
				cout << "Enter name for machine " << i << " in lowercase letters: ";
				string n1;
				cin >> n1;
				machine.insertMachine(n1);
				node* temp = machine.searchByName(n1);
				cout << "\nMachine inserted with name " << temp->name << " and hash " << temp->data << endl;
			}
			machine.print();
			cout << endl;
		}
		if (choice == 2) {
			string temp;
			for (int i = 0; i < numMachines; i++) {
				int random = rand() % 10 + 1;
				string randomAlphabet = generateRandomAlphabet();
				temp += to_string(i + random) + randomAlphabet;
				machine.insertMachine(temp);
			}
			autoInsert = true;
			machine.print();
			cout << endl;
		}
		routingTableOrinsertFile();
		while (true) {
			if (numm == 1) {
				cout << "Enter name of machine: ";
				string talk;
				cin >> talk;
				machine.printTable(talk);
			}
			if (numm == 2) {
				cout << "Insert existing file or new file? Press 1 or 2: ";
				int jod = 0;
				cin >> jod;
				if (jod == 1) {
					cout << "Enter name of existing file: ";
					string Efile;
					cin >> Efile;
					
					machine.insertExistingFile(Efile);
				}
				if (jod == 2) {
					cout << "Enter file name: ";
					string Efile;
					cin >> Efile;
					cout << "Enter content of file: ";
					string cont;
					cin.ignore();
					getline(cin, cont);
					cout << cont << endl;
					machine.insertFile(Efile, cont);
				}
			}
			if (numm == 3) {
				cout << "Enter name of machine: ";
				string mach;
				cin >> mach;
				machine.printTree(mach);
			}
			if (numm == 4) {
				if (machine.size >= pow(2, identifier_space)) {
					cout << "Sorry max number of machines inserted!";
					break;
				}
				else {
					cout << "Enter name of machine: ";
					string str;
					cin >> str;
					machine.insertMachine(str);
					machine.print();
				}
			}
			if (numm == 5) {
				cout << "Enter name of machine: ";
				string str;
				cin >> str;
				machine.deleteMachine(str);
			}
			if (numm == 6) {
				cout << "Enter key of file: ";
				BigInt str;
				cin >> str;
				machine.deleteF(str);
			}
			if (numm == 7) {
				cout << "Enter hash of file you want to search: ";
				BigInt temp;
				cin >> temp;
				cout << "Enter machine you want to start searching from: ";
				string str;
				cin >> str;
				machine.search(temp, str);
			}
			if (numm == 0)
				break;
			if (numm < 0 || numm > 7) {
				cout << "Invalid number! Try again" << endl;
				routingTableOrinsertFile();
			}
				
			cout << "\n\n";
			routingTableOrinsertFile();
		}
	}
};
