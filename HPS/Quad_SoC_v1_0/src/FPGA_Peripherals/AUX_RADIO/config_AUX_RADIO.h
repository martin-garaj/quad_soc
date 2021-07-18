
#ifndef CONFIG_AUX_RADIO_H_
#define CONFIG_AUX_RADIO_H_

//==============================================================================================//
//											USER INPUT											//
//==============================================================================================//
	// define, whether the addresses of registers are in byte/word format
	//#define BYTE_FORMAT		// used by NIOS II/bare-metal
	#define DWORD_FORMAT	// used by HPS/Linux
	
	// define whether the code runs on AUX or HPS
	//#define AUX_PLATFORM
	#define HPS_PLATFORM

#endif /* CONFIG_AUX_RADIO_H_ */
