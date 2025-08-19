#pragma once

#include <functional>
#include <memory>
#include <string_view>
#include <string>
#include <utility>
#include <vector>

#ifdef ENABLE_STD_FORMAT
#include <format>
#endif
#ifdef ENABLE_FMTLIB
#include <fmt/format.h>
#endif


namespace fassert {

class Finalizers
{
public:
    static std::shared_ptr<Finalizers> singleton();

public:
    virtual ~Finalizers() =default;
    virtual void register_finalizer(std::function<void(std::string_view)>) =0;
    virtual void clear() =0;
};

}

namespace fassert::_impl {

class _Assert {
public:
    _Assert(
        const char* fn,
        int line,
        const char* func)
    :   mFile(fn),
        mLine(line),
        mFunc(func)
    {}

    ~_Assert();

    template<class... Args>
    requires (sizeof...(Args) > 0)
    _Assert ctx(::std::string_view fmt, Args&&... args) && {

#ifdef ENABLE_STD_FORMAT
        auto m = ::std::vformat(fmt, ::std::make_format_args(args...));
#endif
#ifdef ENABLE_FMTLIB
        auto m = ::fmt::vformat(fmt, ::fmt::make_format_args(args...));
#endif
        mHints.push_back(std::move(m));
        return *this;
    }

    void what(std::string_view msg);

private:
    std::vector<std::string> mHints;
    std::string mWhat;
    std::string_view mFile;
    int mLine;
    std::string_view mFunc;
};

}

#define FASSERT_LIKELY(x) __builtin_expect(!!(x), 1)

#define FASSERT(cond) \
    if (FASSERT_LIKELY(cond)) {} \
    else std::move(::fassert::_impl::_Assert(__FILE__, __LINE__, __func__)) \
        .ctx("Condition: {}", #cond)
