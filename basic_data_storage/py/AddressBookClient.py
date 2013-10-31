#!/usr/bin/env python

import sys
sys.path.append('../gen-py')

from addressbook import AddressBook
from addressbook.ttypes import *

# Import the Thrift stack
from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol


def AddContact(client, filename):
  description_str = raw_input("Enter person's description: ")
  desc = Description(description=description_str)

  num_str = raw_input("Enter person's phone number: ")
  phone = PhoneNumber(number=num_str)

  name = raw_input("Enter person's name: ")
  person_id = int(raw_input("Enter person ID number: "))
  person = Person(name=name, id=person_id, number=phone, description=desc)

  # Call server and send binary data over to server.
  server_response = client.addContact(person, filename)
  print 'Adding person to address book...%s' % (server_response)

def ViewAllUsers(client, contacts_list):
  # Call server and send binary data over to server.
  server_response = client.listAll(contacts_list)
  print 'Listing contacts...\n%s' % (server_response)


# Main program
try: 
  # Make socket.
  transport = TSocket.TSocket('localhost', 9090)
  # Buffering is critical. Raw sockets are very slow.
  transport = TTransport.TBufferedTransport(transport)
  # Wrap in Thrift protocol.
  protocol = TBinaryProtocol.TBinaryProtocol(transport)
  # Create a client to use the protocol encoder.
  client = AddressBook.Client(protocol)
  # Connect to server.
  transport.open()

  addr_book_file = raw_input("Enter address book's filename: ")
  while True:
    resp = raw_input("Add person (A), view all (V), quit (Q): ")
    if resp.upper() == "Q":
      # Close connection.
      transport.close()
      # Break out of while loop.
      break
    elif resp.upper() == "V":
      # Pass data to method that calls a server command to list users.
      ViewAllUsers(client, addr_book_file)
    elif resp.upper() == "A":
      # Pass data to method that calls a server command to add contact.
      AddContact(client, addr_book_file)
except Thrift.TException, tx:
  print 'ERROR: %s\n' % (tx.message)