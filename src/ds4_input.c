
// ***************************************************************************
// Includes
// ***************************************************************************
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>

#include <linux/input.h>

#include <ds4_input.h>

// ***************************************************************************
// Definitions
// ***************************************************************************
static char g_hid_raw_node_path[128];

#define PRT_PRIFIX    "[libds4 INPUT] "

#define LIBDS4_USE_BT
// #define LIBDS4_USE_USB

#if defined( LIBDS4_USE_BT )
#define LIBDS4_INPUT_DATA_SIZE   ( 78 )
#elif defined( LIBDS4_USE_USB )
#define LIBDS4_INPUT_DATA_SIZE   ( 64 )
#endif
static char g_debug_flag = 0;
#define HIDS4T_PRT( format, ... )   printf("\033[37m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )
#define HIDS4T_DPRT( format, ... )  if( g_debug_flag ) printf("\033[37m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )
#define HIDS4T_EPRT( format, ... )  printf("\033[31m" PRT_PRIFIX format"\033[m\n", ## __VA_ARGS__ )

#define HIDIOCGFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, len)

// ***************************************************************************
// Struct
// ***************************************************************************

// ***************************************************************************
// Declarations
// ***************************************************************************
static void* event_read_thread( void* pParam );

// ***************************************************************************
// Variables
// ***************************************************************************
static int ( *gInput_evt_callback )( struct _stDS4InputEvt* pstInputEvt );
static pthread_t gEvent_read_thread_id;
static bool gIsNeedStopEvtThread = false;
static int is_exist_hid_raw_node( void );

// ***************************************************************************
// Functions
// ***************************************************************************

// Initialize some settings
int ds4_input_initialize( struct _stDS4InputInitialize* pstInitialize ){
    strncpy( g_hid_raw_node_path, pstInitialize->p_hid_raw_node_path,
                sizeof( g_hid_raw_node_path ) );
    HIDS4T_DPRT( "ds4_input_initialize() start" );
    g_debug_flag = pstInitialize->debug_flag;
    HIDS4T_DPRT( "ds4_input_initialize() end" );
    return 0;
}

int ds4_input_start_listening_event( int ( *callback )( struct _stDS4InputEvt* pstInputEvt ) ){
    HIDS4T_DPRT( "ds4_input_start_listening_event() start" );
    gIsNeedStopEvtThread = false;
    gInput_evt_callback = callback;

    // Create HID Evnet read thread.
    pthread_create( &gEvent_read_thread_id, NULL, event_read_thread, NULL );

    HIDS4T_DPRT( "ds4_input_start_listening_event() end" );
    return 0;
}

int ds4_input_stop_listening_event( void ){
    HIDS4T_DPRT( "ds4_input_stop_listening_event() start" );

    gIsNeedStopEvtThread = true;

    // Wait for pthread_exit()...
    pthread_join( gEvent_read_thread_id, NULL );

    HIDS4T_DPRT( "ds4_input_stop_listening_event() end" );
    return 0;
}

static void* event_read_thread( void* pParam __attribute__((__unused__)) ){

    FILE *hid_event_fp;
    ssize_t read_size = 0;
    unsigned char event_data_buf[256];
    unsigned char get_feature_buf[256];
    struct _stDS4InputEvt stDS4InputEvt;
    // int i;

    HIDS4T_DPRT( "event_read_thread start!" );

    // Initialize variables.
    memset( &stDS4InputEvt, 0x00, sizeof( stDS4InputEvt ) );

    // Open HID event device node = HID raw node
    if( ( hid_event_fp = fopen( g_hid_raw_node_path, "r" ) ) == NULL ) {
        HIDS4T_EPRT( "%s File open error. [%s]", g_hid_raw_node_path, strerror( errno ) );
        goto SUB_RET;
    }

    /* Get Feature */
    get_feature_buf[0] = 0x2; /* Report Number */
    get_feature_buf[1] = 0x35; /* Buffer size */
    get_feature_buf[2] = 0x00;
    int res = ioctl( fileno( hid_event_fp ), HIDIOCGFEATURE( 256 ), get_feature_buf );
    if ( res < 0 ) {
        HIDS4T_EPRT( "HIDIOCGFEATURE" );
    } else {
        HIDS4T_DPRT( "ioctl HIDIOCGFEATURE returned: %d", res );
        HIDS4T_DPRT( "Report data (not containing the report number):" );
        // for (i = 0; i < res; i++)
        //        printf("%hhx ", get_feature_buf[i]);
        // puts("\n");
    }

    // Event get loop.
    while( gIsNeedStopEvtThread == false ){
        
        // Reopen file for achieve a beginning of event data.
        if( ( hid_event_fp = freopen( g_hid_raw_node_path, "r", hid_event_fp ) ) == NULL ) {
            HIDS4T_EPRT( "%s File reopen error. [%s]", g_hid_raw_node_path, strerror( errno ) );
            stDS4InputEvt.evtType = 0xFF;
            gInput_evt_callback( &stDS4InputEvt );
            goto FCLOSE_RET;        }
        /*
        if( is_exist_hid_raw_node() == 0 ){
            HIDS4T_EPRT( "is_exist_hid_raw_node false" );
            stDS4InputEvt.evtType = 0xFF;
            gInput_evt_callback( &stDS4InputEvt );
            goto FCLOSE_RET;
        }
        */
        // Read HID Event.
        read_size = fread( event_data_buf, sizeof(char), LIBDS4_INPUT_DATA_SIZE, hid_event_fp );
        if ( read_size == (ssize_t) -1 && errno != EINTR ) {
            HIDS4T_EPRT( "HID event fread error. [%s]", strerror(errno) );
            
        }else{
            HIDS4T_DPRT( "read_size[%d]", read_size );
            HIDS4T_DPRT( "buf:%02X,%02X,%02X,%02X ",
                            event_data_buf[0] & 0xFF, event_data_buf[1] & 0xFF,
                            event_data_buf[2] & 0xFF, event_data_buf[3] & 0xFF );
        }
        
        stDS4InputEvt.evtType = 0x00;
        
        unsigned char hid_data_offset = 0;
        #if defined( LIBDS4_USE_BT )
        hid_data_offset = 3;
        #elif defined( LIBDS4_USE_USB )
        hid_data_offset = 1;
        #endif
        
        // Handle Event packet
        stDS4InputEvt.L3analogH = event_data_buf[ hid_data_offset ];
        stDS4InputEvt.L3analogV = event_data_buf[ hid_data_offset + 1 ];
        stDS4InputEvt.R3analogH = event_data_buf[ hid_data_offset + 2 ];
        stDS4InputEvt.R3analogV = event_data_buf[ hid_data_offset + 3 ];

        stDS4InputEvt.btnUp    = false;
        stDS4InputEvt.btnDown  = false;
        stDS4InputEvt.btnRight = false;
        stDS4InputEvt.btnLeft  = false;
        switch( event_data_buf[ hid_data_offset + 4 ] & 0x0F ){
          case 0x00:
            stDS4InputEvt.btnUp = true;
            break;
          case 0x01:
            stDS4InputEvt.btnUp = true;
            stDS4InputEvt.btnRight = true;
            break;
          case 0x02:
            stDS4InputEvt.btnRight = true;
            break;
          case 0x03:
            stDS4InputEvt.btnRight = true;
            stDS4InputEvt.btnDown = true;
            break;
          case 0x04:
            stDS4InputEvt.btnDown = true;
            break;
          case 0x05:
            stDS4InputEvt.btnDown = true;
            stDS4InputEvt.btnLeft = true;
            break;
          case 0x06:
            stDS4InputEvt.btnLeft = true;
            break;
          case 0x07:
            stDS4InputEvt.btnUp = true;
            stDS4InputEvt.btnLeft = true;
            break;
          default:
            break;
        }

        stDS4InputEvt.btnSquare   = ( ( event_data_buf[ hid_data_offset + 4 ] & 0x10 ) != 0 ) ? true : false;
        stDS4InputEvt.btnCross    = ( ( event_data_buf[ hid_data_offset + 4 ] & 0x20 ) != 0 ) ? true : false;
        stDS4InputEvt.btnCircle   = ( ( event_data_buf[ hid_data_offset + 4 ] & 0x40 ) != 0 ) ? true : false;
        stDS4InputEvt.btnTriangle = ( ( event_data_buf[ hid_data_offset + 4 ] & 0x80 ) != 0 ) ? true : false;

        stDS4InputEvt.btnL1 = ( ( event_data_buf[ hid_data_offset + 5 ] & 0x01 ) != 0 ) ? true : false;
        stDS4InputEvt.btnR1 = ( ( event_data_buf[ hid_data_offset + 5 ] & 0x02 ) != 0 ) ? true : false;
        stDS4InputEvt.btnL2 = ( ( event_data_buf[ hid_data_offset + 5 ] & 0x04 ) != 0 ) ? true : false;
        stDS4InputEvt.btnR2 = ( ( event_data_buf[ hid_data_offset + 5 ] & 0x08 ) != 0 ) ? true : false;
        stDS4InputEvt.btnShare = ( ( event_data_buf[ hid_data_offset + 5 ] & 0x10 ) != 0 ) ? true : false;
        stDS4InputEvt.btnOptions = ( ( event_data_buf[ hid_data_offset + 5 ] & 0x20 ) != 0 ) ? true : false;
        stDS4InputEvt.btnL3 = ( ( event_data_buf[ hid_data_offset + 5 ] & 0x40 ) != 0 ) ? true : false;
        stDS4InputEvt.btnR3 = ( ( event_data_buf[ hid_data_offset + 5 ] & 0x80 ) != 0 ) ? true : false;

        stDS4InputEvt.btnPlaystation = ( ( event_data_buf[ hid_data_offset + 6 ] & 0x01 ) != 0 ) ? true : false;
        stDS4InputEvt.btnTouchpad    = ( ( event_data_buf[ hid_data_offset + 6 ] & 0x02 ) != 0 ) ? true : false;

        stDS4InputEvt.L2analog = event_data_buf[ hid_data_offset + 7 ];
        stDS4InputEvt.R2analog = event_data_buf[ hid_data_offset + 8 ];
        stDS4InputEvt.batteryLevel = event_data_buf[  hid_data_offset + 11 ];

        gInput_evt_callback( &stDS4InputEvt );

    }

FCLOSE_RET:
    fclose( hid_event_fp );

SUB_RET:
    HIDS4T_DPRT( "event_read_thread exit!" );
    pthread_exit( NULL );
}

static int is_exist_hid_raw_node( void ){
    
    struct stat st;
    int ret = stat( g_hid_raw_node_path, &st );
    return ( ret == 0 ) ? 1 : 0;
    
}
