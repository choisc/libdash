#include "MultiThreading.h"


#if defined _WIN32 || defined _WIN64
#include <process.h>
struct ThreadParams
{
    void *(*start_routine) (void *);
    void *arg;
    HANDLE handle;
};
static unsigned __stdcall THREAD_START_ROUTINE(void* lpThreadParameter)
{
    struct ThreadParams tp = *(struct ThreadParams*)(lpThreadParameter);    
    /*void* result=*/tp.start_routine(tp.arg);    
    return 0;//yes. The return value is not used.
}
#endif
THREAD_HANDLE   CreateThreadPortable    (void *(*start_routine) (void *), void *arg)
{
    #if defined _WIN32 || defined _WIN64
        struct ThreadParams* tp = (struct ThreadParams*)malloc(sizeof(struct ThreadParams));
        tp->start_routine = start_routine;
        tp->arg = arg;
        tp->handle = (HANDLE)_beginthreadex(NULL, 0, THREAD_START_ROUTINE, (void*)tp, 0, nullptr);
        if (tp->handle ==NULL)
        {
            free(tp);
            return NULL;
        }
        return tp;
    #else
        THREAD_HANDLE th = (THREAD_HANDLE)malloc(sizeof(pthread_t));
    
        if (!th)
        {
            std::cerr << "Error allocating thread." << std::endl;
            free(th);
            return NULL;
        }

        if(int err = pthread_create(th, NULL, start_routine, arg))
        {
            std::cerr << strerror(err) << std::endl;
            free(th);
            return NULL;
        }
        return th;
    #endif
}
void            DestroyThreadPortable   (THREAD_HANDLE th)
{
    if(th)
    {
    #if defined _WIN32 || defined _WIN64
        struct ThreadParams* tp = (struct ThreadParams*)(th);
        WaitForSingleObject(tp->handle, INFINITE);
        CloseHandle(tp->handle);
    #else
        void* retval;
        pthread_join(*th,&retval);
    #endif
        free(th);
    }
}

/****************************************************************************
* Condition variables for Windows XP and older windows sytems
*****************************************************************************/
#if defined WINXPOROLDER
    void InitCondition       (condition_variable_t *cv)
    {
        InitializeCriticalSection(&cv->waitersCountLock);

        cv->waitersCount        = 0;
        cv->waitGenerationCount = 0;
        cv->releaseCount        = 0;

        cv->waitingEvent = CreateEvent (NULL,  // no security
                                        TRUE,  // manual-reset
                                        FALSE, // non-signaled initially
                                        NULL); // unnamed
    }
    void WaitCondition       (condition_variable_t *cv, CRITICAL_SECTION *externalMutex)
    {
        EnterCriticalSection(&cv->waitersCountLock);

        cv->waitersCount++;

        int currentGenerationCount = cv->waitGenerationCount;

        LeaveCriticalSection(&cv->waitersCountLock);
        LeaveCriticalSection(externalMutex);

        bool isWaitDone = false;
        while(!isWaitDone)
        {
            WaitForSingleObject (cv->waitingEvent, INFINITE);
            EnterCriticalSection (&cv->waitersCountLock);

            isWaitDone = (cv->releaseCount > 0 && cv->waitGenerationCount != currentGenerationCount);
            LeaveCriticalSection (&cv->waitersCountLock);
        }

        EnterCriticalSection(externalMutex);
        EnterCriticalSection(&cv->waitersCountLock);

        cv->waitersCount--;
        cv->releaseCount--;
        bool isLastWaiter = (cv->releaseCount == 0);

        LeaveCriticalSection(&cv->waitersCountLock);

        if(isLastWaiter)
            ResetEvent(cv->waitingEvent);
    }
    void SignalCondition     (condition_variable_t *cv)
    {
        EnterCriticalSection(&cv->waitersCountLock);

        if(cv->waitersCount > cv->releaseCount) 
        {
            SetEvent(cv->waitingEvent);
            cv->releaseCount++;
            cv->waitGenerationCount++;
        }

        LeaveCriticalSection(&cv->waitersCountLock);
    }
    void BroadcastCondition  (condition_variable_t *cv)
    {
        EnterCriticalSection(&cv->waitersCountLock);

        if(cv->waitersCount > 0) 
        {
            SetEvent(cv->waitingEvent);
            cv->releaseCount = cv->waitersCount;
            cv->waitGenerationCount++;
        }

        LeaveCriticalSection(&cv->waitersCountLock);
}
#endif
