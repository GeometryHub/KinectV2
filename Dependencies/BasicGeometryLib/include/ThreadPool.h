/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include <vector>
#include <list>
#ifdef USECPP11
#include <thread>
#include <mutex> 
#include <condition_variable>
#endif

namespace GPP
{
    extern BGL_EXPORT Int GetProcessorCount();

#ifdef USECPP11
    typedef std::mutex Mutex;
#else
    class MutexImpl;
    class BGL_EXPORT Mutex
    {
    public:
        Mutex();
        void Lock();
        void Unlock();
        void* GetHandle();
        ~Mutex();

    private:
        MutexImpl* mpImpl;
    };
#endif

    class BGL_EXPORT ScopedLock
    {
    public:
        ScopedLock(Mutex& mutex);
        ~ScopedLock();

    private:
        Mutex& mMutex;
    };

#ifndef USECPP11
    class ConditionVariableImpl;
    class ConditionVariable
    {
    public:
        ConditionVariable();
        Int Sleep(Mutex& mutex);
        Int WakeSingle();
        Int WakeAll();
        ~ ConditionVariable();

    private:
        ConditionVariableImpl* mpImpl;
    };
#endif

    enum BGL_EXPORT TaskType
    {
        TP_Exit = 0,
        TP_Normal
    };

    class BGL_EXPORT TaskBase
    {
    public:
        TaskBase(TaskType tp = TP_Normal);
        virtual ErrorCode Run(void);
        virtual void OnComplete(void);
        TaskType GetType() const;
        virtual ~TaskBase();

    private:
        TaskType mTP;
    };

    class ThreadPool;
    class BGL_EXPORT Thread
    {
    public:
        Thread(ThreadPool* threadPool);
        ErrorCode Start();
        ErrorCode Run();
        ~Thread();

    private:
        ThreadPool* mpThreadPool;
    };

    class BGL_EXPORT ThreadPool
    {
    public:
        ThreadPool();
        Int Init(Int threadCount);
        ErrorCode InsertTask(TaskBase* pTask);
        void WaitUntilAllDone();

        TaskBase* GetTask(void);
        void FinishATask(void);
        ~ThreadPool();

    private:
        std::vector<Thread*> mThreadList;
        std::list<TaskBase*> mTaskList;
        Int mTaskLeftCount;
#ifdef USECPP11
        std::mutex mMutex;
        std::condition_variable mGetTaskCV;
        std::condition_variable mFinishTaskCV;
#else
        Mutex mMutex;
        ConditionVariable mGetTaskCV;
        ConditionVariable mFinishTaskCV;
#endif
        
    };
}
