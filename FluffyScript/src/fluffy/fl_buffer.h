#pragma once
#include <fstream>
#include "fl_defs.h"

namespace fluffy
{
	/**
	 * BufferBase
	 */

	class BufferBase
	{
	public:
		BufferBase();
		virtual					~BufferBase();

		virtual void			load(const I8* sourcePtr, const U32 len) = 0;
		virtual void			loadFromFile(const I8* fileName) = 0;
		virtual U32				getPosition() = 0;
		virtual const I8		readByte(U8 offset = 0) = 0;
		virtual void			nextByte() = 0;
		virtual void			reset(U32 position) = 0;
	};

	/**
	 * DirectBuffer
	 */

	class DirectBuffer : public BufferBase
	{
	public:
		DirectBuffer();
		virtual					~DirectBuffer();

		virtual void			load(const I8* sourcePtr, const U32 len) override;
		virtual void			loadFromFile(const I8* fileName) override;
		virtual U32				getPosition() override;
		virtual const I8		readByte(U8 offset) override;
		virtual void			nextByte() override;
		virtual void			reset(U32 position) override;

	private:
		I8* m_memory;
		U32						m_cursor;
		U32						m_length;

	};

	/**
	 * LazyBuffer
	 */

	class LazyBuffer : public BufferBase
	{
	public:
		LazyBuffer(const U32 bufferSize = 512);
		virtual					~LazyBuffer();

		virtual void			load(const I8* sourcePtr, const U32 len) override;
		virtual void			loadFromFile(const I8* fileName) override;
		virtual U32				getPosition() override;
		virtual const I8		readByte(U8 offset = 0) override;
		virtual void			nextByte() override;
		virtual void			reset(U32 position) override;

	private:
		const I8* cacheSourceFile(const I8* sourcePtr, const U32 len);

	private:
		std::ifstream			m_stream;
		String					m_cacheFileName;
		I8* m_memory;
		U32						m_cursor;
		U32						m_length;
		U32						m_fileSize;
		Bool					m_hasCache;
	};
}
