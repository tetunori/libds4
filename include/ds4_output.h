
#ifndef _DS4_OUTPUT_H_
#define _DS4_OUTPUT_H_

// ***************************************************************************
// Struct
// ***************************************************************************
struct _stDS4OutputEmitData{
    char rumbleRight;
    char rumbleLeft;
    char r;
    char g;
    char b;
    char flashOn;
    char flashOff;
};

struct _stDS4OutputInitialize{
    char* p_hid_raw_node_path;
    char debug_flag;
};

// ***************************************************************************
// Functions
// ***************************************************************************
int ds4_output_initialize( struct _stDS4OutputInitialize* pstInitialize );
int ds4_output_emit_data( struct _stDS4OutputEmitData* pstEmitData );

#endif // _DS4_OUTPUT_H_
