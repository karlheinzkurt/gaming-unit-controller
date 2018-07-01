#include "../include/CRunningStrategy.h"

namespace GSC
{
namespace Common
{
   CDaemonRunningStrategy::CDaemonRunningStrategy(std::chrono::seconds dutyCycle) : m_dutyCycle(dutyCycle)
   {}
   
   void CDaemonRunningStrategy::run()
   {}
   
   void CStraightRunningStrategy::run()
   {}
}}

