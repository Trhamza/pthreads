#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

//#define SUBJECT1
//#define SUBJECT2
//#define SUBJECT3
//#define SUBJECT4
//#define SUBJECT5
//#define SUBJECT6
//#define SUBJECT7

#ifdef SUBJECT1 
void *threadPrintf(void *);
void *threadPrintf(void *ptr)
{
    char *message;
    message  = (char *)ptr;
    printf("%s \n", message);
}
#endif

#ifdef SUBJECT2
struct thread_data{
    char *message;
    int thread_id;
    int sum;
};

void* printHello(void *thread_arg);

void* printHello(void *thread_arg){
    struct thread_data *my_data;
    int taskid = 0, sum = 0;
    char hello_msg[255];

    my_data = (struct thread_data *) thread_arg;
    taskid = my_data->thread_id;
    sum = my_data->sum;
    strcpy(hello_msg, my_data->message);
    printf("taskid = %d, sum = %d, message=%s\n",taskid,sum,hello_msg);
}
#endif

#ifdef SUBJECT3
void * hello_return(void * args);
void * hello_return(void* args)
{
    char * hello = strdup("Hello World!\n");
    return (void *) hello;
}

#endif

#ifdef SUBJECT4
void* calls(void* ptr)
{
    printf("In function \n thread id = %d\n", pthread_self());
    pthread_exit(NULL);
    return NULL;
}

#endif

#ifdef SUBJECT5
pthread_attr_t attr;
void *dowork(void *threadid);

void *dowork(void *threadid)
{
    long tid;
    size_t mystacksize;

    tid = (long)threadid;
    pthread_attr_getstacksize(&attr, &mystacksize);
    printf("Thread %ld: stack size = %li bytes \n",tid,mystacksize);

    pthread_exit(NULL);
}
#endif

#ifdef SUBJECT6
int j = 0;
pthread_mutex_t lock;

void doIncrease()
{
    pthread_mutex_lock(&lock);
    int i = 0;
    while(i < 5)
    {
        printf("%d",j);
        sleep(1);
        i++;
    }
    printf("done.\n");
    j++;
    pthread_mutex_unlock(&lock);
}


#endif

#ifdef SUBJECT7
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;

void *functionCount1(void);
void *functionCount2(void);
int count = 0;

#define COUNT_DONE 10
#define COUNT_HALT1 3
#define COUNT_HALT2 6

void *functionCount1()
{
    for(;;)
    {
        pthread_mutex_lock(&condition_mutex);
        while (count >= COUNT_HALT1 && count <= COUNT_HALT2){
            pthread_cond_wait(&condition_cond, &condition_mutex);
        }

        pthread_mutex_unlock( &condition_mutex );

        pthread_mutex_lock( &count_mutex );
        count++;
        printf( "Counter value functionCount1: %d\n", count );
        pthread_mutex_unlock( &count_mutex );

        if( count >= COUNT_DONE ) return( NULL );
        
    }
}

void *functionCount2()
{
    for(;;)
    {
        pthread_mutex_lock(&condition_mutex);
        if (count < COUNT_HALT1 || count > COUNT_HALT2){
            pthread_cond_signal(&condition_cond);
        }

        pthread_mutex_unlock(&condition_mutex);

        pthread_mutex_lock(&count_mutex);
        count++;
        printf("Counter value functionCount2: %d\n",count);
        pthread_mutex_unlock(&count_mutex);

        if(count >= COUNT_DONE) return(NULL);
        
    }
}


#endif
int main()
{
    #ifdef SUBJECT1
    pthread_t thread1, thread2;
    int t1 = 1, t2 = 1;
    char *mes1 = "Thread 1";
    char *mes2 = "Thread 2";
    t1 = pthread_create(&thread1, NULL, &threadPrintf, (void *) mes1 );
    t2 = pthread_create(&thread2, NULL, &threadPrintf, (void *) mes2 );

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Thread 1: %d\n", t1);
    printf("Thread 2: %d\n", t2);

    pthread_exit(NULL);
    #endif
    
    #ifdef SUBJECT2
    pthread_t my_thread;
    struct thread_data my_thread_data;
    my_thread_data.message = malloc(sizeof(char) * 255);

    my_thread_data.thread_id = 10;
    my_thread_data.sum = 35;
    my_thread_data.message = "Hello World";

    pthread_t my_id = pthread_create(&my_thread, NULL , printHello, (void *) &my_thread_data);
    pthread_join(my_thread, NULL);
    #endif
       
    #ifdef SUBJECT3
    char *str;
    pthread_t thread;
    pthread_create(&thread, NULL, hello_return, NULL);
    pthread_join(thread, (void  **) &str);
    //pthread_exit(NULL);
    printf("%s", str);

    #endif

    #ifdef SUBJECT4
    pthread_t thread;
    pthread_create(&thread, NULL, calls, NULL);
    printf("In main \nthread id= %d \n ", thread);
    pthread_join(thread, NULL);
    #endif

    #ifdef SUBJECT5
    pthread_t myThread;
    size_t stacksize;
    pthread_t myId;
    long t;

    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr, &stacksize);
    printf("Default stack size = %li\n", stacksize);
    stacksize = 9000000;
    printf("Amount of stack  needed per thread = %li\n",stacksize);
    pthread_attr_setstacksize(&attr, stacksize);

    printf("Creating thread with stack size = %li bytes\n",stacksize);

    myId = pthread_create(&myThread,&attr, dowork,(void *)t);
    if(myId){
        printf("ERROR; return code from pthread_create() is %lud \n",t);
        exit(-1);
    }

    pthread_exit(NULL);
    #endif

    #ifdef SUBJECT6
    pthread_t t1, t2;
    
    /* init yerine  ptherad_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER kullanılabilir*/

    if(pthread_mutex_init(&lock,NULL) != 0)
    {
        printf("Mutex initialization error.\n");
        return -1;
    }

    j = 0;

    pthread_create(&t1, NULL, doIncrease, NULL);
    pthread_create(&t2, NULL, doIncrease, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    #endif

    #ifdef SUBJECT7
    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, &functionCount1, NULL);
    pthread_create(&thread2, NULL, &functionCount2, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    exit(0);
    #endif

    return 0;
}


