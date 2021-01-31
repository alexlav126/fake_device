#ifndef __MEAS_CHANNEL_H_
#define __MEAS_CHANNEL_H_

#include <thread>

class MeasChannel
{
public:
    enum class Status
    {
        Error,
        Idle,
        Measure,
        Busy
    };
    MeasChannel(int id);
    ~MeasChannel();
    bool StartMeasure();
    bool StopMeasure();
    bool SetRange(int input_range, int *output_range);
    Status GetStatus() const;
    bool GetResult(float *result) const;

private:
    static const int NUM_RANGES = 4;
    int id;
    Status status;
    int range; // 0 .. NUM_RANGES-1
    float result;

    float fake_signal;
    int noise_percent;

    std::thread *thread_handler;
    bool thread_handler_enable;

    void Handler();
    float Measure();
    const char *StatusToStr(Status status);
};

#endif // __MEAS_CHANNEL_H_