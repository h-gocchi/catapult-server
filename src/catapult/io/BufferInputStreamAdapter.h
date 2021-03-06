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

#pragma once
#include "Stream.h"
#include "catapult/exceptions.h"

namespace catapult { namespace io {

	/// Adapt a typed buffer to be used as an input stream.
	template<typename TContainer>
	class BufferInputStreamAdapter : public InputStream {
	public:
		/// Creates an input stream around \a input.
		explicit BufferInputStreamAdapter(const TContainer& input)
				: m_input(input)
				, m_position(0)
		{}

	public:
		/// Returns read position.
		size_t position() const {
			return m_position;
		}

	public:
		void read(const MutableRawBuffer& buffer) override {
			RawBuffer input(m_input);
			if (buffer.Size + m_position > input.Size)
				CATAPULT_THROW_FILE_IO_ERROR("BufferInputStreamAdapter invalid read");

			std::memcpy(buffer.pData, input.pData + m_position, buffer.Size);
			m_position += buffer.Size;
		}

	private:
		const TContainer& m_input;
		size_t m_position;
	};
}}
