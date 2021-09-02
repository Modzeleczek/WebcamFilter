#ifndef ConcurrentPipeline_HPP
#define ConcurrentPipeline_HPP

#include "ConcurrentSink.hpp"

#include <pthread.h>

class ConcurrentPipeline : public ConcurrentSink
{
private:
    pthread_mutex_t Mutex2;
    pthread_cond_t Cond2;
    bool FrameReadyForTarget;
    ITarget &Target;

public:
    ConcurrentPipeline(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target);
    ConcurrentPipeline(const ConcurrentPipeline &) = delete;
    virtual ~ConcurrentPipeline();

    virtual void Start() override;
private:
    void Process(); // uruchamiamy w głównym wątku
    static void* Upload(void* caller); // uruchamiamy w innym wątku
};

#endif // ConcurrentPipeline_HPP
