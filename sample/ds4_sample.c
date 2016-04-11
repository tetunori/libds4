
// ***************************************************************************
// Includes
// ***************************************************************************
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <ds4_output.h>
#include <ds4_input.h>

// ***************************************************************************
// Definitions
// ***************************************************************************
#define PRT_PRIFIX    "[DS4 Sample] "

#define DS4_DEBUG_ON ( 1 )
#define DS4_PRT( format, ... )   printf("\033[37m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )
#define DS4_DPRT( format, ... )  if( DS4_DEBUG_ON ) printf("\033[37m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )
#define DS4_EPRT( format, ... )  printf("\033[31m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )

// ***************************************************************************
// Struct
// ***************************************************************************

// ***************************************************************************
// Declarations
// ***************************************************************************
static int hid_output_to_ds4_test( void );
static int InputEventCallback( struct _stDS4InputEvt* pstInputEvt );
static void dumpInputEvent( struct _stDS4InputEvt* pstInputEvt );
static void showUsage( char* pStrAppName );

// ***************************************************************************
// Variables
// ***************************************************************************

// ***************************************************************************
// Functions
// ***************************************************************************

// Test Sequence.
int hid_output_to_ds4_test( void ){
    struct _stDS4OutputEmitData stEmitData;
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );

    // Light bar color Red.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.r = 0xff;
    ds4_output_emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color Green.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.g = 0xff;
    ds4_output_emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color Blue.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.b = 0xff;
    ds4_output_emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color Yellow.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.r = 0xff;
    stEmitData.g = 0xff;
    ds4_output_emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color Magenta.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.r = 0xff;
    stEmitData.b = 0xff;
    ds4_output_emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color Cyan.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.g = 0xff;
    stEmitData.b = 0xff;
    ds4_output_emit_data( &stEmitData );
    usleep( 200000 );

    // Light bar color White.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.r = 0xff;
    stEmitData.g = 0xff;
    stEmitData.b = 0xff;
    ds4_output_emit_data( &stEmitData );
    usleep( 500000 );

    // Flash Light bar
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.g = 0xff;
    stEmitData.flashOn = 0x30;
    stEmitData.flashOff = 0x30;
    ds4_output_emit_data( &stEmitData );
    usleep( 2000000 );

    // All Off.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    ds4_output_emit_data( &stEmitData );

    // rumbleRight
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.rumbleRight = 0x40;
    stEmitData.r = 0x30;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleRight = 0x80;
    stEmitData.r = 0x40;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleRight = 0xC0;
    stEmitData.r = 0x50;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.r = 0x60;
    stEmitData.rumbleRight = 0xff;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );

    // rumbleLeft
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.rumbleLeft = 0x40;
    stEmitData.r = 0x70;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0x80;
    stEmitData.r = 0x80;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0xC0;
    stEmitData.r = 0x90;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0xff;
    stEmitData.r = 0xA0;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );

    // rumbleLeft & rumbleRight
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    stEmitData.rumbleLeft = 0x40;
    stEmitData.rumbleRight = 0x40;
    stEmitData.r = 0xC0;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0x80;
    stEmitData.rumbleRight = 0x80;
    stEmitData.r = 0xD0;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0xC0;
    stEmitData.rumbleRight = 0xC0;
    stEmitData.r = 0xE0;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );
    stEmitData.rumbleLeft = 0xff;
    stEmitData.rumbleRight = 0xff;
    stEmitData.r = 0xFF;
    ds4_output_emit_data( &stEmitData );
    usleep( 1000000 );

    // All Off.
    memset( &stEmitData, 0x00, sizeof( stEmitData ) );
    ds4_output_emit_data( &stEmitData );

    return 0;
}

int InputEventCallback( struct _stDS4InputEvt* pstInputEvt ){
    dumpInputEvent( pstInputEvt );
    return 0;
}

void dumpInputEvent( struct _stDS4InputEvt* pstInputEvt ){
    // Dump Input Event Data.
    DS4_DPRT( "\033[1;1H -- Current Controller Status." );
    DS4_DPRT( "Square:%s, Circle:%s, Triangle:%s, Cross:%s",
        pstInputEvt->btnSquare ? "X" : " ", pstInputEvt->btnCircle ? "X" : " ",
        pstInputEvt->btnTriangle ? "X" : " ", pstInputEvt->btnCross ? "X" : " " );
    DS4_DPRT( "L1:%s, L2:%s, L3:%s, R1:%s, R2:%s, R3:%s",
        pstInputEvt->btnL1 ? "X" : " ", pstInputEvt->btnL2 ? "X" : " ",
        pstInputEvt->btnL3 ? "X" : " ", pstInputEvt->btnR1 ? "X" : " ",
        pstInputEvt->btnR2 ? "X" : " ", pstInputEvt->btnR3 ? "X" : " " );
    DS4_DPRT( "Share:%s, PS:%s, TouchPad:%s, Options:%s",
        pstInputEvt->btnShare ? "X" : " ", pstInputEvt->btnPlaystation ? "X" : " ",
        pstInputEvt->btnTouchpad ? "X" : " ", pstInputEvt->btnOptions ? "X" : " " );
    DS4_DPRT( "Up:%s, Down:%s, Left:%s, Right:%s",
        pstInputEvt->btnUp ? "X" : " ", pstInputEvt->btnDown ? "X" : " ",
        pstInputEvt->btnLeft ? "X" : " ", pstInputEvt->btnRight ? "X" : " " );
    DS4_DPRT( "L2analog:%03d, R2analog:%03d",
        pstInputEvt->L2analog, pstInputEvt->R2analog );
    DS4_DPRT( "L3analogH:%03d, L3analogV:%03d, R3analogH:%03d, R3analogV:%03d",
        pstInputEvt->L3analogH, pstInputEvt->L3analogV,
        pstInputEvt->R3analogH, pstInputEvt->R3analogV );
    DS4_DPRT( "Battery Level:%03d percent.", pstInputEvt->batteryLevel );

}
// ***************************************************************************
// Main Procedure
// ***************************************************************************
int main( int argc, char **argv ){

    int err, ret = 0;
    char hid_raw_node_path[] = "/dev/hidraw0";

    // Initialize for Output
    struct _stDS4OutputInitialize stDS4OutputInitialize;
    stDS4OutputInitialize.p_hid_raw_node_path = hid_raw_node_path;
    stDS4OutputInitialize.debug_flag = 0;

    // Initialize for Input
    struct _stDS4InputInitialize stDS4InputInitialize;
    stDS4InputInitialize.p_hid_raw_node_path = hid_raw_node_path;
    stDS4InputInitialize.debug_flag = 0;

    if( argc == 2 ){
        if( argv[1][0] == 'i' ){
            // Input Sample
            err = ds4_input_initialize( &stDS4InputInitialize );
            if( err ){
                DS4_EPRT( "ds4_input_initialize() error: %d", err );
                ret = -1;
                goto SUB_RET;
            }

            err = ds4_input_start_listening_event( InputEventCallback );
            if( err ){
                DS4_EPRT( "ds4_input_start_listening_event() error: %d", err );
                ret = -1;
                goto SUB_RET;
            }

            sleep(10);

            err = ds4_input_stop_listening_event();
            if( err ){
                DS4_EPRT( "ds4_input_stop_listening_event() error: %d", err );
                ret = -1;
                goto SUB_RET;
            }

        }else if( argv[1][0] == 'o' ){
            // Output Sample
            err = ds4_output_initialize( &stDS4OutputInitialize );
            if( err ){
                DS4_EPRT( "ds4_output_initialize() error: %d", err );
                ret = -1;
                goto SUB_RET;
            }

            err = hid_output_to_ds4_test();
            if( err ){
                DS4_EPRT( "hid_output_to_ds4_test() error: %d", err );
                ret = -1;
                goto SUB_RET;
            }
        }else{
            showUsage( argv[0] );
        }
    }else{
        showUsage( argv[0] );
    }

SUB_RET:
    return ret;

}

static void showUsage( char* pStrAppName ){
    // Usage.
    DS4_PRT( "Usage for Samples : " );
    DS4_PRT( " Output Sample > %s o", pStrAppName );
    DS4_PRT( " Input Sample  > %s i", pStrAppName );
}
