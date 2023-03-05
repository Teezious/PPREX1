#include <iostream>
#include <string>
#include <stdexcept>
#include <omp.h>
#include <vector>
#include <mutex>
#include <stdlib.h>
#include <time.h>
#include <chrono> 
#include <thread>
#include <atomic>
#include <signal.h>

std::atomic<bool> running(true);

void signal_handler(int sig) {
    if (sig == 1 || sig == 2 || sig == 3) {
        running = false;
    }
    exit(1);
}

int main(void)
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    int philosophers = 0;
    int thinkingTime = 0;
    int eatingTime = 0;

    std::cout<< "Enter the number of philosophers: " << std::endl;
    std::cin >> philosophers;

    
    std::cout<<"Enter thinking Time: " << std::endl;
    std::cin >> thinkingTime;

    
    std::cout<<"Enter eating Time: " << std::endl;
    std::cin >> eatingTime;

    omp_set_num_threads(philosophers);

    std::vector<std::mutex> forks;
    std::vector<std::mutex> list(philosophers);
    forks.swap(list);

    #pragma omp parallel
    {
        while(running.load())
        {
            srand (time(0));
            int id = omp_get_thread_num();
            std::cout << id;
            int leftForkIndex = (id + 1) % philosophers;
            int rightForkIndex = id;

            int _thinkTime = rand() % thinkingTime;
            std::this_thread::sleep_for(std::chrono::milliseconds(_thinkTime));
            std::cout<< "Philosopher " << id << " finished thinking!" << std::endl;

            while(!forks[leftForkIndex].try_lock())
            {
                // waiting for fork                
            }
            std::cout<< "Philosopher " << id << " took first fork " <<  leftForkIndex  << std::endl;

            while(!forks[rightForkIndex].try_lock())
            {
                // waiting for fork
            }
            std::cout<< "Philosopher " << id << " took second fork " <<  rightForkIndex  << std::endl;

            int _eatTime = rand() % eatingTime;
            std::this_thread::sleep_for(std::chrono::milliseconds(_eatTime));
            std::cout<< "Philosopher " << id << " is done eating!" << std::endl;


            if(rand() % 2 == 0)
            {
                forks[leftForkIndex].unlock();
                forks[rightForkIndex].unlock();
            }
            else
            {
                forks[rightForkIndex].unlock();
                forks[leftForkIndex].unlock();
            }
        } 
    }
    std::cout<< "done";
    
}