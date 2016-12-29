
#include "../include/IMatcher.h"

#include <ostream>

namespace Utility
{
namespace ProgramController
{
namespace API
{

   std::ostream& operator<<( std::ostream& os, IMatch const& match )
   {
      return os << match.toString();
   }
   
   bool operator<( IMatch const& a, IMatch const& b )
   {  
      return a.getName() < b.getName(); 
   }
      
}}}
