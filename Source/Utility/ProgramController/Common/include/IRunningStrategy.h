#pragma once

namespace Utility
{
namespace ProgramController
{
namespace API
{
   struct IRunningStrategy
   {
      virtual ~IRunningStrategy() = default;
      
      virtual void run() = 0;
   };   
}}}
