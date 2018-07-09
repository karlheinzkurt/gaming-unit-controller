
#include "../include/ISignalHandler.h"

#include <iostream>
#include <cassert>

namespace Infrastructure
{
namespace API
{
   std::ostream& operator<<(std::ostream& os, Signal const& signal)
   {
      switch(signal)
      {
         case Signal::Hup:  return os << "HUP";
         case Signal::Term: return os << "TERM";
         case Signal::Kill: return os << "KILL";
      }
      assert(false); ///< Extend switch block above
      return os;
   }
   
   std::ostream& operator<<(std::ostream& os, ISignalHandler const& signalHandler)
   {  return os << signalHandler.toString(); }
}}
