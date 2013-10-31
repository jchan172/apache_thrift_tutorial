#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

// include the generated code for address book
#include "../gen-cpp/AddressBook.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

// multi is the namespace defined in multi.thrift
using namespace addressbook;

using namespace boost;


// void quit() // write error message and quit
// {
// 	fprintf(stderr, "memory exhausted\n");
// 	exit(1);
// }

// char* getInput(const char* question) {
// 	int max = 20;
// 	char* response = (char*)malloc(max); // allocate buffer
// 	if (response == 0) {
// 		quit();
// 	}

// 	cout << question;

// 	while (true) { // skip leading whitespace
// 		int c = getchar();
// 			if (c == EOF) break; // end of file
// 		if (!isspace(c)) {
// 			ungetc(c, stdin);
// 			break;
// 		}
// 	}

// 	int i = 0;
// 	while (true) {
// 		int c = getchar();
// 		if (isspace(c) || c == EOF) { // at end, add terminating zero
// 			response[i] = 0;
// 			break;
// 		}
// 		response[i] = c;
// 		if (i==max-1) { // buffer full
// 			max = max+max;
// 			response = (char*)realloc(response,max); // get a new and larger buffer
// 			if (response == 0) quit();
// 		}
// 		i++;
// 	}

// 	return response;
// }

void addContact(AddressBookClient& client, string filename) {
	//cout << "Adding contact.\n";
	string description;
	cout << "Enter person's description: ";
	getline (cin, description);
	Description desc;
	desc.description = description;

	string num;
	cout << "Enter person's phone number: ";
	getline(cin, num);
	PhoneNumber phone;
	phone.number = num;

	string name;
	cout << "Enter person's name: ";
	getline(cin, name);

	string id;
	cout << "Enter person's ID number: ";
	getline (cin, id);
	int person_id = atoi(id.c_str());
	Person person;
	person.id = person_id;
	person.name = name;
	person.number = phone;
	person.description = desc;

	string response = "";
	client.addContact(response, person, filename);
	// printf(response.c_str());
}

void viewAllUsers(AddressBookClient& client) {
	cout << "Viewing all users.\n";
	const string contacts;
	string response = "";
	client.listAll(response, contacts);
}

int main(int argc, char** argv) {
	try {
		// Make socket.
		shared_ptr<TSocket> socket(new TSocket("localhost", 9090));
		// Buffering is critical. Raw sockets are very slow.
		shared_ptr<TTransport> transport(new TBufferedTransport(socket));
		// Wrap in Thrift protocol.
		shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
		// Create a client to use the protocol encoder.
		AddressBookClient client(protocol);
		// Connect to server.
		transport->open();

		string addr_book_file;
		cout << "Enter addres book's filename: ";
		getline(cin, addr_book_file);

		bool stop = false;
		while (stop == false) {
			string input;
			cout << "Add person (A), view all (V), quit (Q): ";
			getline(cin, input);

			// Compare the constant char* version of input.
			if (strcmp(input.c_str(), "Q") == 0) {
				// Close connection.
				transport->close();
				// Break out of while loop.
				stop = true;
			} else if (strcmp(input.c_str(), "V") == 0) {
				// Pass data to method that calls a server command to list users.
				viewAllUsers(client);
			} else if (strcmp(input.c_str(), "A") == 0) {
				// Pass data to method that calls a server command to add contact.
				addContact(client, addr_book_file);
			} 
		}
	} catch (TException& tx) {
		printf("ERROR: %s\n", tx.what());
	}
	return 0;
}
