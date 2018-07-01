#pragma once

#include "guclib/infrastructure/api/include/ISystem.h"

namespace Infrastructure
{
namespace Common
{
   struct System
   {
      static Infrastructure::API::ISystem& get();
   };
}}
