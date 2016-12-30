#pragma once

#include "Infrastructure/API/include/ISystem.h"

namespace Infrastructure
{
namespace Common
{
   struct System
   {
      static Infrastructure::API::ISystem& get();
   };
}}
