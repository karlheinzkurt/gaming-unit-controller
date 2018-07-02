#pragma once

#include <functional>

namespace GSC
{
namespace Common
{
namespace API
{
   struct IRunningStrategy
   {
      virtual ~IRunningStrategy() = default;
      
      virtual void run(std::function<void()>) = 0;
      virtual void cancel() = 0;
   };   
}}}
