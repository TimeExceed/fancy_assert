/*
BSD 3-Clause License

Copyright (c) 2022, Taoda
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "fassert.hpp"
#include <deque>
#include <cstdlib>
#include <cstdio>

using namespace std;

namespace fassert {

namespace {

class FinalizersImpl: public Finalizers
{
public:
    virtual void register_finalizer(function<void()> f) override
    {
        mFinalizers.push_back(std::move(f));
    }

    deque<function<void()>> mFinalizers;
};

void finalize()
{
    const shared_ptr<Finalizers>& fins = Finalizers::singleton();
    shared_ptr<FinalizersImpl> finsimpl = dynamic_pointer_cast<FinalizersImpl>(fins);
    deque<function<void()>> finalizers;
    std::swap(finalizers, finsimpl->mFinalizers);
    for(; !finalizers.empty(); finalizers.pop_front()) {
        auto const& f = finalizers.front();
        f();
    }
}

} // namespace

shared_ptr<Finalizers> Finalizers::singleton()
{
    static shared_ptr<Finalizers> sFinalizer(new FinalizersImpl());
    return sFinalizer;
}

namespace impl {

enum TriggerBehaviour
{
    ABORT,
    EXIT,
    DO_NOTHING,
};

TriggerBehaviour kTriggerBehaviour = ABORT;

AssertHelper& AssertHelper::append(const char* msg, const std::string& x)
{
    mValues.push_back(std::make_pair(std::string(msg), x));
    return *this;
}

void AssertHelper::what(const std::string& msg)
{
    mWhat = msg;
}

AssertHelper::~AssertHelper()
{
    finalize();

    string res;

    if (!mWhat.empty()) {
        res.append(mWhat);
    }
    pp::prettyPrint(res, "\n");

    pp::prettyPrint(res, "  LOCATION: ");
    res.append(mFile);
    pp::prettyPrint(res, ":");
    pp::prettyPrint(res, mLine);
    pp::prettyPrint(res, "\n");

    pp::prettyPrint(res, "  FUNCTION: ");
    res.append(mFunc);
    pp::prettyPrint(res, "\n");

    for(int64_t i = 0, sz = mValues.size(); i < sz; ++i) {
        pp::prettyPrint(res, "  ");
        res.append(mValues[i].first);
        pp::prettyPrint(res, ": ");
        res.append(mValues[i].second);
        pp::prettyPrint(res, "\n");
    }

    fprintf(stderr, "Assertion fails: %s", res.c_str());
    switch(kTriggerBehaviour) {
        case ABORT: abort();
        case EXIT: _Exit(1);
        case DO_NOTHING: {}
    }
}

} // namespace impl

} // namespace fassert
