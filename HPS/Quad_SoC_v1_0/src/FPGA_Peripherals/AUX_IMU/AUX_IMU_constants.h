//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//
#ifndef AUX_IMU_CONSTANTS_H_
#define AUX_IMU_CONSTANTS_H_

//==============================================================================================//
//					MEMORY STRUCTURE - AUXILIARY CONTROLLER for IMU ROUTINE						//
//==============================================================================================//
//																								//
//	The communication between HPS (master) and Auxiliary Controller (slave) is 2 step 			//
//	communication, which is facilitated by Intermediate memory. 								//
//	The intermediate memory is deployed in FPGA as dual-port RAM, where both HPS and 			//
//	Auxiliary Controller has its own port to access it. Both ports are 32-bits wide.			//
//	The structure of intermediate memory is described by the diagram below, 					//
//	the structure dimension is relative to the bus size.										//
//																								//
//	| WORD |----------------32 bits wide bus---------------|									//
//	|   0  |    AUX_IMU_CONTROL_REG                        |=== CONTROL REGISTER ===			//
//	|   1  |    AUX_IMU_STATUS_REG                         |=== STATUS REGISTER ===				//
//	|   2  |    AUX_IMU_ROUTINE_STATUS_REG                 |=== ROUTINE STATUS REGISTER ===		//
//	|   3  |    AUX_IMU_ROUTINE_CONTROL_REG                |=== ROUTINE CONTROL REGISTER ===	//
//	|   4  |===================NOT USED====================|									//
//	|   5  |===================NOT USED====================|									//
//	|   6  |===================NOT USED====================|									//
//	|   7  |===================NOT USED====================|									//
//	|   8  |    AUX_IMU_DATA_HPS2AUX_0			           |=== DATA HPS TO AUX ===				//
//	|   9  |    AUX_IMU_DATA_HPS2AUX_1			           |===         |        ===			//
//	|  10  |    AUX_IMU_DATA_HPS2AUX_2			           |===         |        ===			//
//	|  11  |    AUX_IMU_DATA_HPS2AUX_3			           |===         |        ===			//
//	|  12  |    AUX_IMU_DATA_HPS2AUX_4			           |===         |        ===			//
//	|  13  |    AUX_IMU_DATA_HPS2AUX_5			           |===         |        ===			//
//	|  14  |    AUX_IMU_DATA_HPS2AUX_6			           |===         |        ===			//
//	|  15  |    AUX_IMU_DATA_HPS2AUX_7			           |===         |        ===			//
//	|  16  |    AUX_IMU_DATA_HPS2AUX_8			           |===         |        ===			//
//	|  17  |    AUX_IMU_DATA_HPS2AUX_9			           |===         |        ===			//
//	|  18  |    AUX_IMU_DATA_HPS2AUX_10	                   |===         |        ===			//
//	|  19  |    AUX_IMU_DATA_HPS2AUX_11                    |===         |        ===			//
//	|  20  |    AUX_IMU_DATA_HPS2AUX_12                    |===         |        ===			//
//	|  21  |    AUX_IMU_DATA_HPS2AUX_13                    |===         |        ===			//
//	|  22  |    AUX_IMU_DATA_HPS2AUX_14                    |===         |        ===			//
//	|  23  |    AUX_IMU_DATA_HPS2AUX_15                    |=== DATA HPS TO AUX ===			//
//	|  24  |===================NOT USED====================|									//
//	|  25  |===================NOT USED====================|									//
//	|  26  |===================NOT USED====================|									//
//	|  27  |===================NOT USED====================|									//
//	|  28  |===================NOT USED====================|									//
//	|  29  |===================NOT USED====================|									//
//	|  30  |===================NOT USED====================|									//
//	|  31  |===================NOT USED====================|									//
//	|  32  |    AUX_IMU_DATA_AUX2HPS_0			           |=== DATA AUX TO HPS ===			//
//	|  33  |    AUX_IMU_DATA_AUX2HPS_1			           |===         |        ===			//
//	|  34  |    AUX_IMU_DATA_AUX2HPS_2			           |===         |        ===			//
//	|  35  |    AUX_IMU_DATA_AUX2HPS_3			           |===         |        ===			//
//	|  36  |    AUX_IMU_DATA_AUX2HPS_4			           |===         |        ===			//
//	|  37  |    AUX_IMU_DATA_AUX2HPS_5			           |===         |        ===			//
//	|  38  |    AUX_IMU_DATA_AUX2HPS_6			           |===         |        ===			//
//	|  39  |    AUX_IMU_DATA_AUX2HPS_7			           |===         |        ===			//
//	|  40  |    AUX_IMU_DATA_AUX2HPS_8			           |===         |        ===			//
//	|  41  |    AUX_IMU_DATA_AUX2HPS_9			           |===         |        ===			//
//	|  42  |    AUX_IMU_DATA_AUX2HPS_10                    |===         |        ===			//
//	|  43  |    AUX_IMU_DATA_AUX2HPS_11                    |===         |        ===			//
//	|  44  |    AUX_IMU_DATA_AUX2HPS_12                    |===         |        ===			//
//	|  45  |    AUX_IMU_DATA_AUX2HPS_13                    |===         |        ===			//
//	|  46  |    AUX_IMU_DATA_AUX2HPS_14                    |===         |        ===			//
//	|  47  |    AUX_IMU_DATA_AUX2HPS_15                    |=== DATA AUX TO HPS ===			//
//	|  48  |===================NOT USED====================|									//
//	|  49  |===================NOT USED====================|									//
//	|  50  |===================NOT USED====================|									//
//	|  51  |===================NOT USED====================|									//
//	|  52  |===================NOT USED====================|									//
//	|  53  |===================NOT USED====================|									//
//	|  54  |===================NOT USED====================|									//
//	|  55  |===================NOT USED====================|									//
//	|  56  |    AUX_IMU_DATA_PACKET_0_0			           |=== IMU DATA PACKET  0 ===			//
//	|  57  |    AUX_IMU_DATA_PACKET_0_1			           |===          |         ===			//
//	|  58  |    AUX_IMU_DATA_PACKET_0_2			           |===          |         ===			//
//	|  59  |    AUX_IMU_DATA_PACKET_0_3			           |===          |         ===			//
//	|  60  |    AUX_IMU_DATA_PACKET_0_4			           |===          |         ===			//
//	|  61  |    AUX_IMU_DATA_PACKET_0_5			           |===          |         ===			//
//	|  62  |    AUX_IMU_DATA_PACKET_0_6			           |===          |         ===			//
//	|  63  |    AUX_IMU_DATA_PACKET_0_7			           |===          |         ===			//
//	|  64  |    AUX_IMU_DATA_PACKET_0_8			           |===          |         ===			//
//	|  65  |    AUX_IMU_DATA_PACKET_0_9			           |===          |         ===			//
//	|  66  |    AUX_IMU_DATA_PACKET_0_10			       |===          |         ===			//
//	|  67  |    AUX_IMU_DATA_PACKET_0_11			       |===          |         ===			//
//	|  68  |    AUX_IMU_DATA_PACKET_0_12			       |===          |         ===			//
//	|  69  |    AUX_IMU_DATA_PACKET_0_13			       |===          |         ===			//
//	|  70  |    AUX_IMU_DATA_PACKET_0_14			       |===          |         ===			//
//	|  71  |    AUX_IMU_DATA_PACKET_0_15			       |=== IMU DATA PACKET  0 ===			//
//	|  72  |===================NOT USED====================|									//
//	|  73  |===================NOT USED====================|									//
//	|  74  |===================NOT USED====================|									//
//	|  75  |===================NOT USED====================|									//
//	|  76  |===================NOT USED====================|									//
//	|  77  |===================NOT USED====================|									//
//	|  78  |===================NOT USED====================|									//
//	|  79  |===================NOT USED====================|									//
//	|  80  |    AUX_IMU_DATA_PACKET_1_0			           |=== IMU DATA PACKET  1 ===			//
//	|  81  |    AUX_IMU_DATA_PACKET_1_1			           |===          |         ===			//
//	|  82  |    AUX_IMU_DATA_PACKET_1_2			           |===          |         ===			//
//	|  83  |    AUX_IMU_DATA_PACKET_1_3			           |===          |         ===			//
//	|  84  |    AUX_IMU_DATA_PACKET_1_4			           |===          |         ===			//
//	|  85  |    AUX_IMU_DATA_PACKET_1_5			           |===          |         ===			//
//	|  86  |    AUX_IMU_DATA_PACKET_1_6			           |===          |         ===			//
//	|  87  |    AUX_IMU_DATA_PACKET_1_7			           |===          |         ===			//
//	|  88  |    AUX_IMU_DATA_PACKET_1_8			           |===          |         ===			//
//	|  89  |    AUX_IMU_DATA_PACKET_1_9			           |===          |         ===			//
//	|  90  |    AUX_IMU_DATA_PACKET_1_10			       |===          |         ===			//
//	|  91  |    AUX_IMU_DATA_PACKET_1_11			       |===          |         ===			//
//	|  92  |    AUX_IMU_DATA_PACKET_1_12			       |===          |         ===			//
//	|  93  |    AUX_IMU_DATA_PACKET_1_13			       |===          |         ===			//
//	|  94  |    AUX_IMU_DATA_PACKET_1_14			       |===          |         ===			//
//	|  95  |    AUX_IMU_DATA_PACKET_1_15			       |=== IMU DATA PACKET  1 ===			//
//	|  96  |===================NOT USED====================|									//
//	|  97  |===================NOT USED====================|									//
//	|  98  |===================NOT USED====================|									//
//	|  99  |===================NOT USED====================|									//
//	| 100  |===================NOT USED====================|									//
//	| 101  |===================NOT USED====================|									//
//	| 102  |===================NOT USED====================|									//
//	| 103  |===================NOT USED====================|									//
//	| 104  |    AUX_IMU_DATA_PACKET_2_0			           |=== IMU DATA PACKET  2 ===			//
//	| 105  |    AUX_IMU_DATA_PACKET_2_1			           |===          |         ===			//
//	| 106  |    AUX_IMU_DATA_PACKET_2_2			           |===          |         ===			//
//	| 107  |    AUX_IMU_DATA_PACKET_2_3			           |===          |         ===			//
//	| 108  |    AUX_IMU_DATA_PACKET_2_4			           |===          |         ===			//
//	| 109  |    AUX_IMU_DATA_PACKET_2_5			           |===          |         ===			//
//	| 110  |    AUX_IMU_DATA_PACKET_2_6			           |===          |         ===			//
//	| 111  |    AUX_IMU_DATA_PACKET_2_7			           |===          |         ===			//
//	| 112  |    AUX_IMU_DATA_PACKET_2_8			           |===          |         ===			//
//	| 113  |    AUX_IMU_DATA_PACKET_2_9			           |===          |         ===			//
//	| 114  |    AUX_IMU_DATA_PACKET_2_10			       |===          |         ===			//
//	| 115  |    AUX_IMU_DATA_PACKET_2_11			       |===          |         ===			//
//	| 116  |    AUX_IMU_DATA_PACKET_2_12			       |===          |         ===			//
//	| 117  |    AUX_IMU_DATA_PACKET_2_13			       |===          |         ===			//
//	| 118  |    AUX_IMU_DATA_PACKET_2_14			       |===          |         ===			//
//	| 119  |    AUX_IMU_DATA_PACKET_2_15			       |=== IMU DATA PACKET  2 ===			//
//	| 120  |===================NOT USED====================|									//
//	| 121  |===================NOT USED====================|									//
//	| 122  |===================NOT USED====================|									//
//	| 123  |===================NOT USED====================|									//
//	| 124  |===================NOT USED====================|									//
//	| 125  |===================NOT USED====================|									//
//	| 126  |===================NOT USED====================|									//
//	| 127  |===================NOT USED====================|									//
//	| 128  |    AUX_IMU_DATA_PACKET_3_0			           |=== IMU DATA PACKET  3 ===			//
//	| 129  |    AUX_IMU_DATA_PACKET_3_1			           |===          |         ===			//
//	| 130  |    AUX_IMU_DATA_PACKET_3_2			           |===          |         ===			//
//	| 131  |    AUX_IMU_DATA_PACKET_3_3			           |===          |         ===			//
//	| 132  |    AUX_IMU_DATA_PACKET_3_4			           |===          |         ===			//
//	| 133  |    AUX_IMU_DATA_PACKET_3_5			           |===          |         ===			//
//	| 134  |    AUX_IMU_DATA_PACKET_3_6			           |===          |         ===			//
//	| 135  |    AUX_IMU_DATA_PACKET_3_7			           |===          |         ===			//
//	| 136  |    AUX_IMU_DATA_PACKET_3_8			           |===          |         ===			//
//	| 137  |    AUX_IMU_DATA_PACKET_3_9			           |===          |         ===			//
//	| 138  |    AUX_IMU_DATA_PACKET_3_10			       |===          |         ===			//
//	| 139  |    AUX_IMU_DATA_PACKET_3_11			       |===          |         ===			//
//	| 140  |    AUX_IMU_DATA_PACKET_3_12			       |===          |         ===			//
//	| 141  |    AUX_IMU_DATA_PACKET_3_13			       |===          |         ===			//
//	| 142  |    AUX_IMU_DATA_PACKET_3_14			       |===          |         ===			//
//	| 143  |    AUX_IMU_DATA_PACKET_3_15			       |=== IMU DATA PACKET  3 ===			//
//	| 144  |===================NOT USED====================|									//
//	| 145  |===================NOT USED====================|									//
//	| 146  |===================NOT USED====================|									//
//	| 147  |===================NOT USED====================|									//
//	| 148  |===================NOT USED====================|									//
//	| 149  |===================NOT USED====================|									//
//	| 150  |===================NOT USED====================|									//
//	| 151  |===================NOT USED====================|									//
//==============================================================================================//



//==============================================================================================//
//										PARAMETERS												//
//==============================================================================================//
#define AUX_IMU_NUMBER_OF_ROUTINES				(4)


//==============================================================================================//
//										CONTROL REGISTER										//
//==============================================================================================//
//																								//
//	The Control Register is 32-bit long register to control the Auxiliary Controller.			//
//																								//
//==============================================================================================//
// register offset
#define AUX_IMU_CONTROL_REG_DWORD_OFST			(0)
#define AUX_IMU_CONTROL_REG_BYTE_OFST			(4*AUX_IMU_CONTROL_REG_DWORD_OFST)
// bits
// default hand-shake routine bits are below this  line according to HPS-AUX SYNCHRONIZATION figure
#define AUX_IMU_CONTROL_RESET_N					(0x00000001)	// reset bit for Auxiliary Controller
#define AUX_IMU_CONTROL_ENABLE					(0x00000002)	// enable execution bit


//==============================================================================================//
//										STATUS REGISTER											//
//==============================================================================================//
//																								//
//	The Status Register is 32-bit long register to provide the status of Auxiliary Controller.	//
//																								//
//==============================================================================================//
// register offset
#define AUX_IMU_STATUS_REG_DWORD_OFST			(1)
#define AUX_IMU_STATUS_REG_BYTE_OFST			(4*AUX_IMU_STATUS_REG_DWORD_OFST)
// bits
// default hand-shake routine bits are below this  line according to HPS-AUX SYNCHRONIZATION figure
#define AUX_IMU_STATUS_FLAG_RESET				(0x00000001)		// Auxiliary Controller is in reset state
#define AUX_IMU_STATUS_FLAG_ENABLE				(0x00000002)		// Auxiliary Controller is enabled for execution
#define AUX_IMU_STATUS_FLAG_INIT				(0x00000004)		// Auxiliary Controller finished initialisation

//==============================================================================================//
//										ROUTINE CONTROL REGISTER								//
//==============================================================================================//
//																								//
//	The Routine Control Register is 32-bit long register to control the Routine.				//
//																								//
//==============================================================================================//
#define AUX_IMU_ROUTINE_CONTROL_REG_DWORD_OFST	(2)
#define AUX_IMU_ROUTINE_CONTROL_REG_BYTE_OFST	(4*AUX_IMU_ROUTINE_CONTROL_REG_DWORD_OFST)
// bits
#define AUX_IMU_ROUTINE_0_ENABLE				(0x00000001)	// enable imu routine execution bit
#define AUX_IMU_ROUTINE_1_ENABLE				(0x00000002)	// enable imu routine execution bit
#define AUX_IMU_ROUTINE_2_ENABLE				(0x00000004)	// enable imu routine execution bit
#define AUX_IMU_ROUTINE_3_ENABLE				(0x00000008)	// enable imu routine execution bit

//==============================================================================================//
//										ROUTINE STATUS REGISTER									//
//==============================================================================================//
//																								//
//	The Routine Status Register is 32-bit long register to provide the status of Routine.		//
//																								//
//==============================================================================================//
#define AUX_IMU_ROUTINE_STATUS_REG_DWORD_OFST	(3)
#define AUX_IMU_ROUTINE_STATUS_REG_BYTE_OFST	(4*AUX_IMU_ROUTINE_STATUS_REG_DWORD_OFST)
// bits
#define AUX_IMU_ROUTINE_0_FLAG_ENABLE			(0x00000001)	// imu routine 0 execution enabled
#define AUX_IMU_ROUTINE_1_FLAG_ENABLE			(0x00000002)	// imu routine 1 execution enabled
#define AUX_IMU_ROUTINE_2_FLAG_ENABLE			(0x00000004)	// imu routine 2 execution enabled
#define AUX_IMU_ROUTINE_3_FLAG_ENABLE			(0x00000008)	// imu routine 3 execution enabled
#define AUX_IMU_ROUTINE_0_FLAG_PKT_RDY			(0x00000010)	// imu packet 0 received
#define AUX_IMU_ROUTINE_1_FLAG_PKT_RDY			(0x00000020)	// imu packet 1 received
#define AUX_IMU_ROUTINE_2_FLAG_PKT_RDY			(0x00000040)	// imu packet 2 received
#define AUX_IMU_ROUTINE_3_FLAG_PKT_RDY			(0x00000080)	// imu packet 3 received
#define AUX_IMU_ROUTINE_0_FLAG_VALID			(0x00000100)	// imu packet 0 received
#define AUX_IMU_ROUTINE_1_FLAG_VALID			(0x00000200)	// imu packet 1 received
#define AUX_IMU_ROUTINE_2_FLAG_VALID			(0x00000400)	// imu packet 2 received
#define AUX_IMU_ROUTINE_3_FLAG_VALID			(0x00000800)	// imu packet 3 received

//==============================================================================================//
//										OTHER VALUES											//
//==============================================================================================//
#define AUX_IMU_ALL_BITS						(0xFFFFFFFF)		//	refers to all bits within 32 bit register

//==============================================================================================//
//										HPS-AUX SYNCHRONIZATION									//
//==============================================================================================//
//																								//
//	The ASCII-figure shows the initial synchronization between HPS and AUXILIARY CONTROLLER.	//
//																								//
//	HPS accesses Intermediate memory															//
//	The left side of the figure shows HPS accessing Intermediate memory. Read operations 		//
//	(arrow* pointing to the left) and write operations (arrow* point to the right) 				//
//	of the Control and Status registers. 														//
//																								//
//	AUX accesses Intermediate memory															//
//	The right side of the figure shows AUXILIARY CONTROLLER accessing Intermediate memory. 		//
//	Read operations (arrow* pointing to the left) and write operations 							//
//	(arrow* point to the right) of the Control and Status registers.							//
//																								//
//	REGISTERS																					//
//	The middle column shows the values of bits inside the Control and Status registers.			//
//	The HPS (MASTER) writes to CONTROL_REG and reads the STATUS_REG, 							//
//	while the AUXILIARY CONTROLLER (SLAVE) reads the CONTROL_REG and writes to STATUS_REG.		//
//																								//
//	* 	the ARROWS shows the access to Intermediate memory by HPS and AUXILIARY CONTROLLER		//
//		and the necessary delay, which is required to pass the value through the 				//
//		Intermediate memory.																	//
//																								//
//==============================================================================================//
//																								//
//				HPS (MASTER)											AUX (SLAVE)				//
//		(writes to CONTROL REGISTER)							(reads from CONTROL REGISTER)	//
//		(reads from STATUS REGISTER)							(writes to STATUS REGISTER)		//
//------------------|-------------------------------------------------------|-------------------//
//		EXECUTION 	|						| REGISTERS |					|	EXECUTION		//
//		BEGINNING	|														|	BEGINNING		//
//			|		|														|		|			//
//=1.0= INIT STATE=	|					  |RESET_N|ENABLE |					|=== INIT STATE===	//
//			|		|		| CONTROL_REG |   0   |   0   |					|	state after		//
//			|		|	        	      |RESETED|ENABLED|INIT_DONE| 		|	programming		//
//			|		|		| STATUS_REG  |   0   |   0   |    0    |  		|	the FPGA		//
//			|		|														|		|			//
//=1.!=	FORCE INIT	|														|		|			//
//	SEE NOTES BELOW	|														|		|			//
//			|		|														|		|			//
//			|		|					  |RESET_N|ENABLE |					|		|			//
//			|		|		| CONTROL_REG |   0   |   0   |------------\	|		|			//
//			|		|													\-->|=== RESET ===		//
//			|		|														|	reset			//
//			|		|														|	all variables	//
//			|		|														|		|			//
//			|		|	        	      |RESETED|ENABLED|INIT_DONE|		|		|			//
//			|		|	 /--| STATUS_REG  |   1   |   0   |    0    |<------|-------|			//
//=1.1= AUX IS OFF =|<--/													|		|			//
//	AUX can be 		|													  	|		|			//
//	bi-passed,	 	|													  	|		|			//
//	HPS can access	|													  	|		|			//
//	FPGA peripherals|													  	|		|			//
//	directly		|													  	|		|			//
//			|		|														|		|			//
//=1.2= PASS DATA =	|														|		|			//
//	HPS can set 	|														|		|			//
//	AUX_RADIO_... 	|														|		|			//
//	DATA_0_TO_SLAVE	|														|		|			//
//	and				|														|		|			//
//	AUX_RADIO_... 	|														|		|			//
//	DATA_1_TO_SLAVE	|														|		|			//
//			|		|														|		|			//
//=1.*= AUX IS ON =	|														|		|			//
//	SEE NOTES BELOW	|														|		|			//
//			|		|					  |RESET_N|ENABLE |					|		|			//
//			|-------|------>| CONTROL_REG |   1   |   0   |------------\	|		|			//
//			|		|													\-->|=== STALLING ===	//
//			|		|														|	no execution,	//
//			|		|														|	variables keep	//
//			|		|														|	their previous	//
//			|		|														|	values			//
//			|		|														|		|			//
//=1.3= ENABLE AUX =|														|		|			//
//	enable AUX		|														|		|			//
//			|		|					  |RESET_N|ENABLE |					|		|			//
//			|-------|------>| CONTROL_REG |   1   |   1   |------------\	|		|			//
//			|		|													\-->|=== EXEC ENABLED ==//
//			|		|														|		|			//
//			|		|	        	      |RESETED|ENABLED|INIT_DONE|		|		|			//
//			|		|	 /--| STATUS_REG  |   0   |   1   |    0    |<------|-------|			//
//=1.4= AUX IN INIT=|<--/													|		|			//
//			|		|														|	initialization	//
//			|		|														|		|			//
//			|		|														|	AUX can set 	//
//			|		|														|	AUX_RADIO_... 	//
//			|		|														|	DATA_0_TO_MASTER//
//			|		|														|	and				//
//			|		|														|	AUX_RADIO_... 	//
//			|		|														|	DATA_0_TO_MASTER//
//			|		|														|		|			//
//			|		|	        	      |RESETED|ENABLED|INIT_DONE|		|		|			//
//			|		|	 /--| STATUS_REG  |   0   |   1   |    1    |<------|-------|			//
//=1.5= AUX IN EXE=	|<--/													|		|			//
//	other routines	|														|		|			//
//	can be started	|														|		|			//
//	using additional|														|		|			//
//	ROUTINE_ENABLE	|														|		|			//
//	bits in 		|														|		|			//
//	CONTROL_REG		|														|		|			//
//			|		|														|		|			//
//			|		|														|	initialization	//
//			|		|														|	DONE,			//
//			|		|														|	AUX waits to 	//
//			|		|														|	start a routine	//
//			|		|														|		|			//
//	AUX can be		|													  	|		|			//
//	still bi-passed,|													  	|		|			//
//	HPS can access	|													  	|		|			//
//	FPGA peripherals|													  	|		|			//
//	directly, 		|													  	|		|			//
//	IF NO ROUTINE	|													  	|		|			//
//	IS RUNNING		|													  	|		|			//
//			|		|													  	|		|			//
//=1.6= ROUTINE=	|													  	|		|			//
//			|		|													  	|		|			//
//			|		|					  |RESET_N|ENABLE |ROUTINE_EN..|	|		|			//
//			|-------|------>| CONTROL_REG |   1   |   1   |      1     |\	|		|			//
//			|		|													 \->|=== ENABLE ROUT... //
//			|		|														|		|			//
//	HPS CANNOT 		|													  	|		|			//
//	ACCESS 			|													  	|		|			//
//	PERIPHERALS		|													  	|		|			//
//	CONTROLLED BY	|													  	|		|			//
//	AUXs ROUTINE	|													  	|		|			//
//			|		|													  	|		|			//
//			...		|														|		...			//
//==============================================================================================//
//																								//
//	1.!	The HPS can simulate the initial state of the FPGA by reseting both CONTROL and STATUS	//
//		registers of the AUX. This is special procedure for debugging.							//
//																								//
//	1.*	Stalling is a state, when the AUX is not in reset, but its main routine is not 			//
//		enabled either.																			//
//																								//
//==============================================================================================//


//==============================================================================================//
//										HPS 2 AUX DATA REGISTERS								//
//==============================================================================================//
//																								//
//	These registers serve for more complex communication between HPS and Auxiliary Controller.	//
//																								//
//==============================================================================================//
// DWORD offset
#define AUX_IMU_DATA_HPS2AUX_0_DWORD_OFST		(8)
// BYTE offset
#define AUX_IMU_DATA_HPS2AUX_0_BYTE_OFST		(4*AUX_IMU_DATA_HPS2AUX_0_DWORD_OFST)
// number of registers
#define AUX_IMU_DATA_HPS2AUX_LENGTH_DWORD		(16)
#define AUX_IMU_DATA_HPS2AUX_LENGTH_BYTE		(4*AUX_IMU_DATA_HPS2AUX_LENGTH_DWORD)

//==============================================================================================//
//										AUX 2 HPS DATA REGISTERS								//
//==============================================================================================//
//																								//
//	These registers serve for more complex communication between HPS and Auxiliary Controller.	//
//																								//
//==============================================================================================//
// DWORD offset
#define AUX_IMU_DATA_AUX2HPS_0_DWORD_OFST		(32)
// BYTE offset
#define AUX_IMU_DATA_AUX2HPS_0_BYTE_OFST		(4*AUX_IMU_DATA_AUX2HPS_0_DWORD_OFST)
// number of registers
#define AUX_IMU_DATA_AUX2HPS_LENGTH_DWORD		(16)
#define AUX_IMU_DATA_AUX2HPS_LENGTH_BYTE		(4*AUX_IMU_DATA_AUX2HPS_LENGTH_DWORD)

//==============================================================================================//
//											IMU PACKET DATA REGISTERS							//
//==============================================================================================//
//																								//
//	The following constants mark the offsets of packets within intermediate memory.				//
//	Below is the IMU6050 packet structure from original documentation.							//
//																								//
//==============================================================================================//
// DWORD offset
#define AUX_IMU_DATA_PACKET_0_DWORD_OFST 		(56)
#define AUX_IMU_DATA_PACKET_1_DWORD_OFST 		(80)
#define AUX_IMU_DATA_PACKET_2_DWORD_OFST 		(104)
#define AUX_IMU_DATA_PACKET_3_DWORD_OFST 		(128)
// BYTE offset
#define AUX_IMU_DATA_PACKET_0_BYTE_OFST 		(4*AUX_IMU_DATA_PACKET_0_DWORD_OFST)
#define AUX_IMU_DATA_PACKET_1_BYTE_OFST 		(4*AUX_IMU_DATA_PACKET_1_DWORD_OFST)
#define AUX_IMU_DATA_PACKET_2_BYTE_OFST 		(4*AUX_IMU_DATA_PACKET_2_DWORD_OFST)
#define AUX_IMU_DATA_PACKET_3_BYTE_OFST 		(4*AUX_IMU_DATA_PACKET_3_DWORD_OFST)
// number of registers
#define AUX_IMU_DATA_PACKET_LENGTH_DWORD		(16)
#define AUX_IMU_DATA_PACKET_LENGTH_BYTE			(4*AUX_IMU_DATA_PACKET_LENGTH_DWORD)

#endif /* AUX_IMU_CONSTANTS_H_ */
