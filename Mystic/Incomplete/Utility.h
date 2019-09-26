///  CURRENTLY TURNED OFF UNTIL FUNCTIONALITY IS PERFECTED AT WHICH TIME THIS WILL BE REPLACED
/*


#pragma once
#include <thread>
#include <vector>
#include"Common.h"

/// Dekkers and Petersons Algorithms
///==================================================
///Thread 1
// flag1 = 1 // A: Declare desire to enter Critical section
// if (flag2 != 0) // B:
// { resolve contention }
// else { enter critical section }

///Thread 2
// flag 2 = 1;
// if (flag1 != 0)// C: Declare desire to enter Critical section
// { resolve contention }// D: Enter Critical
// else { enter critical section }
/// Comments: If A can pass B or C can pass D this breaks
///- Must turn Optimization off because if code is reordered it will break
/// Flags need be Atomic


/// ================================================================================================================================================================================================================================
/// RING BUFFER
/// ================================================================================================================================================================================================================================
/*

*/
/// ================================================================================================================================================================================================================================
// namespace unpolluted { // Look into this more
// 	/*  Cannot call unqualified free functions begin() and end() from
// 		within a class with members begin() and end() without this hack. */
// 	template< typename u >
// 	auto b(u &c) -> decltype(begin(c)) { return begin(c); }
// 	template< typename u >
// 	auto e(u &c) -> decltype(end(c)) { return end(c); }
// }
/// ANOTHER ONE
/*


#include<array>



/// Value Type
/// Allocator
/// Iterator
/// Container

template<typename _Ty>
class rbIterator
{
public:

	using pointer = _Ty * ;
	using reference = _Ty & ;
	using value_type = _Ty;

	rbIterator()
		:
		data()
	{}
	rbIterator(pointer _data)
		:
		data(_data)
	{}

	explicit rbIterator(int _position = 0)
		:
		i(_position)
	{}

	int operator*() const
	{
		return i;
	}


	reference operator++()
	{
		++i;
		return *this;
	}
	bool operator !=(const rbIterator<value_type> &other) const
	{
		return i != other.i;
	}

	int location() { return i; }

	rbIterator& operator++() const
	{	// preincrement
 		++*(pointer)this->data;
		
		return (*this);
	}

	rbIterator operator++(int)
	{	// postincrement
		rbIterator<value_type> _Tmp = *this;
		++*this->data;
		return (_Tmp);
	}

	rbIterator& operator--()
	{	// predecrement
		--*(pointer)this->data;
		return (*this);
	}

	rbIterator operator--(int)
	{	// postdecrement
		rbIterator<value_type> _Tmp = *this;
		--*this->data;
		return (_Tmp);
	}


#define LOG(x) 		std::cout << x << " : " <<  typeid(*this).name() << "\n" << "Line: " << __LINE__ << "\n" <<  "Function: " << __func__ << "\n\n" << std::endl;



	rbIterator begin() noexcept
	{	// return iterator for beginning of mutable sequence
		return (rbIterator(this->_Myfirst(), std::addressof(this->Get_data())));
	}

	reference Get_data()
	{
		return data;
	}
private:
	int i;
	std::atomic<pointer> data;
};

template<typename _Ty, size_t _SZ>
class ring_buffer
{
public:
	using value_type = _Ty;
	using reference = _Ty&;
	using pointer = const _Ty*;
	using iterator = rbIterator<_Ty>;

	bool push(reference _element)
	{// Adds new Element to Queue
		std::atomic<size_t> OldWritePosition = WritePosition.load();
		std::atomic<size_t> NewWritePosition = GetPositionAfter(OldWritePosition);
		if (NewWritePosition == ReadPosition.load())
		{// Perhaps for performance make this method return void and just return on fail
			return false;
		}
		/// WRITER
		Data[OldWritePosition] = _element;
		WritePosition = NewWritePosition;
		
	}
	/// WE MIGHT BE ABLE TO PACK THE READER AND WRITER INTO THE SAME ATOMIC INTEGER WHICH WILL REDUCE THE OVERHEAD
	bool pop(reference _returnElement)
	{// Returns True if more elements in Queue
		while (true)
		{/// READER Multiple in Existance
			std::atomic<size_t> OldReadPosition = ReadPosition.load();

			if (WritePosition.load() == OldReadPosition.load())
			{// If attempting to read the same position again or buffer is full return false;
				return false;
			}

			_returnElement = Data[OldReadPosition];// Perhaps std::move() would be better for performance
			if (ReadPosition.compare_exchange_strong(OldReadPosition, GetPositionAfter(OldReadPosition)))
			{
				return true;
			}
		}
	}

	ring_buffer()
	{
 	}

	//  reference operator[](const int _Off) const
	//  {	// subscript
	//  	return (*(*this + _Off));
	//  }
	reference operator[](const int _offset)
	{
		return Data[_offset]; //[ReadPosition % BufferSize];
	}

	bool destroy()
	{
		delete[](Data);
	}
//	using _Mybase = _Vector_alloc<_Vec_base_types<_Ty, _Alloc>>;

//	using size_type = typename _Ty::size_type;
	size_t size() const noexcept
	{	// return length of sequence
///		return (static_cast<size_t>(this->WritePosition.location() - this->_Myfirst()));
	}

	size_t size()
	{
		return BufferSize;
	}

	pointer begin()
	{
		return &Data[ReadPosition.location()];
	}
	pointer end()
	{
		return &Data[WritePosition.location()];
	}

// _NODISCARD iterator begin() noexcept
// {	// return iterator for beginning of mutable sequence
// 	return (iterator(this->_Myfirst(), _STD addressof(this->_Get_data())));
// }

private:

	std::array<std::atomic<_Ty>, _SZ + 1> Data;

	iterator NextElement(size_t _pos) 
	{
		return ++_pos == BufferSize ? rbIterator{ 0 } : rbIterator{ _pos };
	}
	iterator
		ReadPosition{ 0 },
		WritePosition{ 0 };
	size_t Length, Size, Start, End, Elements;
	size_t BufferSize = _SZ + 1;
//	_Ty *Data;
	iterator MyFirst() { return Data; }
	iterator MyLast() { return Data; }
	iterator MyEnd() { return MyLast(); }
};





template<class _Ty>
class rb_value_type : public ring_buffer<_Ty, NULL>
{	// base class for vector to hold data
public:
	using value_type = typename _Ty::value_type;
	using size_type = typename _Ty::size_type;
	using difference_type = typename _Ty::difference_type;
	using pointer = typename _Ty::pointer;
	using const_pointer = typename _Ty::const_pointer;
	using reference = value_type & ;
	using const_reference = const value_type&;

	rb_value_type()
		:
		Myfirst(),
		Mylast(),
		Myend()
	{	// initialize values default
	}

	rb_value_type(pointer _first, size_t _size)
		:
		Myfirst(_first),
		Mylast(_first),
		Myend(_first + _size)
	{	// initialize values via existing array
	}
	pointer Myfirst;	// pointer to beginning of array
	pointer Mylast;	// pointer to current end of sequence
	pointer Myend;	// pointer to end of array
};


/// ================================================================================================================================================================================================================================

	//typedef _Ty value_type;
	//typedef const _Ty* pointer;
	//typedef  const _Ty& reference;
//bool isFull(){ return}
//bool isEmpty(){return}
//bool failed() { return !Data; }


//  rbIterator begin() const
//  {
//  	return rbIterator{ ReadPosition };
//  }
//  rbIterator end() const
//  {
//  	return  rbIterator{ WritePosition };
//  }
//  






/// ================================================================================================================================================================================================================================
/// LOCK FREE QUEUE
/// ================================================================================================================================================================================================================================


/*
Does not Allocate
~ PreAllocated Ring buffer

Wait free- Implement Wait in usecase if need be

push() can Invalidate Cache if resource is shared.
 lfQueue
=========
lfQueue::lfQueue();
*/
/// ================================================================================================================================================================================================================================
/// sizeof() typeid, decltype and noexcept are NEVER evauluated not even at compile time


/*
Mostly Experimental in Nature. We do need some File managing and Steam managing functions around here to lighten the work load overall.
*/
template< class T, T v>
struct integral_constant {
	static constexpr T value = v;
	constexpr operator T() const noexcept { return value; }
	constexpr T operator()() const noexcept { return value; }
};


template<typename N>
struct abs 
{
	static_assert(N != -2147483648);// INT_MIN Cxx17 | Style Guard C++17
	static constexpr int value = (N < 0) ? -N : N; // equive to return in a function
};

/// Inhearit from different baseclasses by chosing type
template<bool p, class T, class F>
struct IF_t :type_is<...> {};//p? T: F
//int const q = 1;
//IF_t<(q < 0), int, unsigned> k;
// k declared to have 1 of these 2 integer types
//IF_t<(q < 0), F, G>{}(...)
//	class D: public IF_t<(q<0), B1, B2>{...}
/// inherit from one of these two base classes

template<bool b>
using bool_const = integral_constant<bool, b>

template<typename T, size_t SZ>
class lfQueue
{
public:
	typedef T value_type;
	bool push(T& _element)
	{// Adds new Element to Queue
		std::atomic<size_t> OldWritePosition = WritePosition.load();
		std::atomic<size_t> NewWritePosition = GetPositionAfter(OldWritePosition);
		if (NewWritePosition == ReadPosition.load())
		{// Perhaps for performance make this method return void and just return on fail
			return false;
		}
		/// WRITER
		RingBuffer[OldWritePosition] = _element;
		WritePosition = NewWritePosition;
	}
	/// WE MIGHT BE ABLE TO PACK THE READER AND WRITER INTO THE SAME ATOMIC INTEGER WHICH WILL REDUCE THE OVERHEAD
	bool pop(T& _returnElement)
	{// Returns True if more elements in Queue
		while (true)
		{/// READER Multiple in Existance
			std::atomic<size_t> OldReadPosition = ReadPosition.load();
			                            
			if (WritePosition.load() == OldReadPosition.load())
			{// If attempting to read the same position again return false;
				return false;
			}

			_returnElement = RingBuffer[OldReadPosition];// Perhaps std::move() would be better for performance
			if (ReadPosition.compare_exchange_strong(OldReadPosition, GetPositionAfter(OldReadPosition)))
			{
				return true;
			}
		}
	}

	size_t size() const noexcept { return WritePosition - ReadPosition; }
	inline void clear()
	{
		RingBuffer.clear();
	}

	//size_t pushRange(InputIterator* _first, InputIterator* _last) {} // Return amount of Elements Pushed
	//size_t pushElements(InputIterator *_first, size_t _numElements) {} // Return amount of Elements Pushed

	//size_t popAll(OutputIterator* _itr) {}
//	size_t popElements(OutputIterator *_itr, size_t _elementCount) {}
//
	bool empty() { return RingBuffer.empty(); }
private:
	static inline size_t GetPositionAfter(size_t _pos) noexcept
	{
		return ++_pos == RingBufferSize ? 0 : _pos;
	}
	size_t RingBufferSize = SZ + 1;
	std::array<std::atomic<T>, SZ + 1> RingBuffer;
	std::atomic<size_t>
		ReadPosition = { 0 },
		WritePosition = { 0 };
};



/// ================================================================================================================================================================================================================================
/// MPMC Queue
/// ================================================================================================================================================================================================================================
/*
Multi Producer MultiConsumer Queue

*/
// template<typename T>
// struct entry_t
// {
// 	T Data;
// 	int Generation;
// };
// 
// template<size_t SIZE>
// class MPMCqueue
// {
// public:
// 	std::array<entry_t, SIZE> Buffer;
// 
// 	std::atomic<size_t>
// 		HeadPosition = { 0 },
// 		TailPosition = { 0 };
// };
// template<typename T> struct RelaxAtomic : atomic<T> {
// 	...function name...(..., memory_order = memory_order_relaxed) {}
// };
/// ================================================================================================================================================================================================================================

 










 

// Backup GL state
#include"Opengl.h"
#include"Common.h"


GLenum ActiveTextureUnit();
GLenum ActiveTexture();
GLuint ActiveShader();
GLuint BoundSampler();
GLuint ActiveVAO();
Vec4 ActiveViewport();

//GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
// GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
// GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
// GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
// GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
// GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
// GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
//
//GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
//GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
//GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
//GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
//