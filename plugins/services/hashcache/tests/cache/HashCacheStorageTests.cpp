#include "src/cache/HashCacheStorage.h"
#include "tests/test/core/mocks/MemoryStream.h"
#include "tests/TestHarness.h"

namespace catapult { namespace cache {

	namespace {
		using ValueType = state::TimestampedHash;
		constexpr auto Value_Size = sizeof(Timestamp) + sizeof(ValueType::HashType);
	}

	TEST(HashCacheStorageTests, CanSaveValue) {
		// Arrange:
		std::vector<uint8_t> buffer;
		mocks::MemoryStream stream("", buffer);

		// - create a random value
		ValueType originalValue;
		test::FillWithRandomData({ reinterpret_cast<uint8_t*>(&originalValue), Value_Size });

		// Act:
		HashCacheStorage::Save(originalValue, stream);

		// Assert:
		ASSERT_EQ(Value_Size, buffer.size());
		const auto& savedValue = reinterpret_cast<const ValueType&>(*buffer.data());
		EXPECT_EQ(originalValue, savedValue);

		EXPECT_EQ(0u, stream.numFlushes());
	}

	TEST(HashCacheStorageTests, CanLoadValue) {
		// Arrange:
		HashCache cache(utils::TimeSpan::FromHours(32));
		auto delta = cache.createDelta();

		std::vector<uint8_t> buffer(Value_Size);
		test::FillWithRandomData(buffer);
		mocks::MemoryStream stream("", buffer);

		// Act:
		HashCacheStorage::Load(stream, *delta);

		// Assert:
		EXPECT_EQ(1u, delta->size());
		EXPECT_TRUE(delta->contains(reinterpret_cast<const ValueType&>(*buffer.data())));
	}
}}