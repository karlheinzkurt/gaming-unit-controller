
#include "../include/IProcess.h"

#include <iostream>

namespace Lib
{
namespace Infrastructure
{
namespace API
{
   
   IProcess::~IProcess() {}   
   
   IProcessSet::~IProcessSet() {}
   
   std::ostream& operator<<( std::ostream& os, IProcess const& process )
   {  return ( os << process.toString() ); }
   
   bool operator==( IProcess const& a, IProcess const& b )
   {  return a.getProcessId() == b.getProcessId(); }
   
   bool operator<( IProcess const& a, IProcess const& b )
   {  return a.getProcessId() < b.getProcessId(); }

}}}
