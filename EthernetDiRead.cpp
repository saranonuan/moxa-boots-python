/******************************************************************************/
/*                                                                            */
/*    Example program : EthernetDiRead.cpp                                    */
/*                                                                            */
/*    Description:                                                            */
/*        1. Set/Get DI mode.                                                 */
/*        2. Read contiguous channel or specific channel from ioLogik         */
/*           2000 DI Ethernet Module.                                         */
/*                                                                            */
/*     List of MXIO Functions used in this example:                           */
/*         MXEIO_Connect, MXEIO_Disconnect, DI2K_SetModes, DI2K_GetModes,     */
/*         DI2K_SetMode, DI2K_GetMode, DI2K_SetFilters, DI2K_GetFilters,      */
/*         DI2K_SetFilter, DI2K_GetFilter , DI_Reads, DI_Read                 */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <mxio.h>

#define IPAddr      "192.168.127.254"       //IP address for your ioLogik 2000 module
#define Port        502                     //Modbus TCP port
#define TimeOut     2000                    //Connection timeout(ms) or
                                            //function execution timeout(ms)

void CheckErr( int iHandle, int iRet, char * szFunctionName );        //check function execution result

int main()
{
    //***************************************
    // Initiate socket and create connection
    //***************************************
    int iRet;               //stored return code
    int iHandle;            //stored handle for ioLogik 2000 module

    //creates a connection to the port of the device that is to be controlled
    iRet = MXEIO_Connect( (char*)IPAddr,                   //IP address
                          Port,                     //TCP port number
                          TimeOut,                  //timeout
                          &iHandle);                //connection handle

    CheckErr( iHandle, iRet, (char*)"MXEIO_Connect" );

    printf( "Creates module connection succeeded.\n" );

    //*****************
    // Set/Get DI Mode
    //*****************
    WORD wSetModes[12] = { 0, 0, 0, 1, 0, 0,        //Set ch3 is Counter modes
                           0, 0, 0, 0, 0, 0 };      //other are DI mode

    //set ch0 ~ ch11 DI modes
    iRet = DI2K_SetModes( iHandle,                  //the handle for a connection
                          0,                        //starting channel
                          12,                       //channel count
                          wSetModes );              //DI setting mode

    CheckErr( iHandle, iRet, (char*)"DI2K_SetModes" );

    printf( "Set ch0 ~ ch12 DI modes succeeded.\n" );

    WORD wGetModes[12];

    //get cho ~ ch11 DI modes
    iRet = DI2K_GetModes( iHandle,                  //the handle for a connection
                          0,                        //starting channel
                          12,                       //channel count
                          wGetModes );              //DI getting mode

    CheckErr( iHandle, iRet, (char*)"DI2K_GetModes" );

    printf( "Get ch0 ~ ch12 DI modes : %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
            wGetModes[0], wGetModes[1], wGetModes[2], wGetModes[3],
            wGetModes[4], wGetModes[5], wGetModes[6], wGetModes[7],
            wGetModes[8], wGetModes[9], wGetModes[10], wGetModes[11] );

    WORD wSetMode = 0;

    //Set ch3 DI mode
    iRet = DI2K_SetMode( iHandle,                   //the handle for a connection
                         3,                         //specific channel
                         wSetMode );                //DI setting mode

    CheckErr( iHandle, iRet, (char*)"DI2K_SetMode" );

    printf( "Set ch3 DI mode succeeded.\n" );

    WORD wGetMode;

    iRet = DI2K_GetMode( iHandle,                   //the handle for a connection
                         3,                         //specific channel
                         &wGetMode );               //DI getting mode

    CheckErr( iHandle, iRet, (char*)"DI2K_GetMode" );

    printf( "Get ch3 DI mode : %d\n", wGetMode );

    //*******************
    // Set/Get DI filter
    //*******************
    WORD wSetFilters[12] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                             0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C };

    //Set ch0 ~ ch11 DI filter
    iRet = DI2K_SetFilters( iHandle,                //the handle for a connection
                            0,                      //starting channel
                            12,                     //channel count
                            wSetFilters );          //filter setting value

    CheckErr( iHandle, iRet, (char*)"DI2K_SetFilters" );

    printf( "Set ch0 ~ ch11 DI filter succeeded." );

    WORD wGetFilters[12];

    //get ch0 ~ ch11 filter value
    iRet = DI2K_GetFilters( iHandle,                //the handle for a connection
                            0,                      //starting channel
                            12,                     //channel count
                            wGetFilters );          //filter getting value

    CheckErr( iHandle, iRet, (char*)"DI2K_GetFilter" );

    printf( "Get ch0 ~ ch11 DI filter : %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
            wSetFilters[0], wSetFilters[1], wSetFilters[2], wSetFilters[3],
            wSetFilters[4], wSetFilters[5], wSetFilters[6], wSetFilters[7],
            wSetFilters[8], wSetFilters[9], wSetFilters[10], wSetFilters[11] );

    WORD wSetFilter = 0x03;

    //set ch2 filter value
    iRet = DI2K_SetFilter( iHandle,                 //the handle for a connection
                            2,                      //specific channel
                            wSetFilter );           //filter setting value

    CheckErr( iHandle, iRet, (char*)"DI2K_SetFilter" );

    printf( "Set ch2 DI filter succeeded.\n" );

    WORD wGetFilter;

    //get ch2 filter value
    iRet = DI2K_GetFilter( iHandle,                 //the handle for a connection
                           2,                       //specific channel
                           &wGetFilter );           //filter getting value

    CheckErr( iHandle, iRet, (char*)"DI2K_GetFilter" );

    printf( "Get ch2 DI filter : %d\n", wGetFilter );

    //***************
    // DI Read Value
    //***************
    DWORD dwValue;      //stored contiguous channel DI values, each bit holds one channel value
                        //A bit value of 0 represents the digital input status of the start
                        //channel. A bit value of 1 represents the second digital input
                        //channel's status.

    //read ch0 ~ch11 DI value
    iRet = DI_Reads( iHandle,                       //the handle for a connection
                     0,                             //unused
                     0,                             //starting channel
                     12,                            //read channel count
                     &dwValue );                    //DI reading value

    CheckErr( iHandle, iRet, (char*)"DI_Reads" );

    printf( "Contiguous DI Value : %d\n", dwValue );

    BYTE bytValue;
    //read ch6 DI value
    iRet = DI_Read( iHandle,                        //the handle for a connection
                    0,                              //unused
                    6,                              //specific channel
                    &bytValue );                    //DI reading value

    CheckErr( iHandle, iRet, (char*)"DI_Read" );

    printf( "specific channel's DI Value : %d\n", bytValue );

    //*********************************
    // disconnect and terminate socket
    //*********************************
    //Disconnect I/O module
    iRet = MXEIO_Disconnect( iHandle );

    CheckErr( iHandle, iRet, (char*)"MXEIO_Disconnect" );

    printf("Disconnect module connection succeeded.\n");

    return 0;
}

//  After each MXIO function call, the application checks whether the call succeeded.
//  If the call failed, this procedure prints an error message and exits.
void CheckErr( int iHandle, int iRet, char * szFunctionName )
{
    if( iRet != MXIO_OK )
    {
        const char * szErrMsg;

        switch( iRet )
        {
        case ILLEGAL_FUNCTION:
            szErrMsg = "ILLEGAL_FUNCTION";
            break;
        case ILLEGAL_DATA_ADDRESS:
            szErrMsg = "ILLEGAL_DATA_ADDRESS";
            break;
        case ILLEGAL_DATA_VALUE:
            szErrMsg = "ILLEGAL_DATA_VALUE";
            break;
        case SLAVE_DEVICE_FAILURE:
            szErrMsg = "SLAVE_DEVICE_FAILURE";
            break;
        case SLAVE_DEVICE_BUSY:
            szErrMsg = "SLAVE_DEVICE_BUSY";
            break;
        case EIO_TIME_OUT:
            szErrMsg = "EIO_TIME_OUT";
            break;
        case EIO_INIT_SOCKETS_FAIL:
            szErrMsg = "EIO_INIT_SOCKETS_FAIL";
            break;
        case EIO_CREATING_SOCKET_ERROR:
            szErrMsg = "EIO_CREATING_SOCKET_ERROR";
            break;
        case EIO_RESPONSE_BAD:
            szErrMsg = "EIO_RESPONSE_BAD";
            break;
        case EIO_SOCKET_DISCONNECT:
            szErrMsg = "EIO_SOCKET_DISCONNECT";
            break;
        case PROTOCOL_TYPE_ERROR:
            szErrMsg = "PROTOCOL_TYPE_ERROR";
            break;
        case SIO_OPEN_FAIL:
            szErrMsg = "SIO_OPEN_FAIL";
            break;
        case SIO_TIME_OUT:
            szErrMsg = "SIO_TIME_OUT";
            break;
        case SIO_CLOSE_FAIL:
            szErrMsg = "SIO_CLOSE_FAIL";
            break;
        case SIO_PURGE_COMM_FAIL:
            szErrMsg = "SIO_PURGE_COMM_FAIL";
            break;
        case SIO_FLUSH_FILE_BUFFERS_FAIL:
            szErrMsg = "SIO_FLUSH_FILE_BUFFERS_FAIL";
            break;
        case SIO_GET_COMM_STATE_FAIL:
            szErrMsg = "SIO_GET_COMM_STATE_FAIL";
            break;
        case SIO_SET_COMM_STATE_FAIL:
            szErrMsg = "SIO_SET_COMM_STATE_FAIL";
            break;
        case SIO_SETUP_COMM_FAIL:
            szErrMsg = "SIO_SETUP_COMM_FAIL";
            break;
        case SIO_SET_COMM_TIME_OUT_FAIL:
            szErrMsg = "SIO_SET_COMM_TIME_OUT_FAIL";
            break;
        case SIO_CLEAR_COMM_FAIL:
            szErrMsg = "SIO_CLEAR_COMM_FAIL";
            break;
        case SIO_RESPONSE_BAD:
            szErrMsg = "SIO_RESPONSE_BAD";
            break;
        case SIO_TRANSMISSION_MODE_ERROR:
            szErrMsg = "SIO_TRANSMISSION_MODE_ERROR";
            break;
        case PRODUCT_NOT_SUPPORT:
            szErrMsg = "PRODUCT_NOT_SUPPORT";
            break;
        case HANDLE_ERROR:
            szErrMsg = "HANDLE_ERROR";
            break;
        case SLOT_OUT_OF_RANGE:
            szErrMsg = "SLOT_OUT_OF_RANGE";
            break;
        case CHANNEL_OUT_OF_RANGE:
            szErrMsg = "CHANNEL_OUT_OF_RANGE";
            break;
        case COIL_TYPE_ERROR:
            szErrMsg = "COIL_TYPE_ERROR";
            break;
        case REGISTER_TYPE_ERROR:
            szErrMsg = "REGISTER_TYPE_ERROR";
            break;
        case FUNCTION_NOT_SUPPORT:
            szErrMsg = "FUNCTION_NOT_SUPPORT";
            break;
        case OUTPUT_VALUE_OUT_OF_RANGE:
            szErrMsg = "OUTPUT_VALUE_OUT_OF_RANGE";
            break;
        case INPUT_VALUE_OUT_OF_RANGE:
            szErrMsg = "INPUT_VALUE_OUT_OF_RANGE";
            break;
        }

        printf( "Function \"%s\" execution Fail. Error Message : %s\n", szFunctionName, szErrMsg );

        MXEIO_Disconnect( iHandle );

        exit(1);
    }
}
