#include "config.h"
#include "TimeCounter.h"

#if ENABLE(VMOLAB_TIME)

#include "wtf/CurrentTime.h"

namespace JSC {

double TimeCounter::accumTime[TimeCounter::TYPEsize];
double TimeCounter::startTime[TimeCounter::TYPEsize];
double TimeCounter::max_waitTime;

void TimeCounter::start(Type type)
{
    ASSERT(!startTime[type]);

    startTime[type] = currentTimeMS();
}

void TimeCounter::stop(Type type)
{
    ASSERT(startTime[type]);

    double elapsed = currentTimeMS() - startTime[type];

    if (type == TYPEwait) {
        if (elapsed > max_waitTime)
            max_waitTime = elapsed;
    }

    accumTime[type] += elapsed;
    startTime[type] = 0;
}

void TimeCounter::reset()
{
    for (int i = 0; i < TYPEsize; i++) {
        startTime[i] = 0;
        accumTime[i] = 0;
    }
    max_waitTime = 0;
}

void TimeCounter::count()
{
    accumTime[TYPEstring]++;
}

void TimeCounter::report()
{
    printf("========== REPORT TIME ==========\n");
    printf("JSEvaluateTime: %f ms\n", accumTime[TYPEeval]);
    printf("JSFuncCallTime: %f ms\n", accumTime[TYPEcall]);
    printf("JSGlobParseTime: %f ms\n", accumTime[TYPEglobparse]);
    printf("JSThreadWaitTime: %f ms\n", accumTime[TYPEwait]);
    printf("JSThreadReadTime: %f ms\n", accumTime[TYPEread]);
    printf("MaxWaitTime: %f ms\n", max_waitTime);
    printf("StringCount: %f \n", accumTime[TYPEstring]);
    printf("========== REPORT TIME ==========\n");
}

}
#endif // VMOLAB_TIME
