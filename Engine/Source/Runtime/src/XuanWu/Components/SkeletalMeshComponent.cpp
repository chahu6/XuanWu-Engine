#include "xwpch.h"
#include "SkeletalMeshComponent.h"
#include "XuanWu/3D/Model.h"

namespace XuanWu
{
	Model* SkeletalMeshComponent::GetSkeletalMeshAsset() const
	{
		return SkeletalMeshAsset.get();
	}
}
