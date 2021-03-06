
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2016-2017 Francois Beaune, The appleseedhq Organization
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// Interface header.
#include "settingsparsing.h"

// appleseed.renderer headers.
#include "renderer/global/globallogger.h"
#include "renderer/utility/paramarray.h"

// appleseed.foundation headers.
#include "foundation/platform/system.h"
#include "foundation/utility/containers/dictionary.h"
#include "foundation/utility/makevector.h"
#include "foundation/utility/string.h"

// Standard headers.
#include <algorithm>

using namespace foundation;
using namespace std;

namespace renderer
{

Spectrum::Mode get_spectrum_mode(const ParamArray& params)
{
    const string spectrum_mode =
        params.get_required<string>(
            "spectrum_mode",
            "rgb",
            make_vector("rgb", "spectral"));

    return
        spectrum_mode == "rgb"
            ? Spectrum::RGB
            : Spectrum::Spectral;
}

string get_spectrum_mode_name(const Spectrum::Mode mode)
{
    switch (mode)
    {
      case Spectrum::RGB: return "rgb";
      case Spectrum::Spectral: return "spectral";
      default: return "unknown";
    }
}

SamplingContext::Mode get_sampling_context_mode(const ParamArray& params)
{
    const string sampling_mode =
        params.get_required<string>(
            "sampling_mode",
            "rng",
            make_vector("rng", "qmc"));

    return
        sampling_mode == "rng"
            ? SamplingContext::RNGMode
            : SamplingContext::QMCMode;
}

string get_sampling_context_mode_name(const SamplingContext::Mode mode)
{
    switch (mode)
    {
      case SamplingContext::RNGMode: return "rng";
      case SamplingContext::QMCMode: return "qmc";
      default: return "unknown";
    }
}

size_t get_rendering_thread_count(const ParamArray& params)
{
    const size_t core_count = System::get_logical_cpu_core_count();

    static const char* ThreadCountParameterName = "rendering_threads";

    if (!params.strings().exist(ThreadCountParameterName))
        return core_count;

    const string thread_count_str = params.strings().get<string>(ThreadCountParameterName);

    if (thread_count_str == "auto")
        return core_count;

    bool conversion_failed = false;
    size_t thread_count;

    try
    {
        const int num_threads = from_string<int>(thread_count_str);
        if (num_threads < 0)
        {
            // If num_threads is negative, use all cores except -num_threads.
            thread_count = max(static_cast<int>(core_count) + num_threads, 1);
        }
        else
            thread_count = num_threads;
    }
    catch (const ExceptionStringConversionError&)
    {
        conversion_failed = true;
    }

    if (conversion_failed || thread_count == 0)
    {
        RENDERER_LOG_ERROR(
            "invalid value \"%s\" for parameter \"%s\", using default value \"%s\".",
            thread_count_str.c_str(),
            ThreadCountParameterName,
            "auto");

        return core_count;
    }

    return thread_count;
}

}   // namespace renderer
