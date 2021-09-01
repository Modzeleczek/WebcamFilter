#ifndef ConcurrentPipeline_HPP
#define ConcurrentPipeline_HPP

#include "Pipeline.hpp"

#include <pthread.h>

class ConcurrentPipeline : public Pipeline
{
private:
    pthread_mutex_t Mutex1, Mutex2;
    pthread_cond_t Cond1, Cond2;
    bool FrameReadyForOOPP, FrameReadyForTarget;

public:
    ConcurrentPipeline(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp, ITarget &target);
    ConcurrentPipeline(const ConcurrentPipeline &) = delete;
    virtual ~ConcurrentPipeline();

    virtual void Start() override;
private:
    static void* Download(void* caller); // uruchamiamy w innym wątku
    void Process(); // uruchamiamy w głównym wątku
    static void* Upload(void* caller); // uruchamiamy w innym wątku
};

#endif // ConcurrentPipeline_HPP
