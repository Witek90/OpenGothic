#pragma once

#include <Tempest/Matrix4x4>
#include <Tempest/UniformBuffer>

#include <memory>
#include <list>
#include <random>

#include "graphics/visualobjects.h"
#include "resources.h"

class SceneGlobals;
class RendererStorage;
class ParticleFx;
class PfxBucket;
class Painter3d;

class PfxObjects final {
  public:
    PfxObjects(const SceneGlobals& scene, VisualObjects& visual);
    ~PfxObjects();

    class Emitter final {
      public:
        Emitter()=default;
        ~Emitter();
        Emitter(Emitter&&);
        Emitter& operator=(Emitter&& b);

        Emitter(const Emitter&)=delete;

        bool   isEmpty() const { return bucket==nullptr; }
        void   setPosition (float x,float y,float z);
        void   setPosition (const Tempest::Vec3& pos);
        void   setTarget   (const Tempest::Vec3& pos);
        void   setDirection(const Tempest::Matrix4x4& pos);
        void   setObjMatrix(const Tempest::Matrix4x4& mt);
        void   setActive(bool act);
        bool   isActive() const;
        void   setLooped(bool loop);

        uint64_t effectPrefferedTime() const;

      private:
        Emitter(PfxBucket &b,size_t id);

        PfxBucket*      bucket = nullptr;
        size_t          id     = size_t(-1);

      friend class PfxBucket;
      friend class PfxObjects;
      };

    struct VboContext {
      Tempest::Vec3 left = {};
      Tempest::Vec3 top  = {};
      Tempest::Vec3 z    = {};

      Tempest::Vec3 leftA = {};
      Tempest::Vec3 topA  = {0,1,0};
      };

    Emitter get(const ParticleFx& decl);
    Emitter get(const ZenLoad::zCVobData& vob);

    void    setViewerPos(const Tempest::Vec3& pos);

    void    resetTicks();
    void    tick(uint64_t ticks);

    void    preFrameUpdate(uint8_t fId);

  private:
    struct SpriteEmitter {
      uint8_t                     visualCamAlign = 0;
      int32_t                     zBias          = 0;
      ZMath::float2               decalDim = {};
      std::unique_ptr<ParticleFx> pfx;
      };

    PfxBucket&                    getBucket(const ParticleFx& decl);
    PfxBucket&                    getBucket(const Material& mat, const ZenLoad::zCVobData& vob);

    const SceneGlobals&           scene;
    VisualObjects&                visual;
    std::recursive_mutex          sync;

    std::vector<std::unique_ptr<PfxBucket>> bucket;
    std::vector<SpriteEmitter>              spriteEmit;

    Tempest::Vec3                 viewerPos={};
    uint64_t                      lastUpdate=0;
  };
