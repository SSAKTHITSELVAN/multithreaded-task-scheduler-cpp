#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include<mutex>

using namespace std;


class Task
{
protected:
int task_id;
string task_title;
int task_duration;

public:

Task(int id, string title, int duration)
    {
        task_id = id;
        task_title = title;
        task_duration = duration;
    }
    
    virtual void execute() = 0;
};

class Download : public Task
{
    public:
    
    Download(int id, string title, int duration)
    : Task(id, title, duration)
    {
    }
    
    void execute() override
    {
        cout << "Downloading: "
        << task_title
        << endl;
        
        this_thread::sleep_for(
            chrono::seconds(task_duration)
        );
        
        cout << "Download "<<task_title<<" -- Finished\n";
    }
};


class Email: public Task {
    public:
    Email(int id, string title, int duration): Task(id, title, duration){
        
    }
    
    void execute() override
    {
        cout<<"Sending Email: "<<task_title<<endl;
        this_thread::sleep_for(chrono::seconds(task_duration));
        cout << "Email Sent\n";
    }
};

class Compression : public Task
{
    public:
    
    Compression(int id, string title, int duration)
    : Task(id, title, duration)
    {
    }
    
    void execute() override
    {
        cout << "Compressing File: "
        << task_title
        << endl;
        
        this_thread::sleep_for(
            chrono::seconds(task_duration)
        );
        
        cout << "Compression Finished\n";
    }
};

queue<Task*> taskQueue;
mutex queueMutex;

void worker(int id)
{
    while (true)
    {
        queueMutex.lock();
        
        if(taskQueue.empty())
        {
            queueMutex.unlock();
            break;
        }
        
        Task* currentTask = taskQueue.front();
        
        taskQueue.pop();
        
        queueMutex.unlock();
        
        cout << "Worker "
             << id
             << " processing task\n";
             
             currentTask->execute();
            }
        }
        

int main(){
    cout<<"\nstarted\n";
    Task* t1 = new Download(1, "downloading song - 1", 5);
    Task* t2 = new Email(2, "emailing song", 15);
    Task* t3 = new Download(1, "downloading song - 2", 12);

    taskQueue.push(t1);
    taskQueue.push(t2);
    taskQueue.push(t3);
    thread w1(worker, 1);
    thread w2(worker, 2);
    thread w3(worker, 3);
    w1.join();
    w2.join();
    w3.join();

    return 0;
}