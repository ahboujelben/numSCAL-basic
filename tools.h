/////////////////////////////////////////////////////////////////////////////
/// Name:        tools.h
/// Purpose:     static utility functions are declared and defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef TOOLS_H
#define TOOLS_H

#include <cmath>
#include <cstdlib>

#if defined(_WIN32)
#include <Windows.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>

#else
#error "Unknown OS."
#endif

using namespace std;

class tools {

public:
    static double pi(){return 3.14159265358979323846264;}
    static double g(){return 9.8;}
    static double pAtm(){return 0.980665*1e5;}
    static double R(){return 8.314;}
    static double T(){return 313;}

    static double PsiToPa(double const &pressure)
    {
        return pressure/14.50377*1e5;
    }

    static double PaToPsi(double const &pressure)
    {
        return pressure*14.50377/1e5;
    }

    static void renderVideo()
    {
    #if defined(_WIN32)
    /* Windows -------------------------------------------------- */
        system("ffmpeg\\ffmpeg -r 25 -i Videos\\IMG1%9d.png -r 30 -pix_fmt yuv420p -vf \"scale=trunc(iw/2)*2:trunc(ih/2)*2\" -y Videos\\video.mp4 > nul 2>&1");
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */
        system("./ffmpeg/ffmpeg -r 25 -i Videos/IMG1%9d.png -r 30 -pix_fmt yuv420p -vf \"scale=trunc(iw/2)*2:trunc(ih/2)*2\" -y Videos/video.mp4 > /dev/null 2>&1");
    #endif
    }

    static void createRequiredFolders()
    {
    #if defined(_WIN32)
    /* Windows -------------------------------------------------- */
        system("mkdir \"Input Data\" > nul 2>&1");
        system("mkdir Results > nul 2>&1");
        system("mkdir Results\\Network_Status > nul 2>&1");
        system("mkdir Results\\Network_Description > nul 2>&1");
        system("mkdir Videos > nul 2>&1");
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */
        system("mkdir -p 'Input Data' > /dev/null 2>&1");
        system("mkdir -p Results/Network_Status > /dev/null 2>&1");
        system("mkdir -p Results/Network_Description > /dev/null 2>&1");
        system("mkdir -p Videos > /dev/null 2>&1");
    #endif
    }

    static void cleanVideosFolder()
    {
    #if defined(_WIN32)
    /* Windows -------------------------------------------------- */
        system("del Videos\\*.png > nul 2>&1");
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */
        system("rm Videos/*.png > /dev/null 2>&1");
    #endif
    }

    static void cleanResultsFolder()
    {
    #if defined(_WIN32)
    /* Windows -------------------------------------------------- */
        system("del Results\\*.txt > nul 2>&1");
        system("del Results\\*.png > nul 2>&1");
        system("del Results\\Network_Status\\*.txt > nul 2>&1");
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */
        system("rm Results/*.txt > /dev/null 2>&1");
        system("rm Results/*.png > /dev/null 2>&1");
        system("rm Results/Network_Status/*.txt > /dev/null 2>&1");
    #endif
    }

    static void cleanNetworkDescriptionFolder()
    {
    #if defined(_WIN32)
    /* Windows -------------------------------------------------- */
        system("del Results\\Network_Description\\*.txt > nul 2>&1");
        system("del Results\\Network_Description\\*.png > nul 2>&1");
    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */
        system("rm Results/Network_Description/*.txt > /dev/null 2>&1");
        system("rm Results/Network_Description/*.png > /dev/null 2>&1");
    #endif
    }

    static double getCPUTime( )
    {
    /**
     * Returns the amount of CPU time used by the current process,
     * in seconds, or -1.0 if an error occurred.
     */
    #if defined(_WIN32)
        /* Windows -------------------------------------------------- */
        FILETIME createTime;
        FILETIME exitTime;
        FILETIME kernelTime;
        FILETIME userTime;
        if ( GetProcessTimes( GetCurrentProcess( ),
            &createTime, &exitTime, &kernelTime, &userTime ) != -1 )
        {
            SYSTEMTIME userSystemTime;
            if ( FileTimeToSystemTime( &userTime, &userSystemTime ) != -1 )
                return (double)userSystemTime.wHour * 3600.0 +
                    (double)userSystemTime.wMinute * 60.0 +
                    (double)userSystemTime.wSecond +
                    (double)userSystemTime.wMilliseconds / 1000.0;
        }

    #elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
        /* AIX, BSD, Cygwin, HP-UX, Linux, OSX, and Solaris --------- */

    #if _POSIX_TIMERS > 0
        /* Prefer high-res POSIX timers, when available. */
        {
            clockid_t id;
            struct timespec ts;
    #if _POSIX_CPUTIME > 0
            /* Clock ids vary by OS.  Query the id, if possible. */
            if ( clock_getcpuclockid( 0, &id ) == -1 )
    #endif
    #if defined(CLOCK_PROCESS_CPUTIME_ID)
                /* Use known clock id for AIX, Linux, or Solaris. */
                id = CLOCK_PROCESS_CPUTIME_ID;
    #elif defined(CLOCK_VIRTUAL)
                /* Use known clock id for BSD or HP-UX. */
                id = CLOCK_VIRTUAL;
    #else
                id = (clockid_t)-1;
    #endif
            if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
                return (double)ts.tv_sec +
                    (double)ts.tv_nsec / 1000000000.0;
        }
    #endif

    #if defined(RUSAGE_SELF)
        {
            struct rusage rusage;
            if ( getrusage( RUSAGE_SELF, &rusage ) != -1 )
                return (double)rusage.ru_utime.tv_sec +
                    (double)rusage.ru_utime.tv_usec / 1000000.0;
        }
    #endif

    #if defined(_SC_CLK_TCK)
        {
            const double ticks = (double)sysconf( _SC_CLK_TCK );
            struct tms tms;
            if ( times( &tms ) != (clock_t)-1 )
                return (double)tms.tms_utime / ticks;
        }
    #endif

    #if defined(CLOCKS_PER_SEC)
        {
            clock_t cl = clock( );
            if ( cl != (clock_t)-1 )
                return (double)cl / (double)CLOCKS_PER_SEC;
        }
    #endif

    #endif

        return -1.0;		/* Failed. */
    }
};

#endif // TOOLS_H
