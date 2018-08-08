
#include "../include/IMatchingRule.h"

#include <boost/range/combine.hpp>

namespace GSC { namespace API {
     
   bool operator==(IMatchingRule const& a, IMatchingRule const& b)
   {
      auto const al(a.getLimit());
      auto const bl(b.getLimit());
      return   std::tie(a.getName(), al, a.getWhitelist(), a.getBlacklist()) 
            == std::tie(b.getName(), bl, b.getWhitelist(), b.getBlacklist());
   }
   
   bool operator!=(IMatchingRule const& a, IMatchingRule const& b)
   {  return !(a == b); }
   
   bool operator==(IMatchingRule::SetType const& a, IMatchingRule::SetType const& b)
   {
      for (auto const& item : boost::range::combine(a, b))
      {  if (*boost::get<0>(item) != *boost::get<1>(item)) { return false; } }
      return true;
   }

   bool operator!=(IMatchingRule::SetType const& a, IMatchingRule::SetType const& b)
   {  return !(a == b); }
   
   std::ostream& operator<<( std::ostream& os, IMatchingRule const& rule )
   {  return os << rule.toString(); }
   
}}
