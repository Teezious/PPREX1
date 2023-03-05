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
std::vector<std::mutex> forks;

void signal_handler(int sig) {
    if (sig == 1 || sig == 2 || sig == 3) {
        running = false;
    }
}

void think(int time, int id)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    std::cout<< "Philosopher " << id << " finished thinking!" << std::endl;
}

void eat(int time, int id)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    std::cout<< "Philosopher " << id << " is done eating!" << std::endl;
}

int grabForks(int firstFork, int  secondFork, int id)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while(!forks[firstFork].try_lock())
    {
        // waiting for fork                
    }
    std::cout<< "Philosopher " << id << " took first fork " <<  firstFork  << std::endl;

    while(!forks[secondFork].try_lock())
    {
        // waiting for fork
    }
    std::cout<< "Philosopher " << id << " took second fork " <<  secondFork  << std::endl;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
}

void putBackForks(int firstFork, int secondFork)
{
    forks[firstFork].unlock();
    forks[secondFork].unlock();
}


int main(void)
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    int philosophers = 0;
    int thinkingTime = 0;
    int eatingTime = 0;
    int waitingTime = 0;

    std::cout<< "Enter the number of philosophers: " << std::endl;
    std::cin >> philosophers;

    
    std::cout<<"Enter thinking Time: " << std::endl;
    std::cin >> thinkingTime;

    
    std::cout<<"Enter eating Time: " << std::endl;
    std::cin >> eatingTime;

    omp_set_num_threads(philosophers);

    std::vector<std::mutex> list(philosophers);
    forks.swap(list);

    #pragma omp parallel reduction(+:waitingTime)
    {
        while(running.load())
        {
            srand (time(0));
            int id = omp_get_thread_num();
            std::cout << id;
            int leftForkIndex = (id + 1) % philosophers;
            int rightForkIndex = id;

            int myThinkTime = rand() % thinkingTime;
            think(myThinkTime, id);

            if(id % 2 == 1)
            {
                waitingTime += grabForks(leftForkIndex, rightForkIndex, id);
            }
            else
            {
                waitingTime += grabForks(rightForkIndex, leftForkIndex, id);
            }    

            int myEatTime = rand() % eatingTime;
            eat(myEatTime, id);

            if(rand() % 2 == 0)
            {
                putBackForks(leftForkIndex, rightForkIndex);
            }
            else
            {
                putBackForks(rightForkIndex, leftForkIndex);
            }
        } 
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Total runtime = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Total Time spent waiting = " << waitingTime << "[µs]" << std::endl;
    std::cout << "Avg Time spent waiting per philosopher = " << waitingTime / philosophers << "[µs]" << std::endl;
    
}