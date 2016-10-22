/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <algorithm>
#include <set>
#include <list>

#include "eckit/types/Types.h"
#include "eckit/parser/JSON.h"
#include "eckit/log/Log.h"
#include "eckit/config/Resource.h"
#include "eckit/utils/Translator.h"
#include "eckit/utils/MD5.h"
#include "eckit/parser/StringTools.h"
#include "eckit/memory/ScopedPtr.h"

#include "metkit/MarsLanguage.h"
#include "eckit/parser/JSONParser.h"
#include "metkit/types/TypesFactory.h"
#include "metkit/types/Type.h"

using namespace eckit;


static pthread_once_t once = PTHREAD_ONCE_INIT;
static Value languages_;
static std::vector<std::string> verbs_;

static void init() {

    eckit::PathName language("~metkit/etc/language.json");
    std::cout << "Loading " << language << std::endl;
    std::ifstream in(language.asString().c_str());
    if (!in) {
        throw eckit::CantOpenFile(language);
    }

    eckit::JSONParser parser(in);

    languages_ =  parser.parse();
    Value verbs = languages_.keys();
    for (size_t i = 0; i < verbs.size(); ++i) {
        verbs_.push_back(verbs[i]);
    }

}



namespace metkit {

MarsLanguage::MarsLanguage(const std::string& verb):
    verb_(verb) {
    pthread_once(&once, init);
    Value lang = languages_[verb];
    Value params = lang.keys();

    for (size_t i = 0; i < params.size(); ++i) {
        std::string keyword = params[i];
        Value settings = lang[keyword];
        types_[keyword] = TypesFactory::build(keyword, settings);
        keywords_.push_back(keyword);
    }
    std::cout << verb << " loaded" << std::endl;
}

MarsLanguage::~MarsLanguage() {
    std::cout << "MarsLanguage::~MarsLanguage " << verbs_ << std::endl;
    for (std::map<std::string, Type* >::iterator j = types_.begin(); j != types_.end(); ++j) {
        delete (*j).second;
    }
}


static std::string bestMatch(const std::string& name, const std::vector<std::string>& values) {
    for (size_t i = 0; i < values.size(); ++i) {
        const std::string& value = values[i];
        if (value.find(name) == 0) {
            return value;
        }
    }

    std::ostringstream oss;
    oss << "Cannot match '" << name << "' in " << values;
    throw eckit::UserError(oss.str());
}

std::string MarsLanguage::expandVerb(const std::string& verb) {
    pthread_once(&once, init);

    return bestMatch(verb, verbs_);

}

MarsRequest MarsLanguage::expand(const MarsRequest& r) const {
    std::cout << "expand " << r << std::endl;
    MarsRequest result(verb_);

    std::vector<std::string> params;
    r.getParams(params);


    std::cout << params << std::endl;

    for (std::vector<std::string>::iterator j = params.begin(); j != params.end(); ++j) {
        std::string p = bestMatch(*j, keywords_);

        std::cout << p << std::endl;

        std::vector<std::string> values;
        r.getValues(*j, values);

        std::cout << values << std::endl;

        std::map<std::string, Type* >::const_iterator k = types_.find(p);
        ASSERT(k != types_.end());
        Type* type = (*k).second;
        std::cout << *type << std::endl;

        type->expand(values);

        result.setValues(p, values);
    }

    std::cout << "result " << result << std::endl;

    return result;
}


const std::string& MarsLanguage::verb() const {
    return verb_;
}


void MarsLanguage::flatten(const MarsRequest& request,
                           const std::vector<std::string>& params,
                           size_t i,
                           MarsRequest& result) {

    if (i == params.size()) {
        std::cout << result << std::endl;
        return;
    }

    const std::string& param = params[i];

    std::map<std::string, Type* >::const_iterator k = types_.find(param);
    ASSERT(k != types_.end());

    std::vector<std::string> values;
    (*k).second->flattenValues(request, values);

    if(values.empty()) {
        flatten(request, params, i+1, result);
        return;
    }

    for(std::vector<std::string>::const_iterator j = values.begin(); j != values.end(); ++j) {
        result.setValue(param, *j);
        flatten(request, params, i+1, result);
    }

}

void MarsLanguage::flatten(const MarsRequest& request) {
    std::vector<std::string> params;
    request.getParams(params);

    MarsRequest result(request);
    flatten(request, params, 0, result);

}

//----------------------------------------------------------------------------------------------------------------------
void MarsLanguage::set(const std::string& name, const std::vector<std::string>& values) {
    std::cout << "set " << name << " " << values << std::endl;
    std::map<std::string, Type* >::const_iterator k = types_.find(name);
    ASSERT(k != types_.end());
    (*k).second->setDefaults(values);
    std::cout << "done" << std::endl;
}


//----------------------------------------------------------------------------------------------------------------------

} // namespace metkit
