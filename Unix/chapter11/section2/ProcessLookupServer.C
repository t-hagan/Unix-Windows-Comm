#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int iGetProcess(char *_sName)
{
    char sBuf[255];        // Stores results of pipe    
    char sGrep[512];    // Stores the grep command
    int iPid = 0;        // Stores the ID of requested process
    char sJunk[50];        // Temporary storage
    FILE *pipe_in;        // A file descriptor 

    // Create the 'process grep' command
    sprintf(sGrep, "/usr/bin/ps -elf | grep %s", _sName);

    // The 'process grep' command is executed and its results are piped in
    if ((pipe_in = popen(sGrep, "r")) == 0)
    {
        cout << "Cannot Open system call ps" << endl;
        return 0;
    }

    // The results of the 'process grep' are looped through until the
    // process requested is found.  If the process is found its ID is stored
    while (fgets(sBuf, 255, pipe_in))
    {
        // If the process is 'grep' it is skipped    
        if (strstr(sBuf, "grep") != 0)
            continue;
        sscanf(sBuf, "%s %s %s %d", sJunk, sJunk, sJunk, &iPid);
        break;
    }

    return iPid;
}

main()
{
    char sCommand[80];        // Stores the command sent by the remote client 
    char sBuf[255];            // Stores the results of a process kill
    char process_name[100];
    char sKill_command[80];        // Stores the command used to kill the requested process
    char sLookup_string[80];    // Stores the name of the process to lookup    
    FILE *pipe_in;
    char sHostName[80];        // Stores the name of the current UNIX host

    // The name of the UNIX host is retrieved
    if (gethostname(sHostName, sizeof(sHostName)) != 0)
        strcpy(sHostName, "UNKNOWN");
    
    while(1)
    {
        // The command sent by the client is retrieved
        read(1, sCommand, 255);

        // If the client's command is QUIT this process exits    
        if (!strcmp(sCommand, "QUIT"))
            exit(0);

        else if (!strncmp(sCommand, "LOOKUP", 6))
        {
            // The name of the command to lookup is extracted
            strcpy(sLookup_string, (char *)sCommand+7);

            // If the process name cannot be found, a 0 is returned.  Otherwise the
            // process was found
            int iPid = iGetProcess(sLookup_string);
            if (iPid == 0)
            {
                cout << "NOT FOUND on " << sHostName << endl;
            }
            else
            {
                cout << "FOUND on " << sHostName << endl;
            }    
        }
        else if (!strncmp(sCommand, "KILL", 4))
        {
            // The name of the command to kill is extracted
            strcpy(sLookup_string, (char *)sCommand+5);

            // Set the kill confirmation flag to FALSE
            int iKill_confirmation = 0;

            while(1)    
            {    
                // The ID of the process requested is retrieved        
                int iPid = iGetProcess(sLookup_string);

                // If the process does not exist the kill loop is exited
                if (iPid == 0)
                {
                    break;
                }
                else
                    iKill_confirmation = 1;

                // The kill command is generated using the process ID    
                sprintf(sKill_command, "kill -9 %d\0", iPid);
                if ((pipe_in = popen(sKill_command, "r")) == 0)
                {
                    cout << "Cannot Open Kill Command" << endl;
                    continue;
                }

                while(fgets(sBuf, 255, pipe_in))
                    iKill_confirmation = 0;
    
                pclose(pipe_in);
            }
            if (iKill_confirmation)
                cout << "KILLED ON " << sHostName << endl;
            else
                cout << "NOT KILLED ON " << sHostName << endl;
        }
        else
        {
            cout << "ERROR -- Command: " << sCommand << " is Unknown" << endl;
        }

        // The "finish indicator" is returned to the client
        cout << "** DONE **" << endl;
    }
}
