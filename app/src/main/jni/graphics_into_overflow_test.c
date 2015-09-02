
#include <dlfcn.h>
#include <errno.h>

#include <stdio.h>
#include <strings.h>

#include <android/log.h>


#define LOG_TAG "testing"
#define LOG_D(...) do{ __android_log_print( ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__); printf( __VA_ARGS__ ); }while( 0 )


int main( int argc, char *argv[] )
{
    const char *libname = "libui.so";
    int classBuf[ 100 ];
    int r1[ 10 ];
    int r2[ 10 ];
    int r3[ 10 ];
    int jellybean = 0;
    int ( *unflatten )( int *r0, int *r1, int *r2, int *r3 ) = NULL;

    printf( "hello world\n" );
    fflush( stdout );

    void *handle = dlopen( libname, RTLD_NOW | RTLD_GLOBAL );
    if( !handle )
    {
        LOG_D( "error opening %s: %s\n", libname, dlerror() );
        return -1;
    }

    bzero( classBuf, sizeof( classBuf ) );
    bzero( r1, sizeof( r1 ) );
    bzero( r2, sizeof( r2 ) );
    bzero( r3, sizeof( r3 ) );

    int ( *constructor )( int *r0 ) = dlsym( handle, "_ZN7android13GraphicBufferC2Ev" );
    if( !constructor )
    {
        LOG_D( "missing android::GraphicBuffer::GraphicBuffer(void)\n" );
        return -1;
    }

    unflatten = dlsym( handle, "_ZN7android13GraphicBuffer9unflattenERPKvRjRPKiS4_" );
    if( !unflatten )
    {
        unflatten = dlsym( handle, "_ZN7android13GraphicBuffer9unflattenEPKvjPij" );
        if( !unflatten )
        {
            LOG_D( "missing android::GraphicBuffer::unflatten\n" );
            return -1;
        }
        jellybean = 1;
    }

    constructor( classBuf );


    // setup bad values
    int r1Ref = (int)(&r1[0]);

    // this must match
    r1[0] = 0x47424652;

    // size must be > 0x1f
    r2[0] = 0x20;

    // attempt to overflow
    r1[8] = 0x1000;
    r1[9] = 0xFF5;

    // make sure we error out on unpatched libs before getting to the point where we corrupt the heap
    r1[6] = 0x20;
    r1[7] = 0x20;


    int ret = 0;
    if( !jellybean )
    {
        ret = unflatten( classBuf, &r1Ref, r2, r3 );
    }
    else
    {
        int * val = (int*)(r2[0]);
        ret = unflatten( classBuf, r1, val, r3 );
    }



    // -12 = unpatched 4.4.2
    // -22 = patches 5.1.1
    switch( ret )
    {
    case -ENOMEM:
        printf( "unpatched\n" );
        break;
    case -EINVAL:
        printf( "patched\n" );
        break;
    default:
        printf( "test is broken ret: %d (%08x)\n", ret, ret );
        break;
    }



    return 0;

}
