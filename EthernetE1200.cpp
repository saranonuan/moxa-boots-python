/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.
*/
// test.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "stdlib.h"
#include "mxio.h"
#include <sys/time.h>

#define IPAddr		"10.50.8.28" 		//IP address for your ioLogik Device
#define Port		502						//Modbus TCP port


#define MOXA_PRINTF( STRING)	printf( STRING)

void CheckErr( int iRet, char * szFunctionName );		//check function execution result

char DbgStr[1024] = {'\0'};

int main(int   argc,   char   *argv[])
{
	int iRet;			//stored return code
	int	iHandle, iHandle2;		//stored handle for ioLogik 2000 serial
	BYTE bytStatus;	 	//stored connection status
	char RetString[64];
	char IPAddress[16] = {'\0'};
	DWORD dwTimeOut=5000;
	char bytInputValue[32] = {'\0'};
	char Password[8] = {'\0'};
	//char Password[8] = {0x31, 0x32, 0x33, 0x34, 0, 0, 0, 0};
	//==========================
	BYTE bytStartChannel = 0;
	BYTE bytCount = 8;
	double dValue[16] = {'\0'};
	WORD wValue[16] = {'\0'};
	DWORD dwValue[16] = {'\0'};
	WORD wHiValue[16] = {'\0'};
	WORD wLoValue[16] = {'\0'};
	BYTE bytValue[16] = {'\0'};
	int k;
	bool ALL_DO = false;
	bool ALL_DI = false;
	//=========================================================================
	sprintf(IPAddress, "%s", IPAddr);
	if(argc > 1)
	{
		sprintf(IPAddress, "%s", argv[1]);
		sprintf( DbgStr,  DbgStr, "IP Address=%s\n", IPAddress);
		MOXA_PRINTF( DbgStr);
	}
	if(argc > 2)
	{
		sprintf(RetString, "%s", argv[2]);
		dwTimeOut = atoi(RetString);
		sprintf( DbgStr, "TimeOut=%d\n", dwTimeOut);
		MOXA_PRINTF( DbgStr);
	}
	if(argc > 3)
	{
		sprintf(Password, "%s", argv[3]);
		sprintf( DbgStr, "Password=%s\n", Password);
		MOXA_PRINTF( DbgStr);
	}

	//=========================================================================
	iRet = MXEIO_Init();
	CheckErr( iRet, (char*)"MXEIO_Init" );
	if(iRet == MXIO_OK)
	{
		sprintf( DbgStr, "MXEIO_Init Initiate the socket succeed.\n");
		MOXA_PRINTF( DbgStr);
	}
	
	//==========================
	iRet = MXEIO_E1K_Connect( IPAddress,		//IP address
						  Port,					//TCP port number
						  dwTimeOut,			//timeout
						  &iHandle,				//connection handle
						  Password);			//ligin password
	CheckErr( iRet, (char*)"MXEIO_E1K_Connect" );
	if(iRet == MXIO_OK)
	{
		sprintf( DbgStr,  "MXEIO_E1K_Connect Creates Adapter IP=%s connection succeed. TimeOut=%d, Password=%s\n",IPAddress, dwTimeOut, Password);
		MOXA_PRINTF( DbgStr);
	}
	else
	{
		sprintf( DbgStr,  "MXEIO_E1K_Connect Failed Adapter IP=%s connection succeed. TimeOut=%d, Password=%s\n",IPAddress, dwTimeOut, Password);
		MOXA_PRINTF( DbgStr);
	}

	//=========================================================================
	//This function establishes a connection to the port of the Ethernet I/O server.
	iRet = MXEIO_CheckConnection( iHandle,			//Connection handle
								  dwTimeOut,		//check connection timeout(ms)
								  &bytStatus);		//Connection Status
	CheckErr( iRet, (char*)"MXEIO_CheckConnection" );

	switch ( bytStatus )
	{
    	case CHECK_CONNECTION_OK :
    		sprintf( DbgStr,  "Check connection ok ! \n" );
    		MOXA_PRINTF( DbgStr);
    		break;
    	case CHECK_CONNECTION_FAIL :
    		sprintf( DbgStr,  "Check connection fail !\n");
    		MOXA_PRINTF( DbgStr);
    		break;
    	case CHECK_CONNECTION_TIME_OUT :
    		sprintf( DbgStr,  "Check connection time out !\n");
    		MOXA_PRINTF( DbgStr);
	}

	//==========================
    iRet = MXIO_GetDllVersion();
	sprintf( DbgStr, "MXIO_GetDllVersion DLL Version:%01d.%01d.%01d.%01d\r\n",(iRet>>12)&0xF, (iRet>>8)&0xF,(iRet>>4)&0xF,(iRet)&0xF);
	//==========================
    iRet = MXIO_GetDllBuildDate();
	if(iRet == MXIO_OK)
	{
		sprintf( DbgStr, "MXIO_GetDllBuildDate DLL release date:%04X/%02X/%02X\r\n",(iRet>>16)&0xFFFF, (iRet>>8)&0xFF, (iRet)&0xFF);
		MOXA_PRINTF( DbgStr);
	}
	//==========================
	BYTE bytRevision[5] = {'\0'};
	iRet = MXIO_ReadFirmwareRevision( iHandle, bytRevision);
	CheckErr( iRet, (char*)"MXIO_ReadFirmwareRevision" );
	if(iRet == MXIO_OK)
	{
		sprintf( DbgStr, "MXIO_ReadFirmwareRevision firmware revision :V%01d.%01d, Release:%01d, build:%01d\r\n",bytRevision[0], bytRevision[1], bytRevision[2], bytRevision[3]);
		MOXA_PRINTF( DbgStr);
	}
	//==========================
	WORD wGetFirmwareDate[2] = {'\0'};
	iRet = MXIO_ReadFirmwareDate( iHandle, wGetFirmwareDate);
	CheckErr( iRet, (char*)"MXIO_ReadFirmwareDate" );
	sprintf( DbgStr, "MXIO_ReadFirmwareDate firmware Release Date:%04X/%02X/%02X\r\n",wGetFirmwareDate[1], (wGetFirmwareDate[0]>>8)&0xFF, (wGetFirmwareDate[0])&0xFF);
	MOXA_PRINTF( DbgStr);
	//=========================================================================
	/*
	iRet = MXIO_Restart( iHandle );

	CheckErr( iRet, (char*)"MXIO_Restart" );
	sprintf( DbgStr, "Restart ioLogik 1200 succeeded.\n");
	MOXA_PRINTF( DbgStr);
	//==========================
	iRet = MXIO_Reset( iHandle );

	CheckErr( iRet, (char*)"MXIO_Reset" );
	sprintf( DbgStr, "MXIO_Reset ioLogik 1200 succeeded.\n");
	MOXA_PRINTF( DbgStr);
	*/
	//=========================================================================
	WORD wType=0;
	iRet = MXIO_GetModuleType( iHandle,			//the handle for a connection
							   0,				//unused
							   &wType );		//module type

	CheckErr( iRet, (char*)"MXIO_GetModuleType" );
	sprintf( DbgStr,  "Module Type : E%X\n", wType );
	MOXA_PRINTF( DbgStr);
	//=========================================================================
	if(wType == 0x1240)
	{
    	//***********************
    	// Write output register
    	//***********************
    	WORD wRegister[8] = { 0x3131, 0x3232, 0x3333, 0x3434, 0x3535, 0x3636, 0x3737, 0x3838 };
    	#define 	E1240_AI_ALIAS_NAME		0x10A8
    	//write 0x0000 ~ 0x0001 Output Register
    	iRet = MXIO_WriteRegs( iHandle,				//the handle for a connection
    						   E1240_AI_ALIAS_NAME,	//starting register address
    						   8,					//write registers count
    						   wRegister );			//write registers value

    	CheckErr( iRet, (char*)"MXIO_WriteRegs" );

    	//*********************************
    	// Read input and output registers
    	//*********************************
    	for(int i=0; i < 8; i++)
    		wRegister[i] = 0;
    	iRet = MXIO_ReadRegs( iHandle,				//the handle for a connection
    						  3,					//register type
    												//		3 : read holding registers
    												//		4 : read input register
    						  E1240_AI_ALIAS_NAME,	//starting register address
    						  8,					//read registers count
    						  wRegister ); 			//read registers value

    	CheckErr( iRet, (char*)"MXIO_Regs" );

    	sprintf( DbgStr,  "Output Registers : 0x%X, 0x%X\n", wRegister[0], wRegister[1] );
    	MOXA_PRINTF( DbgStr);
    	//==========================
    	//********************
    	// Write output coils
    	//********************
    	BYTE bytCoils[1] = { 0x0F };
    	#define		E1240_AI_ENABLE		0x1010
    	//write 0x0000 ~ 0x0007 output bits
    	iRet = MXIO_WriteCoils( iHandle,			//the handle for a connection
    							E1240_AI_ENABLE,	//starting coil address
    							8,					//write coils count
    							bytCoils);			//wirte coils value

    	CheckErr( iRet, (char*)"MXIO_WriteCoils" );
    	sprintf( DbgStr,  "Write coils succeeded.\n" );
    	MOXA_PRINTF( DbgStr);

    	//*****************************
    	// Read input and output coils
    	//*****************************
    	//read 0x0000 ~ 0x0007 output bits
    	bytCoils[0] = 0;
    	iRet = MXIO_ReadCoils( iHandle,				//the handle for a connection
    						   1,					//coil type   1:read coils, 2:read discrete
    						   E1240_AI_ENABLE,		//starting coil address
    						   8,					//read coils count
    						   bytCoils	);			//an array that stored the coil value

    	CheckErr( iRet, (char*)"MXIO_ReadCoils" );
    	sprintf( DbgStr,  "Output Bits : %d\n", bytCoils[0] );
    	MOXA_PRINTF( DbgStr);
    }
	//==========================
	//***********************
	// Get/Clear Safe status
	//***********************
	WORD wSafeStatus;		//Safe status :
							// 0 : Normal
							// 1 : Safe Mode
	//Get Safe status
	iRet = E1K_GetSafeStatus( iHandle,			//the handle for a connection
	   					      &wSafeStatus );	//Safe status

	CheckErr( iRet, (char*)"E1K_GetSafeStatus" );
	sprintf( DbgStr,  "Get SafeStatus succeeded. Status : %d\n", wSafeStatus );
	MOXA_PRINTF( DbgStr);

	//==========================
	if( wSafeStatus == 1 )
	{
		//Clear Safe status
		iRet = E1K_ClearSafeStatus( iHandle );

		CheckErr( iRet, (char*)"E1K_ClearSafeStatus" );
		sprintf( DbgStr,  "Clear SafeStatus succeeded.\n" );
		MOXA_PRINTF( DbgStr);
	}
	//=========================================================================
	if(wType == 0x1212)
	{
		bytStartChannel = 0;
		bytCount = 8;
		dwValue[0] = 0;
		iRet = E1K_DIO_GetIOModes(iHandle, bytStartChannel, bytCount, &dwValue[0]);
		CheckErr( iRet, (char*)"E1K_DIO_GetIOModes" );
		if(iRet == MXIO_OK)
		{
			sprintf( DbgStr, "E1K_DIO_GetIOModes succeed.\n");
			MOXA_PRINTF( DbgStr);
			for(int i=bytStartChannel; i < bytCount; i++ )
			{
				if(dwValue[0]&(1 << i))
					ALL_DO = true;
				else
					ALL_DI = true;
				sprintf( DbgStr, "ch[%d]=%s\n", i-bytStartChannel, (dwValue[0]&(1 << i))?"DO":"DI");
				MOXA_PRINTF( DbgStr);
			}
		}
		//==========================
		#define E1212_DIO_MODE_W_ADDR		0x1000
		for(int i=0; i < 8; i++)
    		wValue[i] = 0;
    	iRet = MXIO_ReadRegs( iHandle,				//the handle for a connection
    						  3,					//register type
    												//		3 : read holding registers
    												//		4 : read input register
    						  E1212_DIO_MODE_W_ADDR,	//starting register address
    						  8,					//read registers count
    						  wValue ); 			//read registers value

    	CheckErr( iRet, (char*)"MXIO_Regs" );

    	sprintf( DbgStr,  "Output Registers[0]~[7] : 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X, 0x%X\n",
			wValue[0], wValue[1], wValue[2], wValue[3], wValue[4], wValue[5], wValue[6], wValue[7]);
    	MOXA_PRINTF( DbgStr);	
	}
	//=========================================================================
	// DI Channel
	//==========================
	sprintf( DbgStr, "***** Do you Want to test DI channel(DI Mode)?\nPress Enter to bypass this Test, 1:Test =");
	MOXA_PRINTF( DbgStr);
	gets(bytInputValue);
	if(bytInputValue[0] != 0)
	{
		if((wType == 0x1210) || (wType == 0x1212) || (wType == 0x1214))
    	{
    		bytStartChannel = 0;
    		if(wType == 0x1210)
    			bytCount = 16;
			if(wType == 0x1212)
			{
				bytCount = 8;
				if(ALL_DI)
    				bytCount += 8;
			}
			if(wType == 0x1214)
    			bytCount = 6;
    		//==========================
    		sprintf( DbgStr, "***** Write DI Direction to Count Mode (0:DI Mode) ***** : \n");
    		MOXA_PRINTF( DbgStr);
    		for(k=0; k < bytCount; k++)
        		wValue[k] = 0;
        	iRet = E1K_DI_SetModes(iHandle, bytStartChannel, bytCount, wValue);
        	CheckErr( iRet, (char*)"E1K_DI_SetModes" );
        	if(iRet == MXIO_OK)
        	{
        		sprintf( DbgStr, "E1K_DI_SetModes succeed.\n");
        		MOXA_PRINTF( DbgStr);
        		for(int i=bytStartChannel; i < bytCount; i++ )
        		{
        			sprintf( DbgStr, "ch[%d]=%s\n", i-bytStartChannel, wValue[i-bytStartChannel]?"Count Mode":"DI mode");
        			MOXA_PRINTF( DbgStr);
        		}
        	}
        	//==========================
        	for(int i=0; i < bytCount; i++ )
        		wValue[i] = 0;
        	iRet = E1K_DI_GetModes(iHandle, bytStartChannel, bytCount, wValue);
        	CheckErr( iRet, (char*)"E1K_DI_GetModes" );
        	if(iRet == MXIO_OK)
        	{
        		sprintf( DbgStr, "E1K_DI_GetModes succeed.\n");
        		MOXA_PRINTF( DbgStr);
        		for(int i=bytStartChannel; i < bytCount; i++ )
        		{
        			sprintf( DbgStr, "ch[%d]=%s\n", i-bytStartChannel, wValue[i-bytStartChannel]?"Count Mode":"DI mode");
        			MOXA_PRINTF( DbgStr);
        		}
        	}
    		sprintf( DbgStr, "***** Write DI Direction DI Filter(0~65535) ***** : \n");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) < 65536)
            		for(k=0; k < bytCount; k++)
    					wValue[k] = atoi(bytInputValue);
            	else
            		for(k=0; k < bytCount; k++)
    					wValue[k] = 100;
            }
        	iRet = E1K_DI_SetFilters(iHandle, bytStartChannel, bytCount, wValue);
        	CheckErr( iRet, (char*)"E1K_DI_SetFilters" );
        	if(iRet == MXIO_OK)
        	{
        		sprintf( DbgStr, "E1K_DI_SetFilters succeed.\n");
        		MOXA_PRINTF( DbgStr);
        		for(int i=bytStartChannel; i < bytCount; i++ )
        		{
        			sprintf( DbgStr, "ch[%d]=%d\n", i-bytStartChannel, wValue[i-bytStartChannel]);
        			MOXA_PRINTF( DbgStr);
        		}
        	}//==========================
        	iRet = E1K_DI_GetFilters(iHandle, bytStartChannel, bytCount, wValue);
        	CheckErr( iRet, (char*)"E1K_DI_GetFilters" );
        	if(iRet == MXIO_OK)
        	{
        		sprintf( DbgStr, "E1K_DI_GetFilters succeed.\n");
        		MOXA_PRINTF( DbgStr);
        		for(int i=bytStartChannel; i < bytCount; i++ )
        		{
        			sprintf( DbgStr, "ch[%d]=%d\n", i-bytStartChannel, wValue[i-bytStartChannel]);
        			MOXA_PRINTF( DbgStr);
        		}
        	}
			//==========================
			//after set DI mode function, FW will clear DI Status to OFF, We should wait DI filter time to get correct status
    		iRet = E1K_DI_Reads(iHandle, bytStartChannel, bytCount, dwValue);
        	CheckErr( iRet, (char*)"E1K_DI_Reads" );
        	if(iRet == MXIO_OK)
        	{
        		sprintf( DbgStr, "E1K_DI_Reads succeed.\n");
        		MOXA_PRINTF( DbgStr);
        		for(int i=bytStartChannel; i < bytCount; i++ )
        		{
    				sprintf( DbgStr, "ch[%d]=%s\n", i-bytStartChannel, (dwValue[0]&(1 << (i-bytStartChannel)))?"ON":"OFF");
    				MOXA_PRINTF( DbgStr);
    			}
        	}
        	//==========================
    	}
    }
	//=========================================================================
	// Cnt Channel
	//==========================
	sprintf( DbgStr, "***** Do you Want to test DI channel(Count Mode)?\nPress Enter to bypass this Test, 1:Test =");
	MOXA_PRINTF( DbgStr);
	gets(bytInputValue);
	if(bytInputValue[0] != 0)
	{
		if((wType == 0x1210) || (wType == 0x1212) || (wType == 0x1214))
    	{
    		bytStartChannel = 0;
			if(wType == 0x1210)
    			bytCount = 16;
			if(wType == 0x1212)
    		{
				bytCount = 8;
				if(ALL_DI)
    				bytCount += 8;
			}
			if(wType == 0x1214)
    			bytCount = 6;
    		//==========================
    		sprintf( DbgStr, "***** Write DI Direction to Count Mode (1:Count Mode) ***** : \n");
    		MOXA_PRINTF( DbgStr);
    		for(k=0; k < bytCount; k++)
        		wValue[k] = 1;
        	iRet = E1K_DI_SetModes(iHandle, bytStartChannel, bytCount, wValue);
        	CheckErr( iRet, (char*)"E1K_DI_SetModes" );
        	if(iRet == MXIO_OK)
        	{
        		sprintf( DbgStr, "E1K_DI_SetModes succeed.\n");
        		MOXA_PRINTF( DbgStr);
        		for(int i=bytStartChannel; i < bytCount; i++ )
        		{
        			sprintf( DbgStr, "ch[%d]=%s\n", i-bytStartChannel, wValue[i-bytStartChannel]?"Count Mode":"DI mode");
        			MOXA_PRINTF( DbgStr);
        		}
        	}
        	//==========================
        	for(int i=0; i < bytCount; i++ )
        		wValue[i] = 0;
        	iRet = E1K_DI_GetModes(iHandle, bytStartChannel, bytCount, wValue);
        	CheckErr( iRet, (char*)"E1K_DI_GetModes" );
        	if(iRet == MXIO_OK)
        	{
        		sprintf( DbgStr, "E1K_DI_GetModes succeed.\n");
        		MOXA_PRINTF( DbgStr);
        		for(int i=bytStartChannel; i < bytCount; i++ )
        		{
        			sprintf( DbgStr, "ch[%d]=%s\n", i-bytStartChannel, wValue[i-bytStartChannel]?"Count Mode":"DI mode");
        			MOXA_PRINTF( DbgStr);
        		}
        	}
    		//==========================
    		sprintf( DbgStr, "***** Write DI channel count start status to (0:Stop, 1:Start) ***** : \n");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) == 1)
            		dwValue[0] = 0xFFFFFFFF;
            	else
            		dwValue[0] = 0;
            }
    		iRet = E1K_Cnt_SetStartStatuses( iHandle, bytStartChannel, bytCount, dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Cnt_SetStartStatuses" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_SetStartStatuses success,DI %d Channels: %s\r\n", bytCount, ((dwValue[0]&0x01) >= 1)?"Start":"Stop");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		dwValue[0] = 0;
    		iRet = E1K_Cnt_GetStartStatuses( iHandle, bytStartChannel, bytCount, &dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Cnt_GetStartStatuses" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_GetStartStatuses success,DI %d Channels:\r\n", bytCount);
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, ((dwValue[0] & (0x01 << k)) > 0)?"Start":"Stop");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
			//==========================
    		sprintf( DbgStr, "***** Write DI Direction Count Filter(0~65535) ***** : \n");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) < 65536)
            		for(k=0; k < bytCount; k++)
    					wValue[k] = atoi(bytInputValue);
            	else
            		for(k=0; k < bytCount; k++)
    					wValue[k] = 400;
            }
    		iRet = E1K_Cnt_SetFilters( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_Cnt_SetFilters" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_SetFilters success,All DI Count %d Channel: %d\r\n", bytCount, wValue[0]);
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		for(k=0; k < bytCount; k++)
    			wValue[k] = 0;
    		iRet = E1K_Cnt_GetFilters( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_Cnt_GetFilters" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_GetFilters success,Channel Value:\r\n");
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %d, ", k, wValue[k]);
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
        	//==========================
        	for(int i=0; i < bytCount; i++ )
        		dwValue[i] = 0;
    		iRet = E1K_Cnt_Reads( iHandle, bytStartChannel, bytCount, dwValue);
    		CheckErr( iRet, (char*)"E1K_Cnt_Reads" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_Reads success,All DI Count %d Channel:\r\n", bytCount);
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %d, ", k, dwValue[k]);
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		iRet = E1K_Cnt_Clears( iHandle, bytStartChannel, bytCount);
    		CheckErr( iRet, (char*)"E1K_Cnt_Clears" );
    		if(iRet == MXIO_OK)
    			sprintf( DbgStr, "E1K_Cnt_Clears success, All DI Channel Counts Clear.\r\n");
    		//==========================
    		dwValue[0] = 0;
    		iRet = E1K_Cnt_GetOverflows( iHandle, bytStartChannel, bytCount, &dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Cnt_GetOverflows" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_GetOverflows success,All DI Count %d Channel:\r\n", bytCount);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, ((dwValue[0]&(0x01 << k)) >= 1)?"Overflow":"Normal");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		iRet = E1K_Cnt_ClearOverflows( iHandle, bytStartChannel, bytCount);
    		CheckErr( iRet, (char*)"E1K_Cnt_ClearOverflows" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_ClearOverflows success, All DI Channel Overflow Clear.\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		sprintf( DbgStr, "***** Write DI channel Count Power On Value to (0: OFF, 1: ON) ***** : \n");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) == 1)
            		dwValue[0] = 0xFFFFFFFF;
            	else
            		dwValue[0] = 0;
            }
    		iRet = E1K_Cnt_SetPowerOnValues( iHandle, bytStartChannel, bytCount, dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Cnt_SetPowerOnValues" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_SetPowerOnValues success,DI %d Channels: %s\r\n", bytCount, ((dwValue[0]&0x01) >= 1)?"ON":"OFF");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		dwValue[0] = 0;
    		iRet = E1K_Cnt_GetPowerOnValues( iHandle, bytStartChannel, bytCount, &dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Cnt_GetPowerOnValues" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_GetPowerOnValues success,DI %d Channels:\r\n", bytCount);
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, ((dwValue[0] & (0x01 << k)) > 0)?"ON":"OFF");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		sprintf( DbgStr, "***** Write DI channel Count Safe Value to (0: OFF, 1: ON) ***** : ");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) == 1)
            		dwValue[0] = 0xFFFFFFFF;
            	else
            		dwValue[0] = 0;
            }
    		iRet = E1K_Cnt_SetSafeValues( iHandle, bytStartChannel, bytCount, dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Cnt_SetSafeValues" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_SetSafeValues success,DI %d Channels: %s\r\n", bytCount, ((dwValue[0]&0x01) >= 1)?"ON":"OFF");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		dwValue[0] = 0;
    		iRet = E1K_Cnt_GetSafeValues( iHandle, bytStartChannel, bytCount, &dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Cnt_GetSafeValues" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_GetSafeValues success,DI %d Channels:\r\n", bytCount);
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, ((dwValue[0] & (0x01 << k)) > 0)?"ON":"OFF");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		sprintf( DbgStr, "***** Write DI Direction Count Trigger type(0: LoToHi, 1: HiToLo, 2: Both) ***** : \n");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) < 3)
            		for(k=0; k < bytCount; k++)
    					wValue[k] = atoi(bytInputValue);
            	else
            		for(k=0; k < bytCount; k++)
    					wValue[k] = 2;
            }
    		iRet = E1K_Cnt_SetTriggerTypeWords( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_Cnt_SetTriggerTypeWords" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_SetTriggerTypeWords success,All DI Count %d Channel: %d\r\n", bytCount, wValue[0]);
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		for(int i=0; i < bytCount; i++ )
        		wValue[i] = 0;
    		iRet = E1K_Cnt_GetTriggerTypeWords( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_Cnt_GetTriggerTypeWords" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_GetTriggerTypeWords success,Channel Value:\r\n");
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %d, ", k, wValue[k]);
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		sprintf( DbgStr, "***** Write DI channel count save status on Power Fail to (0: OFF, 1: ON) ***** : \n");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) == 1)
            		dwValue[0] = 0xFFFFFFFF;
            	else
            		dwValue[0] = 0;
            }
    		iRet = E1K_Cnt_SetSaveStatusesOnPowerFail( iHandle, bytStartChannel, bytCount, dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Cnt_SetSaveStatusesOnPowerFail" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_SetSaveStatusesOnPowerFail success,DI %d Channels: %s\r\n", bytCount, ((dwValue[0]&0x01) >= 1)?"ON":"OFF");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		dwValue[0] = 0;
    		iRet = E1K_Cnt_GetSaveStatusesOnPowerFail( iHandle, bytStartChannel, bytCount, &dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Cnt_GetSaveStatusesOnPowerFail" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Cnt_GetSaveStatusesOnPowerFail success,DI %d Channels:\r\n", bytCount);
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, ((dwValue[0] & (0x01 << k)) > 0)?"ON":"OFF");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    	}
	}
	//=========================================================================
	// DO Channel
	//==========================
	sprintf( DbgStr, "***** Do you Want to test DO channel(DO Mode)?\nPress Enter to bypass this Test, 1:Test =");
	MOXA_PRINTF( DbgStr);
	gets(bytInputValue);
	if(bytInputValue[0] != 0)
	{
		if((wType == 0x1211) || (wType == 0x1212) || (wType == 0x1214))
    	{
			bytStartChannel = 0;
			if(wType == 0x1211)
    			bytCount = 16;
			if(wType == 0x1212)
    		{
				if(ALL_DO)
    				bytCount = 8;
			}
			if(wType == 0x1214)
    			bytCount = 6;

    		for( k=0; k < bytCount; k++)
    			wValue[k] = 0;
    		sprintf( DbgStr, "***** Write DO Direction to DO Mode (0:DO Mode) ***** : \n");
    		MOXA_PRINTF( DbgStr);
    		iRet = E1K_DO_SetModes( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_DO_SetModes" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_SetModes success,All DO %d Channel: %s\r\n", bytCount, (wValue[0] > 0)?"Pulse Mode":"DO Mode");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		iRet = E1K_DO_GetModes( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_DO_GetModes" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_GetModes success,Channel Value:\r\n");
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, (wValue[k] > 0)?"Pulse Mode":"DO Mode");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		DWORD dwDoGetValue = 0;
    		sprintf( DbgStr, "Set DO PowerOn Value (0=OFF or 1=ON):");
    		MOXA_PRINTF( DbgStr);
        	gets(bytInputValue);
        	if(atoi(bytInputValue) == 1)
    			dwDoGetValue = 0xFFFFFFFF;
    		else
        		dwDoGetValue = 0;

    		iRet = E1K_DO_SetPowerOnValues( iHandle, bytStartChannel, bytCount, dwDoGetValue);
    		CheckErr( iRet, (char*)"E1K_DO_SetPowerOnValues" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_SetPowerOnValues success,Set all DO Power On Value = %s\r\n", (dwDoGetValue == 0)?"OFF":"ON");
    			MOXA_PRINTF( DbgStr);
        	}
    		//==========================
    		sprintf( DbgStr, "Get DO PowerOn Value (0=OFF or 1=ON):");
    		MOXA_PRINTF( DbgStr);
    		iRet = E1K_DO_GetPowerOnValues( iHandle, bytStartChannel, bytCount, &dwDoGetValue);
    		CheckErr( iRet, (char*)"E1K_DO_GetPowerOnValues" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_GetPowerOnValues success,\r\n");
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				sprintf( DbgStr, "Get Power On DO[%d]= %s\r\n", k, ((dwDoGetValue & (0x01 << k)) > 0)?"ON":"OFF");
    				MOXA_PRINTF( DbgStr);
    			}
        	}
    		//==========================
    		sprintf( DbgStr, "Set DO Safe Value (0=OFF, 1=ON or 2: Hold Last(E1214 Only)):");
    		MOXA_PRINTF( DbgStr);
        	gets(bytInputValue);
        	if(atoi(bytInputValue) <= 2)
        	{
            	for(k=0; k < bytCount; k++)
            		wValue[k] = atoi(bytInputValue);
            }
            else
    		{
            	for(k=0; k < bytCount; k++)
            		wValue[k] = 0;
            }
    		iRet = E1K_DO_SetSafeValues_W( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_DO_SetSafeValues_W" );
    		if(iRet == MXIO_OK)
    		{
    			if(wValue[0] < 2)
    			{
    				sprintf( DbgStr, "E1K_DO_SetSafeValues_W success,Set all DO Safe Value = %s\r\n", (wValue[0] == 0)?"OFF":"ON");
    				MOXA_PRINTF( DbgStr);
    			}
    			else
    			{
    				sprintf( DbgStr, "E1K_DO_SetSafeValues_W success,Set all DO Safe Value = %s\r\n", "Hold Last");
    				MOXA_PRINTF( DbgStr);
    			}
        	}
    		//==========================
    		sprintf( DbgStr, "Get DO Safe Value (0=OFF, 1=ON or 2: Hold Last):");
    		MOXA_PRINTF( DbgStr);
    		iRet = E1K_DO_GetSafeValues_W( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_DO_GetSafeValues_W" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_GetSafeValues_W success,\r\n");
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if(wValue[k] < 2)
    				{
    					sprintf( DbgStr, "Get Safe DO[%d]= %s\r\n", k, (wValue[k] == 0)?"OFF":"ON");
    					MOXA_PRINTF( DbgStr);
    				}
    				else if(wValue[k] == 2)
    				{
    					sprintf( DbgStr, "Get Safe DO[%d]= %s\r\n", k, "Hold Last");
    					MOXA_PRINTF( DbgStr);
    				}
    				else
    				{
    					sprintf( DbgStr, "Get Safe DO[%d]= %s (%d)\r\n", k, "Unknown Value", wValue[k]);
    					MOXA_PRINTF( DbgStr);
    				}
    			}
        	}
        	//==========================
    		sprintf( DbgStr, "***** Enter power on sequence delay time (Second, 0~300)***** : \n");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) < 300)
            		for(k=0; k < bytCount; k++)
    					wValue[k] = atoi(bytInputValue);
            	else
            		for(k=0; k < bytCount; k++)
    					wValue[k] = 5;
            }
    		iRet = E1K_DO_SetPowerOnSeqDelaytimes( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_DO_SetPowerOnSeqDelaytimes" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_SetPowerOnSeqDelaytimes success,All DO %d Channel: %s\r\n", bytCount, (wValue[0] > 0)?"Pulse Mode":"DO Mode");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		for(k=0; k < bytCount; k++)
    			wValue[k] = 0;
    		iRet = E1K_DO_GetPowerOnSeqDelaytimes( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_DO_GetPowerOnSeqDelaytimes" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_GetPowerOnSeqDelaytimes success,Channel Value:\r\n");
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %d, ", k, wValue);
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		sprintf( DbgStr, "***** Write DO channel to (0:OFF, 1:ON) ***** : ");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) == 1)
            		dwValue[0] = 0xFFFFFFFF;
            	else
            		dwValue[0] = 0;
            }
    		iRet = E1K_DO_Writes( iHandle, bytStartChannel, bytCount, dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_DO_Writes" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_Writes success,DO %d Channels: %s\r\n", bytCount, ((dwValue[0]&0x01) >= 1)?"ON":"OFF");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		iRet = E1K_DO_Reads( iHandle, bytStartChannel, bytCount, &dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_DO_Reads" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_Reads success,DO %d Channels:\r\n", bytCount);
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, ((dwValue[0] & (0x01 << k)) > 0)?"ON":"OFF");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    	}
	}
	//=========================================================================
	// Pulse Channel
	//==========================
	sprintf( DbgStr, "***** Do you Want to test DO channel(Pulse Mode)?\nPress Enter to bypass this Test, 1:Test =");
	MOXA_PRINTF( DbgStr);
	gets(bytInputValue);
	if(bytInputValue[0] != 0)
	{
		if((wType == 0x1211) || (wType == 0x1212) || (wType == 0x1214))
    	{
    		bytStartChannel = 0;
			if(wType == 0x1211)
    			bytCount = 16;
			if(wType == 0x1212)
    		{
				if(ALL_DO)
    				bytCount = 8;
			}
			if(wType == 0x1214)
    			bytCount = 6;

    		for( k=0; k < bytCount; k++)
    			wValue[k] = 1;
    		sprintf( DbgStr, "***** Write DO Direction to Pulse Mode (1:Pulse Mode) ***** : ");
    		MOXA_PRINTF( DbgStr);
    		iRet = E1K_DO_SetModes( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_DO_SetModes" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_SetModes success,All DO %d Channel: %s\r\n", bytCount, (wValue[0] > 0)?"Pulse Mode":"DO Mode");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		iRet = E1K_DO_GetModes( iHandle, bytStartChannel, bytCount, wValue);
    		CheckErr( iRet, (char*)"E1K_DO_GetModes" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_DO_GetModes success,Channel Value:\r\n");
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, (wValue[k] > 0)?"Pulse Mode":"DO Mode");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		sprintf( DbgStr, "***** Write DO Direction Pulse Signal Width High(0~65535) ***** : ");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) < 65535)
            		for(k=0; k < bytCount; k++)
    					wHiValue[k] = atoi(bytInputValue);
            	else
            		for(k=0; k < bytCount; k++)
    					wHiValue[k] = 65535;
            }
    		sprintf( DbgStr, "***** Write DO Direction Pulse Signal Width Low(0~65535) ***** : ");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) < 65535)
            		for(k=0; k < bytCount; k++)
    					wLoValue[k] = atoi(bytInputValue);
            	else
            		for(k=0; k < bytCount; k++)
    					wLoValue[k] = 65535;
            }
    		iRet = E1K_Pulse_SetSignalWidths( iHandle, bytStartChannel, bytCount, wHiValue, wLoValue);
    		CheckErr( iRet, (char*)"E1K_Pulse_SetSignalWidths" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Pulse_SetSignalWidths success,All DO Count %d Channel: H:%d/L:%d\r\n", bytCount,
    				wHiValue[0], wLoValue[0]);
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		for(k=0; k < bytCount; k++)
    		{
    			wHiValue[k] = 0;
    			wLoValue[k] = 0;
    		}
    		iRet = E1K_Pulse_GetSignalWidths( iHandle, bytStartChannel, bytCount, wHiValue, wLoValue);
    		CheckErr( iRet, (char*)"E1K_Pulse_GetSignalWidths" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Pulse_GetSignalWidths success,Channel Value:\r\n");
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: H:%d/L:%d, ", k, wHiValue[k], wLoValue[k]);
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		sprintf( DbgStr, "***** Write DO Direction Pulse Output Count(0~4294967295) ***** : ");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) < 4294967295)
            		for(k=0; k < bytCount; k++)
    					dwValue[k] = atoi(bytInputValue);
            	else
            		for(k=0; k < bytCount; k++)
    					dwValue[k] = 0xFFFFFFFF;//4294967295
            }
    		iRet = E1K_Pulse_SetOutputCounts( iHandle, bytStartChannel, bytCount, dwValue);
    		CheckErr( iRet, (char*)"E1K_Pulse_SetOutputCounts" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Pulse_SetOutputCounts success,All DO Count %d Channel: %d\r\n", bytCount, dwValue[0]);
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		iRet = E1K_Pulse_GetOutputCounts( iHandle, bytStartChannel, bytCount, dwValue);
    		CheckErr( iRet, (char*)"E1K_Pulse_GetOutputCounts" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Pulse_GetOutputCounts success,Channel Value:\r\n");
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %d, ", k, dwValue[k]);
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		sprintf( DbgStr, "***** Write DO channel to (0: Stop, 1: Start) ***** : ");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    			{
            	if(atoi(bytInputValue) == 1)
            		dwValue[0] = 0xFFFFFFFF;
            	else
            		dwValue[0] = 0;
            }
    		iRet = E1K_Pulse_SetStartStatuses( iHandle, bytStartChannel, bytCount, dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Pulse_SetStartStatuses" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Pulse_SetStartStatuses success,DO %d Channels: %s\r\n", bytCount, ((dwValue[0]&0x01) >= 1)?"Start":"Stop");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		iRet = E1K_Pulse_GetStartStatuses( iHandle, bytStartChannel, bytCount, &dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Pulse_GetStartStatuses" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Pulse_GetStartStatuses success,DO %d Channels:\r\n", bytCount);
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, ((dwValue[0]&(0x01 << k)) >= 1)?"Start":"Stop");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		sprintf( DbgStr, "***** Write DO channel Power On to (0: Stop, 1: Start) ***** : ");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) == 1)
            		dwValue[0] = 0xFFFFFFFF;
            	else
            		dwValue[0] = 0;
            }
    		iRet = E1K_Pulse_SetPowerOnValues( iHandle, bytStartChannel, bytCount, dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Pulse_SetPowerOnValues" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Pulse_SetPowerOnValues success,DO %d Channels: %s\r\n", bytCount, ((dwValue[0]&0x01) >= 1)?"Start":"Stop");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		iRet = E1K_Pulse_GetPowerOnValues( iHandle, bytStartChannel, bytCount, &dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Pulse_GetPowerOnValues" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Pulse_GetPowerOnValues success,DO %d Channels:\r\n", bytCount);
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, ((dwValue[0]&(0x01 << k)) >= 1)?"Start":"Stop");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		sprintf( DbgStr, "***** Write DO channel safe value to (0: Stop, 1: Start) ***** : ");
    		MOXA_PRINTF( DbgStr);
    		gets(bytInputValue);
    		if(bytInputValue[0] != 0)
    		{
            	if(atoi(bytInputValue) == 1)
            		dwValue[0] = 0xFFFFFFFF;
            	else
            		dwValue[0] = 0;
            }
    		iRet = E1K_Pulse_SetSafeValues( iHandle, bytStartChannel, bytCount, dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Pulse_SetSafeValues" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Pulse_SetSafeValues success,DO %d Channels: %s\r\n", bytCount, ((dwValue[0]&0x01) >= 1)?"Start":"Stop");
    			MOXA_PRINTF( DbgStr);
    		}
    		//==========================
    		iRet = E1K_Pulse_GetSafeValues( iHandle, bytStartChannel, bytCount, &dwValue[0]);
    		CheckErr( iRet, (char*)"E1K_Pulse_GetSafeValues" );
    		if(iRet == MXIO_OK)
    		{
    			sprintf( DbgStr, "E1K_Pulse_GetSafeValues success,DO %d Channels:\r\n", bytCount);
    			MOXA_PRINTF( DbgStr);
    			for(k=0; k < bytCount; k++)
    			{
    				if((k != 0) && ((k%4)== 0))
    				{
    					sprintf( DbgStr, "\r\n");
    					MOXA_PRINTF( DbgStr);
    				}
    				sprintf( DbgStr, "Ch[%02d]: %s, ", k, ((dwValue[0]&(0x01 << k)) >= 1)?"Start":"Stop");
    				MOXA_PRINTF( DbgStr);
    			}
    			sprintf( DbgStr, "\r\n");
    			MOXA_PRINTF( DbgStr);
    		}
    	}
	}
	//=========================================================================
	// RLY Channel
	//==========================
	if(wType == 0x1214)
	{
		bytStartChannel = 0;
		bytCount = 6;
    	iRet = E1K_RLY_TotalCntReads(iHandle, bytStartChannel, bytCount, dwValue);
    	CheckErr( iRet, (char*)"E1K_RLY_TotalCntReads" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_RLY_TotalCntReads succeed.\n");
    		MOXA_PRINTF( DbgStr);
    		for(int i=bytStartChannel; i < bytCount; i++ )
    		{
    			sprintf( DbgStr, "ch[%d]=%d\n", i-bytStartChannel, dwValue[i-bytStartChannel]);
    			MOXA_PRINTF( DbgStr);
    		}
    	}
    }
	//=========================================================================
	// AI Channel
	//==========================
	if(wType == 0x1240)
	{
		bytStartChannel = 0;
		bytCount = 8;
    	iRet = E1K_AI_GetRanges(iHandle, bytStartChannel, bytCount, wValue);
    	CheckErr( iRet, (char*)"E1K_AI_GetRanges" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_GetRanges succeed.\n");
    		MOXA_PRINTF( DbgStr);
    		for(int i=bytStartChannel; i < bytCount; i++ )
    		{
    			sprintf( DbgStr, "ch[%d]=%s\n", i-bytStartChannel, wValue[i-bytStartChannel]?"4~20mA":"0~10V");
    			MOXA_PRINTF( DbgStr);
    		}
    	}
    	//==========================
    	for(int i=0; i < bytCount; i++ )
    		wValue[i] = 1;//Enable
    	iRet = E1K_AI_SetChannelStatuses(iHandle, bytStartChannel, bytCount, wValue);
    	CheckErr( iRet, (char*)"E1K_AI_SetChannelStatuses" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_SetChannelStatuses succeed.\n");
    		MOXA_PRINTF( DbgStr);
    		for(int i=bytStartChannel; i < bytCount; i++ )
    		{
    			sprintf( DbgStr, "ch[%d]=%s\n", i-bytStartChannel, (wValue[0])?"Enable":"Disable");
    			MOXA_PRINTF( DbgStr);
    		}
    	}
    	//==========================
    	iRet = E1K_AI_GetChannelStatuses(iHandle, bytStartChannel, bytCount, wValue);
    	CheckErr( iRet, (char*)"E1K_AI_GetChannelStatuses" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_GetChannelStatuses succeed.\n");
    		MOXA_PRINTF( DbgStr);
    		for(int i=bytStartChannel; i < bytCount; i++ )
    		{
    			sprintf( DbgStr, "ch[%d]=%s\n", i-bytStartChannel, (wValue[0])?"Enable":"Disable");
    			MOXA_PRINTF( DbgStr);
    		}
    	}
    	//==========================
    	iRet = E1K_AI_Reads(iHandle, bytStartChannel, bytCount, dValue);
    	CheckErr( iRet, (char*)"E1K_AI_Reads" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_Reads succeed.\n");
    		MOXA_PRINTF( DbgStr);
    		for(int i=bytStartChannel; i < bytCount; i++ )
    		{
    			sprintf( DbgStr, "dValue[%d]=%f\n", i-bytStartChannel, dValue[i-bytStartChannel]);
    			MOXA_PRINTF( DbgStr);
    		}
    	}
    	//==========================
    	iRet = E1K_AI_ReadRaws(iHandle, bytStartChannel, bytCount, wValue);
    	CheckErr( iRet, (char*)"E1K_AI_ReadRaws" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_ReadRaws succeed.\n");
    		MOXA_PRINTF( DbgStr);
    		for(int i=bytStartChannel; i < bytCount; i++ )
    		{
    			sprintf( DbgStr, "ch[%d]=%d\n", i-bytStartChannel, wValue[i-bytStartChannel]);
    			MOXA_PRINTF( DbgStr);
    		}
    	}
    	//==========================
    	iRet = E1K_AI_ReadMins(iHandle, bytStartChannel, bytCount, dValue);
    	CheckErr( iRet, (char*)"E1K_AI_ReadMins" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_ReadMins succeed.\n");
    		MOXA_PRINTF( DbgStr);
    		for(int i=bytStartChannel; i < bytCount; i++ )
    		{
    			sprintf( DbgStr, "dValue[%d]=%f\n", i-bytStartChannel, dValue[i-bytStartChannel]);
    			MOXA_PRINTF( DbgStr);
    		}
    	}
    	//==========================
    	iRet = E1K_AI_ReadMinRaws(iHandle, bytStartChannel, bytCount, wValue);
    	CheckErr( iRet, (char*)"E1K_AI_ReadMinRaws" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_ReadMinRaws succeed.\n");
    		MOXA_PRINTF( DbgStr);
    		for(int i=bytStartChannel; i < bytCount; i++ )
    		{
    			sprintf( DbgStr, "ch[%d]=%d\n", i-bytStartChannel, wValue[i-bytStartChannel]);
    			MOXA_PRINTF( DbgStr);
    		}
    	}
    	//==========================
    	iRet = E1K_AI_ResetMins(iHandle, bytStartChannel, bytCount);
    	CheckErr( iRet, (char*)"E1K_AI_ResetMins" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_ResetMins %d channels succeed.\n", bytCount);
    		MOXA_PRINTF( DbgStr);
    	}
    	//==========================
    	iRet = E1K_AI_ReadMaxs(iHandle, bytStartChannel, bytCount, dValue);
    	CheckErr( iRet, (char*)"E1K_AI_ReadMaxs" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_ReadMaxs succeed.\n");
    		MOXA_PRINTF( DbgStr);
    		for(int i=bytStartChannel; i < bytCount; i++ )
    		{
    			sprintf( DbgStr, "dValue[%d]=%f\n", i-bytStartChannel, dValue[i-bytStartChannel]);
    			MOXA_PRINTF( DbgStr);
    		}
    	}
    	//==========================
    	iRet = E1K_AI_ReadMaxRaws(iHandle, bytStartChannel, bytCount, wValue);
    	CheckErr( iRet, (char*)"E1K_AI_ReadMaxRaws" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_ReadMaxRaws succeed.\n");
    		MOXA_PRINTF( DbgStr);
    		for(int i=bytStartChannel; i < bytCount; i++ )
    		{
    			sprintf( DbgStr, "ch[%d]=%d\n", i-bytStartChannel, wValue[i-bytStartChannel]);
    			MOXA_PRINTF( DbgStr);
    		}
    	}
    	//==========================
    	iRet = E1K_AI_ResetMaxs(iHandle, bytStartChannel, bytCount);
    	CheckErr( iRet, (char*)"E1K_AI_ResetMaxs" );
    	if(iRet == MXIO_OK)
    	{
    		sprintf( DbgStr, "E1K_AI_ResetMaxs %d channels succeed.\n", bytCount);
    		MOXA_PRINTF( DbgStr);
    	}
	}
	//=========================================================================
	//Disconnect I/O module
    iRet = MXEIO_Disconnect( iHandle );
    CheckErr( iRet, (char*)"MXEIO_Disconnect" );
	if(iRet == MXIO_OK)
	{
		sprintf( DbgStr, "\nDisconnect module %d connection succeeded.\n", iHandle);
		MOXA_PRINTF( DbgStr);
	}
	sprintf( DbgStr, "***** Press enter to Exit ***** : ");
	MOXA_PRINTF( DbgStr);
    gets(bytInputValue);
}
//  After each MXIO function call, the application checks whether the call succeed.
//  If a MXIO function call fails, return an error code.
//  If the call failed, this procedure prints an error message and exits.
void CheckErr( int iRet, char * szFunctionName )
{
	const char * szErrMsg;

	if( iRet != MXIO_OK )
	{

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
		case EIO_PASSWORD_INCORRECT:
			szErrMsg = "EIO_PASSWORD_INCORRECT";
			break;
		}

		sprintf( DbgStr,  "Function \"%s\" execution Fail. Error Message : %s\n", szFunctionName, szErrMsg );
		MOXA_PRINTF( DbgStr);
	}
}
