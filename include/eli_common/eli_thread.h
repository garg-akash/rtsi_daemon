#ifndef __THREADSAFE_SETTER_GETTER_H__
#define __THREADSAFE_SETTER_GETTER_H__

#include <pthread.h>
#include <string.h>

#define INIT_MUTEX(mutex) static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER
#define INIT_EXIST_MUTEX(mutex) pthread_mutex_init(&mutex, NULL)
#define DESTROY_MUTEX(mutex) pthread_mutex_destroy(&mutex)
#define LOCK_MUTEX(mutex) pthread_mutex_lock(&mutex)
#define TRYLOCK_MUTEX(mutex) (pthread_mutex_trylock(&mutex) == 0)
#define UNLOCK_MUTEX(mutex) pthread_mutex_unlock(&mutex)

#endif  // __THREADSAFE_SETTER_GETTER_H__