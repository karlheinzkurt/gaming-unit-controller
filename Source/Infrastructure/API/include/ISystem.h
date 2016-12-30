#pragma once

#include "Infrastructure/API/include/IProcess.h"
#include "Infrastructure/API/include/ISignalHandler.h"

#include <memory>
#include <functional>

namespace Infrastructure
{
namespace API
{
   struct ISystem
   {
      virtual ~ISystem() = default;
      
      virtual IProcess::SetType getRunningProcesses() = 0;
      
      virtual IProcess::SetType getSignallableProcesses() = 0;
      
      virtual std::unique_ptr<ISignalHandler> installSignalHandler(Signal signal, std::function<void(Signal)> function) = 0;
   };
   
}}
