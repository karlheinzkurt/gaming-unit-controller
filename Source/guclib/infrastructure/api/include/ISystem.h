#pragma once

#include "guclib/infrastructure/api/include/IProcess.h"
#include "guclib/infrastructure/api/include/ISignalHandler.h"

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
