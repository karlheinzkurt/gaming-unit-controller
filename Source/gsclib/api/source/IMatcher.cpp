
#include "../include/IMatcher.h"

#include <ostream>

namespace GSC
{
namespace API
{

   std::ostream& operator<<( std::ostream& os, IMatch const& match )
   {  return os << match.toString(); }
   
   bool operator<( IMatch const& a, IMatch const& b )
   {  return a.getName() < b.getName(); }
   
   bool operator<( IRatedMatch const& a, IRatedMatch const& b )
   {  return (IMatch const&)a < (IMatch const&)b; }
      
}}
