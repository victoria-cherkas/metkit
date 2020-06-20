/*
 * (C) Copyright 2017- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Baudouin Raoult
/// @date   Jun 2020



#ifndef mars_client_OdbContent_H
#define mars_client_OdbContent_H

#include "metkit/codes/MessageContent.h"
#include "eckit/io/ResizableBuffer.h"

namespace metkit {
namespace codes {

class OdbContent : public MessageContent {
public:
    OdbContent(eckit::DataHandle&);
    ~OdbContent();

private:

    eckit::ResizableBuffer frame_;

    virtual size_t length() const;
    const void* data() const;

    virtual void write(eckit::DataHandle& handle) const;

    virtual void print(std::ostream & s) const;

};


}  // namespace codes
}  // namespace metkit


#endif
