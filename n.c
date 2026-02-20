#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define PORT 5000

typedef struct Replica
{
    int datanode_port;
    struct Replica *next;
} Replica;


typedef struct Block
{
    char block_name[50];
    Replica *replicas; 
} Block;

typedef struct FileNode
{
    char filename[50];
    Block block;
    struct FileNode *next; 
} FileNode;

FileNode *head = NULL;


FileNode *findFile(const char *name)
{
    FileNode *temp = head;
    while (temp)
    {
        if (strcmp(temp->filename, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}


void handleClient(SOCKET c)
{
    char buf[256] = {0};
    recv(c, buf, sizeof(buf), 0);


    if (strncmp(buf, "STORE", 5) == 0)
    {
        char fname[50];
        sscanf(buf, "STORE %s", fname);

        FileNode *node = (FileNode *)malloc(sizeof(FileNode));
        strcpy(node->filename, fname);
        sprintf(node->block.block_name, "%s_block0", fname);

        Replica *r1 = (Replica *)malloc(sizeof(Replica));
        Replica *r2 = (Replica *)malloc(sizeof(Replica));
        
        r1->datanode_port = 5001;
        r2->datanode_port = 5002;
        r1->next = r2;
        r2->next = NULL;
        node->block.replicas = r1;

        node->next = head; 
        head = node;

        send(c, "OK", 2, 0);
    }


    else if (strncmp(buf, "GET", 3) == 0)
    {
        char fname[50];
        sscanf(buf, "GET %s", fname);

        FileNode *f = findFile(fname);
        if (!f)
        {
            send(c, "NOTFOUND", 8, 0);
        }
        else
        {
            char reply[100];
            sprintf(reply, "%s %d %d",
                    f->block.block_name,
                    f->block.replicas->datanode_port,
                    f->block.replicas->next->datanode_port);
            send(c, reply, strlen(reply), 0);
        }
    }

    else if (strncmp(buf, "LIST", 4) == 0)
    {
        char out[512] = "";
        FileNode *t = head;
        while (t)
        {
            strcat(out, t->filename);
            strcat(out, "\n");
            t = t->next;
        }
        send(c, out, strlen(out), 0);
    }
}

int main()
{
    WSADATA wsa;
    SOCKET s, c;
    struct sockaddr_in server;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    s = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 5);
    printf("NameNode running on %d\n", PORT);

    while (1)
    {
        c = accept(s, NULL, NULL);
        handleClient(c);
        closesocket(c);
    }
}
