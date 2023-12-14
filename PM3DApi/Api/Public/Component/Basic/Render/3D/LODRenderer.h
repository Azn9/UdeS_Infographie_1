#include "Api/Public/Component/Component.h"
#include "Renderer.h"
#include <set>
#include <utility>


namespace PM3D_API
{

class LODRenderer : public Component
{
    using SubRenderer = std::pair<float, std::unique_ptr<Renderer>>;
    
public:

    virtual void Initialize();
    virtual void Update();
    virtual void PhysicsUpdate();
    virtual void DrawSelf() const;
    virtual void DrawDebugInfo() const;

    Renderer* GetCurrentRenderer() { return currentRenderer; }
    
private:

    Renderer* currentRenderer = nullptr;
    int currentLOD = -1;
    std::set<SubRenderer> subRenderers;
    
};
    
}