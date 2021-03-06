// This file is a part of the IncludeOS unikernel - www.includeos.org
//
// Copyright 2017-2018 IncludeOS AS, Oslo, Norway
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Autogenerated by NaCl

#include <iostream>
#include <net/interfaces>
#include <net/ip4/cidr.hpp>
#include <microLB>
#include <syslogd>

using namespace net;

namespace nacl {
  class Filter {
  public:
    virtual Filter_verdict<IP4> operator()(IP4::IP_packet_ptr pckt, Inet& stack, Conntrack::Entry_ptr ct_entry) = 0;
    virtual ~Filter() {}
  };
}

static microLB::Balancer* nacl_lb_obj = nullptr;

void register_plugin_nacl() {
	INFO("NaCl", "Registering NaCl plugin");

	auto& outside = Interfaces::get(0);
	outside.network_config(IP4::addr{10,20,17,71}, IP4::addr{255,255,255,0}, IP4::addr{10,20,17,1});
	auto& inside = Interfaces::get(1);
	inside.network_config(IP4::addr{10,20,17,72}, IP4::addr{255,255,255,0}, IP4::addr{10,20,17,1});


	// Load balancers

	nacl_lb_obj = new microLB::Balancer(outside, 80, inside);

	Socket socket_0{ IP4::addr{10,20,17,81}, 80 };
	nacl_lb_obj->nodes.add_node(inside, socket_0, nacl_lb_obj->get_pool_signal());

	Socket socket_1{ IP4::addr{10,20,17,82}, 80 };
	nacl_lb_obj->nodes.add_node(inside, socket_1, nacl_lb_obj->get_pool_signal());

	/*
	Name: lb
	Layer: tcp

	Clients iface: outside
	Clients port: 80
	Clients wait queue limit: 1000
	Clients session limit: 1000

	Servers iface: inside
	Servers algorithm: round_robin
	Servers pool:

	Node address: IP4::addr{10,20,17,81}
	Node port: 80

	Node address: IP4::addr{10,20,17,82}
	Node port: 80
	*/
}
