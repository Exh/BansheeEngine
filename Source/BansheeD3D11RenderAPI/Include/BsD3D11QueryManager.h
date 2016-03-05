//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsD3D11Prerequisites.h"
#include "BsQueryManager.h"

namespace BansheeEngine
{
	/** @addtogroup D3D11
	 *  @{
	 */

	/**	Handles creation of DirectX 11 queries. */
	class BS_D3D11_EXPORT D3D11QueryManager : public QueryManager
	{
	public:
		/** @copydoc QueryManager::createEventQuery */
		EventQueryPtr createEventQuery() const override;

		/** @copydoc QueryManager::createTimerQuery */
		TimerQueryPtr createTimerQuery() const override;

		/** @copydoc QueryManager::createOcclusionQuery */
		OcclusionQueryPtr createOcclusionQuery(bool binary) const override;
	};

	/** @} */
}