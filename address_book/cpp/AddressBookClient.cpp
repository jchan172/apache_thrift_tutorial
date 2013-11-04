// include the generated code for address book
#include "../gen-cpp/AddressBook.h"
// include the Thrift code
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
// include some handy libraries
#include <stdio.h> // provides 'printf'
#include <stdlib.h> // provides 'atoi'
#include <iostream> // provides 'getline'

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

// 'addressbook' is the namespace defined in addressbook.thrift
using namespace addressbook;

using namespace boost;


void addContact(AddressBookClient& client, string filename) {
	//cout << "Adding contact.\n";
	string description;
	cout << "Enter person's description: ";
	getline(cin, description);
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
	getline(cin, id);
	int person_id = atoi(id.c_str());
	Person person;
	person.id = person_id;
	person.name = name;
	person.number = phone;
	person.description = desc;

	string response = "";
	client.addContact(response, person, filename);
	cout << "Adding contact..." << endl;
	cout << response << endl;
}

void viewAllUsers(AddressBookClient& client, string filename) {
	string response = "";
	client.listAll(response, filename);
	cout << "Listing contacts..." << endl;
	cout << response << endl;
}

int main(int argc, char** argv) {
	try {
		int port_number = 9090;
		shared_ptr<TSocket> socket(new TSocket("localhost", port_number)); // Make socket.
		shared_ptr<TTransport> transport(new TBufferedTransport(socket)); // Buffering is critical. Raw sockets are very slow.
		shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport)); // Wrap in Thrift protocol.
		AddressBookClient client(protocol); // Create a client to use the protocol encoder.
		transport->open(); // Connect to server.

		// Alternatively, to use with TNonblockingServer, use this code (change TBufferedTransport to TFramedTransport):
		/* 
		int port_number = 9090;
		shared_ptr<TSocket> socket(new TSocket("localhost", port_number)); // Make socket.
		shared_ptr<TTransport> transport(new TFramedTransport(socket)); // Need to use TFramedTransport if communicating with TNonblockingServer.
		shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport)); // Wrap in Thrift protocol.
		AddressBookClient client(protocol); // Create a client to use the protocol encoder.
		transport->open(); // Connect to server.
		*/

		string addr_book_file;
		cout << "Enter address book's filename: ";
		getline(cin, addr_book_file);
		addr_book_file = "../" + addr_book_file;

		bool stop = false;
		while (stop == false) {
			string input;
			cout << "Add person (A), view all (V), quit (Q): ";
			getline(cin, input);

			// Compare the constant char* version of input.
			if (strcmp(input.c_str(), "Q") == 0) {
				transport->close(); // Close connection.
				stop = true; // Break out of while loop.
			} else if (strcmp(input.c_str(), "V") == 0) {
				// Pass data to method that calls a server command to list users.
				viewAllUsers(client, addr_book_file);
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
