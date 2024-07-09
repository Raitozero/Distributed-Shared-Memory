# Transparent Distributed Shared Memory

Transparent Distributed Shared Memory (DSM) is a concept that focuses on creating a system where memory is distributed across multiple nodes in a network but is accessed and managed as if it were a single, unified memory space. This transparency involves a lot of complexities of memory distribution, brings plenty of advantages such as location, replication, and consistency, which are all hidden from the user and the application.

Multiple nodes(servers/clients) store the data(key,value pairs). Which node stores what data is determined by the hashed key.
Several chunks of memory appear as a whole chunk local memory to the end user, though it is actually distributed across several machines in the backend. 

User can fetch the data, put the data as operating on the local machine without worrying about the limit of the original size. It works like Hadoop HDFS, but in a simple way. The diagrams in the pdf describe how it works.
