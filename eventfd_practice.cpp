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
#include <poll.h>

class EventFd
{
public:
    EventFd()
    {

    }

    ~EventFd()
    {

    }

    void createEvent(int& efd)
    {
        efd = eventfd(0, 0);
    }

    void setEvent(int& efd)
    {
        uint64_t value;
        write(efd, &value, sizeof(uint64_t));
    }

    void resetEvent(int& efd)
    {
        uint64_t value;
        read(efd, &value, sizeof(uint64_t));
    }

    bool waitEvent(int& efd)
    {
        struct pollfd pfd;
        pfd.fd = efd;
        pfd.events = POLLIN;

        int timeout = 1000;
        int ret = poll(&pfd, 1, timeout);
        if (ret > 0)
        {
            if (pfd.revents & POLLIN)
                return true;
        }
        else if (ret == 0)
        {
            return false;
        }
        else
        {
            std::cerr << "Poll failed" << std::endl;
            return false;
        }
        return false;
    }

private:
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

// RobotController robotController;

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
    EventFd eventFd;
    int exitSignal;
    int commandSignal;

    eventFd.createEvent(exitSignal);
    eventFd.createEvent(commandSignal);
    std::cout << "exit signal is: " << exitSignal << std::endl;
    std::cout << "command signal is: " << commandSignal << std::endl;

    eventFd.setEvent(commandSignal);
    // eventFd.setEvent(exitSignal);
    bool isPoll1 = eventFd.waitEvent(commandSignal);  // should be true
    std::cout << "poll1 value is: " << isPoll1 << std::endl;

    eventFd.resetEvent(commandSignal);
    bool isPoll2 = eventFd.waitEvent(commandSignal);  // should be false
    std::cout << "poll2 value is: " << isPoll2 << std::endl;

    return 0;


}