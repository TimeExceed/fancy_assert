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
#ifndef FASSERT_HPP
#define FASSERT_HPP

#include "fassert.ipp"
#include <functional>
#include <memory>

namespace fassert {

class Finalizers
{
public:
    static std::shared_ptr<Finalizers> singleton();

public:
    virtual ~Finalizers() =default;
    virtual void register_finalizer(std::function<void()>) =0;
};

} // namespace fassert

#define FASSERT_LIKELY(x) __builtin_expect(!!(x), 1)

#define FASSERT_PINGPONG_A(...) \
    FASSERT_PINGPONG_OP(B __VA_OPT__(,) __VA_ARGS__)
#define FASSERT_PINGPONG_B(...) \
    FASSERT_PINGPONG_OP(A __VA_OPT__(,) __VA_ARGS__)
#define FASSERT_PINGPONG_OP(next, ...) \
    append(__VA_ARGS__). FASSERT_PINGPONG_##next

#define FASSERT(cond) \
    if (FASSERT_LIKELY(cond)) {} \
    else ::fassert::impl::AssertHelper(__FILE__, __LINE__, __func__) \
             .append("Condition: {}", #cond). FASSERT_PINGPONG_A


#endif
