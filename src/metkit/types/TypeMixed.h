/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file   TypeMixed.h
/// @author Baudouin Raoult
/// @author Tiago Quintino
/// @date   April 2016

#ifndef metkit_TypeMixed_H
#define metkit_TypeMixed_H

#include "metkit/types/TypeEnum.h"

namespace metkit {

//----------------------------------------------------------------------------------------------------------------------

class TypeMixed : public TypeEnum {

public: // methods

    TypeMixed(const std::string &name, const eckit::Value& settings);

    virtual ~TypeMixed();

private: // methods

    virtual void print( std::ostream &out ) const;
    virtual std::string tidy(const std::string &value) const ;
    virtual void expand(const MarsRequest& request, std::vector<std::string>& values) const;

    std::vector<Type*> types_;

};

//----------------------------------------------------------------------------------------------------------------------

} // namespace metkit

#endif
