#pragma once

#pragma once
#include <iostream>
#include <PxPhysicsAPI.h>
#include <vector>
#include <algorithm>
#include "FilterGroup.h"
using namespace physx;

class SimulationCallback : public PxSimulationEventCallback, public PxContactModifyCallback {

    std::vector<PxShape*> dynamicToShrink{};

public:
    // Function called with default contact
    void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
        for (PxU32 i = 0; i < nbPairs; i++) {
            const PxContactPair modifyPair = pairs[i];
            PxShape* shape = modifyPair.shapes[0];
            if (shape->getSimulationFilterData().word0 == FilterGroup::eSNOWBALL)
            {
                PxShapeFlags flags = shape->getFlags();
                flags |= PxShapeFlag::eTRIGGER_SHAPE;
                shape->setFlags(flags);

                dynamicToShrink.push_back(shape);

                PxRigidDynamic* snowBall = static_cast<PxRigidDynamic*>(shape->getActor());
            }
        }

    }

    // Function called on OnContactModify
    void onContactModify(PxContactModifyPair* const pairs, PxU32 count) {
        for (PxU32 i = 0; i < count; i++) {
            PxContactModifyPair& modifyPair = pairs[i];
            /*if (modifyPair.shape[0]->getSimulationFilterData().word0 == FilterGroup::eBALLUP)
            {
                modifyPair.contacts.setNormal(i, PxVec3{ 0.f,1.f,0.f }.getNormalized());
                modifyPair.contacts.setRestitution(i, 100.f);
            }*/

        }
    }

    void applyAfterContactModification(float& size) {
        for (auto shape : dynamicToShrink)
        {
            PxRigidDynamic* snowBall = static_cast<PxRigidDynamic*>(shape->getActor());
            size *= 0.5f;
            shape->setGeometry(PxSphereGeometry(size));

        }
        dynamicToShrink.clear();
    }

    void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {}
    void onWake(PxActor** actors, PxU32 count) {}
    void onSleep(PxActor** actors, PxU32 count) {}
    void onTrigger(PxTriggerPair* pairs, PxU32 count) {}
    void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {}
};