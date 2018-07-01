#pragma once

#include "IRunningStrategy.h"

#include <chrono>

namespace Utility
{
namespace ProgramController
{
   struct CDaemonRunningStrategy : API::IRunningStrategy
   {
      CDaemonRunningStrategy(std::chrono::seconds dutyCycle);
      
      virtual void run() override;
      
   private:
      std::chrono::seconds m_dutyCycle;
   };
   
   struct CStraightRunningStrategy : API::IRunningStrategy
   {
      virtual void run() override;
   };
}}
