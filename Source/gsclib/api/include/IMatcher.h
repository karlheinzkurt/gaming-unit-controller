#pragma once

#include "IMatchingRule.h"

#include "gsclib/infrastructure/api/include/IProcess.h"

#include <string>
#include <memory>
#include <iosfwd> ///< fwd only

#include <boost/property_tree/ptree_fwd.hpp> ///< fwd only
#include <boost/rational.hpp>

namespace GSC
{
namespace API
{
   struct IMatch
   {
      struct MatchLess
      {
         bool operator()(std::unique_ptr<IMatch> const& a, std::unique_ptr<IMatch> const& b) const
         {  return a->getName() < b->getName(); }
      };
      
      typedef std::set<std::unique_ptr<IMatch>, MatchLess> SetType;
      
      virtual ~IMatch() = default;
      
      virtual std::string getName() const = 0;
      
      virtual std::string toString() const = 0;
      
      virtual IMatchingRule const& getRule() const = 0;
      
      virtual Infrastructure::API::IProcess::SetType const& getProcesses() const = 0;
   };
   
   struct IRatedMatch : public virtual IMatch
   {
      struct RatedMatchLess
      {
         bool operator()(std::unique_ptr<IRatedMatch> const& a, std::unique_ptr<IRatedMatch> const& b) const
         {  return a->getName() < b->getName(); }
      };
      
      typedef std::set<std::unique_ptr<IRatedMatch>, RatedMatchLess> SetType;
      
      virtual boost::rational<int> getRatio() const = 0;
   };
   
   std::ostream& operator<<(std::ostream& os, IMatch const& match);
   
   bool operator<(IMatch const&, IMatch const&);
   bool operator<(IRatedMatch const&, IRatedMatch const&);
         
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
