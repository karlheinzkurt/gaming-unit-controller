
#include "../include/System.h"

//#ifdef Linux
#include "gsclib/infrastructure/linux/include/CSystem.h"
namespace
{
   static const std::unique_ptr<Infrastructure::API::ISystem> instance([]
   {
      return std::make_unique<Infrastructure::Linux::CSystem>();
   }());
}
//#elseif Windows
namespace
{
   ///< \todo Implement me
}
//#endif

namespace Infrastructure
{
namespace Common
{

   Infrastructure::API::ISystem& System::get() { return *instance; }
   
}}
