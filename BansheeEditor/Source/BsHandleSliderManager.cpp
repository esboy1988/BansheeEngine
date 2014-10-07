#include "BsHandleSliderManager.h"
#include "BsDrawHelper.h"
#include "BsMaterial.h"
#include "BsBuiltinEditorResources.h"
#include "BsCoreThread.h"
#include "BsRendererManager.h"
#include "BsRenderer.h"
#include "BsTransientMesh.h"
#include "BsCamera.h"
#include "BsHandleSlider.h"

using namespace std::placeholders;

namespace BansheeEngine
{
	HandleSliderManager::HandleSliderManager()
		:mActiveSlider(nullptr), mHoverSlider(nullptr)
	{

	}

	HandleSliderManager::~HandleSliderManager()
	{

	}

	void HandleSliderManager::update(const HCamera& camera, const Vector2I& inputPos, const Ray& inputRay, bool pressed)
	{
		float nearestT = std::numeric_limits<float>::max();
		HandleSlider* overSlider = nullptr;
		for (auto& slider : mSliders)
		{
			float t;
			if (slider->intersects(inputRay, t))
			{
				if (t < nearestT)
				{
					overSlider = slider;
					nearestT = t;
				}
			}
		}

		if (overSlider != nullptr)
		{
			if (!pressed)
			{
				if (mActiveSlider != nullptr)
				{
					mActiveSlider->setInactive();
					mActiveSlider = nullptr;
				}

				if (mHoverSlider != overSlider)
				{
					mHoverSlider->setInactive();
					mHoverSlider = overSlider;
					overSlider->setHover();
				}
			}
			else
			{
				if (mHoverSlider != nullptr)
				{
					mHoverSlider->setInactive();
					mHoverSlider = nullptr;
				}

				if (mActiveSlider != overSlider)
				{
					mActiveSlider->setInactive();
					mActiveSlider = overSlider;
					overSlider->setActive(inputPos);
				}
			}
		}
		else
		{
			if (mActiveSlider != nullptr)
			{
				mActiveSlider->setInactive();
				mActiveSlider = nullptr;
			}

			if (mHoverSlider != nullptr)
			{
				mHoverSlider->setInactive();
				mHoverSlider = nullptr;
			}
		}

		if (mActiveSlider != nullptr)
		{
			mActiveSlider->update(camera, inputPos, inputRay);
		}
	}

	bool HandleSliderManager::isSliderActive() const
	{
		return mActiveSlider != nullptr;
	}

	void HandleSliderManager::_registerSlider(HandleSlider* slider)
	{
		mSliders.insert(slider);
	}

	void HandleSliderManager::_unregisterSlider(HandleSlider* slider)
	{
		mSliders.erase(slider);

		if (mActiveSlider == slider)
			mActiveSlider = nullptr;
	}
}