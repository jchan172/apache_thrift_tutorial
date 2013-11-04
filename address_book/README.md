Running address_book Tutorial
=============================

This is an example demonstrating how to use the Thrift stack to build RPC client and server that stores data. The example is an address book service that saves and reads a Thrift formatted data file. The client accepts user input, and then sends that information to the server. The server will store that data by creating a Thrift binary file or accessing an existing one. Server and client are written in C++ and Python to demonstrate that a Thrift binary file can be accessed by multiple languages.

First generate the C++ and Python code.

    thrift -r --gen cpp addressbook.thrift
    thrift -r --gen py addressbook.thrift

This will create 'gen-cpp' and 'gen-py' code. You can run a server in one language, and connect to it using a client of another language.

Python
------

Make server and client executable (if they're not already executable).

    sudo chmod+x MultiplicationServer.py
    sudo chmod+x MultiplicationClient.py

To run server,

    ./MultiplicationServer.py

To run client,

    ./MultiplicationClient.py


C++
---

First, generate the C++ thrift code from the top level directory.

    thrift -r --gen cpp addressbook.thrift

C++ requires compiling and linking. The commands are performed by running a bash script. The script will also create executable server and client files named `MultiplicationClient` and `MultiplicationServer`.

Make the script executable (if it isn't already).

    sudo chmod+x build.sh

Run script to compile and link files.

    ./build.sh

To run server,

    ./AddressBookServer

To run client,

    ./AddressBookClient


*Reference*

[Apache Thrift C++ Reference](http://wiki.apache.org/thrift/ThriftUsageC%2B%2B)


Usage
-----

Run a server, and then run a client. The client will ask for a file name. For demonstration, simply type "jamesbond" to use the "jamesbond" file located in the address_book level. Note that servers will look for data files from the address_book level instead of the directory they are located in (cpp/ or py/). 

It will then ask whether you want to add person, view all, or quit. Type "V" and enter to view all. It should list the contacts inside the "jamesbond" file.

You can then add a person and view the file again to see that you have added a person to the "jamesbond" file.

To quit and use another file or create another file, type "Q" and run the client again.