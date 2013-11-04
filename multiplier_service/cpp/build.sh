#! /bin/bash

# Remove old files
rm MultiplicationServer MultiplicationClient

# Change into generated C++ files directory
cd ../gen-cpp/

# Remove old files in generated C++ files directory
rm *.o

# Compile AddressBookServer
g++ -Wall -I/usr/local/include/thrift -c MultiplicationService.cpp -o MultiplicationService.o
g++ -Wall -I/usr/local/include/thrift -c ../cpp/MultiplicationServer.cpp -o MultiplicationServiceServer.o
g++ -Wall -I/usr/local/include/thrift -c multi_constants.cpp -o multi_constants.o
g++ -Wall -I/usr/local/include/thrift -c multi_types.cpp -o multi_types.o

# Link AddressBookServer
g++ -L/usr/local/lib *.o -o ../cpp/MultiplicationServer -lthrift
# Note: I had to run `export LD_LIBRARY_PATH=/usr/bin/local/` as a hack to load the library path.

# Compile AddressBookClient
g++ -Wall -I/usr/local/include/thrift -c ../cpp/MultiplicationClient.cpp -o MultiplicationClient.o

# Link AddressBookClient
g++ -L/usr/local/lib MultiplicationClient.o MultiplicationService.o multi_constants.o multi_types.o -o ../cpp/MultiplicationClient -lthrift