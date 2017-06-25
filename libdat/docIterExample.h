//
// MIT License
//
// Copyright (c) 2017 Stellacore Corporation.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject
// to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
// AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
// IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// example for setting up iterators
/**
 * Our data structure (e.g., a linked list)
 */
template <class ValueType>;

class MyDataStructure
{
	/**
	* Inner class that describes a const_iterator and 'regular'
	iterator at the same time, depending * on the bool template
	parameter (default: true - a const_iterator)
	*/
	template<bool is_const_iterator = true>
	class ItType
		: public std::iterator<std::bidirectional_iterator_tag, ValueType>
	{
		/**
		* For const_iterator:   define DataStructurePointerType to
		be a   const MyDataStructure* * For regular iterator: define
		DataStructurePointerType to be a   MyDataStructure*
		*/
		typedef typename std::conditional<is_const_iterator, const MyDataStructure*, MyDataStructure*>::type DataStructurePointerType;

		/**
		* For const_iterator:   define ValueReferenceType to
		be a   const ValueType& * For regular iterator: define
		ValueReferenceType to be a   ValueType&
		*/
		typedef typename std::conditional<is_const_iterator, const ValueType&, ValueType&>::type ValueReferenceType;

		/**
		* Regular constructor: set up your iterator. 
		* */
		ItType(DataStructurePointerType pointer_to_datastructure) : _datastructure(pointer_to_datastructure)
		{
			// You might want to do something here, but not necessarily
		}

		/**
		* Copy constructor. Allows for implicit conversion from a
		regular iterator to a const_iterator
		*/
		ItType(const ItType<false>& other) : _datastructure(other._datastructure)
		{
			// Copy constructor. Depending on your iterator, you might want to add something here.
		}

		/**
		* Equals comparison operator
		* */
		bool operator== (const ItType& other) const
		{
			// Up to you to define
		}

		/**
		* Not-equals comparison operator * @see operator==(const
		ItType&) const
		*/
		bool operator!= (const ItType& other) const
		{
			return !(*this == other);
		}

		/**
		* Dereference operator * @return the value of the element this
		iterator is currently pointing at
		*/
		ValueReferenceType operator*()
		{
			// Up to you to define: get a reference to an element in your data structure
		}

		/**
		* Prefix decrement operator (e.g., --it)
		* */
		ItType &operator--()
		{
			// Up to you to define: move iterator backwards
		}

		/**
		* Postfix decrement operator (e.g., it--)
		* */
		ItType operator--(int)
		{
			// Use operator--()
			const ItType old(*this);
			--(*this);
			return old;
		}

		/**
		* Prefix increment operator (e.g., ++it)
		* */
		ItType &operator++()
		{
			// Up to you to define: move iterator forwards
		}

		/**
		* Postfix increment operator (e.g., it++)
		* */
		ItType operator++(int)
		{
			// Use operator++()
			const ItType old(*this);
			++(*this);
			return old;
		}

		/**
		* Make ItType<true> a friend class of ItType<false> so * the
		copy constructor can access the private member variables. 
		*/
		friend class ItType<true>;

		private:
		DataStructurePointerType _list; // store a reference to MyDataStructure

	} // end of nested class ItType

	/**
	* Shorthand for a regular iterator (non-const) type for
	MyDataStructure. 
	*/
	typedef ItType<false> iterator;

	/**
	* Shorthand for a constant iterator (const_iterator) type for
	a MyDataStructure. 
	*/
	typedef ItType<true> const_iterator;

	// (...)
} // end of class MyDataStructure

