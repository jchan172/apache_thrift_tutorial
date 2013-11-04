// include the generated code for address book
#include "../gen-cpp/AddressBook.h"
// include the Thrift code
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
// include some handy libraries
#include <iostream> // provides 'cout'
#include <fstream> // provides 'ofstream' and 'ifstream'
#include <sstream> // provides 'stringstream'

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace apache::thrift::concurrency;

using boost::shared_ptr;

using namespace addressbook;

class AddressBookHandler : virtual public AddressBookIf {
private:
	// This function serializes a Thrift object into Thrift binary string
	// so that object can be written to filesystem.
	// Arguments:
	//   obj: Thrift object you want to serialize
	//	 filename: 
	// Output:
	//   serialized_string: serialized string version of the input
	string serialize(ContactsList& obj) {
		// Create a memory buffer.
		shared_ptr<TMemoryBuffer> transportOut(new TMemoryBuffer());
		// Wrap the memory buffer in compact protocol.
		shared_ptr<TCompactProtocol> protocolOut(new TCompactProtocol(transportOut)); // Alternatively, use binary protocol: shared_ptr<TBinaryProtocol> protocolOut(new TBinaryProtocol(transportOut));
		// Get pointer to protocol and feed it to input object's .write() method. Object basically writes into buffer.
		obj.write(protocolOut.get());
		// Get the buffer's contents as a string.
		string serialized_string = transportOut->getBufferAsString();
		// Return that string.
		return serialized_string;
	}

	// This function deserializes a Thrift binary file back into an object
	// Arguments:
	//   object: empty version Thrift object you want to get back
	//   input: the string you want to deserialize
	//	 input_length: the length of the string you want to deserialize
	// Output:
	//   object: populated Thrift object
	ContactsList deserialize(ContactsList& object, char* input, int input_length) {
		// Create a memory buffer from input (input_copy because TMemoryBuffer needs to be initialized with unsigned char*).
		shared_ptr<TMemoryBuffer> transportIn(new TMemoryBuffer((unsigned char*)input, input_length, TMemoryBuffer::OBSERVE));
		// Wrap memory buffer in compact protocol.
		shared_ptr<TCompactProtocol> protocolIn(new TCompactProtocol(transportIn)); // Alternatively, use binary protocol: shared_ptr<TBinaryProtocol> protocolIn(new TBinaryProtocol(transportIn));
		// Get pointer to protocol and feed it into object's .read() method. Object reads memory buffer and populates itself.
		object.read(protocolIn.get()); // can use .get() method because protocolIn is a boost shared_ptr
		// Return the populated object.
		return object;
	}

	string intToString(int number) {
		stringstream ss; // Create a stringstream
		ss << number; // Add number to the stream
		return ss.str(); // Return a string with the contents of the stream
	}

public:
	AddressBookHandler() {
		// Initialization (nothing to initialize)
	}

	void listAll(std::string& _return, const std::string& filename) {
		// Create a reader to read file.
		ifstream reader(filename.c_str(), std::ifstream::binary);
		// Get length of file by setting reader position to end of file, saving position (which is length of file), then moving reader back to beginning position)
		reader.seekg (0, reader.end);
		int length = reader.tellg();
		reader.seekg (0, reader.beg);

		// Check if file is empty before doing anything. Main task here is to load contacts from file.
		if (length > 0) {
			// Read file.
			char* contacts_binary = new char[length]();	// Create empty string to save contents of file.
			reader.read(contacts_binary, length); // Read the file into empty contacts_binary string.
			reader.close(); // Close the reader and file.

			// Deserialize file and load contents into 'contacts' variable.
			ContactsList contacts;
			contacts = deserialize(contacts, contacts_binary, length); // Load the contacts from address book file.

			// Build up string of contacts.
			for (std::vector<Person>::iterator contact = contacts.list_of_people.begin(); contact != contacts.list_of_people.end(); ++contact) {
				_return += "Name: " + contact->name + '\n';
				_return += "    ID: " + intToString(contact->id) + '\n';
				_return += "    Phone: " + contact->number.number + '\n';
			}
			_return += "Done listing.";
		} else {
			_return = filename + ": Error reading file.";
		}
	}

	void addContact(std::string& _return, const Person& person, const std::string& filename) {
		ContactsList contacts;

		// Create a reader to read file.
		ifstream reader(filename.c_str(), std::ifstream::binary);
		// Get length of file by setting reader position to end of file, saving position (which is length of file), then moving reader back to beginning position)
		reader.seekg (0, reader.end);
		int length = reader.tellg();
		reader.seekg (0, reader.beg);

		// Check if file is empty before doing anything. Main task here is to load contacts from file.
		if (length > 0) {
			// Read file.
			char* contacts_binary = new char[length]();	// Create empty string to save contents of file.
			reader.read(contacts_binary, length); // Read the file into empty contacts_binary string.
			reader.close(); // Close the reader and file.

			// Deserialize file and load contents into 'contacts' variable.
			contacts = deserialize(contacts, contacts_binary, length);	// Load the contacts from address book file.
		} else {
			cout << filename + ": File not found. Creating new file." << endl;
		}

		// Add person to contacts and serialize
		contacts.list_of_people.push_back(person); // Append the new person to the loaded contacts.
		string serialized = serialize(contacts); // Serialize the contacts into Thrift binary so that it's ready to be written out to disk.

		// Write the contacts out to disk
		ofstream writer(filename.c_str(), std::ofstream::binary); // Create a file writer for writing to disk.
		writer.write(serialized.c_str(), serialized.size()); // Write the serialized string to disk.
		writer.close(); // Close file writer.
		_return = "Done.";
	}

};

int main(int argc, char **argv) {
	int port = 9090;
	shared_ptr<AddressBookHandler> handler(new AddressBookHandler()); // Create a handler for AddressBook.
	shared_ptr<TProcessor> processor(new AddressBookProcessor(handler)); // Create a processor.
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port)); // Create a socket using specified port.
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory()); // Create a TBufferedTransportFactory
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory()); // Use binary protocol

	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory); // Create a simple server.
	// printf("Starting server...\n");
	cout << "Starting the Address Book server...\n";
	server.serve();
	cout << "Done.\n";
	return 0;

	// Alternatively, you can use a TNonblockingServer to accept a lot of connections while throttling processor threads using a pool.
	// There were some linker errors when I tried this.
	/*
	int port = 9090;
	shared_ptr<AddressBookHandler> handler(new AddressBookHandler());
	shared_ptr<TProcessor> processor(new AddressBookProcessor(handler));
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	// using thread pool with maximum 15 threads to handle incoming requests
	int threads = 15;
	shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(threads);
	shared_ptr<PosixThreadFactory> threadFactory = shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
	threadManager->threadFactory(threadFactory);
	threadManager->start();

	TNonblockingServer server(processor, protocolFactory, port, threadManager);
	// printf("Starting server...\n");
	cout << "Starting server...\n";
	server.serve();
	cout << "Done.\n";
	return 0;
	*/
}

