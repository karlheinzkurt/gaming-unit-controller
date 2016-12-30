#pragma once

#include "Infrastructure/API/include/ISignalHandler.h"

#include <functional>
#include <memory>

namespace Infrastructure
{
namespace Linux
{
   /** \remark This is not thread save, use it globally
    */
   struct CSignalHandler : API::ISignalHandler
   {
      CSignalHandler(API::Signal signal, std::function<void(API::Signal)> function);
      
      CSignalHandler(CSignalHandler&) = delete; ///< no copy
      CSignalHandler& operator=(CSignalHandler&) = delete;
      
      CSignalHandler(CSignalHandler&&);
      CSignalHandler& operator=(CSignalHandler&&);      
      virtual ~CSignalHandler();
      
      virtual std::string toString() const override;
      
   private:
      struct Impl; ///< Don't want to have platform types in the header
      std::unique_ptr<Impl> m_impl;
   };
}}
