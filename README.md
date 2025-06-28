# ft_malcolm

*** WIP ***

## Introduction

This is a networking project, based on ARP and 2nd (Data Link) Layer of OSI model.
Purpose of the project is to carry out spoofing attack on a target host using
modified ARP and Ethernet headers. As such it also serves as an introduction to
`man in the middle attack`.

Goal of the assignment is to see an entry of attacker's spoofed IP and MAC
addresses in the ARP table on target host machine.

### Description

Program uses raw socket so that it can access and send modified ARP / Ethernet
headers. Normally, when sending packets via socket, an operating system adds
headers by default (i.e., TCP, UDP, IP, Ethernet).

It then sends an ARP request which is part of broadcast frame. In this case ARP
request is sent to each host in LAN. Only the target host replies to source
(spoofed) IP address, which results in creating / updating entry in its ARP Table.

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

There are several utilities which help at scanning and examining sent packets
over socket.

* ```ping <IP address>```

* ```sudo tcpdump -i <interface> ...```

* ```sudo arp-scan --localnet```

* ```sudo arping -b -I <interface> <IP address>```

* ```sudo nmap -sn <IP address>/24```

* Wireshark
