// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/std/string.h"
#define OPENTELEMETRY_HAVE_STD_STRING

#if !defined(OPENTELEMETRY_HAVE_STD_STRING)
#  include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace nostd
{

using Traits = std::char_traits<char>;

/**
 * Back port of nostd::string_view to work with pre-cpp-17 compilers.
 *
 * Note: This provides a subset of the methods available on nostd::string_view but
 * tries to be as compatible as possible with the nostd::string_view interface.
 */
class string {};

}  // namespace nostd
OPENTELEMETRY_END_NAMESPACE

#endif /* OPENTELEMETRY_HAVE_STD_STRING */
