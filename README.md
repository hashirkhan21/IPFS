# IPFS
This project implements a simulation of the InterPlanetary File System (IPFS)
using a Distributed Hash Table (DHT) with a ring topology. The IPFS is a peer-to-peer hypermedia protocol
that allows for decentralized storage and sharing of files.

You can run this project just by compiling it and running the main.cpp as you would do for a normal cpp program

The provided set of C++ files constitutes a simple implementation of a 
Distributed Hash Table (DHT) system. DHTs are decentralized systems 
that provide a scalable and fault-tolerant way of storing and retrieving 
key-value pairs across a network. This DHT system has been designed to 
manage a distributed file storage system.

The implementation consists of three primary components: 
 
BigInt.h: This file contains the implementation of BigInt, which is a user 
defined data type that is used to deal with large numbers. The BigInt 
class can help deal with SHA-1 hash effectively as the BigInt class 
contains functionality for changing between difference types of data 
type. In this project BigInt basically works in place of string 
functionality. 
 
SHA1.h: This file contain the implementation of the SHA-1 hashing 
algorithm. SHA-1 is utilized for generating unique identifiers for 
machines and files within the DHT system. 
 
 
Btree.h: This file contain the implementation of a B-tree data structure. 
The B-tree is used to organize and manage the files stored within each 
machine in the DHT. The Btree.h contains implementation of min B-tree 
that works for even degree only.  
 
Machine.h: This file defines the Machine class, which represents an 
individual machine in the DHT system. The Machine class includes 
functionalities for inserting and deleting machines, inserting and 
deleting files, searching for files, and managing routing tables. 
 
DHT.h: This file define the DHT class, which serves as the main interface 
for interacting with the DHT system. The DHT class includes 
functionalities for starting the system, inserting machines, inserting 
files, printing routing tables, printing B-trees, deleting machines, and 
searching for files. 
 
main.cpp: The main entry point for the application, which creates an 
instance of the DHT class and initiates the DHT system.
