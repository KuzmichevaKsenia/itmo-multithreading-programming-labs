#include <pthread.h>
#include <dlfcn.h>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
using namespace std;

static int (*orig_pthread_mutex_lock)(pthread_mutex_t *__mutex) = NULL;
static int (*orig_pthread_mutex_unlock)(pthread_mutex_t *__mutex) = NULL;

map <unsigned long int, vector<pthread_mutex_t *>> cur_locks;
multimap <pthread_mutex_t *, pthread_mutex_t *> edges;
vector <pthread_mutex_t *> vertexes;

pthread_mutex_t san_mutex = PTHREAD_MUTEX_INITIALIZER;

void check_cycles() {
    multimap <pthread_mutex_t *, pthread_mutex_t *> cp_edges(edges);
    bool updated;
    do {
        updated = false;
        for(const auto& value: vertexes) {
            bool in_first = false;
            bool in_second = false;
            for(const auto &x: cp_edges) {
                if(x.first == value) {
                    in_first = true;
                }
                if(x.second == value) {
                    in_second = true;
                }
            }
            if(in_first xor in_second) {
                updated = true;
                for(auto pos = cp_edges.begin(); pos != cp_edges.end();) {
                    if(pos->first == value || pos->second == value) {
                        pos = cp_edges.erase(pos);
                    } else {
                        ++pos;
                    }
                }
                break;
            }
        }
    } while(updated);
    if(cp_edges.size() > 0) {
        cout << "potential deadlock found" << endl;
        for(const auto &x: cp_edges) {
            cout << x.first << " > " << x.second << endl;
        }
        exit(EXIT_FAILURE);
    }
}

int pthread_mutex_lock(pthread_mutex_t *__mutex) {
    if (orig_pthread_mutex_lock == NULL) {
        orig_pthread_mutex_lock = (int (*)(pthread_mutex_t *)) dlsym(RTLD_NEXT, "pthread_mutex_lock");
    }
    if (orig_pthread_mutex_unlock == NULL) {
        orig_pthread_mutex_unlock = (int (*)(pthread_mutex_t *)) dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    }

    orig_pthread_mutex_lock(&san_mutex);
    if (find(vertexes.begin(), vertexes.end(), __mutex) == vertexes.end()) {
        vertexes.push_back(__mutex);
    }

    if(cur_locks.count(pthread_self()) > 0) {
        if (cur_locks[pthread_self()].size() > 0) {
            edges.insert(pair <pthread_mutex_t *, pthread_mutex_t *> (cur_locks[pthread_self()].back(), __mutex));
        }
        cur_locks[pthread_self()].push_back(__mutex);
    } else {
        vector <pthread_mutex_t*> mutexes;
        mutexes.push_back(__mutex);
        cur_locks.insert(pair <unsigned long int, vector <pthread_mutex_t*>> (pthread_self(), mutexes));
    }

    check_cycles();
    orig_pthread_mutex_unlock(&san_mutex);

    return orig_pthread_mutex_lock(__mutex);
}

int pthread_mutex_unlock(pthread_mutex_t *__mutex) {
    if (orig_pthread_mutex_lock == NULL) {
        orig_pthread_mutex_lock = (int (*)(pthread_mutex_t *)) dlsym(RTLD_NEXT, "pthread_mutex_lock");
    }
    if (orig_pthread_mutex_unlock == NULL) {
        orig_pthread_mutex_unlock = (int (*)(pthread_mutex_t *)) dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    }

    orig_pthread_mutex_lock(&san_mutex);
    cur_locks[pthread_self()].erase(
            remove(cur_locks[pthread_self()].begin(),
                   cur_locks[pthread_self()].end(),
                   __mutex
                   ),
            cur_locks[pthread_self()].end()
    );
    orig_pthread_mutex_unlock(&san_mutex);

    return orig_pthread_mutex_unlock(__mutex);
}
