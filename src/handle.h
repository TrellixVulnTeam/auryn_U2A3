// Copyright David Corticchiato
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef MNC_HANDLE_H_
#define MNC_HANDLE_H_

#include <cstdlib>

namespace mnc {

template<class ResourceType>
class Handle {
 public:
  static inline bool Is(
      ResourceType* resource,
      const Handle<ResourceType>& reference) {
    return resource == reference.resource_;
  }

  Handle<ResourceType> & operator= (const Handle<ResourceType> & other) {
    this->resource_ = other.resource_;
    this->allow_delete_ = false;
    return *this;
  }

  inline Handle(ResourceType* resource) {
    resource_ = resource;
    allow_delete_ = true;
  }

  inline ResourceType* operator->() const {
    return resource_;
  }

  ~Handle() {
    if (allow_delete_) delete resource_;
  }

 private:
  bool allow_delete_;
  ResourceType * resource_;
};

}  // namespace mnc


#endif  // MNC_HANDLE_H_
