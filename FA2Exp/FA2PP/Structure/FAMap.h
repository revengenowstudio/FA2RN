#pragma once

#include "FATree.h"

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */

template<class _A, class _R>
struct unary_function
{
	using argument_type = _A;
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

namespace std{
	template<class _K, class _Ty, DWORD nil_addr = 0, DWORD nilrefs_addr = 0,
		class _Pr = less<_K>,class _A = allocator<_Ty> >
		class FAMap {
		public:
			typedef FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A> _Myt;
			typedef pair<const _K, _Ty> value_type;
			struct _Kfn : public unary_function<value_type, _K> {
				const _K& operator()(const value_type& _X) const
				{
					return (_X.first);
				}
			};
			class value_compare
				: public binary_function<value_type, value_type, bool> {
				friend class FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>;
			public:
				bool operator()(const value_type& _X,
					const value_type& _Y) const
				{
					return (comp(_X.first, _Y.first));
				}
			public:
				value_compare(_Pr _Pred)
					: comp(_Pred) {}
				_Pr comp;
			};
			typedef _K key_type;
			typedef _Ty referent_type;
			typedef _Pr key_compare;
			typedef _A allocator_type;
			typedef _Ty& _Tref;
			typedef FATree<_K, value_type, _Kfn, _Pr, _A, nil_addr, nilrefs_addr> _Imp;
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef typename _Imp::iterator iterator;
			typedef pair<iterator, bool> _Pairib;
			typedef pair<iterator, iterator> _Pairii;
			explicit FAMap(const _Pr& _Pred = _Pr(), const _A& _Al = _A())
				: _Tr(_Pred, false, _Al) {}
			typedef const value_type* _It;
			FAMap(_It _F, _It _L, const _Pr& _Pred = _Pr(),
				const _A& _Al = _A())
				: _Tr(_Pred, false, _Al)
			{
				for (; _F != _L; ++_F)
					_Tr.insert(*_F);
			}
			iterator begin()
			{
				return (_Tr.begin());
			}
			const iterator begin() const
			{
				return const_cast<FAMap*>(this)->begin();
			}
			iterator end()
			{
				return (_Tr.end());
			}
			const iterator end() const
			{
				return const_cast<FAMap*>(this)->end();
			}
			size_t size() const
			{
				return (_Tr.size());
			}
			size_t max_size() const
			{
				return (_Tr.max_size());
			}
			bool empty() const
			{
				return (_Tr.empty());
			}
			_A get_allocator() const
			{
				return (_Tr.get_allocator());
			}
			_Tref operator[](const key_type& _Kv)
			{
				iterator _P = insert(value_type(_Kv, _Ty())).first;
				return ((*_P).second);
			}
			_Pairib insert(const value_type& _X)
			{
				_Imp::_Pairib _Ans = _Tr.insert(_X);
				return (_Pairib(_Ans.first, _Ans.second));
			}
			iterator insert(iterator _P, const value_type& _X)
			{
				return (_Tr.insert((_Imp::iterator&)_P, _X));
			}
			void insert(_It _F, _It _L)
			{
				for (; _F != _L; ++_F)
					_Tr.insert(*_F);
			}
			iterator erase(iterator _P)
			{
				return (_Tr.erase((_Imp::iterator&)_P));
			}
			iterator erase(iterator _F, iterator _L)
			{
				return (_Tr.erase((_Imp::iterator&)_F,
					(_Imp::iterator&)_L));
			}
			size_type erase(const _K& _Kv)
			{
				return (_Tr.erase(_Kv));
			}
			void clear()
			{
				_Tr.clear();
			}
			void swap(_Myt& _X)
			{
				std::swap(_Tr, _X._Tr);
			}
			friend void swap(_Myt& _X, _Myt& _Y)
			{
				_X.swap(_Y);
			}
			key_compare key_comp() const
			{
				return (_Tr.key_comp());
			}
			value_compare value_comp() const
			{
				return (value_compare(_Tr.key_comp()));
			}
			iterator find(const _K& _Kv)
			{
				return (_Tr.find(_Kv));
			}
			const iterator find(const _K& _Kv) const
			{
				return const_cast<FAMap*>(this)->find(_Kv);
			}
			size_t count(const _K& _Kv) const
			{
				return (_Tr.count(_Kv));
			}
			iterator lower_bound(const _K& _Kv)
			{
				return (_Tr.lower_bound(_Kv));
			}
			iterator upper_bound(const _K& _Kv)
			{
				return (_Tr.upper_bound(_Kv));
			}
			_Pairii equal_range(const _K& _Kv)
			{
				return (_Tr.equal_range(_Kv));
			}
		protected:
			_Imp _Tr;
	};
	// map TEMPLATE OPERATORS
	template<class _K, class _Ty, DWORD nil_addr, DWORD nilrefs_addr, class _Pr, class _A> inline
		bool operator==(const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _X,
			const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _Y)
	{
		return (_X.size() == _Y.size()
			&& equal(_X.begin(), _X.end(), _Y.begin()));
	}
	template<class _K, class _Ty, DWORD nil_addr, DWORD nilrefs_addr, class _Pr, class _A> inline
		bool operator!=(const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _X,
			const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _Y)
	{
		return (!(_X == _Y));
	}
	template<class _K, class _Ty, DWORD nil_addr, DWORD nilrefs_addr, class _Pr, class _A> inline
		bool operator<(const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _X,
			const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _Y)
	{
		return (lexicographical_compare(_X.begin(), _X.end(),
			_Y.begin(), _Y.end()));
	}
	template<class _K, class _Ty, DWORD nil_addr, DWORD nilrefs_addr, class _Pr, class _A> inline
		bool operator>(const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _X,
			const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _Y)
	{
		return (_Y < _X);
	}
	template<class _K, class _Ty, DWORD nil_addr, DWORD nilrefs_addr, class _Pr, class _A> inline
		bool operator<=(const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _X,
			const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _Y)
	{
		return (!(_Y < _X));
	}
	template<class _K, class _Ty, DWORD nil_addr, DWORD nilrefs_addr, class _Pr, class _A> inline
		bool operator>=(const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _X,
			const FAMap<_K, _Ty, nil_addr, nilrefs_addr, _Pr, _A>& _Y)
	{
		return (!(_X < _Y));
	}
}

#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */