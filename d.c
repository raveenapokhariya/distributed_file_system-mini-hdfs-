#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

typedef struct Block
{
    char block_name[50];
    char data[1024];
    struct Block *next; 
} Block;

Block *head = NULL;

void handleClient(SOCKET c)
{
    char buf[1024] = {0};
    recv(c, buf, sizeof(buf), 0);

    if (strncmp(buf, "STORE", 5) == 0)
    {
        char bname[50], content[1024];
        sscanf(buf, "STORE %s %[^\n]", bname, content);

        Block *b = (Block *)malloc(sizeof(Block));
        strcpy(b->block_name, bname);
        strcpy(b->data, content);
        b->next = head;
        head = b;

        send(c, "OK", 2, 0);
    }


    else if (strncmp(buf, "GET", 3) == 0)
    {
        char bname[50];
        sscanf(buf, "GET %s", bname);

        Block *t = head;
        while (t)
        {
            if (strcmp(t->block_name, bname) == 0)
            {
                send(c, t->data, strlen(t->data), 0);
                return;
            }
            t = t->next;
        }
        send(c, "NOTFOUND", 8, 0);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: datanode <port>\n");
        return 1;
    }

    int port = atoi(argv[1]);
    WSADATA wsa;
    SOCKET s, c;
    struct sockaddr_in server;

    WSAStartup(MAKEWORD(2, 2), &wsa);
    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 5);
    printf("DataNode running on %d\n", port);

    while (1)
    {
        c = accept(s, NULL, NULL);
        handleClient(c);
        closesocket(c);
    }
}
