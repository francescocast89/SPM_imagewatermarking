#include <iostream>
#include <mutex>
#include <condition_variable>
#include <deque>

template <typename T>
class task_queue{
	private:

	  mutable std::mutex      d_mutex;
	  mutable std::condition_variable d_condition;
	  std::deque<T>           d_queue;

	public:
	  task_queue(){}

	  void insert_task(T value) {
	    {
	      std::unique_lock<std::mutex> lock(this->d_mutex);
	      d_queue.push_front(value);
	    }
	    //std::cout<<"insert_task"<<value<<std::endl;
	    this->d_condition.notify_one();
	  }

	  T get_task() {
	    std::unique_lock<std::mutex> lock(this->d_mutex);
	    this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
	    T rc = this->d_queue.back();
	    this->d_queue.pop_back();
	    //std::cout<<"get_task"<<rc<<std::endl;
	    return rc;
	  }
};
