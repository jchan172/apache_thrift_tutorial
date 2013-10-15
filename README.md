Installing Apache Thrift
========================

This is a guide to install Apache Thrift on Ubuntu 13.04

Install tools first.

	sudo apt-get install libboost-dev libboost-test-dev libboost-program-options-dev libevent-dev automake libtool flex bison pkg-config g++ libssl-dev 

If you want to have java, install `ant` as well (`sudo apt-get install ant`)

Install Ruby using RVM, and then install these:

	ruby-full ruby-dev librspec-ruby rake rubygems libdaemons-ruby  

Clone from the git repo to get the source.

	git clone https://git-wip-us.apache.org/repos/asf/thrift.git thrift

Go into the new 'thrift' directory.

	cd thrift

Generate configure scripts. Note that I had messages at the end that looked like it failed, but it didn't.

	./bootstrap.sh

Configure thrift. There will be a list of languages it's configured to build. For me, it was C++, Python, and Ruby. It will list where it thinks they exist. Make sure it is pointing to the Ruby inside the .rvm folder and NOT the system Ruby located at /usr/bin/ruby

	./configure

Build thrift.

	make

Check that thrift installed correctly. Note that on "slow" systems (VM is probably going to be "slow"), there will be one test called Transport Test where it fails. There should be 1 of 11 tests that fail. 

	make check

Install thrift. Note that this will fail when installing rb, which is ok. Ruby support needs to be installed from lib/rb/.

	sudo make install

Now, install Ruby support by going into lib/rb/ and running `gem install thrift`.

	cd lib/rb/
	gem install thrift

Also install Python support by going into lib/py/ and running `python setup.py install`

    cd lib/py/
    python setup.py install

Just in case, go into lib/cpp/ and run `make` and then `make install`.

    cd lib/cpp/
    make
    make install

Thrift should be installed. Check it by running:

	thrift --version

Check That Apache Thrift Works Correctly
========================================

Go into the tutorial directory under the top level thrift directory.

	cd tutorial/

Generate C++ code from the .thrift file.

	thrift --gen cpp tutorial.thrift

Generate Ruby code from the .thrift file.

	thrift --gen rb tutorial.thrift

These should complete without errors, and there will be new directories named 'gen-cpp' and 'gen-rb'


Running Basic Tutorial
======================

First generate the Python and Ruby code.

    thrift -r --gen py multi.thrift
    thrift -r --gen rb multi.thrift

This will create 'gen-py' and 'gen-rb' code. You can then run server and client in different languages to see that they work together. For example, run a Python server, and then run the Ruby client. The Python server will perform multiplication that the Ruby client requested.

Python
------

Create executable server and clients (if they're not already created).

    sudo chmod+x MultiplicationServer.py
    sudo chmod+x MultiplicationClient.py

To run server,

    ./MultiplicationServer.py

To run client,

    ./MultiplicationClient.py


Ruby
----

Create executable server and clients (if they're not already created).

    sudo chmod+x MultiplicationServer.rb
    sudo chmod+x MultiplicationClient.rb

To run server,

    ./MultiplicationServer.rb

To run client,

    ./MultiplicationClient.rb

C++
---

C++ is a bit more involved to set up. First generate the C++ thrift code from the top level directory.

    thrift -r --gen cpp multi.thrift

Copy the C++ code from the cpp/ directory into the newly generated gen-cpp/ directory

    cp cpp/MultiplicationServer.cpp gen-cpp/
    cp cpp/MultiplicationClient.cpp gen-cpp/

Now you'll need to compile and link the code before you can execute.

###Server

Compiling
    
    g++ -Wall -I/usr/local/include/thrift -c MultiplicationService.cpp -o MultiplicationService.o
    g++ -Wall -I/usr/local/include/thrift -c MultiplicationServer.cpp -o MultiplicationServiceServer.o
    g++ -Wall -I/usr/local/include/thrift -c multi_constants.cpp -o multi_constants.o
    g++ -Wall -I/usr/local/include/thrift -c multi_types.cpp -o multi_types.o

Linking
    
    g++ -L/usr/local/lib *.o -o MultiplicationServer -lthrift

###Client

Compiling

    g++ -Wall -I/usr/local/include/thrift -c MultiplicationClient.cpp -o MultiplicationClient.o

Linking

    g++ -L/usr/local/lib MultiplicationClient.o MultiplicationService.o multi_constants.o multi_types.o -o MultiplicationClient -lthrift
