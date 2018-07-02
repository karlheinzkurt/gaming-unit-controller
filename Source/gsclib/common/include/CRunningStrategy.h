#pragma once

#include "IRunningStrategy.h"

#include <chrono>
#include <mutex>
#include <condition_variable>

namespace GSC
{
namespace Common
{
   struct CDaemonRunningStrategy : API::IRunningStrategy
   {
      CDaemonRunningStrategy(std::chrono::seconds dutyCycle);
      
      virtual void run(std::function<void()>) override;
      virtual void cancel() override;
      
   private:
      std::chrono::seconds m_dutyCycle;
      std::mutex m_mutex;
      std::condition_variable m_condition;
      bool m_terminate;
   };
   
   struct CStraightRunningStrategy : API::IRunningStrategy
   {
      virtual void run(std::function<void()>) override;
      virtual void cancel() override;
   };
}}
