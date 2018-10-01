
#include "../include/IMatchingRule.h"

#include <boost/range/combine.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bimap.hpp>

namespace GSC { namespace API {
     
   bool operator==(IMatchingRule const& a, IMatchingRule const& b)
   {
      auto const al(a.getLimit());
      auto const bl(b.getLimit());
      auto const ai(a.getImplication());
      auto const bi(b.getImplication());
      return   std::tie(a.getName(), al, ai, a.getWhitelist(), a.getBlacklist()) 
            == std::tie(b.getName(), bl, bi, b.getWhitelist(), b.getBlacklist());
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
   
   typedef boost::bimap<IMatchingRule::Implication, std::string> ImplicationMapType;
   static ImplicationMapType const implicationMap([]
   {
      ImplicationMapType m;
      m.insert(ImplicationMapType::value_type(IMatchingRule::Implication::None, "none"));
      m.insert(ImplicationMapType::value_type(IMatchingRule::Implication::Warn, "warn"));
      m.insert(ImplicationMapType::value_type(IMatchingRule::Implication::Kill, "kill"));
      return m;
   }());
   
   std::ostream& operator<<(std::ostream& os, IMatchingRule::Implication const& implication)
   {  return os << implicationMap.left.at(implication); }
   
   std::istream& operator>>(std::istream& is, IMatchingRule::Implication& implication)
   {  
      std::string s;
      is >> s;
      implication = implicationMap.right.at(boost::to_lower_copy(boost::trim_copy(s)));
      return is;
   }
   
}}
