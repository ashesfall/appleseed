
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010-2012 Francois Beaune, Jupiter Jazz Limited
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

#ifndef APPLESEED_RENDERER_UTILITY_TESTUTILS_H
#define APPLESEED_RENDERER_UTILITY_TESTUTILS_H

// appleseed.renderer headers.
#include "renderer/global/globaltypes.h"
#include "renderer/modeling/entity/entity.h"
#include "renderer/modeling/object/object.h"
#include "renderer/modeling/object/regionkit.h"
#include "renderer/modeling/project/project.h"

// appleseed.foundation headers.
#include "foundation/core/concepts/noncopyable.h"
#include "foundation/image/color.h"
#include "foundation/platform/compiler.h"
#include "foundation/utility/autoreleaseptr.h"
#include "foundation/utility/lazy.h"

// appleseed.main headers.
#include "main/dllsymbol.h"

// Forward declarations.
namespace renderer      { class Assembly; }
namespace renderer      { class Scene; }

namespace renderer
{

class TestFixtureBaseProjectHolder
  : public foundation::NonCopyable
{
  public:
    TestFixtureBaseProjectHolder();

    Project& get_project();

  private:
    foundation::auto_release_ptr<Project> m_project;
};

class TestFixtureBase
  : public TestFixtureBaseProjectHolder
{
  public:
    Project&    m_project;
    Scene&      m_scene;
    Assembly&   m_assembly;

    TestFixtureBase();

    void create_color_entity(
        const char*                 name,
        const foundation::Color3f&  linear_rgb);

    void create_color_entity(
        const char*                 name,
        const Spectrum&             spectrum);

    void create_texture_instance(
        const char*                 name,
        const char*                 texture_name);

    void bind_inputs();
};

class DLLSYMBOL DummyEntity
  : public Entity
{
  public:
    virtual void release() override;

  private:
    friend class DummyEntityFactory;

    explicit DummyEntity(const char* name);
};

class DLLSYMBOL DummyEntityFactory
  : public foundation::NonCopyable
{
  public:
    static foundation::auto_release_ptr<DummyEntity> create(const char* name);
};

class BoundingBoxObject
  : public Object
{
  public:
    BoundingBoxObject(
        const char*                 name,
        const GAABB3&               bbox);

    virtual void release() override;

    virtual const char* get_model() const override;

    virtual GAABB3 compute_local_bbox() const override;

    virtual foundation::Lazy<RegionKit>& get_region_kit() override;

  private:
    const GAABB3&                   m_bbox;
    RegionKit                       m_region_kit;
    foundation::Lazy<RegionKit>     m_lazy_region_kit;
};

}       // namespace renderer

#endif  // !APPLESEED_RENDERER_UTILITY_TESTUTILS_H
