/*
 * (C) Copyright 1996-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "metkit/types/TypesFactory.h"
#include "metkit/types/TypeEnum.h"


namespace metkit {

//----------------------------------------------------------------------------------------------------------------------

TypeEnum::TypeEnum(const std::string &name, const eckit::Value& settings) :
    Type(name, settings) {

    eckit::Value values = settings["values"];
    for(size_t i = 0; i < values.size(); ++i) {
        std::string v = values[i];
        values_[v] = v;
    }
}

TypeEnum::~TypeEnum() {
}

void TypeEnum::print(std::ostream &out) const {
    out << "TypeEnum[name=" << name_ << "]";
}

void TypeEnum::expand(std::vector<std::string>& values) const {

    std::vector<std::string> newval;
    for(std::vector<std::string>::const_iterator j = values.begin(); j != values.end(); ++j) {
        for(std::map<std::string, std::string>::const_iterator k = values_.begin(); k != values_.end(); ++k) {
            if((*k).first.find(*j) == 0) {
                newval.push_back((*k).second);
                break;
            }
        }
    }
    std::swap(values, newval);
}

static TypeBuilder<TypeEnum> type("enum");

//----------------------------------------------------------------------------------------------------------------------

} // namespace metkit
