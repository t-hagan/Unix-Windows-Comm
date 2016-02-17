#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream.h>

// Define program constants
#define TRUE 1
#define STDIN 1
#define ERROR -1

main()
{
    FILE *pipe_in;        // File descriptor to read from a pipe    
    char sCommand[80];    // Holds the command read from the client on standard input
    char sBuf[255];        // Holds 255 bytes of return from the ps command
    char sProcess_id[10];    // The process id sent from the client
    char sKill_command[20];    // The command to kill the process id sent by the client

    // Loop until the command sent by the client is "QUIT".
    while(TRUE)
    {
        // Read a command from standard input.
        //int iNumBytes = read(STDIN, sCommand, 80);
        //sCommand[iNumBytes] = 0;
        gets(sCommand);
    
        // If the command is QUIT exit.
        if (!strcmp(sCommand, "QUIT"))
            exit(0);

        // If the command is PS , list all processes running on the server and output them to standard out.
        else if (!strcmp(sCommand, "PS"))
        {
            if ((pipe_in = popen("/usr/bin/ps -elf", "r")) == 0)
            {
                cout << "Cannot open system call ps" << endl;
                continue;
            }
            
            while (fgets(sBuf, 255, pipe_in))
            cout << sBuf << flush;
            pclose(pipe_in);
            // Send the client  "*DONE*"  to indicate that no more output is being sent and input is expected
            cout << "*DONE*" << endl;
        }
        
        // If the command is KILL read the sProcess_id from the server.  Create a command to kill the process 
        // id and spawn the command.
        else if (!strncmp(sCommand, "KILL", 4))
        {
            gets(sProcess_id);
            // strcpy(sProcess_id, (char *)sCommand+5);

            sprintf(sKill_command, "kill -9 %s", sProcess_id);
            if (system(sKill_command) != ERROR)
               cout << "Successfully Spawned Kill Command" << endl;
            // Send the client  "*DONE*"  to indicate that no more output is being sent and input is expected
            cout << "*DONE*" << endl;
        }

        // If the command is not recognized inform the client.
        else
        {
            cout << "Command: " << sCommand << " is Unknown" << endl;
            // Send the client  "*DONE*"  to indicate that no more output is being sent and input is expected
            cout << "*DONE*" << endl;
        }
    }
}
