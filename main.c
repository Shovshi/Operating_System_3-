#include <string.h>
#include "chat.h"
#include "Client.h"
#include "Server.h"

int main(int argc, char *argv[])
{
    if(strcmp(argv[1], "-c") == 0)
    {
        if(argc >= 5 && strcmp(argv[4], "-p")==0)
        {
            client_options(argc, argv);
        }
        else
        {
            chat(argc, argv);
        }
    }
    else if(strcmp(argv[1], "-s") == 0)
    {
        if(argc >= 5 && strcmp(argv[3], "-p")==0)
        {
            server_options(argc, argv);
        }
        else
        {
            chat(argc, argv);
        }
    }
    return 0;
}