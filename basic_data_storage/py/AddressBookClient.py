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


# Prompt for input to populate a person's details
# When all person's details are entered, send off to server
# so that server can create the person and save into
# address book file.
def PromptForUserInfo():
  # Create empty contacts list
  contacts_list = ContactsList()
  contacts_list.list_of_people = []
  
  # Add people to contacts list
  while True:
    description = raw_input("Enter person's description: ")
    desc = Description(description=description)

    num = raw_input("Enter person's phone number: ")
    phone = PhoneNumber(number=num)

    name = raw_input("Enter person's name: ")
    person_id = int(raw_input("Enter person ID number: "))
    person = Person(name=name, id=person_id, number=phone)

    # Add person to contacts list
    contacts_list.list_of_people.append(person)
    
    response = raw_input("Do you want to add more people? (Y/N): ")

    if response.upper() == "N":
      break
  
  # Now that contacts_list is populated, send it to server to save.
  try:
    # Make socket.
    transport = TSocket.TSocket('localhost', 9090)
    # Buffering is critical. Raw sockets are very slow.
    transport = TTransport.TBufferedTransport(transport)
    # Wrap in a protocol.
    protocol = TBinaryProtocol.TBinaryProtocol(transport)
    # Create a client to use the protocol encoder.
    client = AddressBook.Client(protocol)
    # Connect to server.
    transport.open()

    # Ask for filename and pass data to server for saving.
    filename = raw_input("Enter desired filename: ")
    server_response = client.saveContacts(contacts_list, filename)
    print 'Saving contacts...%s' % (server_response)
    
    # Close connection.
    transport.close()

  except Thrift.TException, tx:
    print '%s' % (tx.message)

def ViewAllUsers(contacts_list):
  try:
    # Make socket
    transport = TSocket.TSocket('localhost', 9090)
    # Buffering is critical. Raw sockets are very slow
    transport = TTransport.TBufferedTransport(transport)
    # Wrap in a protocol
    protocol = TBinaryProtocol.TBinaryProtocol(transport)
    # Create a client to use the protocol encoder
    client = AddressBook.Client(protocol)
    # Connect!
    transport.open()

    server_response = client.listAll(contacts_list)
    print 'Listing contacts...\n%s' % (server_response)
    
    # Close!
    transport.close()

  except Thrift.TException, tx:
    print '%s' % (tx.message)


while True:
  
  resp = raw_input("Add people (A), view all (V), quit (Q): ")
  if resp.upper() == "Q":
    break
  elif resp.upper() == "V":
    # Read the existing address book.
    addr_book_file = raw_input("Enter file name: ")    
    try:
      f = open(addr_book_file, "rb")
      contacts_list = f.read()
      f.close()
      ViewAllUsers(contacts_list)
    except IOError:
      print sys.argv[1] + ": File not found.  Creating a new file."
  elif resp.upper() == "A":
    PromptForUserInfo()