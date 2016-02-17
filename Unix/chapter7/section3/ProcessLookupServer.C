#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

main()
{
    FILE *pipe_in;        // File descriptor to read from a pipe.
    char sCommand[80];    // Holds the command read from the client on standard input.
    char sProcess_name[80];    // The process name being searched.
    char sLookup_command[255];    // The command to locate the process name.
    char sHost_name[80];    // The host on which the server is running.
    char sBuf[255];        // Holds data from pipe

    // Loop until the command sent by the client is "QUIT"
    while (1)
    {
        // Read a command from standard input
        gets(sCommand);

        // If the command read is LOOKUP, read the process name from standard input.
        if (!strcmp(sCommand, "LOOKUP"))
        {
            gets(sProcess_name);
            sprintf(sLookup_command, "/usr/bin/ps -elf | grep %s\0", sProcess_name);
            if ((pipe_in = popen(sLookup_command, "r")) == 0)
            {
                cout << "Cannot open system call ps" << endl;
                continue;
            }
            
            // Get the host name on which the server is running.
            if (gethostname(sHost_name, 80) == -1)
                strcpy(sHost_name, "UNKNOWN");

            // Send the host and processes that match the query.
            while (fgets(sBuf, 255, pipe_in))
                cout << "HOST: " << sHost_name << " PS: " << sBuf << endl;
            // Send the client "*DONE*" to indicate that no more server output is sent and input is expected.
            cout << "*DONE*" << endl;
        }

        // If the command is QUIT exit the server.
        else if (!strcmp(sCommand, "QUIT"))
            break;

        // If the command is not QUIT or LOOKUP inform the client of the error.
        else
        {
            cout << "Command: " << sCommand << " is Unknown" << endl;
            // Send the client "*DONE*" to indicate that no more server output is sent and input is expected.
            cout << "*DONE*" << endl;
        }
    }
}
