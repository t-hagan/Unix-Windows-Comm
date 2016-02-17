#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

main()
{
    FILE *pipe_in;            // File descriptor to read from a pipe    
    char sCommand[80];        // Holds the command read from the client on standard input
    char sBuf[255];        // Holds 255 bytes of return from the ps command
    char sProcess_id[10];        // The process id sent from the client
    char sKill_command[20];    // The command to kill the process id send by the client

    // Loop until the command sent by the client is "QUIT".
    while(1)
    {
        // Send the client a command to change its command prompt
        cout << "CMDPMPT|Enter a Command <PS | KILL | QUIT>" << endl;

        // Send the client  "*DONE*"  to indicate that no more output is being sent and 
        // input is expected
        cout << "*DONE*" << endl;
        
        // Read a command from standard input.
        int iNumBytes = read(1, sCommand, 80);
        sCommand[iNumBytes] = 0;
    
        // If the command is QUIT exit.
        if (!strcmp(sCommand, "QUIT"))
            exit(0);

        // If the command is PS, list all processes running on the server and output them
        // to standard out.
        else if (!strcmp(sCommand, "PS"))
        {
            if ((pipe_in = popen("/usr/bin/ps -elf", "r")) == 0)
            {
                cout << "Cannot open system call ps" << endl;
                continue;
            }
            
            while (fgets(sBuf, 255, pipe_in))
                cout << sBuf << endl;
        }
        
        // If the command is KILL read the process_id from the server.  Create a command 
        // to kill the process id and spawn the command.
        else if (!strcmp(sCommand, "KILL"))
        {
            // Send a command to the client to change its command prompt
            cout << "CMDPMPT|Enter a PID #" << endl;

            // Indicate to the client that no more output is sent
            cout << "*DONE*" << endl;

            int iNumBytes = read(1, sProcess_id, 10);        
            sProcess_id[iNumBytes] = 0;

            sprintf(sKill_command, "kill -9 %s", sProcess_id);
            if (system(sKill_command) != -1)
                cout << "Successfully Spawned Kill Command" << endl;
        }

        // If the command is not recognized inform the client to have the error displayed
        // in its status window
        else
            cout << "ERROR|Command: " << sCommand << " is Unknown" << endl;
    }
}

