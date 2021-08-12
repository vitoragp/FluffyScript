/*
 * fl_buffer.h
 *
 *  Created on: 12 de ago. de 2021
 *      Author: NPShinigami
 */

#ifndef FL_BUFFER_H_
#define FL_BUFFER_H_
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
		virtual const I8		readByte(U8 offset = 0) = 0;
		virtual void			nextByte() = 0;
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
		virtual const I8		readByte(U8 offset) override;
		virtual void			nextByte() override;

	private:
		I8*						m_memory;
		U32						m_cursor;
		U32						m_length;

	};
}
#endif /* FL_BUFFER_H_ */
