#pragma once

#include "IMatchingRule.h"

#include "gsclib/infrastructure/api/include/IProcess.h"

#include <string>
#include <memory>
#include <iosfwd> ///< fwd only

#include <boost/property_tree/ptree_fwd.hpp> ///< fwd only

namespace GSC
{
namespace API
{
   struct IMatch
   {
      typedef std::set<std::unique_ptr<IMatch>> SetType;
      
      virtual ~IMatch() = default;
      
      virtual std::string getName() const = 0;
      
      virtual std::string toString() const = 0;
      
      virtual IMatchingRule const& getRule() const = 0;
      
      virtual Infrastructure::API::IProcess::SetType const& getProcesses() const = 0;
   };
   
   std::ostream& operator<<(std::ostream& os, IMatch const& match);
   
   bool operator<(IMatch const&, IMatch const&);
         
   struct IMatcher
   {
      virtual ~IMatcher() = default;
      
      virtual IMatcher& add(std::unique_ptr<IMatchingRule> rule) = 0;
      
      virtual IMatchingRule::SetType const& getRules() const = 0;
           
      virtual IMatch::SetType matches(std::shared_ptr<Infrastructure::API::IProcess> const& process) const = 0;
      
      virtual IMatch::SetType matches(Infrastructure::API::IProcess::SetType const& process) const = 0;
      
      virtual boost::property_tree::ptree serialize() const = 0;
   };

}}
