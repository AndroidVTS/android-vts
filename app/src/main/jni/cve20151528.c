#include <dlfcn.h>
#include <errno.h>
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>

//#include <cutils/native_handle.h>

#define LOG_TAG "CVE_2015_1528"
#define LOG_D(...) do{ __android_log_print( ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__); printf( __VA_ARGS__ ); }while( 0 )


int Check_CVE_2015_1528()
{
    const char *libname = "libcutils.so";
    size_t * ( *native_handle_create )( int numFds, int numInts ) = NULL;

    void *handle = dlopen( libname, RTLD_NOW | RTLD_GLOBAL );
    if( !handle )
    {
        printf( "error opening %s: %s\n", libname, dlerror() );
        return -1;
    }

    native_handle_create = dlsym( handle, "native_handle_create" );
    if( !native_handle_create )
    {
        printf( "missing native_handle_create\n" );
        return -2;
    }

    int ret = -3;

    int numFds = 1025;
    int numInts = 1;
    size_t *bla = native_handle_create( numFds, numInts );
    if( !bla )
    {
        // fixed
        printf( "looks fixed to me\n" );
        ret = 0;
        goto done;
    }

    // sanity checks
    switch(bla[0])// version
    {
    case 12://android wear 5.0.2 LWX49K
        if( bla[1] != numFds || bla[2] != numInts )
        {
            LOG_D( "got back unexpected values\n" );
        }
        else
        {
            LOG_D( "its vulnerable\n" );
            return 1;
        }
        break;
    default:
        LOG_D( "failed.  version %d  %d %d\n", bla[0], bla[1], bla[2] );
        break;
    }


done:
    // done with this
    dlclose( handle );

    // should be allocated with malloc
    //! if its already null, then free does nothing
    free( bla );

    return ret;
}


JNIEXPORT jint JNICALL Java_fuzion24_device_vulnerability_vulnerabilities_system_CVE20151528_doCheck(JNIEnv *env, jobject obj)
{
    return Check_CVE_2015_1528();
}


int main( int argc, char *argv[] )
{
    return Check_CVE_2015_1528();
}
