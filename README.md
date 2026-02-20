# distributed_file_system-mini-hdfs-
# Distributed File System – Mini HDFS
A mini distributed file system simulation developed in **C using socket programming**, inspired by the Hadoop HDFS architecture.  
This project demonstrates client–server communication, metadata management, and data replication concepts.
## Features
- Client–Server architecture using TCP sockets
- File **Store**, **Retrieve**, and **List** operations
- Data replication across multiple DataNodes
- NameNode-based metadata management
- Basic distributed system simulation
## Technologies Used
- C Programming
- Winsock2 (Socket Programming)
- TCP/IP Networking
- Distributed Systems Concepts
## Project Components
- **Client** – Takes user input and communicates with nodes  
- **NameNode** – Maintains file metadata and locations  
- **DataNodes** – Store replicated file blocks  
## ▶️ How to Run
1. Start the NameNode server.
2. Start DataNode servers.
3. Run the Client program.
4. Choose operations from the menu (Store / Retrieve / List).
