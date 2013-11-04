#! /bin/bash

# Remove old files
rm AddressBookClient AddressBookServer

# Change into generated C++ files directory
cd ../gen-cpp/

# Remove old files in generated C++ files directory
rm *.o

# Compile AddressBookServer
g++ -Wall -I/usr/local/include/thrift -c AddressBook.cpp -o AddressBook.o
g++ -Wall -I/usr/local/include/thrift -c ../cpp/AddressBookServer.cpp -o AddressBookServer.o
g++ -Wall -I/usr/local/include/thrift -c addressbook_constants.cpp -o addressbook_constants.o
g++ -Wall -I/usr/local/include/thrift -c addressbook_types.cpp -o addressbook_types.o

# Link AddressBookServer
g++ -L/usr/local/lib *.o -o ../cpp/AddressBookServer -lthrift

# Compile AddressBookClient
g++ -Wall -I/usr/local/include/thrift -c ../cpp/AddressBookClient.cpp -o AddressBookClient.o

# Link AddressBookClient
g++ -L/usr/local/lib AddressBookClient.o AddressBook.o addressbook_constants.o addressbook_types.o -o ../cpp/AddressBookClient -lthrift