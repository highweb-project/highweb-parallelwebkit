#ifndef TimeCounter_h
#define TimeCounter_h

#if ENABLE(VMOLAB_TIME)

namespace JSC {

class TimeCounter {
public:
    enum Type {
        TYPEeval,
        TYPEcall,
        TYPEglobparse,
        TYPEglobbyte,
        TYPEfuncparse,
        TYPEfuncbyte,
        TYPEread,
        TYPEwait,
        TYPEstring,
        TYPEsize
    };

    static void start(Type type);
    static void stop(Type type);

    static void count();

    static void reset();
    static void report();

private:
    static double accumTime[TYPEsize];
    static double startTime[TYPEsize];
    static double max_waitTime;
};

}

#endif // VMOLAB_TIME
#endif // TimeCounter_h
