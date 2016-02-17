#include <XPCSignal.h>
#include <string.h>
#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>

main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cerr << "USAGE: Notifier <ON | OFF> <process id>" << endl;
        return 0;
    }

    if (strcmp(argv[1], "ON") == 0)
    {    
        // If the command-line argument is ON, a signal is sent to the specified process
        // using the user-defined signal SIGUSR1
        try
        {
            XPCSignal mySignal(SIGUSR1);
            mySignal.vSendSignal(atoi(argv[2]));
        }
        catch(XPCException &myExcept)
        {
            cout << myExcept.sGetException() << endl;
            return 0;
        }
    }
    else if (strcmp(argv[1], "OFF") == 0)
    {
        // If the command-line argument is OFF, a signal is sent to the specified process
        // using the user-defined signal SIGUSR2
        try
                {
                        XPCSignal mySignal(SIGUSR2);  
                        mySignal.vSendSignal(atoi(argv[2])); 
                }
                catch(XPCException &myExcept)
                {
                        cout << myExcept.sGetException() << endl;
            return 0;
                }    
    }
    else
    {
        cout << "Unkown Command" << endl;
        return 0;
    }

    return 1;
}
