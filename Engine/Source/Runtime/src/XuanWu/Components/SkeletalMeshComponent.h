#pragma once
#include "SceneComponent.h"

namespace XuanWu
{
	class Model;

	class SkeletalMeshComponent : public SceneComponent
	{
	public:
		SkeletalMeshComponent() = default;

		void SetSkeletalMeshAsset(Ref<Model> NewModel) { SkeletalMeshAsset = NewModel; }

		Model* GetSkeletalMeshAsset() const;

	private:
		Ref<Model> SkeletalMeshAsset;
	};
}