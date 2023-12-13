#pragma once
#include "PxPhysicsAPI.h"
#include <iostream>
#include "Api/Public/Util/FilterGroup.h"
#include "Api/Public/EventSystem/EventSystem.h"
#include <Api/Public/EventSystem/InTunnelEvent.h>


using namespace physx;

PxFilterFlags FilterShader(
	PxFilterObjectAttributes attributes0,
	PxFilterData filterData0,
	PxFilterObjectAttributes attributes1,
	PxFilterData filterData1,
	PxPairFlags& pairFlags,
	const void* constantBlock,
	PxU32 constantBlockSize)
{

	std::cout << FilterGroup::eSNOWBALL << " " << filterData0.word0 << "||" << filterData1.word0 << std::endl;

	if ((filterData0.word0 == FilterGroup::eSNOWBALL && filterData1.word0 == FilterGroup::eOBSTACLE) || (filterData1.word0 == FilterGroup::eSNOWBALL && filterData0.word0 == FilterGroup::eOBSTACLE))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eCONTACT_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	if (filterData0.word0 == FilterGroup::eSKIER && filterData1.word0 == FilterGroup::eSKIER) 
	{
		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	if ((filterData0.word0 == FilterGroup::eSNOWBALL && filterData1.word0 == FilterGroup::eSKIER) || (filterData1.word0 == FilterGroup::eSNOWBALL && filterData0.word0 == FilterGroup::eSKIER))
	{
		if (filterData0.word0 == FilterGroup::eSNOWBALL)
			if (filterData0.word2 == BuriablePenguin::eCanBury)
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT ;
			else
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eCONTACT_DEFAULT;
		else 
			if (filterData1.word2 == BuriablePenguin::eCanBury)
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			else
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eCONTACT_DEFAULT;

		return PxFilterFlag::eDEFAULT;
	}
	if (filterData0.word0 == FilterGroup::eFLOOR || filterData1.word0 == FilterGroup::eFLOOR)
	{
		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	if (filterData0.word0 == FilterGroup::eTUNNEL || filterData1.word0 == FilterGroup::eTUNNEL)
	{
		PM3D_API::EventSystem::Publish(InTunnelEvent(true));
		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	return PxFilterFlags(PxFilterFlag::eSUPPRESS);
}