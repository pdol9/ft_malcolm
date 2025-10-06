# ft_malcolm

## Introduction

This is a networking project based on ARP (Address Resolution Protocol). ARP
operates between OSI Layers 2 and 3 - it’s a link-layer protocol that maps
Layer-3 (IPv4) addresses to Layer-2 (MAC) addresses, bridging the Data Link and
Network layers.

Purpose of this project is to move from abstract network theory to visible,
verifiable evidence of how link-layer trust breaks down. Observing a spoofed
IP–MAC pair in a target’s ARP table is a concrete demonstration that an attacker
can mislead devices about “who” is on the LAN. It also serves as an introduction
to `man in the middle attack`.

## Description

[OBJECTIVE]

After gradual and thorough research and consequently clarification of the vague
project assignment, the actual goal became clearer, including requirements and
restrictions this project entails.

The assignment’s goal is to carry out a spoofing attack on target machine using
modified ARP and Ethernet headers. The end result is to ensure that the target
machine’s ARP cache table contains an entry of the attacker’s spoofed MAC and IP
addresses.

[DETAILS]

Assignment requires that our program waits for ARP request sent on the broadcast
by the target which requests the source IP. After that, program should send a single
ARP reply and exit. As a result, target host should contain spoofed addresses in
its ARP table.

## ARP Resolution Mechanism

The program uses raw/packet sockets to construct and send custom Ethernet frames
containing ARP messages. Normally, the operating system’s network stack generates
and processes headers (Ethernet, ARP, IP, TCP/UDP) automatically, so applications
only interact with higher-level sockets. By working at the raw socket level, the
program bypasses the OS’s default header handling and directly crafts packets at
the data-link layer, allowing full control over ARP and Ethernet fields.

Nonetheless, many OSes ignore unsolicited ARP replies (the designated method to
achieve the objective) for unknown IPs unless the target actually looked up that
IP. The typical attack is to reply for an IP that the victim will use (e.g.,
gateway IP) or to reply just after they send a request for that IP.

Expected behaviour and packet layout, among many other things are specified and
defined by RFC 826 and RFC 6747 which provide basis for concept and mechanism
of ARP communication.

Here will be listed only the most relevant part of ARP communication which
gives an insightful peek into ARP traffic.

### 1. Standard ARP request

[ARP] 192.168.124.108 is asking for 192.168.178.1

* This is host resolving the router’s IP (gateway) when sending packet(s) to target.

Takes place when ARP cache expires or after boot.

### 2. Standard ARP reply

[ARP REPLY] 192.168.124.1 is at aa:bb:cc:dd:ee:ff

* Sent as a direct unicast Ethernet frame to the original requester’s MAC address.

The ARP reply contains the responder’s MAC in the ARP payload so the requester
updates its ARP cache (IP→MAC mapping) and can immediately send layer-2 frames
to that MAC.

### 3. ARP probe (source IP = 0.0.0.0)

[ARP] 0.0.0.0 is asking for 192.168.124.108

* That is how ARP probing works during DHCP or IP assignment: before a host
takes an IP, it asks “who has 192.168.124.108?” but sets its source to 0.0.0.0
because it hasn’t configured its own IP yet.

This is to check for conflicts before claiming the IP. So here, a device is
probing to see if '.108' is already in use before assigning it to itself.
That could be host booting or renewing DHCP.

### 4. Gratuitous ARP (host asks about itself)

[ARP] 192.168.124.108 is asking for 192.168.124.108

* What might have been seen as an odd behaviour when a host seems to ask for
itself multiple times — is actually refreshing its presence on the LAN. It
is called a gratuitous ARP (ARP announcement).

A host tells the network “192.168.124.108 is at <my MAC>”, phrased as a
“who-has myself” request. This way host updates switch's/router's (caches)
about its own MAC binding. In addition, it warns of an IP conflict if another
device replies.


## Installation

To compile the program, run:
```bash
make
```
```bash
# to display additional info about LAN, like available network interfaces,
# ETH / ARP header info, etc. run:
make bonus
```

## Usage

Provide following parameters to the executable.
```bash
./ft_malcolm <source_ip> <source_mac_address> <target_ip> <target_mac_address>
```
Using command below we verify ARP table on target host machine.
```bash
arp -a      # or arp -e
```

```bash
# NOTE: a Makefile rule *make run* automatically runs program with provided
# arguments inside config.mk file where ```ARG``` variable is set 
make run
```

### Tools / utilities

There are several utilities / tools available when scanning and examining sent packets
over ARP layer.

* ```sudo tcpdump -i <interface> -n -vv arp```

* Wireshark

* Set-up of (virtual) cybersecurity lab: Kali Linux VMs with QEMU
