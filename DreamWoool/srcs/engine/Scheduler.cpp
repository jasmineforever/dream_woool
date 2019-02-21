#include "pch.h"
#include "Scheduler.h"
namespace DW
{

	Scheduler::Scheduler() :
		timeline_(0.0)
	{

	}

	Scheduler::~Scheduler()
	{
	}

	void Scheduler::Update(float dt)
	{
		timeline_ += dt;

		for (auto it = schedule_objs_.begin(); it != schedule_objs_.end();)
		{
			if (timeline_ > it->second.next_call_time)
			{
				it->second.func(it->second.next_call_time - it->second.call_time);
				it->second.call_time = it->second.next_call_time;
				it->second.loop_count--;
				if (it->second.loop_count <= 0)
				{
					it = schedule_objs_.erase(it);
				}
				else
				{
					it->second.next_call_time = timeline_ + it->second.interval;
					it++;
				}
			}
			else
			{
				it++;
			}
		}
		if (!functions_.empty())
		{
			func_run_mutex_.lock();
			auto temp = std::move(functions_);
			func_run_mutex_.unlock();

			for (const auto &function : temp) 
			{
				function();
			}
		}
	}
	uint32_t Scheduler::GenerateScheduleID()
	{
		static uint32_t id = 0;
		return id++;
	}
	void Scheduler::RunFunctionInMainLoop(std::function<void()> func)
	{
		std::lock_guard<std::mutex> lock(func_run_mutex_);
		functions_.push_back(std::move(func));
	}
	uint32_t Scheduler::Schedule(std::function<void(float)> func,
		float interval_in_seconds,
		unsigned int repeat,
		float delay_in_seconds)
	{
		if (interval_in_seconds > 0.0)
		{
			SchedulingObject obj;
			obj.loop_count = (repeat >= 1) ? repeat : 1;
			obj.func = func;
			obj.interval = interval_in_seconds;
			obj.next_call_time = delay_in_seconds + timeline_;
			obj.call_time = timeline_;
			auto id = GenerateScheduleID();
			schedule_objs_.insert(std::pair<int, SchedulingObject>(id, obj));
			return id;
		}
		return -1;
	}

	void Scheduler::UnSchedule(uint32_t schedule_id)
	{
		auto it = schedule_objs_.find(schedule_id);
		if (it != schedule_objs_.end())
		{
			schedule_objs_.erase(it);
		}
	}
}