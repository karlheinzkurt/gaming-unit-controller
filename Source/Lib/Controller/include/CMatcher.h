#pragma once

#include "Lib/Infrastructure/API/include/IProcess.h"

#include <string>
#include <map>
#include <set>
#include <tuple>
#include <regex>
#include <functional>
#include <iosfwd>

namespace Lib
{
namespace Controller
{
   using Infrastructure::API::IProcess;
   using Infrastructure::API::IProcessSet;

   struct Match
   {
      typedef std::set< std::reference_wrapper< IProcess const > > ProcessSetType;
      
      Match( std::string const& name );
      
      Match( Match const& ) = default;
      Match& operator=( Match const& ) = default;
      
      Match& add( IProcess const& process );
      Match& add( ProcessSetType const& processes );
      
      std::string const& getName() const;
      ProcessSetType getProcesses() const;
      
   private:   
      std::string m_name;
      ProcessSetType m_processes;
   };
   
   std::ostream& operator<<( std::ostream& os, Match const& match );
      
   struct Matcher
   {
      /* We use this comparator only for the result set here, 
       * comparing only names would not fit as generic comparison. */
      struct MatchComparator
      {
         bool operator()( Match const& a, Match const& b ) const
         {  return a.getName() < b.getName(); }
      };
      typedef std::set< Match, MatchComparator > ResultSet; 
      
      Matcher& add( 
          std::string name
         ,std::initializer_list< std::string > whiteList
         ,std::initializer_list< std::string > blackList = {} );
      
      ResultSet matches( IProcess const& process );
      
      ResultSet matches( IProcessSet::SetType const& process );
      
   private:
      typedef std::map< std::string, std::tuple< std::regex, std::regex > > MapType;
      MapType m_matcher;
   };

}}
