#include "mxio.h"
#include <iostream>

/************************************************************
 * Declare BOOST PYTHON MODULE 
 * *********************************************************/
#include <boost/python.hpp>
#include <boost/python/tuple.hpp>

using namespace boost::python;

std::string moxa_checkret( int iRet, std::string szFunctionName );

int moxa_init(void)
{
    return MXEIO_Init();
}

/***************************************************************
 * Connect to the ioLogick device
 * 
 * SUCCESS : return handle(integer) to device
 * FAIL : return error value
 * ****************************************************************/
int moxa_connect(std::string ipaddress, int port, DWORD timeout)
{
    int iRet;           //stored return code
    char Password[8] = {'\0'};
    int iHandle;
    
    iRet = MXEIO_E1K_Connect(const_cast<char*>(ipaddress.c_str()), port, timeout, &iHandle, Password);
    
    if(iRet == MXIO_OK)
        return iHandle;
    else
        return iRet;
}

/***************************************************************
 * Check the connection to the ioLogick device
 * 
 * SUCCESS : return MXIO_OK
 * FAIL : return error value
 * ****************************************************************/
int moxa_checkconnection(int iHandle, DWORD timeout)
{
    int iRet;
    BYTE bytStatus;
    
    iRet = MXEIO_CheckConnection(iHandle,timeout, &bytStatus);
    
    if(iRet == MXIO_OK)
    {
        switch ( bytStatus )
        {
            case CHECK_CONNECTION_OK :
                std::cout << "Check connection ok !" << std::endl;
                return iRet;
                break;
            case CHECK_CONNECTION_FAIL :
                std::cout << "Check connection fail !" << std::endl;
                return -1;
                break;
            case CHECK_CONNECTION_TIME_OUT :
                std::cout << "Check connection time out !" << std::endl;
                return -2;
                break;
            default:
                std::cout << "Unknown bytStatus" << std::endl;
                return -3;
                break;
        }   
    }
    else
        return iRet;
}

/***************************************************************
 * Check the connected device corresponds to the expected device
 * 
 * SUCCESS : return MXIO_OK
 * FAIL : return error value
 * ****************************************************************/
int moxa_checkdevice(int iHandle, int Device)
{
    WORD wType=0;
    int iRet;
    
    iRet = MXIO_GetModuleType( iHandle, 0, &wType );
    
    if((iRet == MXIO_OK) && wType == Device)
    {
        return iRet;
    }
    else if(iRet != MXIO_OK)
    {
        return iRet;
    }
    else
    {
        return -4;
    }
}

/***************************************************************
 * Read Analog Range for port number.
 * 
 * SUCCESS : return type of analog range, 1 = "4~20mA" or 2 = "0~10V"
 * FAIL : return error value -1 if data error or 0 for input argrument error
 * ****************************************************************/
boost::python::tuple moxa_get_analog_range(int iHandle)
{
    BYTE bytStartChannel = 0;
    BYTE bytCount = 8;
    WORD wValue[16] = {'\0'};
    int* data = new int[bytCount-bytStartChannel];
    int iRet;

    iRet = E1K_AI_GetRanges(iHandle, bytStartChannel, bytCount, wValue);
    moxa_checkret( iRet, (char*)"E1K_AI_GetRanges" );
    if(iRet == MXIO_OK)
    {
        for(int i=bytStartChannel; i < bytCount; i++ )
        {
            data[i-bytStartChannel] = wValue[i-bytStartChannel]?1:2;
        }
    }else{
        for(int i=bytStartChannel; i < bytCount; i++ )
        {
            data[i-bytStartChannel] = -1;
        }
    }
    return make_tuple(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
}

/***************************************************************
 * Read Analog Status
 * 
 * SUCCESS : return tuple of status
 * FAIL : return tuple of -1
 * ****************************************************************/
boost::python::tuple moxa_get_analog_status(int iHandle)
{
    BYTE bytStartChannel = 0;
    BYTE bytCount = 8;
    int* data = new int[bytCount-bytStartChannel];
    int iRet;
    WORD wValue[16] = {'\0'};

    iRet = E1K_AI_GetChannelStatuses(iHandle, bytStartChannel, bytCount, wValue);
    moxa_checkret( iRet, (char*)"E1K_AI_GetChannelStatuses" );
    if(iRet == MXIO_OK)
    {
        for(int i=bytStartChannel; i < bytCount; i++ )
        {
            data[i-bytStartChannel] = wValue[i-bytStartChannel]?1:0;
        }
    }else{
        for(int i=bytStartChannel; i < bytCount; i++ )
        {
            data[i-bytStartChannel] = -1;
        }
    }
    return make_tuple(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
}


/**************************************************************
 * Set Analog Status
 * 
 * SUCCESS : return tuple of new status
 * FAIL : return tuple of -1
 * ***************************************************************/
boost::python::tuple moxa_set_analog_status(int iHandle, boost::python::tuple status)
{
    BYTE bytStartChannel = 0;
    BYTE bytCount = 8;
    int iRet;

    WORD wValue[16] = {'\0'};
    //==========================
    for(int i=0; i < bytCount; i++ ){
        wValue[i] = extract<int>(status[i]);
    }   

    iRet = E1K_AI_SetChannelStatuses(iHandle, bytStartChannel, bytCount, wValue);
    moxa_checkret( iRet, (char*)"E1K_AI_SetChannelStatuses" );
    if(iRet == MXIO_OK)
    {
        return moxa_get_analog_status(iHandle);;
    }else{
        return make_tuple(-1, -1, -1, -1, -1, -1, -1, -1);
    }
}

/**************************************************************
 * Read Analog Value
 * 
 * SUCCESS : return new state
 * FAIL : return error value -1
 * ***************************************************************/
boost::python::tuple moxa_get_analog(int iHandle)
{
    BYTE bytStartChannel = 0;
    BYTE bytCount = 8;
    double dValue[16] = {'\0'};
    int iRet;

    iRet = E1K_AI_Reads(iHandle, bytStartChannel, bytCount, dValue);
    moxa_checkret( iRet, (char*)"E1K_AI_Reads" );
    if(iRet == MXIO_OK)
    {
        return make_tuple(dValue[0], dValue[1], dValue[2], dValue[3], dValue[4], dValue[5], dValue[6], dValue[7]);
    }else{
        return make_tuple(-1, -1, -1, -1, -1, -1, -1, -1);
    }
}

/***************************************************************
 * Close connection to device
 * 
 * SUCCESS : return MXIO_OK
 * FAIL : return error value
 * ****************************************************************/

int moxa_close(int iHandle)
{
    return MXEIO_Disconnect(iHandle);
}

/***************************************************************
 * Convert return code to string
 * ****************************************************************/

std::string moxa_checkret( int iRet, std::string szFunctionName )
{
    std::string szErrMsg;
    std::string retMsg = "";
    
    switch( iRet )
    {
    case MXIO_OK:
        szErrMsg = "FUNCTION_SUCCESS";
        break;
    case -1:
        szErrMsg = "CHECK_CONNECT_FAIL";
        break;
    case -2:
        szErrMsg = "CHECK_CONNECT_TIMEOUT";
        break;
    case -3:
        szErrMsg = "CHECK_CONNECT_FAIL";
        break;
    case -4:
        szErrMsg = "DEVICE_NOT_COMPATIBLE";
        break;
    case -5:
        szErrMsg = "MODE_NOT_YET_SUPPORTED";
        break;
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
    case EIO_PASSWORD_INCORRECT:
        szErrMsg = "EIO_PASSWORD_INCORRECT";
        break;
    default:
        szErrMsg = "DEVICE_HANDLE";
        break;
    }
    
    retMsg = szFunctionName + "," + szErrMsg;
    
    return retMsg;
}

/************************************************************
 * The name in BOOST_PYTHON_MODULE(__name__) has to be the same 
 * that your output file, in this case, we will have to compile
 * with -o moxa.so (see Makefile)
 * **********************************************************/
BOOST_PYTHON_MODULE(moxa)
{
    /* defines the relation between the python function name and the c++ function name */
    def("moxa_init", moxa_init);
    def("moxa_connect", moxa_connect);
    def("moxa_checkconnection", moxa_checkconnection);
    def("moxa_checkdevice", moxa_checkdevice);
    def("moxa_get_analog_range", moxa_get_analog_range);
    def("moxa_get_analog_status", moxa_get_analog_status);
    def("moxa_set_analog_status", moxa_set_analog_status);
    def("moxa_get_analog", moxa_get_analog);
    def("moxa_close", moxa_close);
    def("moxa_checkret", moxa_checkret);
}