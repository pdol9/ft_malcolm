# ARP protocoll & Ethernet

## Ethernet

### Ethernet II Frame: (including ARP structure)
+-----------------------------------+---------------------------------+-----------------------------+-------------------------------+-----------------+
|   Dest MAC (FF:FF:FF:FF:FF:FF)    |    Src MAC (1A:2B:3C:4D:5E:6F)  |     EtherType (0x0806)      |   ARP Packet (28 bytes)       |   CRC checksum  |
+-----------------------------------+---------------------------------+-----------------------------+-------------------------------+-----------------+
                48 bits                             48 bits                         16 bits                 -> Payload : Data             32 bits

* Ethernet II Frame is the container used to send different protocols like IPv4, ARP, IPv6, etc. (look at the end of the file for list)
- EtherType is a two-octet field in an Ethernet frame. It is used to indicate which protocol is encapsulated in the payload of the frame and is used at the receiving end by the data link layer to determine how the payload is processed.
- 0x0806 EtherType value is used to identify ARP frames.


## ARP

* ARP Packet is a protocol that is encapsulated inside an Ethernet II frame when transmitted over Ethernet.

### Address Resolution Protocol (ARP) Overview
                       Description
Protocol Type	        - ARP (Address Resolution Protocol)
Purpose	                - Resolves IP addresses to MAC addresses in a local network (Layer 2)
Network Layer	        - Data Link Layer (Layer 2)
Encapsulation	        - Encapsulated in Ethernet frames
Common Use Case	        - Converts an IPv4 address to a MAC address in LAN communication

### ARP Packet Structure

                	Size (Bytes)	Description
Hardware Type	       ->  2	     * Specifies network type (Ethernet = 0x0001)
Protocol Type	       ->  2	     * Specifies protocol (IPv4 = 0x0800)
Hardware Size	       ->  1	     * Length of MAC address (6 for Ethernet)
Protocol Size	       ->  1	     * Length of IP address (4 for IPv4)
Opcode	               ->  2	     * Request (1), Reply (2)
Sender MAC	           ->  6	     * MAC address of sender
Sender IP	           ->  4	     * IP address of sender
Target MAC	           ->  6	     * MAC address of receiver (set to 00:00:00:00:00:00 in request)
Target IP	           ->  4	     * IP address of receiver
=> 48 bits

### Inside ARP Packet:
+---------------+---------------+----------------------+----------------------+-----------+--------------------------------+-------------------------------+
| Hardware Type | Protocol Type | Hardware Size/Length | Protocol Size/Length |   Opcode  | Sender MAC (1A:2B:3C:4D:5E:6F) |  Sender IP (192.168.1.100)    |
+---------------+---------------+----------------------+----------------------+-----------+--------------------------------+-------------------------------+
    16 bits         16 bits             8 bits                  8 bits           16 bits             48 bits                         32 bits
+-------------+----------------+-------------------------------------+
| Target MAC (00:00:00:00:00:00) |      Target IP (192.168.1.1)      |
+--------------------------------+-----------------------------------+
            48 bits                             32 bits

### Description

* Hardware Type (HTYPE): 16 bits
     This field specifies the network link protocol type. In this example, a value of 1 indicates Ethernet.

* Protocol Type (PTYPE): 16 bits
     This field specifies the internetwork protocol for which the ARP request is intended. For IPv4, this has the value 0x0800. The permitted PTYPE values share a numbering space with those for EtherType.

* Hardware Length (HLEN): 8 bits
     Length (in octets) of a hardware address. For Ethernet, the address length is 6.

* Protocol Length (PLEN): 8 bits
     Length (in octets) of internetwork addresses. The internetwork protocol is specified in PTYPE. In this example: IPv4 address length is 4.

* Operation (OPER): 16 bits
     Specifies the operation that the sender is performing: 1 for request, 2 for reply.

* Sender Hardware Address (SHA): 48 bits
     Media address of the sender. In an ARP request this field is used to indicate the address of the host sending the request. In an ARP reply this field is used to indicate the address of the host that the request was looking for.

* Sender protocol address (SPA): 32 bits
     Internetwork address of the sender.

* Target hardware address (THA): 48 bits
     Media address of the intended receiver. In an ARP request this field is ignored. In an ARP reply this field is used to indicate the address of the host that originated the ARP request.

* Target protocol address (TPA): 32 bits
    Internetwork address of the intended receiver.


##  EtherType values for some notable protocols EtherType value (hexadecimal)
- Protocol:

0x0800 	Internet Protocol version 4 (IPv4)
0x0806 	Address Resolution Protocol (ARP)
0x0842 	Wake-on-LAN
0x22EA 	Stream Reservation Protocol
0x22F0 	Audio Video Transport Protocol (AVTP)
0x22F3 	IETF TRILL Protocol
0x6002 	DEC MOP RC
0x6003 	DECnet Phase IV, DNA Routing
0x6004 	DEC LAT
0x8035 	Reverse Address Resolution Protocol (RARP)
0x809B 	AppleTalk (EtherTalk)
0x80D5 	LLC PDU (in particular, IBM SNA), preceded by 2 bytes length and 1 byte padding
0x80F3 	AppleTalk Address Resolution Protocol (AARP)
0x8100 	VLAN-tagged frame (IEEE 802.1Q) and Shortest Path Bridging IEEE 802.1aq with NNI compatibility
0x8102 	Simple Loop Prevention Protocol (SLPP)
0x8103 	Virtual Link Aggregation Control Protocol (VLACP)
0x8137 	IPX
0x8204 	QNX Qnet
0x86DD 	Internet Protocol Version 6 (IPv6)
0x8808 	Ethernet flow control
0x8809 	Ethernet Slow Protocols[11] such as the Link Aggregation Control Protocol (LACP)
0x8819 	CobraNet
0x8847 	MPLS unicast
0x8848 	MPLS multicast
0x8863 	PPPoE Discovery Stage
0x8864 	PPPoE Session Stage
0x887B 	HomePlug 1.0 MME
0x888E 	EAP over LAN (IEEE 802.1X)
0x8892 	PROFINET Protocol
0x889A 	HyperSCSI (SCSI over Ethernet)
0x88A2 	ATA over Ethernet
0x88A4 	EtherCAT Protocol
0x88A8 	Service VLAN tag identifier (S-Tag) on Q-in-Q tunnel
0x88AB 	Ethernet Powerlink[citation needed]
0x88B8 	GOOSE (Generic Object Oriented Substation event)
0x88B9 	GSE (Generic Substation Events) Management Services
0x88BA 	SV (Sampled Value Transmission)
0x88BF 	MikroTik RoMON (unofficial)
0x88CC 	Link Layer Discovery Protocol (LLDP)
0x88CD 	SERCOS III
0x88E1 	HomePlug Green PHY
0x88E3 	Media Redundancy Protocol (IEC62439-2)
0x88E5 	IEEE 802.1AE MAC security (MACsec)
0x88E7 	Provider Backbone Bridges (PBB) (IEEE 802.1ah)
0x88F7 	Precision Time Protocol (PTP) over IEEE 802.3 Ethernet
0x88F8 	NC-SI
0x88FB 	Parallel Redundancy Protocol (PRP)
0x8902 	IEEE 802.1ag Connectivity Fault Management (CFM) Protocol / ITU-T Recommendation Y.1731 (OAM)
0x8906 	Fibre Channel over Ethernet (FCoE)
0x8914 	FCoE Initialization Protocol
0x8915 	RDMA over Converged Ethernet (RoCE)
0x891D 	TTEthernet Protocol Control Frame (TTE)
0x893a 	1905.1 IEEE Protocol
0x892F 	High-availability Seamless Redundancy (HSR)
0x9000 	Ethernet Configuration Testing Protocol[12]
0xF1C1 	Redundancy Tag (IEEE 802.1CB Frame Replication and Elimination for Reliability) 

https://en.wikipedia.org/wiki/Ethernet_frame#Ethernet_II
https://en.wikipedia.org/wiki/Address_Resolution_Protocol
