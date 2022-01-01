#pragma once

#include "FATree.h"

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */

template<class TAllocator, class _R>
struct unary_function
{
	using argument_type = TAllocator;
	using result_type = _R;
};
// TEMPLATE STRUCT binary_function
template<class _A1, class _A2, class _R>
struct binary_function
{
	using first_argument_type = _A1;
	using second_argument_type = _A2;
	using result_type = _R;
};

namespace std {
	template<class TKey, class TValue, DWORD nil_addr = 0, DWORD nilrefs_addr = 0,
		class TPredicator = less<TKey>, class TAllocator = allocator<pair<const TKey, TValue>> >
		class FAMap {
		public:
			typedef FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator> _MyType;
			typedef pair<const TKey, TValue> value_type;
			struct _Kfn : public unary_function<value_type, TKey> {
				const TKey& operator()(const value_type& _X) const
				{
					return (_X.first);
				}
			};
			class value_compare
				: public binary_function<value_type, value_type, bool> {
				friend class FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>;
			public:
				bool operator()(const value_type& _X,
					const value_type& _Y) const
				{
					return (comp(_X.first, _Y.first));
				}
			public:
				value_compare(TPredicator _Pred)
					: comp(_Pred) {}
				TPredicator comp;
			};
			typedef TKey key_type;
			typedef TValue referent_type;
			typedef TPredicator key_compare;
			typedef TAllocator allocator_type;
			typedef TValue& _Tref;
			typedef FATree<TKey, value_type, _Kfn, TPredicator, TAllocator, nil_addr, nilrefs_addr> _Imp;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef typename _Imp::iterator iterator;
			typedef pair<iterator, bool> _Pairib;
			typedef pair<iterator, iterator> _Pairii;
			FAMap(noinit_t) : _Tree(noinit_t()) {}
			explicit FAMap(const TPredicator& _Pred = TPredicator(), const TAllocator& _Al = TAllocator())
				: _Tree(_Pred, false, _Al) {}
			typedef const value_type* pValue;
			FAMap(pValue first, pValue last, const TPredicator& pred = TPredicator(),
				const TAllocator& alloc = TAllocator())
				: _Tree(pred, false, alloc)
			{
				for (; first != last; ++first)
					_Tree.insert(*first);
			}
			iterator begin()
			{
				return (_Tree.begin());
			}
			const iterator begin() const
			{
				return const_cast<FAMap*>(this)->begin();
			}
			iterator end()
			{
				return (_Tree.end());
			}
			const iterator end() const
			{
				return const_cast<FAMap*>(this)->end();
			}
			size_t size() const
			{
				return (_Tree.size());
			}
			size_t max_size() const
			{
				return (_Tree.max_size());
			}
			bool empty() const
			{
				return (_Tree.empty());
			}
			TAllocator get_allocator() const
			{
				return (_Tree.get_allocator());
			}
			_Tref operator[](const key_type& _Kv)
			{
				iterator _P = insert(value_type(_Kv, TValue())).first;
				return ((*_P).second);
			}
			_Pairib insert(const value_type& _X)
			{
				_Imp::_Pairib _Ans = _Tree.insert(_X);
				return (_Pairib(_Ans.first, _Ans.second));
			}
			iterator insert(iterator _P, const value_type& _X)
			{
				return (_Tree.insert((_Imp::iterator&)_P, _X));
			}
			void insert(pValue _F, pValue _L)
			{
				for (; _F != _L; ++_F)
					_Tree.insert(*_F);
			}
			iterator erase(iterator _P)
			{
				return (_Tree.erase((_Imp::iterator&)_P));
			}
			iterator erase(iterator _F, iterator _L)
			{
				return (_Tree.erase((_Imp::iterator&)first,
					(_Imp::iterator&)last));
			}
			size_type erase(const TKey& _Kv)
			{
				return (_Tree.erase(_Kv));
			}
			void clear()
			{
				_Tree.clear();
			}
			void swap(_MyType& _X)
			{
				std::swap(_Tree, _X._Tree);
			}
			friend void swap(_MyType& _X, _MyType& _Y)
			{
				_X.swap(_Y);
			}
			key_compare key_comp() const
			{
				return (_Tree.key_comp());
			}
			value_compare value_comp() const
			{
				return (value_compare(_Tree.key_comp()));
			}
			iterator find(const TKey& _Kv)
			{
				return (_Tree.find(_Kv));
			}
			const iterator find(const TKey& _Kv) const
			{
				return const_cast<FAMap*>(this)->find(_Kv);
			}
			size_t count(const TKey& _Kv) const
			{
				return (_Tree.count(_Kv));
			}
			iterator lower_bound(const TKey& _Kv)
			{
				return (_Tree.lower_bound(_Kv));
			}
			iterator upper_bound(const TKey& _Kv)
			{
				return (_Tree.upper_bound(_Kv));
			}
			_Pairii equal_range(const TKey& _Kv)
			{
				return (_Tree.equal_range(_Kv));
			}
		protected:
			_Imp _Tree;
	};
	// map TEMPLATE OPERATORS
	template<class TKey, class TValue, DWORD nil_addr, DWORD nilrefs_addr, class TPredicator, class TAllocator> inline
		bool operator==(const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _X,
			const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _Y)
	{
		return (_X.size() == _Y.size()
			&& equal(_X.begin(), _X.end(), _Y.begin()));
	}
	template<class TKey, class TValue, DWORD nil_addr, DWORD nilrefs_addr, class TPredicator, class TAllocator> inline
		bool operator!=(const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _X,
			const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _Y)
	{
		return (!(_X == _Y));
	}
	template<class TKey, class TValue, DWORD nil_addr, DWORD nilrefs_addr, class TPredicator, class TAllocator> inline
		bool operator<(const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _X,
			const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _Y)
	{
		return (lexicographical_compare(_X.begin(), _X.end(),
			_Y.begin(), _Y.end()));
	}
	template<class TKey, class TValue, DWORD nil_addr, DWORD nilrefs_addr, class TPredicator, class TAllocator> inline
		bool operator>(const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _X,
			const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _Y)
	{
		return (_Y < _X);
	}
	template<class TKey, class TValue, DWORD nil_addr, DWORD nilrefs_addr, class TPredicator, class TAllocator> inline
		bool operator<=(const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _X,
			const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _Y)
	{
		return (!(_Y < _X));
	}
	template<class TKey, class TValue, DWORD nil_addr, DWORD nilrefs_addr, class TPredicator, class TAllocator> inline
		bool operator>=(const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _X,
			const FAMap<TKey, TValue, nil_addr, nilrefs_addr, TPredicator, TAllocator>& _Y)
	{
		return (!(_X < _Y));
	}
}

#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */