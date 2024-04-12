// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <prometheus/metric_family.h>
#include <string>
#include <vector>
#include "opentelemetry/metrics/provider.h"
#include "opentelemetry/nostd/string_view.h"
#include "opentelemetry/sdk/metrics/meter.h"
#include "opentelemetry/version.h"

OPENTELEMETRY_BEGIN_NAMESPACE
namespace exporter
{
namespace metrics
{
/**
 * The Prometheus Utils contains utility functions for Prometheus Exporter
 */
class PrometheusExporterUtils
{
public:
  /**
   * Helper function to convert OpenTelemetry metrics data collection
   * to Prometheus metrics data collection
   *
   * @param records a collection of metrics in OpenTelemetry
   * @param populate_target_info whether to populate target_info
   * @param without_otel_scope whether to populate otel_scope_name and otel_scope_version
   * attributes
   * @return a collection of translated metrics that is acceptable by Prometheus
   */
  static std::vector<::prometheus::MetricFamily> TranslateToPrometheus(
      const sdk::metrics::ResourceMetrics &data,
      bool populate_target_info = true,
      bool without_otel_scope   = false);

private:
  /**
   * Sanitize the given metric name or label according to Prometheus rule.
   *
   * This function is needed because names in OpenTelemetry can contain
   * alphanumeric characters, '_', '.', and '-', whereas in Prometheus the
   * name should only contain alphanumeric characters and '_'.
   * @param name name
   */
  static nostd::string SanitizeNames(nostd::string name);

  /**
   * Sanitize the given metric name or label according to Prometheus rule.
   *
   * @param name label name
   * @param value label value
   * @param labels target labels
   */
  static void AddPrometheusLabel(nostd::string name,
                                 nostd::string value,
                                 std::vector<::prometheus::ClientMetric::Label> *labels);

  static nostd::string MapToPrometheusName(const nostd::string &name,
                                         const nostd::string &unit,
                                         ::prometheus::MetricType prometheus_type);

  /**
   * A utility function that returns the equivalent Prometheus name for the provided OTLP metric
   * unit.
   *
   * @param raw_metric_unitName The raw metric unit for which Prometheus metric unit needs to be
   *     computed.
   * @return the computed Prometheus metric unit equivalent of the OTLP metric unit
   */
  static nostd::string GetEquivalentPrometheusUnit(const nostd::string &raw_metric_unitName);

  /**
   * This method retrieves the expanded Prometheus unit name for known abbreviations. OTLP metrics
   * use the c/s notation as specified at <a href="https://ucum.org/ucum.html">UCUM</a>. The list of
   * mappings is adopted from <a
   * href="https://github.com/open-telemetry/opentelemetry-collector-contrib/blob/9a9d4778bbbf242dba233db28e2fbcfda3416959/pkg/translator/prometheus/normalize_name.go#L30">OpenTelemetry
   * Collector Contrib</a>.
   *
   * @param unit_abbreviation The unit that name that needs to be expanded/converted to Prometheus
   *     units.
   * @return The expanded/converted unit name if known, otherwise returns the input unit name as-is.
   */
  static nostd::string GetPrometheusUnit(const nostd::string &unit_abbreviation);

  /**
   * This method retrieves the expanded Prometheus unit name to be used with "per" units for known
   * units. For example: s => per second (singular)
   *
   * @param per_unit_abbreviation The unit abbreviation used in a 'per' unit.
   * @return The expanded unit equivalent to be used in 'per' unit if the input is a known unit,
   *     otherwise returns the input as-is.
   */
  static nostd::string GetPrometheusPerUnit(const nostd::string &per_unit_abbreviation);

  /**
   * Replaces all characters that are not a letter or a digit with '_' to make the resulting string
   * Prometheus compliant. This method also removes leading and trailing underscores - this is done
   * to keep the resulting unit similar to what is produced from the collector's implementation.
   *
   * @param str The string input that needs to be made Prometheus compliant.
   * @return the cleaned-up Prometheus compliant string.
   */
  static nostd::string CleanUpString(const nostd::string &str);

  /**
   * This method is used to convert the units expressed as a rate via '/' symbol in their name to
   * their expanded text equivalent. For instance, km/h => km_per_hour. The method operates on the
   * input by splitting it in 2 parts - before and after '/' symbol and will attempt to expand any
   * known unit abbreviation in both parts. Unknown abbreviations & unsupported characters will
   * remain unchanged in the final output of this function.
   *
   * @param rate_expressed_unit The rate unit input that needs to be converted to its text
   * equivalent.
   * @return The text equivalent of unit expressed as rate. If the input does not contain '/', the
   *     function returns it as-is.
   */
  static nostd::string ConvertRateExpressedToPrometheusUnit(const nostd::string &rate_expressed_unit);

  /**
   * This method drops all characters enclosed within '{}' (including the curly braces) by replacing
   * them with an empty string. Note that this method will not produce the intended effect if there
   * are nested curly braces within the outer enclosure of '{}'.
   *
   * <p>For instance, {packet{s}s} => s}.
   *
   * @param unit The input unit from which text within curly braces needs to be removed.
   * @return The resulting unit after removing the text within '{}'.
   */
  static nostd::string RemoveUnitPortionInBraces(const nostd::string &unit);

  static opentelemetry::sdk::metrics::AggregationType getAggregationType(
      const opentelemetry::sdk::metrics::PointType &point_type);

  /**
   * Translate the OTel metric type to Prometheus metric type
   */
  static ::prometheus::MetricType TranslateType(opentelemetry::sdk::metrics::AggregationType kind,
                                                bool is_monotonic = true);

  /**
   * Add a target_info metric to collect resource attributes
   */
  static void SetTarget(const sdk::metrics::ResourceMetrics &data,
                        std::chrono::nanoseconds time,
                        const opentelemetry::sdk::instrumentationscope::InstrumentationScope *scope,
                        std::vector<::prometheus::MetricFamily> *output);

  /**
   * Set metric data for:
   * Counter => Prometheus Counter
   */
  template <typename T>
  static void SetData(std::vector<T> values,
                      const opentelemetry::sdk::metrics::PointAttributes &labels,
                      const opentelemetry::sdk::instrumentationscope::InstrumentationScope *scope,
                      ::prometheus::MetricType type,
                      std::chrono::nanoseconds time,
                      ::prometheus::MetricFamily *metric_family,
                      const opentelemetry::sdk::resource::Resource *resource);

  /**
   * Set metric data for:
   * Histogram => Prometheus Histogram
   */
  template <typename T>
  static void SetData(std::vector<T> values,
                      const std::vector<double> &boundaries,
                      const std::vector<uint64_t> &counts,
                      const opentelemetry::sdk::metrics::PointAttributes &labels,
                      const opentelemetry::sdk::instrumentationscope::InstrumentationScope *scope,
                      std::chrono::nanoseconds time,
                      ::prometheus::MetricFamily *metric_family,
                      const opentelemetry::sdk::resource::Resource *resource);

  /**
   * Set time and labels to metric data
   */
  static void SetMetricBasic(
      ::prometheus::ClientMetric &metric,
      const opentelemetry::sdk::metrics::PointAttributes &labels,
      std::chrono::nanoseconds time,
      const opentelemetry::sdk::instrumentationscope::InstrumentationScope *scope,
      const opentelemetry::sdk::resource::Resource *resource);

  /**
   * Convert attribute value to string
   */
  static nostd::string AttributeValueToString(
      const opentelemetry::sdk::common::OwnedAttributeValue &value);

  /**
   * Handle Counter and Gauge.
   */
  template <typename T>
  static void SetValue(std::vector<T> values,
                       ::prometheus::MetricType type,
                       ::prometheus::ClientMetric *metric);

  /**
   * Handle Gauge from MinMaxSumCount
   */
  static void SetValue(double value, ::prometheus::ClientMetric *metric);

  /**
   * Handle Histogram
   */
  template <typename T>
  static void SetValue(std::vector<T> values,
                       const std::vector<double> &boundaries,
                       const std::vector<uint64_t> &counts,
                       ::prometheus::ClientMetric *metric);

  // For testing
  friend class SanitizeNameTester;
};
}  // namespace metrics
}  // namespace exporter
OPENTELEMETRY_END_NAMESPACE
