#pragma once

namespace GSC
{
namespace Common
{
namespace API
{
   struct IRunningStrategy
   {
      virtual ~IRunningStrategy() = default;
      
      virtual void run() = 0;
   };   
}}}
