#include "XPCEvent.h"
#include <iostream.h>
#include <fstream.h>

int main (int argc, char* argv[])
{
    XPCEvent *event;

    try
    {
        event= new XPCEvent ("MyEvent");

        ofstream outfile ("\\\\server1\temp.out",
            ios::app | ios::trunc);
        outfile << "test" << endl;
        outfile.close();
        event->vSendEvent();
    }
    catch (XPCException ex)
    {
        cout << ex.sGetException() << endl;
    }
    

    delete event;
    return 0;
}
