// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "opentelemetry/nostd/shared_ptr.h"
#include "opentelemetry/nostd/string_view.h"
#include "opentelemetry/sdk/metrics/aggregation/aggregation_config.h"
#include "opentelemetry/sdk/metrics/aggregation/default_aggregation.h"
#include "opentelemetry/sdk/metrics/instruments.h"
#include "opentelemetry/sdk/metrics/view/attributes_processor.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace sdk
{
namespace metrics
{

class View;

/**
 * Factory class for View.
 */
class OPENTELEMETRY_EXPORT ViewFactory
{
public:
  static std::unique_ptr<View> Create(const nostd::string &name);

  static std::unique_ptr<View> Create(const nostd::string &name, const nostd::string &description);

  static std::unique_ptr<View> Create(const nostd::string &name,
                                      const nostd::string &description,
                                      const nostd::string &unit);

  static std::unique_ptr<View> Create(const nostd::string &name,
                                      const nostd::string &description,
                                      const nostd::string &unit,
                                      AggregationType aggregation_type);

  static std::unique_ptr<View> Create(const nostd::string &name,
                                      const nostd::string &description,
                                      const nostd::string &unit,
                                      AggregationType aggregation_type,
                                      std::shared_ptr<AggregationConfig> aggregation_config);

  static std::unique_ptr<View> Create(const nostd::string &name,
                                      const nostd::string &description,
                                      const nostd::string &unit,
                                      AggregationType aggregation_type,
                                      std::shared_ptr<AggregationConfig> aggregation_config,
                                      std::unique_ptr<AttributesProcessor> attributes_processor);
};

}  // namespace metrics
}  // namespace sdk
OPENTELEMETRY_END_NAMESPACE
