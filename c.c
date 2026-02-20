#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define SERVER "127.0.0.1"
#define NN_PORT 5000

SOCKET connectTo(int port)
{
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in a = {0};
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = inet_addr(SERVER);
    a.sin_port = htons(port);
    connect(s, (struct sockaddr *)&a, sizeof(a));
    return s;
}

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    int ch;
    char fname[50], data[1024], buf[1024];

    while (1)
    {
        printf("\n1.Store\n2.Retrieve\n3.List\n4.Exit\nChoice: ");
        scanf("%d", &ch);
        getchar();

        /* STORE */
        if (ch == 1)
        {
            printf("File name: ");
            gets(fname);
            printf("Content: ");
            gets(data);

            SOCKET nn = connectTo(NN_PORT);
            sprintf(buf, "STORE %s", fname);
            send(nn, buf, strlen(buf), 0);
            closesocket(nn);

            int dns[2] = {5001, 5002};
            for (int i = 0; i < 2; i++)
            {
                SOCKET dn = connectTo(dns[i]);
                sprintf(buf, "STORE %s_block0 %s", fname, data);
                send(dn, buf, strlen(buf), 0);
                closesocket(dn);
            }

            printf("File stored with replication\n");
        }

        /* RETRIEVE */
        else if (ch == 2)
        {
            printf("File name: ");
            gets(fname);

            SOCKET nn = connectTo(NN_PORT);
            sprintf(buf, "GET %s", fname);
            send(nn, buf, strlen(buf), 0);
            int n = recv(nn, buf, 1023, 0);
            buf[n] = '\0';
            closesocket(nn);

            if (strcmp(buf, "NOTFOUND") == 0)
            {
                printf("NOTFOUND\n");
            }
            else
            {
                char block[50];
                int p1, p2;
                sscanf(buf, "%s %d %d", block, &p1, &p2);

                SOCKET dn = connectTo(p1);
                sprintf(buf, "GET %s", block);
                send(dn, buf, strlen(buf), 0);
                n = recv(dn, buf, 1023, 0);
                buf[n] = '\0';
                closesocket(dn);

                printf("Content: %s\n", buf);
            }
        }

        /* LIST */
        else if (ch == 3)
        {
            SOCKET nn = connectTo(NN_PORT);
            send(nn, "LIST", 4, 0);
            int n = recv(nn, buf, 1023, 0);
            buf[n] = '\0';
            closesocket(nn);
            printf("Files:\n%s\n", buf);
        }

        else
            break;
    }
}
