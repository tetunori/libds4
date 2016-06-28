
#ifndef _DS4_INPUT_H_
#define _DS4_INPUT_H_

// ***************************************************************************
// Includes
// ***************************************************************************
#include <stdbool.h>

// ***************************************************************************
// Struct
// ***************************************************************************
struct _stDS4InputEvt{
    bool btnCross;
    bool btnCircle;
    bool btnTriangle;
    bool btnSquare;
    bool btnL1;
    bool btnL2;
    bool btnL3;
    bool btnR1;
    bool btnR2;
    bool btnR3;
    bool btnTouchpad;
    bool btnOptions;
    bool btnPlaystation;
    bool btnShare;
    bool btnUp;
    bool btnDown;
    bool btnLeft;
    bool btnRight;
    unsigned char L2analog;
    unsigned char R2analog;
    unsigned char L3analogV;
    unsigned char L3analogH;
    unsigned char R3analogV;
    unsigned char R3analogH;
    unsigned char batteryLevel;
    unsigned char evtType;
    /* TODO : other sensor data */
};

struct _stDS4InputInitialize{
    char* p_hid_raw_node_path;
    char debug_flag;
};

// ***************************************************************************
// Functions
// ***************************************************************************
int ds4_input_initialize( struct _stDS4InputInitialize* pstInitialize );
int ds4_input_start_listening_event( int ( *callback )( struct _stDS4InputEvt* pstInputEvt ) );
int ds4_input_stop_listening_event( void );

#endif // _DS4_INPUT_H_
