#pragma once

#include <iosfwd>
#include <string>

namespace Infrastructure
{
namespace API
{
   enum class Signal
   {
       Hup  ///< Hang-up or re-read config
      ,Term ///< Terminate slightly
      ,Kill ///< Just go away
   };
   
   struct ISignalHandler
   {
      virtual ~ISignalHandler() = default;
           
      virtual std::string toString() const = 0;
   };
  
   std::ostream& operator<<(std::ostream& os, Signal const& signal);
   std::ostream& operator<<(std::ostream& os, ISignalHandler const& signalHandler);
}}
