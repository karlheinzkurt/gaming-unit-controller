#include "../include/CRunningStrategy.h"

namespace Utility
{
namespace ProgramController
{
   CDaemonRunningStrategy::CDaemonRunningStrategy(std::chrono::seconds dutyCycle) : m_dutyCycle(dutyCycle)
   {}
   
   void CDaemonRunningStrategy::run()
   {}
   
   void CStraightRunningStrategy::run()
   {}
}}

