
#include "../include/CRunningStrategy.h"

namespace GSC
{
namespace Common
{
   CDaemonRunningStrategy::CDaemonRunningStrategy(std::chrono::seconds dutyCycle) 
      :m_dutyCycle(dutyCycle)
      ,m_mutex()
      ,m_condition()
      ,m_terminate(false)
   {}
   
   void CDaemonRunningStrategy::run(std::function<void()> f)
   {
      f();
      while (1)
      {
         {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condition.wait_for(lock, m_dutyCycle);
            if (m_terminate == true)
            {  break; }
         }
         f();
         if (m_terminate == true)
         {  break; }
      }      
   }
   
   void CDaemonRunningStrategy::cancel()
   {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_terminate = true;
      m_condition.notify_all();
   }
   
   void CStraightRunningStrategy::run(std::function<void()> f)
   {  f(); }
   
   void CStraightRunningStrategy::cancel() {};
}}

