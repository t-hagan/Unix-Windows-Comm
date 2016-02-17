#include <iostream.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <XPCTcpSocket.h>
#include <XPCPrice.h>

#define PRICE_PORTNUM 6101

// Hardcoded list of stock ticker symbols and prices
char *sStockTickers[] = { "IBM", "INTC", "CIG" };
double dPrices[] = { 50.5, 51.6, 62.8, 63.9 };

main(int argc, char *argv[])
{
    XPCPrice aNewPrice;    // Stores a ticker / price update change
    
    try
    {
        // XPCTcpSocket instance
        XPCTcpSocket priceSocket((long int)PRICE_PORTNUM);

        // Connect to stock position client
        priceSocket.vConnect(argv[1]);

        // Loop forever sending stock ticker / price updates
        while(1)
        {
            for (int iTickerCount = 0; iTickerCount < 3; iTickerCount++)
            {
                for (int iPriceCount = 0; iPriceCount < 4; iPriceCount++)
                {
                    aNewPrice.vSetTicker(sStockTickers[iTickerCount]);
                    aNewPrice.vSetPrice(dPrices[iPriceCount]);
            
                    priceSocket.iSendMessage((void *)&aNewPrice, sizeof(aNewPrice));
                    sleep(2);
                }
            }
        }
    }
    catch(XPCException &socketExcept)
    {
        cout << socketExcept.sGetException() << endl;
        return 0;
    }

    return 1;
}
