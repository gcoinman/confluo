#ifndef DIALOG_COLUMN_H_
#define DIALOG_COLUMN_H_

#include <string>
#include <cstdint>

#include "column_snapshot.h"
#include "field.h"
#include "mutable_value.h"
#include "index_state.h"
#include "string_utils.h"

namespace dialog {

class column_t {
 public:
  column_t()
      : idx_(UINT16_MAX),
        type_(),
        offset_(UINT16_MAX) {
  }

  column_t(uint16_t idx, uint16_t offset, const data_type& type,
           const std::string& name, const mutable_value& min,
           const mutable_value& max)
      : idx_(idx),
        type_(type),
        offset_(offset),
        name_(string_utils::to_upper(name)),
        min_(min),
        max_(max) {
  }

  std::string name() const {
    return name_;
  }

  const data_type& type() const {
    return type_;
  }

  uint16_t offset() const {
    return offset_;
  }

  uint16_t idx() const {
    return idx_;
  }

  mutable_value min() const {
    return min_;
  }

  mutable_value max() const {
    return max_;
  }

  uint16_t index_id() const {
    return idx_state_.id();
  }

  double index_bucket_size() const {
    return idx_state_.bucket_size();
  }

  bool is_indexed() const {
    return idx_state_.is_indexed();
  }

  bool set_indexing() {
    return idx_state_.set_indexing();
  }

  void set_indexed(uint16_t index_id, double bucket_size) {
    idx_state_.set_indexed(index_id, bucket_size);
  }

  void set_unindexed() {
    idx_state_.set_unindexed();
  }

  bool disable_indexing() {
    return idx_state_.disable_indexing();
  }

  field_t apply(void* data) const {
    return field_t(idx_, type_,
                   reinterpret_cast<unsigned char*>(data) + offset_,
                   is_indexed(), idx_state_.id(), idx_state_.bucket_size());
  }

  column_snapshot snapshot() const {
    return {type_, offset_, is_indexed(), index_id(), index_bucket_size()};
  }

 private:
  uint16_t idx_;
  data_type type_;
  uint16_t offset_;
  std::string name_;
  mutable_value min_;
  mutable_value max_;
  index_state_t idx_state_;
};

}

#endif /* DIALOG_COLUMN_H_ */
