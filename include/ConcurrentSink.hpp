#ifndef ConcurrentSink_HPP
#define ConcurrentSink_HPP

#include "Runner.hpp"

#include <pthread.h>

class ConcurrentSink : public Runner
{
protected:
    pthread_mutex_t Mutex1;
    pthread_cond_t Cond1;
    bool FrameReadyForOOPP;

public:
    ConcurrentSink(ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target);
    ConcurrentSink(const ConcurrentSink &) = delete;
    virtual ~ConcurrentSink();

    virtual void Start() override;
protected:
    static void* Download(void* caller); // uruchamiamy w innym wątku
private:
    void Process(); // uruchamiamy w głównym wątku
};

#endif // ConcurrentSink_HPP

