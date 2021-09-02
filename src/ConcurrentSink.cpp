#include "../include/ConcurrentSink.hpp"

#include <unistd.h>

ConcurrentSink::ConcurrentSink(OpenGLContext &context, ISource &source, InPlaceProcessor &ipp, OutOfPlaceProcessor &oopp) :
    Runner(context, source, ipp, oopp)
{
    pthread_mutex_init(&Mutex1, NULL);
    pthread_cond_init(&Cond1, NULL);
}

ConcurrentSink::~ConcurrentSink() {}

void ConcurrentSink::Start()
{
    FrameReadyForOOPP = false;
    Running = true;
    pthread_t downloader;
    pthread_create(&downloader, NULL, Download, this);
    Process();
    pthread_join(downloader, NULL);
}

#undef DEBUG
#ifdef DEBUG
    #include <stdio.h>
    #define operationInfo(number, letter) (printf("%i - %c\n", number, letter))
    #define threadEndInfo(name) (printf("%s thread ended\n", name))
#else
    #define operationInfo(number, letter) // no-operation
    #define threadEndInfo(name) // no-operation
#endif

void* ConcurrentSink::Download(void* caller)
{
    ConcurrentSink &mp = *((ConcurrentSink*)caller);
    while (mp.Running == true)
    {
        pthread_mutex_lock(&mp.Mutex1);
        if (mp.FrameReadyForOOPP == true) // bez tego wykonuje się tylko wątek Download, a wątek główny i Upload ulegają zagłodzeniu
            pthread_cond_wait(&mp.Cond1, &mp.Mutex1);
        mp.FrameReadyForOOPP = false;
        operationInfo(1, 's');
        mp.Source.DownloadFrame();
        mp.IPP.ProcessFrame();
        operationInfo(1, 'e');
        mp.FrameReadyForOOPP = true;
        pthread_cond_signal(&mp.Cond1);
        pthread_mutex_unlock(&mp.Mutex1);
        usleep(1000);
    }
    threadEndInfo("Download");
    // ustawiamy flagę i sygnalizujemy zmienną warunkową, aby wątek Process się nie zablokował
    mp.FrameReadyForOOPP = true;
    pthread_cond_signal(&mp.Cond1);
    return NULL;
}

void ConcurrentSink::Process()
{
    while (Running == true)
    {
        pthread_mutex_lock(&Mutex1);
        if (FrameReadyForOOPP == false)
            pthread_cond_wait(&Cond1, &Mutex1);
        operationInfo(2, 's');
        OOPP.UploadFrame();
        operationInfo(2, 'e');
        FrameReadyForOOPP = false;
        pthread_cond_signal(&Cond1); // jak wyżej
        pthread_mutex_unlock(&Mutex1);

        operationInfo(3, 's');
        OOPP.ProcessFrame();
        operationInfo(3, 'e');
    }
    threadEndInfo("Process");
    // ustawiamy flagę i sygnalizujemy zmienną warunkową, aby wątek Download się nie zablokował
    FrameReadyForOOPP = false;
    pthread_cond_signal(&Cond1);
}
