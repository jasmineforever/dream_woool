#pragma once

namespace DW
{
	struct SchedulingObject
	{
		std::function<void(float)> func;
		float next_call_time;
		float call_time;
		float interval;
		int loop_count;
	};

	class Scheduler
	{
	public:
		Scheduler();

		~Scheduler();

		void Update(float dt);

		uint32_t Schedule(std::function<void(float)> func,
			float interval_in_seconds,
			unsigned int repeat,
			float delay_in_seconds);

		void UnSchedule(uint32_t schedule_id);

		void RunFunctionInMainLoop(std::function<void()> func);

	private:
		uint32_t GenerateScheduleID();
	private:
		float timeline_;
		std::mutex									func_run_mutex_;
		std::unordered_map<int, SchedulingObject>	schedule_objs_;
		std::vector< std::function<void()>>			functions_;
	};
}