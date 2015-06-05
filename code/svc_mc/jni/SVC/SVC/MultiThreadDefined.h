#ifndef H_MULTI_THREAD_DEFINED_H
#define H_MULTI_THREAD_DEFINED_H

#include <pthread.h>

/* Thread Handler */
#define CRITICAL_SECTION    pthread_mutex_t
#define CONDITION_VARIABLE  pthread_cond_t

#define PortableSleep(seconds)								usleep(seconds * 1000000)
#define JoinThread(handle)									pthread_join(*(handle), NULL)
#define InitializeCriticalSection(mutex_p)					pthread_mutex_init(mutex_p, NULL)
#define DeleteCriticalSection(mutex_p)						pthread_mutex_destroy(mutex_p)
#define EnterCriticalSection(mutex_p)						pthread_mutex_lock(mutex_p)
#define LeaveCriticalSection(mutex_p)						pthread_mutex_unlock(mutex_p)
#define InitializeConditionVariable(cond_p)					pthread_cond_init(cond_p, NULL)
#define DeleteConditionVariable(cond_p)						pthread_cond_destroy(cond_p)
#define WaitConditionVariableCS(cond_p, mutex_p)			pthread_cond_wait(cond_p, mutex_p)
#define TimedWaitConditionVariableCS(cond_p, mutex_p, time)	pthread_cond_timedwait(cond_p, mutex_p, time)
#define WakeConditionVariable(cond_p)						pthread_cond_signal(cond_p)
#define WakeAllConditionVariable(cond_p)					pthread_cond_broadcast(cond_p)

#endif /* H_MULTI_THREAD_DEFINED_H */