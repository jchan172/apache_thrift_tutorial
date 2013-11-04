Running Basic multiplier_service Tutorial
=========================================

This is a very basic example demonstrating how to use the Thrift stack to build RPC client and server. A server will be brought up in one language, and then clients in different languages can communicate to it.

First generate the Python, Ruby, and C++ code.

    thrift -r --gen py multi.thrift
    thrift -r --gen rb multi.thrift
    thrift -r --gen cpp multi.thrift

This will create 'gen-py' and 'gen-rb' code. You can then run server in one language and run clients in different languages to see that they work together. For example, run a Python server, and then run the Ruby client. The Python server will perform multiplication that the Ruby client requested.

Python
------

Make server and client executable (if they're not already executable).

    sudo chmod+x MultiplicationServer.py
    sudo chmod+x MultiplicationClient.py

To run server,

    ./MultiplicationServer.py

To run client,

    ./MultiplicationClient.py


Ruby
----

Make server and client executable (if they're not already executable).

    sudo chmod+x MultiplicationServer.rb
    sudo chmod+x MultiplicationClient.rb

To run server,

    ./MultiplicationServer.rb

To run client,

    ./MultiplicationClient.rb

C++
---

First, generate the C++ thrift code from the top level directory.

    thrift -r --gen cpp multi.thrift

C++ requires compiling and linking. The commands are performed by running a bash script. The script will also create executable server and client files named `MultiplicationClient` and `MultiplicationServer`.

Make the script executable (if it isn't already).

    sudo chmod+x build.sh

Run script to compile and link files.

    ./build.sh

To run server,

    ./MultiplicationServer

To run client,

    ./MultiplicationClient


*Reference*

[Apache Thrift C++ Reference](http://wiki.apache.org/thrift/ThriftUsageC%2B%2B)