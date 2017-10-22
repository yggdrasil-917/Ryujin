#ifndef THREAD_HPP
#define THREAD_HPP


#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <condition_variable>
#include <chrono>

#include <vector>
#include <queue>
#include <assert.h>

#include "../OS/Platform.hpp"
#include "../CoreUtils/Common.hpp"


namespace Ryujin
{
    
	namespace Threading
	{
#       if _MSC_VER
#           define thread_local __declspec( thread )
#       elif PLATFORM_MAC
#           define thread_local __thread
#       endif

#       define THREAD_LOCAL_VAR(type, name) thread_local type name

		static int32 NumConcurrentThreadsSupported()
		{
			// 0 could still be returned
			return std::thread::hardware_concurrency();
		}

		static PtrSize GetThreadIDHash()
		{
			static std::hash<std::thread::id> hasher;
			return hasher(std::this_thread::get_id());
		}

		using ThreadId = uint64;
#       if PLATFORM_MAC
        static ThreadId GetThreadID()
        {
            uint64 tid;
            pthread_threadid_np(NULL, &tid);
            return tid;
        }
#		elif PLATFORM_WINDOWS
		static ThreadId GetThreadID()
		{
			return (ThreadId)GetCurrentThreadId();
		}
#       else
		static ThreadId GetThreadID()
		{
			return GetThreadIDHash();
		}
#       endif
        
        extern ThreadId mainThreadId;

		static void Sleep(int32 ms)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		}

		static void YieldThread()
		{
			std::this_thread::yield();
		}
        
        
        enum class MemoryOrder : uint8
        {
            MO_Default = std::memory_order_seq_cst,
            MO_Relaxed = std::memory_order_relaxed,
            MO_Consume = std::memory_order_consume,
            MO_Acquire = std::memory_order_acquire,
            MO_Release = std::memory_order_release,
            MO_AcqRel  = std::memory_order_acq_rel
        };
	}

	class Spinlock
	{
	private:
		std::atomic_flag _lock;

	public:
		Spinlock()
		{
			_lock.clear();
		}

		void Lock()
		{
			while (_lock.test_and_set(std::memory_order_acquire))
			{
                Threading::YieldThread();
            }
		}

		void Unlock()
		{
			_lock.clear(std::memory_order_release);
		}
	};

	template <typename T>
    class AtomicVar
	{
	private:
		std::atomic<T> value;

	public:
		AtomicVar() {}
		AtomicVar(T inValue) : value(inValue) {}
        
        AtomicVar(const AtomicVar<T>& other)
        : value(other.GetValue())
        {
            
        }
        
        AtomicVar& operator=(const AtomicVar<T>& other)
        {
            value = other.GetValue();
            return *this;
        }
        
        AtomicVar(AtomicVar&& other)
        : value(Move(other.value))
        {
            
        }
        
        AtomicVar& operator=(AtomicVar&& other)
        {
            value = Move(other.value);
            return *this;
        }

        FORCEINLINE T GetValue(Threading::MemoryOrder memoryOrder = Threading::MemoryOrder::MO_Default) const
        {
            return value.load((std::memory_order)memoryOrder);
        }
        
        FORCEINLINE void Store(T newValue, Threading::MemoryOrder memoryOrder = Threading::MemoryOrder::MO_Default)
        {
            value.store(newValue, (std::memory_order)memoryOrder);
        }

		FORCEINLINE T Increment(T incrementValue) { return value.fetch_add(incrementValue); }

		FORCEINLINE T Decrement(T decrementValue) { return value.fetch_sub(decrementValue); }

		FORCEINLINE T And(T inValue) { return value.fetch_and(inValue); }

		FORCEINLINE T Or(T inValue) { return value.fetch_or(inValue); }

		FORCEINLINE T Xor(T inValue) { return value.fetch_xor(inValue); }

		FORCEINLINE T operator=(const T& val) { return value = val; }

		FORCEINLINE T operator++() { return ++value; }

		FORCEINLINE T operator--() { return --value; }

		FORCEINLINE bool operator>(const T& right) const { return value > right; }

		FORCEINLINE bool operator<(const T& right) const { return value < right; }

		FORCEINLINE bool operator>=(const T& right) const { return value >= right; }

		FORCEINLINE bool operator<=(const T& right) const { return value <= right; }

		FORCEINLINE bool operator==(const T& right) const { return value == right; }

		FORCEINLINE bool operator!=(const T& right) const { return value != right; }
	};

	struct Mutex 
	{
		std::mutex mtx;

		FORCEINLINE void Lock() { mtx.lock(); }
		FORCEINLINE void Unlock() { mtx.unlock(); }
	};

	class ScopedLock
	{
    private:
		std::unique_lock<std::mutex> lock;

    public:
		ScopedLock(Mutex& m) : lock(m.mtx)
		{

		}

		~ScopedLock()
		{
			// unique_lock's dtor calls unlock
		}

		FORCEINLINE const std::unique_lock<std::mutex>& GetUniqueLock() const { return lock; }
        friend class ConditionVariable;
    };
    
    class ConditionalScopedLock
    {
    private:
        std::unique_lock<std::mutex> lock;
        
    public:
        ConditionalScopedLock(Mutex& m, bool bShouldLock)
        {
            if (bShouldLock)
            {
                lock = std::unique_lock<std::mutex>(m.mtx);
            }
        }
        
        ~ConditionalScopedLock()
        {
            // unique_lock's dtor calls unlock
        }
        
        FORCEINLINE const std::unique_lock<std::mutex>& GetUniqueLock() const { return lock; }
    };

	class ConditionVariable
	{
	private:
		std::condition_variable cv;

	public:
		FORCEINLINE void Wait(ScopedLock& scopedLock)
		{
			cv.wait(scopedLock.lock);
		}

		// Predicate must return false to continue the waiting, has to be a function with no arguments
		// This is equivalent to while(!pred()) cv.wait()
		template <class Predicate>
		FORCEINLINE void Wait(ScopedLock& scopedLock, Predicate pred)
		{
			cv.wait(scopedLock.lock, pred);
		}

		FORCEINLINE void WaitFor(ScopedLock& scopedLock, int32 timeoutInMilliseconds)
		{
			while ( cv.wait_for(scopedLock.lock, std::chrono::milliseconds(timeoutInMilliseconds)) == std::cv_status::timeout )
			{
				// the function will return if it reached its time limit
			}
		}

		// Predicate must return false to continue the waiting, has to be a function with no arguments
		// This is equivalent to while(!pred()) cv.wait()
		template <class Predicate>
		FORCEINLINE bool WaitForOrUntilNotified(ScopedLock& scopedLock, int32 timeoutInMilliseconds, Predicate pred)
		{
#           if PLATFORM_WINDOWS
			while (!pred())
				if (cv.wait_until(scopedLock.lock, std::chrono::milliseconds(timeoutInMilliseconds)) == std::cv_status::timeout)
					return pred();
			return true;
#           elif PLATFORM_MAC
            while(!pred())
            {
                Threading::YieldThread();
            }
            return true;
#           else
            return false;
#           endif
		}

		FORCEINLINE void UnblockAllThreads()
		{
			// unblock all threads waiting for this condition
			cv.notify_all();
		}

		FORCEINLINE void UnblockOneThread()
		{
			// no way to determine which thread was unblock
			cv.notify_one();
		}
	};

	// Barrier that blocks a count number of threads before giving them a "go" command
	class ThreadBarrier
	{
	private:
		Mutex m;
		ConditionVariable conditionVariable;
		uint32 count;

	public:
		explicit ThreadBarrier(const uint32 inCount) : count(inCount) {}

		void Wait()
		{
			ScopedLock lock(m);
			if (--count == 0)
			{
				conditionVariable.UnblockAllThreads();
			}
			else
			{
				conditionVariable.Wait(lock, [this]() { return count == 0; });
			}
		}
	};




	template<class Ret>
	class Future : public OnlyRValueCopyable
	{
	private:
		std::future<Ret> _future;

	public:
		Future() {}

		explicit Future(std::future<Ret>&& _fut) : _future(Move(_fut))
		{
		}

		Future(Future&& other)
			: _future(Move(other._future))
		{

		}

		Future& operator=(Future&&  right)
		{
			_future = Move(right._future);
			return *this;
		}

		Future& operator=(std::future<Ret>&& right)
		{
			_future = Move(right);
			return *this;
		}

		FORCEINLINE bool IsValid() const
		{
			return _future.valid();
		}

		FORCEINLINE bool IsReady()
		{
			try
			{
				bool retVal = (_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready);
				return retVal;
			}
			catch (std::future_error& e)
			{
				const std::error_code code = e.code();
				char *errString = (char*)e.what();
//				GConsoleLogger->LogErr("Future Error: ", errString);
			}
			return false;
		}

		FORCEINLINE void BlockUntilDone()
		{
			if (_future.valid())
				_future.wait();
		}

		FORCEINLINE Ret BlockUntilThreadIsDoneAndGetResult()
		{
			return _future.get();
		}
	};

	enum class TaskPriority : uint8
	{
		High	= 0,
		Normal	= 1,
		Low		= 2,
	};

	struct Task
	{
		TaskPriority priority;
		std::function<void()> fun;

		Task() {}

		Task(TaskPriority&& inPriority, std::function<void()>&& inFun)
			: priority(inPriority), fun(inFun) {}

		Task(const Task&& task)
			: priority(task.priority), fun(task.fun) {}

		Task& operator=(const Task&& right)
		{
			priority = right.priority;
			fun = right.fun;
			return *this;
		}

		void operator()()
		{
			fun();
		}
	};

	struct LessThanPriority
	{
		bool operator()(const Task& lhs, const Task& rhs) const
		{
			return lhs.priority < rhs.priority;
		}
	};
	struct GreaterThanPriority
	{
		bool operator()(const Task& lhs, const Task& rhs) const
		{
			return lhs.priority > rhs.priority;
		}
	};
    
    class Thread : public OnlyRValueCopyable
    {
    private:
        std::thread _thread;
        
    public:
        Thread()
        {}
        
        ~Thread()
        {
            if (_thread.joinable())
            {
                Finish();
            }
        }
        
        template<class Fn, class...Args>
        Thread(Fn&& fn, Args&&...args)
        :
        _thread(fn, args...)
        {
            
        }
        
        Thread(Thread&& t)
        :
        _thread(Move(t._thread))
        {
            
        }
        
        Thread& operator=(Thread&& other)
        {
            _thread = Move(other._thread);
            
            return *this;
        }
        
        void Finish()
        {
            if (_thread.joinable())
            {
                _thread.join();
            }
        }
        
        void Detach()
        {
            _thread.detach();
        }
        
        FORCEINLINE std::thread::id GetId() const
        {
            return _thread.get_id();
        }
    };

	class ThreadPool
	{
	private:
		std::vector<std::thread> threadPool;

		std::priority_queue<Task, std::vector<Task>, GreaterThanPriority> tasks;

	public:
		ThreadPool(const uint32 numThreads)
		{
			bStop = false;
			for (uint32 i = 0; i < numThreads; ++i)
			{
				threadPool.emplace_back(
					[this]
					{
						while (true)
						{
							Task task;
							{
								ScopedLock scopedLock(this->queueMutex);

								this->cv.Wait(scopedLock, [this] { return this->bStop.GetValue() || !this->tasks.empty(); });

								if (this->bStop.GetValue() || this->tasks.empty())
									return;

								task = Move(this->tasks.top());
								this->tasks.pop();
							}

							task();
						}
					}
					);
			}
		}

		~ThreadPool()
		{
			bStop = true;
			cv.UnblockAllThreads();

			for (uint32 i = 0; i < threadPool.size(); ++i)
				threadPool[i].join();

			threadPool.clear();
		}

		Mutex queueMutex;
		ConditionVariable cv;
		AtomicVar<bool> bStop;

		// Accepts static functions only
		template<class T, class F, class... Args>
		Future<T> AddTask(TaskPriority priority, F&& f, Args&&... args)
		{
			using return_type = T;

			auto task = std::make_shared< std::packaged_task<return_type()> >(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);

			Future<return_type> res(Move(task->get_future()));
			{
				ScopedLock scopeLock(queueMutex);

				// don't allow enqueueing after stopping the pool
				if (bStop.GetValue())
					throw std::runtime_error("enqueue on stopped ThreadPool");

				tasks.emplace(Move(priority), [task](){ (*task)(); });
			}
			cv.UnblockOneThread();

			return res;
		}

		// Member functions only
		template<class T, class F, class Obj, class... Args>
		Future<T> AddTask(TaskPriority priority, F&& f, Obj&& obj, Args&&... args)
		{
			using return_type = T;

			auto task = std::make_shared< std::packaged_task<return_type()> >(
				std::bind(std::forward<F>(f), std::forward<Obj>(obj), std::forward<Args>(args)...)
				);

			Future<return_type> res(Move(task->get_future()));
			{
				ScopedLock scopeLock(queueMutex);

				// don't allow enqueueing after stopping the pool
				if (bStop.GetValue())
					throw std::runtime_error("enqueue on stopped ThreadPool");

				tasks.emplace(Move(priority), [task](){ (*task)(); });
			}
			cv.UnblockOneThread();

			return res;
		}
	};
}

#endif
