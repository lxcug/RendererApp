// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_TIMER_H
#define RENDERER_TIMER_H

#include "Core.h"
#include <chrono>


namespace RendererSpace {
    class Timer {
    public:
        Timer() {
            reset();
        }

        void reset() {
            m_startTime = std::chrono::steady_clock::now();
        }

        float elapsed() {
            return (float)(std::chrono::steady_clock::now() - m_startTime).count();
        }

        float elapsedMillis() {
            return elapsed() * 1000.f;
        }

    private:
        std::chrono::steady_clock::time_point m_startTime;
    };
}


#endif //RENDERER_TIMER_H
