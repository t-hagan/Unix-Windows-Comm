#ifndef _XPCMsg
#define _XPCMsg

#include <iostream.h>
#include <string.h>
#include <XPCEndian.h>
#include <XPCHeader.h>

// Definition of message types
#define STRING 0
#define INT 1
#define DOUBLE 2

class XPCContainer
{
    char sLabel[512];        // Message component label
    char sValue[512];        // Byte representation of message component value
    char sEndianValue[512];        // Endian byte representation of message component value
    char cOpSystem;            // Operating system definition
    char cType;            // Original component data type
    XPCEndian<int> iId;        // ID of the message component

    // Sets the endian value of the component's original value
    void vSetEndianValue(int _iSize)
    {
        char cTemp;
        memcpy((void *)sEndianValue, (void *)sValue, _iSize);
        int iSwap2 = 0;
        for (int iSwap1 = _iSize-1; iSwap1 >= _iSize/2; iSwap1--)    
                { 
                        cTemp = sEndianValue[iSwap1];       
                        sEndianValue[iSwap1] = sEndianValue[iSwap2];   
                        sEndianValue[iSwap2] = cTemp;       
                        iSwap2++;       
                }
    }    
public:
    // Constructor.  Stores the operating system value
    XPCContainer(char _cOpSystem = OPSYSTEM) 
    { 
        memset(this, 0, sizeof(XPCContainer));
        cOpSystem = _cOpSystem;
    }

    // Constructor.  Stores a component's label
    XPCContainer(char *_sLabel) 
    {
        strcpy(sLabel, _sLabel);
    }

    // Sets the label, value, and ID for an integer  message component
    void vSetValue(char *_sLabel, int _iValue, int _iId)
    {
        memcpy((void *)sValue, (void *)&_iValue, sizeof(int));
        vSetEndianValue(sizeof(int));
        strcpy(sLabel, _sLabel);

        cType = INT;
        iId = _iId;
    }

    // Sets the label, value, and ID for a double  message component    
    void vSetValue(char *_sLabel, double _dValue, int _iId)
    {
        memcpy((void *)sValue, (void *)&_dValue, sizeof(double));
        vSetEndianValue(sizeof(double));
        strcpy(sLabel, _sLabel);

        cType = DOUBLE;
        iId = _iId;
    }

    // Sets the label, value, and ID for a string  message component
    void vSetValue(char *_sLabel, char *_sValue, int _iId)
    {
        strcpy(sValue, _sValue);

        strcpy(sLabel, _sLabel);

        cType = STRING;
        iId = _iId;
    }

    // Returns the original data type of the message component
    char cGetType()
    {
        return cType;
    }

    // Returns the message component's value as an integer
    int iGetValue(int *_iRetval, char _cOpSystem = OPSYSTEM)
    {
        if (cType != INT)
            return 0;

        if (cOpSystem != _cOpSystem)
        {
            memcpy((void *)_iRetval, (void *)sEndianValue, sizeof(int));
        }
        else
            memcpy((void *)_iRetval, (void *)sValue, sizeof(int));
        return 1;
    }

    // Returns the message component's value as a double
    int iGetValue(double *_dRetval, char _cOpSystem = OPSYSTEM)
    {
        if (cType != DOUBLE)
            return 0;

        if (cOpSystem != _cOpSystem)
            memcpy((void *)_dRetval, (void *)sEndianValue, sizeof(double));
        else
            memcpy((void *)_dRetval, (void *)sValue, sizeof(double));
        return 1;
    }

    // Returns the message component's value as a string
    int iGetValue(char *_sRetval, char _cOpSystem = OPSYSTEM)
    {
        if (cType != STRING)
            return 0;

        strcpy(_sRetval, sValue);
        return 1;
    }

    // Returns the message component's label
    char *sGetLabel()
    {
        return sLabel;
    }

    // Returns the ID of the message component
    int iGetId(char _cOpSystem = OPSYSTEM)
    { 
        if (cOpSystem != _cOpSystem)
            return iId.getEndianValue();
        else
            return iId.getOriginalValue();
    }
};

class XPCMessage
{
    XPCEndian<int> iNumComponents;        // The number of components in the message
    XPCEndian<int> iCurrentComponents;    // The current component number
    char cType;                // The type of message (PUBLISH or SUBSCRIBE)
    char cOpSystem;                // The operating system definition
    XPCHeader header;                // The message header definition
    XPCTcpSocket *Socket;        // The TCP socket used for communication

public:
    XPCContainer **ContainerList;    // The list of XPCContainer objects

    // Constructor.  Sets the message type, allocates memory for a specified number of XPCContainer objects,
    // and creates the message header
    XPCMessage(int _iNumParts, char *_sSubject, char _cType, XPCTcpSocket *_Socket, char _cOpSystem = OPSYSTEM)
    {
        int iZero = 0;
        cOpSystem = _cOpSystem;    // Store the operating system definition
        cType = _cType;        // Store the message type

        // Create the header
        header.vSetValues(_cType, _sSubject, _iNumParts);
        iCurrentComponents = iZero;

        // Create the list of XPCContainer objects    
        iNumComponents = _iNumParts;    
        ContainerList = new XPCContainer*[_iNumParts];

        // Store the TCP socket
        Socket = _Socket;
    }

    // Constructor used to receive a subscribed message 
    XPCMessage(int _iNumParts, XPCTcpSocket *_Socket, char *_sSubscription) 
    {
        int iZero = 0;
        cOpSystem = OPSYSTEM; 
        int iNumBytes;
    
        // A header is created with the number of message components specified    
        header.vSetValues(PUBLISH, _sSubscription, _iNumParts);

        iCurrentComponents = iZero;
        iNumComponents = _iNumParts;

        // An array of XPCContainer objects is created
        ContainerList = new XPCContainer*[_iNumParts];

        // The number of XPCContainer objects specified are received
        for (int iCount = 0; iCount < _iNumParts; iCount++)
                {
                         ContainerList[iCount] = new XPCContainer();
#ifdef WINDOWS_NT
            while(1)
            {
                try
                {
                     iNumBytes = _Socket->iRecieveMessage((void *)ContainerList[iCount], sizeof(XPCContainer), MSG_WAITALL);
                }
                catch(XPCException &exceptOb)
                {
                    if (WSAGetLastError() == WSAEWOULDBLOCK)
                    {
                        Sleep(100);
                        continue;
                    }
                    else
                        throw exceptOb;
                }
                break;
            }    
#else
            iNumBytes = _Socket->iRecieveMessage((void *)ContainerList[iCount], sizeof(XPCContainer), MSG_WAITALL);
#endif
            
            if (iNumBytes == 0)
            {
                XPCException newExcept("Error receiving message component");
                throw newExcept;
            }

            // Check message sequence number
                if (iCount != ContainerList[iCount]->iGetId())
            {
                XPCException newExcept("Received message out of sequence");
                throw newExcept;
            } 
                }
        if (iCount != iNumComponents.getOriginalValue())
        {
            XPCException newExcept("Received the incorrect number of message components");
            throw newExcept;
        }
    }

    // Destructor.  Deallocates the ContainerList object
    ~XPCMessage() { delete [] ContainerList; }

    // Returns the subscription name
    char *sGetSubscription()
    {
        return header.sGetSubscription();
    }

    // Returns the number of message components
    int iEntries(char _cOpSystem = OPSYSTEM)
    {
        if (cOpSystem != _cOpSystem)
            return iNumComponents.getEndianValue();
        else
            return iNumComponents.getOriginalValue();
    }

    // Inserts an integer value and label into the message
    void vInsert(char *_sLabel, int _iValue)
    {
        ContainerList[iCurrentComponents.getOriginalValue()] = new XPCContainer();
                ContainerList[iCurrentComponents.getOriginalValue()]->vSetValue(_sLabel, _iValue, iCurrentComponents.getOriginalValue());
        int iCount = iCurrentComponents.getOriginalValue() + 1;
        iCurrentComponents = iCount;
    }    

    // Inserts an double value and label into the message
    void vInsert(char *_sLabel, double _dValue)
    {
        ContainerList[iCurrentComponents.getOriginalValue()] = new XPCContainer();
                ContainerList[iCurrentComponents.getOriginalValue()]->vSetValue(_sLabel, _dValue, iCurrentComponents.getOriginalValue());
        int iCount = iCurrentComponents.getOriginalValue() + 1;
                iCurrentComponents = iCount;
        }

    // Inserts an string value and label into the message
    void vInsert(char *_sLabel, char *_sValue)
    {
        ContainerList[iCurrentComponents.getOriginalValue()] = new XPCContainer();
                ContainerList[iCurrentComponents.getOriginalValue()]->vSetValue(_sLabel, _sValue, iCurrentComponents.getOriginalValue());
        int iCount = iCurrentComponents.getOriginalValue() + 1;
                iCurrentComponents = iCount;
        }

    // Returns a string value given a label
    int iGetValueByName(char *_sLabel, char *_sValue, char _cOpSystem = OPSYSTEM)
    { 
        int iNum;

        if (cOpSystem != _cOpSystem)
            iNum = iNumComponents.getEndianValue();
        else
            iNum = iNumComponents.getOriginalValue();
    
        for (int iCount = 0; iCount < iNum; iCount++)
        {
            if (strcmp(ContainerList[iCount]->sGetLabel(), _sLabel) == 0) 
                return ContainerList[iCount]->iGetValue(_sValue);
        }
        return 0;
    }

    // Returns an integer value given a label
    int iGetValueByName(char *_sLabel, int *_iValue, char _cOpSystem = OPSYSTEM)
    {
        int iNum;
                if (cOpSystem != _cOpSystem)            
                        iNum = iNumComponents.getEndianValue();  
                else
                        iNum = iNumComponents.getOriginalValue();

        for (int iCount = 0; iCount < iNum; iCount++)
                {
                        if (strcmp(ContainerList[iCount]->sGetLabel(), _sLabel) == 0)
                                return ContainerList[iCount]->iGetValue(_iValue);
                }
                return 0;
    }

    // Returns a double value given a label
    int iGetValueByName(char *_sLabel, double *_sValue, char _cOpSystem = OPSYSTEM)
    {
        int iNum;

                if (cOpSystem != _cOpSystem)            
                        iNum = iNumComponents.getEndianValue();  
                else
                        iNum = iNumComponents.getOriginalValue();

        for (int iCount = 0; iCount < iNum; iCount++)
                {
                        if (strcmp(ContainerList[iCount]->sGetLabel(), _sLabel) == 0)
                                return ContainerList[iCount]->iGetValue(_sValue);
                }
                return 0;    
    }

    // Returns a string value given a component ID
    int iGetValueByNumber(int _iLocation, char *_sValue, char _cOpSystem = OPSYSTEM)
    {
        int iNum;

                if (cOpSystem != _cOpSystem)            
                        iNum = iNumComponents.getEndianValue();  
                else
                        iNum = iNumComponents.getOriginalValue();

        if ((_iLocation < 0) || (_iLocation >= iNum))
            return 0;

        return ContainerList[_iLocation]->iGetValue(_sValue);
    }

    // Returns an integer  value given a component ID
    int iGetValueByNumber(int _iLocation, int *_iValue, char _cOpSystem = OPSYSTEM)
    {
        int iNum;

                if (cOpSystem != _cOpSystem)            
                        iNum = iNumComponents.getEndianValue();  
                else
                        iNum = iNumComponents.getOriginalValue();

        if ((_iLocation < 0) || (_iLocation >= iNum))
                        return 0;

                return ContainerList[_iLocation]->iGetValue(_iValue);
    }

    // Returns a double value given a component ID
    int iGetValueByNumber(int _iLocation, double *_dValue, char _cOpSystem = OPSYSTEM)
       {
        int iNum;

                if (cOpSystem != _cOpSystem)            
                        iNum = iNumComponents.getEndianValue();  
                else
                        iNum = iNumComponents.getOriginalValue();

        if ((_iLocation < 0) || (_iLocation >= iNum))
                        return 0;

                return ContainerList[_iLocation]->iGetValue(_dValue);
       }

    // Returns a component lable given a component ID
    char *sGetLabelByNumber(int _iLocation, char _cOpSystem = OPSYSTEM)
    {
        int iNum;

                if (cOpSystem != _cOpSystem)            
                        iNum = iNumComponents.getEndianValue();  
                else
                        iNum = iNumComponents.getOriginalValue();

        if ((_iLocation < 0) || (_iLocation >= iNum))
            return 0;

        return ContainerList[_iLocation]->sGetLabel();
    }

    // Returns a XPCContainer object given a component ID
    XPCContainer *GetContainer(int _iLocation)
    {
        return ContainerList[_iLocation];
    }    

    // Returns the original data type of the component given the component's ID
    int iGetTypeByNumber(char &_cType, int _iLocation, char _cOpSystem = OPSYSTEM)
        {
                int iNum;

                if (cOpSystem != _cOpSystem)
                        iNum = iNumComponents.getEndianValue();
                else
                        iNum = iNumComponents.getOriginalValue();

                if ((_iLocation < 0) || (_iLocation >= iNum))
                        return 0;

                _cType = ContainerList[_iLocation]->cGetType();
        return 1;
    }

    // Returns the original data type of the component given the component's label
    int iGetTypeByName(char &_cType, char *_sLabel, char _cOpSystem = OPSYSTEM)
    {
        int iNum;

                if (cOpSystem != _cOpSystem)
                        iNum = iNumComponents.getEndianValue();
                else
                        iNum = iNumComponents.getOriginalValue();

                for (int iCount = 0; iCount < iNum; iCount++)
                {
                        if (strcmp(ContainerList[iCount]->sGetLabel(), _sLabel) == 0)
            {
                _cType = ContainerList[iCount]->cGetType();
                return 1;
            }
                }
                return 0;
    }    

    // Publishes the message
    void vPublish()
    {
        int iNum;

                iNum = iNumComponents.getOriginalValue();

        Socket->iSendMessage((void *)&header, sizeof(XPCHeader));
        
                for (int iCount = 0; iCount < iNum; iCount++)
        {
            Socket->iSendMessage((void *)ContainerList[iCount], sizeof(XPCContainer));
        }
    }

    // Subscribes to a message
    void vSubscribe()
    {
        Socket->iSendMessage((void *)&header, sizeof(XPCHeader));
    }

    // Retrieves subscribed messages and calls a user-defined callback function passing it the message
    // header
    static void vGetMessages(XPCTcpSocket *_Socket, XPCHeader &_header, void vCallBack(XPCTcpSocket *, int, char *), char _cOpSystem = OPSYSTEM)
    {
        int iNumBytes = _Socket->iRecieveMessage((void *)&_header, sizeof(XPCHeader), MSG_WAITALL);

        // If an error occured receiving the message an exception is thrown
        if (iNumBytes == 0)
        {
            XPCException exceptOb("An error occured receiving the subscribed message");
            throw exceptOb;
            return;
        }    
                vCallBack(_Socket, _header.iGetNumComponents(_cOpSystem), _header.sGetSubscription());
    }
};

#endif
