#pragma once
#include <memory>

namespace PM3D_API
{
class GameObject;

class Component
{
public:
	virtual ~Component() = default;

	virtual void Initialize();
	virtual void Update(double deltaTime);
	virtual void FixedUpdate(double fixedDeltaTime);
	virtual void DrawSelf() const;

protected:
	std::shared_ptr<GameObject> parentObject = nullptr;

private:
	friend class GameObject;
	void SetGameObject(const std::shared_ptr<GameObject>& gameObject);

};
}
