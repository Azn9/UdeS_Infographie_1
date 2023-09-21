#pragma once

namespace PM3D_API
{
class GameObject;

class Component
{
public:
	virtual ~Component() = default;

	virtual void Update(double deltaTime);
	virtual void FixedUpdate(double fixedDeltaTime);
	virtual void DrawSelf() const;

protected:
	GameObject* parentObject;

private:
	friend class GameObject;
	void SetGameObject(GameObject* gameObject);

};
}