/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <time.h>
#include <unistd.h>
#include <mutex>
#include <pthread.h>
#include <sys/eventfd.h>

class EventFd
{
public:
    EventFd() {
        // ignore error
        exitSignal = eventfd(0, 0);
        commandSignal = eventfd(0, 0);
        int gg = eventfd(0, 0);

        std::cout << "check this out: " << exitSignal << " " << commandSignal << " " << gg << std::endl;
    }

    ~EventFd() {

    }

    void setEvent(int efd)
    {
        uint64_t value;
        write(efd, &value, sizeof(uint64_t));
    }

    void resetEvent(int efd)
    {
        uint64_t value;
        read(efd, &value, sizeof(uint64_t));
    }

private:
    int exitSignal;
    int commandSignal;
    std::mutex mutex;
};

class RobotController
{
public:
    RobotController() {
        std::cout << "threadA is created" << std::endl;
        pthread_create(&threadA, NULL, &RobotController::testFunction, NULL);
    };
    ~RobotController() {
        std::cout << "threadA is cleaned up" << std::endl;
        pthread_cancel(threadA);
        pthread_join(threadA, NULL);
    };

    static void* testFunction(void* arg)
    {
        while (1)
        {
            std::cout << "check this out" << std::endl;
            std::cout << "SIMULATE FOR 5 SECONDS" << std::endl;
            usleep(5000000);
        }

        return nullptr;
    }

    void start() {
        pthread_join(threadA, NULL);
    };

    void stop() {
        std::cout << "threadA is cleaned up" << std::endl;
        pthread_cancel(threadA);
        pthread_join(threadA, NULL);
    }

private:
    pthread_t threadA;
};

RobotController robotController;

void mainThread(RobotController* robotController)
{
    int value;
    std::cout << "Please input the value: " << std::endl;
    std::cin >> value;
    std::cout << std::endl << "[THE VALUE IS]: " << value << std::endl;
    robotController->stop();


}

int main()
{
    // mainThread(&robotController);
    // robotController.start();
    // // mainThread();

    EventFd eventFd;
}