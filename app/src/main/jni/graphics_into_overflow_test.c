#include <dlfcn.h>
#include <errno.h>
#include <limits.h>

#include <stdio.h>
#include <strings.h>
#include <jni.h>
#include <android/log.h>


#define LOG_TAG "testing"
//#define LOG_D(...) do{ __android_log_print( ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__); printf( __VA_ARGS__ ); }while( 0 )

enum
{
    JELLYBEAN = 0,
    KITKAT_AND_LOLLIPOP = 1,
    MARSHMELLOW = 2,
    OTHER = 99
};

//#define int uint64_t
int checkGraphicsBufferVuln( int v );


JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_framework_graphics_GraphicBufferTest_checkGraphicsBuffer(JNIEnv *env, jobject obj, jint aVersion)
{
    return checkGraphicsBufferVuln( aVersion );
}

/* 0    1   2   3
 * 4    5   6   7
 * 8    9   10  11
 */

void SetupBufferJ( int *r1, int *r2 )
{
    // gnex JZO54K - fails
    // don't have a patched JB build to test against

    // size must be > 0x1f
    r2[0] = 0x20;

    // this must match
    r1[0] = 0x47424652;

    // attempt to overflow
    r1[8] = 0x1000;
    r1[9] = 0xFF5;

    // make sure we error out on unpatched libs before getting to the point where we corrupt the heap
    r1[6] = 0x20;
    r1[7] = 0x20;
}

void SetupBufferKL( int *r1, int *r2 )
{
    // nexus 7 - KTU84P fails this test.  no patched Kitkat factory images to test?
    // s4 gpe  - LMY47O passing
    // motoG gpe - LMY47M.M003 passing

    // size must be > 0x1f
    r2[0] = 0x20;

    // this must match
    r1[0] = 0x47424652;

    // attempt to overflow
    r1[8] = 0x1000;
    r1[9] = 0xFF5;

    // make sure we error out on unpatched libs before getting to the point where we corrupt the heap
    r1[6] = 0x20;
    r1[7] = 0x20;
}

void SetupBufferM( int * r1, int *r2 )
{
    // nexus 5 MRA58K passes
    // there should be any marshmellow roms that fail this test.  it was already patched
    // this must match
    r1[0] = 0x47424652;

    // size must be >= 0x2c
    r2[0] = 0x2c;

    // attempt to overflow
    r1[9] = 0x1000;
    r1[10] = 0xff5;

    // make sure we error out on unpatched libs before getting to the point where we corrupt the heap
    r1[6] = 0x20;
    r1[7] = 0x20;
}

int checkGraphicsBufferVuln(int v )
{
    const char *libname = "libui.so";
    int classBuf[ 100 ];
    int r1[ 20 ];
    int r2[ 10 ];
    int r3[ 10 ];
    int r4[ 10 ];// r4 is necessary for marchmellow
    int ( *unflatten )( int *r0, int *r1, int *r2, int *r3, int *r4 ) = NULL;

    void *handle = dlopen( libname, RTLD_NOW | RTLD_GLOBAL );
    if( !handle )
    {
        printf( "error opening %s: %s\n", libname, dlerror() );
        return -1;
    }

    bzero( classBuf, sizeof( classBuf ) );
    bzero( r1, sizeof( r1 ) );
    bzero( r2, sizeof( r2 ) );
    bzero( r3, sizeof( r3 ) );
    bzero( r4, sizeof( r4 ) );

    int ( *constructor )( int *r0 ) = dlsym( handle, "_ZN7android13GraphicBufferC2Ev" );
    if( !constructor )
    {
        printf( "missing android::GraphicBuffer::GraphicBuffer(void)\n" );
        return -1;
    }

    unflatten = dlsym( handle, "_ZN7android13GraphicBuffer9unflattenERPKvRjRPKiS4_" );
    if( !unflatten )
    {
        unflatten = dlsym( handle, "_ZN7android13GraphicBuffer9unflattenEPKvjPij" );
        if( !unflatten )
        {
            printf( "missing android::GraphicBuffer::unflatten\n" );
            return -1;
        }
    }

    constructor( classBuf );


    // setup bad values
    int r1Ref = (int)(&r1[0]);

    int ret = 0;
    switch( v )
    {
    case JELLYBEAN:
        SetupBufferJ( r1, r2 );
        ret = unflatten( classBuf, r1, (int*)r2[0], r3, r4 );
        break;
    case KITKAT_AND_LOLLIPOP:
        SetupBufferKL( r1, r2 );
        ret = unflatten( classBuf, &r1Ref, (int*)r2, r3, r4 );
        break;
    case MARSHMELLOW:
        SetupBufferM( r1, r2 );
        r3[0] = 0x20;
        ret = unflatten( classBuf, &r1Ref, (int*) r2, r3, r4 );
        break;
    default:
        printf( "unsupported OS version.\n" );
        return -1;
    }



    // -12 = unpatched 4.4.2
    // -22 = patches 5.1.1
    switch( ret )
    {
    case -ENOMEM:
        printf( "unpatched\n" );
        return 1;
    case -EINVAL:
        printf( "patched\n" );
        return 0;
    default:
        printf( "test is broken ret: %d (%08x)\n", ret, ret );
        if(ret == 0 || ret == 1){
           return -1;
        }else{
           return ret;
        }
    }

}

int main( int argc, char *argv[] )
{
    checkGraphicsBufferVuln( JELLYBEAN );
    return 0;
}
