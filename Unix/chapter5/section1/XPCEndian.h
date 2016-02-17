#ifndef _XPCEndian
#define _XPCEndian

template<class OrigType> class XPCEndian
{
private:
    union EndianType
    {
        OrigType EndianValue;            // Endian value of original type
        char cCharList[sizeof(OrigType)];    // Byte representation of original type

        // Swap the bytes to derive the endian value
        void vEndianSwap()
        {
            char cTemp;
            int iSwap2 = 0;
            for (int iSwap1 = sizeof(OrigType)-1; iSwap1 >= sizeof(OrigType)/2; iSwap1--)
            {
                cTemp = cCharList[iSwap1];
                cCharList[iSwap1] = cCharList[iSwap2];
                cCharList[iSwap2] = cTemp;
                iSwap2++;
            }
        }
    };

    OrigType OriginalVal;    // Stores the data type in its original format
    EndianType EndianVal;     // Stores the data type in its endian format
public:
    // Overladed = operator used to store the value in its original and endian format
    XPCEndian &operator=(OrigType &_Val)
    {
        OriginalVal = _Val;
        EndianVal.EndianValue = _Val;
        EndianVal.vEndianSwap();
        return *this;
    }    

    // Sets the Value of the endian object and stores the value in its original and endia
    // format
    void vSetValue(OrigType _Val)
    {
        OriginalVal = _Val;
        EndianVal.EndianValue = _Val;
        EndianVal.vEndianSwap();
    }

    // Return the endian value
    OrigType getEndianValue() 
    { 
        return EndianVal.EndianValue;
    }

    // Return the original value
    OrigType getOriginalValue()
    {
        return OriginalVal;
    }
};

#endif
