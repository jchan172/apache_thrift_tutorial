#!/usr/bin/env ruby

#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#

$:.push('../gen-rb')
# $:.unshift '../../lib/rb/lib'

require 'thrift'

require 'multiplication_service'
require 'multi_types'

class MultiplicationServiceHandler
  def initialize()
    @log = {}
  end

  def multiply(n1, n2)
    print "multiply(", n1, ",", n2, ")\n"
    return n1*n2
  end

end

handler = MultiplicationServiceHandler.new()
processor = Multi::MultiplicationService::Processor.new(handler) # this is specified in gen-rb/multiplication_service.rb
transport = Thrift::ServerSocket.new(9090)
transportFactory = Thrift::BufferedTransportFactory.new()
server = Thrift::SimpleServer.new(processor, transport, transportFactory)

puts "Starting the server..."
server.serve()
puts "done."
