
// ***************************************************************************
// Includes
// ***************************************************************************
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <zlib.h>

#include <sys/ioctl.h>

#include <ds4_output.h>

// ***************************************************************************
// Definitions
// ***************************************************************************
static char g_hid_raw_interface_path[128];

#define LIBDS4_OUTPUT_DATA_SIZE   ( 79 )
#define LIBDS4_OUTPUT_DATA_CRC_OFFSET   ( 75 )
#define LIBDS4_OUTPUT_EMIT_DATA_OFFSET   ( 1 )

#define PRT_PRIFIX    "[libds4 OUTPUT] "

static char g_debug_flag = 0;
#define HODS4T_PRT( format, ... )   printf("\033[37m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )
#define HODS4T_DPRT( format, ... )  if( g_debug_flag ) printf("\033[37m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )
#define HODS4T_EPRT( format, ... )  printf("\033[31m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )

// ***************************************************************************
// Struct
// ***************************************************************************

// ***************************************************************************
// Declarations
// ***************************************************************************

// ***************************************************************************
// Variables
// ***************************************************************************
const char g_base_data_buffer[ LIBDS4_OUTPUT_DATA_SIZE ] = {
    0xa2, 0x11, 0xc0, 0x00, 0xff, 0x00, 0x00,
//  0xa2, 0x11, 0xc0, 0x05, 0xff, 0x04, 0x00,  // Does not work...
    0x00, /* Rumble (right / weak) */
    0x00, /* Rumble (left / strong) */
    0x00, 0x00, 0x00, /* R, G, B */
    0x00, /* flash ON duration */
    0x00, /* flash OFF duration */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x02, 0x03, 0x04 /* CRC-32 */
 };

// ***************************************************************************
// Functions
// ***************************************************************************

// Initialize some settings
int ds4_output_initialize( struct _stDS4OutputInitialize* pstInitialize ){
    strncpy( g_hid_raw_interface_path, pstInitialize->p_hid_raw_node_path,
                sizeof( g_hid_raw_interface_path ) );
    g_debug_flag = pstInitialize->debug_flag;
    return 0;
}

// Emit data through the HID pipe
int ds4_output_emit_data( struct _stDS4OutputEmitData* pstEmitData )
{
    int fd = 0;
    int res = 0;
    char send_data_buf[ LIBDS4_OUTPUT_DATA_SIZE ];

    // Open HID RAW node.
    fd = open( g_hid_raw_interface_path,
                O_WRONLY/*|O_NONBLOCK*/ );

    if ( fd < 0 ) {
        HODS4T_EPRT( "Unable to open %s.", g_hid_raw_interface_path );
        return -1;
    }

    // Overwrite data.
    memcpy( send_data_buf, g_base_data_buffer, LIBDS4_OUTPUT_DATA_SIZE );
    send_data_buf[ 7] = pstEmitData->rumbleRight;
    send_data_buf[ 8] = pstEmitData->rumbleLeft;
    send_data_buf[ 9] = pstEmitData->r;
    send_data_buf[10] = pstEmitData->g;
    send_data_buf[11] = pstEmitData->b;
    send_data_buf[12] = pstEmitData->flashOn;
    send_data_buf[13] = pstEmitData->flashOff;

    // Calcurate CRC-32.
    uLong ulong_crc = 0L;
    ulong_crc = crc32( ulong_crc, (const Bytef *)send_data_buf, LIBDS4_OUTPUT_DATA_SIZE - sizeof( uLong ) );
    HODS4T_DPRT( "CRC32: 0x%08X", (unsigned int)ulong_crc );

    send_data_buf[ LIBDS4_OUTPUT_DATA_CRC_OFFSET ]     = ulong_crc & 0xFF;
    send_data_buf[ LIBDS4_OUTPUT_DATA_CRC_OFFSET + 1 ] = ( ulong_crc >> 8 ) & 0xFF;
    send_data_buf[ LIBDS4_OUTPUT_DATA_CRC_OFFSET + 2 ] = ( ulong_crc >> 16 ) & 0xFF;
    send_data_buf[ LIBDS4_OUTPUT_DATA_CRC_OFFSET + 3 ] = ( ulong_crc >> 24 ) & 0xFF;

    HODS4T_DPRT( "CRC32: 0x%02X, 0x%02X, 0x%02X, 0x%02X.",
        send_data_buf[ LIBDS4_OUTPUT_DATA_CRC_OFFSET ] & 0xFF,
        send_data_buf[ LIBDS4_OUTPUT_DATA_CRC_OFFSET + 1 ] & 0xFF,
        send_data_buf[ LIBDS4_OUTPUT_DATA_CRC_OFFSET + 2 ] & 0xFF,
        send_data_buf[ LIBDS4_OUTPUT_DATA_CRC_OFFSET + 3 ] & 0xFF );

    // Send data to the Device
    res = write( fd, send_data_buf + LIBDS4_OUTPUT_EMIT_DATA_OFFSET,
                    LIBDS4_OUTPUT_DATA_SIZE - LIBDS4_OUTPUT_EMIT_DATA_OFFSET );
    if ( res < 0 ) {
        HODS4T_EPRT( "Error: %d", errno );
        close( fd );
        return -1;
    } else {
        HODS4T_DPRT( "write() wrote %d bytes", res );
    }

    close( fd );
    return 0;
}
