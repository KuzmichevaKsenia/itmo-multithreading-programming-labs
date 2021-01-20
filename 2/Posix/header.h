#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define ERROR_MAIN_ARGS     -13

typedef struct consumer_args_tag {
    unsigned short int ms;
    bool debug;
    int psum;
    int tid;
    int *number_ptr;
    bool *completed;
    bool *consumer_started;
    bool *new_number;
    pthread_mutex_t *number_mutex;
    pthread_cond_t *producer_cond;
    pthread_cond_t *consumer_cond;
} consumer_args;

typedef struct producer_args_tag {
    int *number_ptr;
    int n;
    bool *completed;
    bool *consumer_started;
    bool *new_number;
    pthread_mutex_t *number_mutex;
    pthread_cond_t *producer_cond;
    pthread_cond_t *consumer_cond;
} producer_args;

typedef struct interrupter_args_tag {
    pthread_t *consumers;
    int n;
    bool *completed;
    bool *consumer_started;
    pthread_mutex_t *number_mutex;
    pthread_cond_t *consumer_cond;
} interrupter_args;

void* producer_routine(void* arg);
void* consumer_routine(void* arg);
void* consumer_interrupter_routine(void* arg);
int run_threads(unsigned short int N, unsigned short int ms, bool debug);
int get_tid(int id=0);