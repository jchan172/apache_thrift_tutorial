Apache Thrift Tutorial
======================

This is a tutorial I've created that consists of two examples: `address_book/` and `multiplier_service/`. 

`multiplier_service/` is a very basic example demonstrating how to use the Thrift stack to build RPC client and server. It demonstrates that you can run a server in one language and communicate with it by using a client that uses a different language.

`address_book/` builds on top of that and demonstrates how to serialize and deserialize a data file defined in a Thrift definition file. It demonstrates that programs of different languages can read/write the same data file, which shows that Thrift can be used as a canonical data format. Going off this example, you'll be able to convert your existing data file into a Thrift data file that can be consumed using other languages.

Installing Apache Thrift
========================

This is a guide to install Apache Thrift on Ubuntu 13.04 (probably works on newer versions as well).

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
----------------------------------------

Go into the tutorial directory under the top level thrift directory.

	cd tutorial/

Generate C++ code from the .thrift file.

	thrift --gen cpp tutorial.thrift

Generate Ruby code from the .thrift file.

	thrift --gen rb tutorial.thrift

These should complete without errors, and there will be new directories named 'gen-cpp' and 'gen-rb'

Now that Apache Thrift is installed, take a look at the two examples. You can start with the easier one (`multiplier_service`) and then take a look at the more complicated one (`address_book`).

Notes
=====

Keep these limitations in mind when developing with Thrift:

- Struct inheritance/polymorphism not supported; use composition instead. Services can inherit from other services though.
- Can't overload methods in services (can't have methods that have same name but accept different inputs); use unique method names instead.
- Heterogeneous containers not supported (cannot have a "list<any/combo_type> listname", must have a specified list containing one type throughout, like "list<Person> listname")
- Can't return `NULL` from a function; return a string or struct or something else to indicate null.