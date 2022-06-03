#pragma once
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
#include "prettyprint.hpp"
#include <string>
#include <utility>
#include <vector>

namespace fassert {
namespace impl {

class AssertHelper
{
public:
    AssertHelper(
        const char* fn,
        int line,
        const char* func)
      : mValues(),
        mFile(fn),
        mLine(line),
        mFunc(func),
        FASSERT_PINGPONG_A(*this),
        FASSERT_PINGPONG_B(*this)
    {}

    ~AssertHelper();

    AssertHelper& append(const char* msg, const std::string& x);
    void what(const std::string& msg);

private:
    std::vector<std::pair<std::string, std::string>> mValues;
    std::string mWhat;
    std::string mFile;
    int mLine;
    std::string mFunc;

public:
    /*
     * Ender of macro expansion
     */
    AssertHelper& FASSERT_PINGPONG_A;
    AssertHelper& FASSERT_PINGPONG_B;
};

} // namespace impl
} // namespace fassert
