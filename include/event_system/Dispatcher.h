#pragma once

#include <condition_variable>
#include <mutex>
#include <deque>
#include <map>
#include <memory>

#include <thread>
#include<list>

//Begin Dispatcher Class Section

using EventType = std::string;

class Subscriber;

class Dispatcher {

    private:
        Dispatcher();
        void Initialize();

        static bool initialized;
        static bool running;

        static Dispatcher* instance;

        std::deque<std::pair<EventType,std::shared_ptr<void>>>*  dispatch_events;
        std::map<EventType,std::list<Subscriber*>*>* mapped_events;

        static std::deque<std::pair<Subscriber*, std::shared_ptr<void>>>*  thread_queue;
        static std::deque<std::pair<Subscriber*, std::shared_ptr<void>>>*  nonserial_queue;

        std::deque<std::thread*>* processing_threads; //using std::deque for constant time size() and O(1) random access

        static std::mutex dispatch_queue_mutex;
        static std::mutex thread_queue_mutex;
        static std::mutex mapped_event_mutex;
        static std::condition_variable thread_signal;


    public:
        static Dispatcher* GetInstance();

        ~Dispatcher();

        Dispatcher(const Dispatcher&); //disallow copying
        Dispatcher& operator= (const Dispatcher&); //disallow copying

        void Terminate();

        void AddEventSubscriber(Subscriber *requestor, const EventType);
        std::list<Subscriber*> GetAllSubscribers(const void* owner);

        void DispatchEvent(const EventType eventID, const std::shared_ptr<void> eventData);
        void DispatchImmediate(const EventType eventID, const std::shared_ptr<void> eventData);

        void Pump();
        void NonSerialProcess();

        //Not entirely threadsafe but as long as the compiler respects the implicit inline then it will work fine
        int QueueSize() { return static_cast<int>(thread_queue->size()); }

    private:
        static void ThreadProcess();
};