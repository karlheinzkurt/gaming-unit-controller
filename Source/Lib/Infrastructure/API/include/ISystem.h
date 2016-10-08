#pragma once

namespace Lib
{
namespace Infrastructure
{
namespace API
{
   struct ISystem
   {
      virtual ~ISystem() = default;
      
      virtual IProcess::SetType getRunningProcesses() = 0;
      
      virtual IProcess::SetType getSignallableProcesses() = 0; 
   };
   
}}}
