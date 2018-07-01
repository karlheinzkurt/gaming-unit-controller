
#include "../include/CSignalHandler.h"

#include <sstream>
#include <csignal>

#include <boost/bimap.hpp>

namespace Infrastructure
{
namespace Linux
{
   typedef boost::bimap<API::Signal, int> SignalMapType;
   static const SignalMapType signalMap([]
   {
      SignalMapType m;
      m.insert(SignalMapType::value_type(API::Signal::Hup,  SIGHUP));
      m.insert(SignalMapType::value_type(API::Signal::Term, SIGTERM));
      m.insert(SignalMapType::value_type(API::Signal::Kill, SIGKILL));
      ///< Add here when you require more signals to be handled
      return m;
   }());
   
   typedef std::map<int, std::function<void(API::Signal)>> FunctionMapType;
   static FunctionMapType functionMap([]
   {
      FunctionMapType m;
      for (auto s : signalMap.left) { m[s.second]; } ///< Foreach signal an empty function
      return m;
   }());   
   
   /** This should not throw when signal is not in the map, so we use operator[]
    */
   static void signalHandler(int signal) 
   { 
      auto f(functionMap.find(signal));
      if (f == functionMap.end()) { return; }
      
      auto s(signalMap.right.find(signal));
      if (s == signalMap.right.end()) { return; }
      
      f->second(s->second); ///< hmpf
   }
   
   struct CSignalHandler::Impl
   {
      API::Signal const m_signal;
      int const m_intSignal;
      void(*m_oldFunction)(int);
      
      Impl(API::Signal signal, std::function<void(API::Signal)> function) : 
          m_signal(signal)
         ,m_intSignal(signalMap.left.at(m_signal)) ///< This shoud throw when the signal is not in the map
         ,m_oldFunction(nullptr)
      {
         functionMap[m_intSignal] = function;
         m_oldFunction = ::signal(m_intSignal, &signalHandler);
      }
      
      ~Impl() { ::signal(m_intSignal, m_oldFunction); } ///< Restore as it was before
   };
   
   CSignalHandler::CSignalHandler(API::Signal signal, std::function<void(API::Signal)> function) :
      m_impl(new Impl(signal, function))
   {}
   
   CSignalHandler::CSignalHandler(CSignalHandler&&) = default;
   CSignalHandler& CSignalHandler::operator=(CSignalHandler&&) = default; 
   CSignalHandler::~CSignalHandler() = default;

   std::string CSignalHandler::toString() const
   {
      std::ostringstream os;
      os << m_impl->m_signal;
      return os.str();
   }
}}
