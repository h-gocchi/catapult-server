/**
*** Copyright (c) 2016-present,
*** Jaguar0625, gimre, BloodyRookie, Tech Bureau, Corp. All rights reserved.
***
*** This file is part of Catapult.
***
*** Catapult is free software: you can redistribute it and/or modify
*** it under the terms of the GNU Lesser General Public License as published by
*** the Free Software Foundation, either version 3 of the License, or
*** (at your option) any later version.
***
*** Catapult is distributed in the hope that it will be useful,
*** but WITHOUT ANY WARRANTY; without even the implied warranty of
*** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*** GNU Lesser General Public License for more details.
***
*** You should have received a copy of the GNU Lesser General Public License
*** along with Catapult. If not, see <http://www.gnu.org/licenses/>.
**/

#include "Validators.h"
#include "ActiveMosaicView.h"
#include "src/cache/NamespaceCache.h"
#include "catapult/cache/ReadOnlyCatapultCache.h"

namespace catapult { namespace validators {

	ActiveMosaicView::ActiveMosaicView(const cache::ReadOnlyCatapultCache& cache) : m_cache(cache)
	{}

	validators::ValidationResult ActiveMosaicView::tryGet(MosaicId id, Height height, FindIterator& iter) const {
		// ensure that the mosic is active
		const auto& mosaicCache = m_cache.sub<cache::MosaicCache>();
		if (!mosaicCache.isActive(id, height))
			return Failure_Mosaic_Expired;

		iter = mosaicCache.find(id);

		// ensure that the namespace is active
		const auto& namespaceCache = m_cache.sub<cache::NamespaceCache>();
		if (!namespaceCache.isActive(iter.get().namespaceId(), height))
			return Failure_Namespace_Expired;

		return ValidationResult::Success;
	}

	validators::ValidationResult ActiveMosaicView::tryGet(MosaicId id, Height height, const Key& owner, FindIterator& iter) const {
		auto result = tryGet(id, height, iter);
		if (!IsValidationResultSuccess(result))
			return result;

		return iter.get().definition().owner() != owner ? Failure_Mosaic_Owner_Conflict : ValidationResult::Success;
	}
}}
