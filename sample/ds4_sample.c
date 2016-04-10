
// ***************************************************************************
// Includes
// ***************************************************************************
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <ds4_output.h>

// ***************************************************************************
// Definitions
// ***************************************************************************
#define PRT_PRIFIX    "[DS4 Sample] "

#define DS4_DEBUG_ON ( 0 )
#define DS4_PRT( format, ... )   printf("\033[37m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )
#define DS4_DPRT( format, ... )  if( DS4_DEBUG_ON ) printf("\033[37m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )
#define DS4_EPRT( format, ... )  printf("\033[31m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )

// ***************************************************************************
// Struct
// ***************************************************************************

// ***************************************************************************
// Declarations
// ***************************************************************************
int hid_output_to_ds4_test( void );

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

// ***************************************************************************
// Main Looooooooooooop
// ***************************************************************************
int main( int argc __attribute__((__unused__)), char **argv __attribute__((__unused__)) ){

    int err, ret = 0;
    struct _stDS4OutputInitialize stDS4OutputInitialize;
    char hid_raw_path[] = "/dev/hidraw0";
    stDS4OutputInitialize.p_hid_raw_node_path = hid_raw_path;
    stDS4OutputInitialize.debug_flag = 0;

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

SUB_RET:
    return ret;

}
