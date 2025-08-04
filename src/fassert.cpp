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
#include <iterator>

#ifdef ENABLE_STD_FORMAT
#include <format>
using std::format_to;
#endif
#ifdef ENABLE_FMTLIB
#include <fmt/core.h>
using fmt::format_to;
#endif

using namespace std;

namespace fassert {

namespace {

class FinalizersImpl: public Finalizers
{
public:
    void register_finalizer(function<void(string_view)> f) override
    {
        mFinalizers.push_back(std::move(f));
    }

    void clear() override {
        mFinalizers.clear();
    }

    deque<function<void(string_view)>> mFinalizers;

    FinalizersImpl() {
        register_finalizer([](string_view msg) {
            fprintf(stderr, "Assertion fails: %s", msg.data());
        });
    }
};

void finalize(string_view trigger_msg)
{
    const shared_ptr<Finalizers>& fins = Finalizers::singleton();
    shared_ptr<FinalizersImpl> finsimpl = dynamic_pointer_cast<FinalizersImpl>(fins);
    deque<function<void(string_view)>> finalizers;
    std::swap(finalizers, finsimpl->mFinalizers);
    for(; !finalizers.empty(); finalizers.pop_front()) {
        auto const& f = finalizers.front();
        f(trigger_msg);
    }
}

enum TriggerBehaviour
{
    ABORT,
    EXIT,
    DO_NOTHING,
};

#ifdef ABORT_WHEN_TRIGGER
TriggerBehaviour kTriggerBehaviour = ABORT;
#else
#ifdef EXIT_WHEN_TRIGGER
TriggerBehaviour kTriggerBehaviour = EXIT;
#else
#ifdef DO_NOTHING_WHEN_TRIGGER
TriggerBehaviour kTriggerBehaviour = DO_NOTHING;
#else
TriggerBehaviour kTriggerBehaviour = ABORT;
#endif
#endif
#endif

} // namespace

shared_ptr<Finalizers> Finalizers::singleton()
{
    static shared_ptr<Finalizers> sFinalizer(new FinalizersImpl());
    return sFinalizer;
}

namespace impl {

void AssertHelper::what(std::string_view msg)
{
    mWhat = msg;
}

AssertHelper::~AssertHelper()
{
    string res;
    auto it = std::back_inserter(res);
    if (!mWhat.empty()) {
        it = format_to(it, "{}", mWhat);
    }
    it = format_to(it, "\n");

    it = format_to(it, "  LOCATION: {}:{}\n", mFile, mLine);
    it = format_to(it, "  FUNCTION: {}\n", mFunc);
    for(auto const& x: mHints) {
        it = format_to(it, "  {}\n", x);
    }

    finalize(res);
    switch(kTriggerBehaviour) {
        case ABORT: abort();
        case EXIT: _Exit(1);
        case DO_NOTHING: {}
    }
}

} // namespace impl

} // namespace fassert
