#pragma once

#include <FAMemory.h>

#include <xmemory>

#ifdef  _MSC_VER
#pragma pack(push,8)
#endif  /* _MSC_VER */

namespace std {
    template<class TValue, class _D>
    struct _Bidit {
        using iterator_category = bidirectional_iterator_tag;
        using value_type = TValue;
        using difference_type = _D;
    };

    template<
        typename key_type,
        typename value_type,
        typename key_getter_type,
        typename comparator_type,
        typename allocator_type,
        DWORD nil_addr = 0,
        DWORD nilrefs_addr = 0
    >
    class FATree
    {
    protected:
        using _MyType =
            FATree<
            key_type,
            value_type,
            key_getter_type,
            comparator_type,
            allocator_type,
            nil_addr,
            nilrefs_addr
            >;

        using Generic_pointer = void*;

        enum FATreeNodeColor { _Red, _Black };
        struct FATreeNode {
            Generic_pointer* _Left, _Parent, _Right;
            value_type _Value;
            FATreeNodeColor _Color;
        };
        static FATreeNodeColor& _Color(FATreeNode* p) {
            return (FATreeNodeColor&)(*p)._Color;
        }
        static FATreeNode*& _Left(FATreeNode* p) {
            return (FATreeNode*&)(*p)._Left;
        }
        static FATreeNode*& _Parent(FATreeNode* p) {
            return (FATreeNode*&)(*p)._Parent;
        }
        static FATreeNode*& _Right(FATreeNode* p) {
            return (FATreeNode*&)(*p)._Right;
        }
        static value_type& _Value(FATreeNode* p) {
            return (value_type&)(*p)._Value;
        }
        static const key_type& _Key(FATreeNode* p) {
            return key_getter_type()(_Value(p));
        }

    public:
        class iterator;
        friend class iterator;
        using TreeNode = FATreeNode;
        using _Alnode = _Rebind_alloc_t<allocator_type, FATreeNode>;
        class iterator : public _Bidit<value_type, ptrdiff_t> {
        public:
            iterator() {}
            iterator(FATreeNode* p) :_Ptr(p) {}

            value_type& operator*() {
                return _Value(_Ptr);
            }
            value_type* operator->() {
                return &**this;
            }
			const value_type* operator->() const {
				return const_cast<iterator*>(this)->operator->();
			}
            iterator& operator++() {
                _Inc();
                return *this;
            }
            iterator operator++(int)
            {
                iterator tmp = *this;
                ++* this;
                return tmp;
            }
            iterator& operator--()
            {
                _Dec();
                return *this;
            }
            iterator operator--(int)
            {
                iterator tmp = *this;
                --* this;
                return tmp;
            }
            bool operator==(const iterator& _X) const
            {
                return (_Ptr == _X._Ptr);
            }
            bool operator!=(const iterator& _X) const
            {
                return (!(*this == _X));
            }
            void _Dec()
            {
                _Lockit _Lk;
                if (_Color(_Ptr) == _Red
                    && _Parent(_Parent(_Ptr)) == _Ptr)
                    _Ptr = _Right(_Ptr);
                else if (_Left(_Ptr) != *_Nil())
                    _Ptr = _Max(_Left(_Ptr));
                else
                {
                    FATreeNode* _P;
                    while (_Ptr == _Left(_P = _Parent(_Ptr)))
                        _Ptr = _P;
                    _Ptr = _P;
                }
            }
            void _Inc()
            {
                _Lockit _Lk;
                if (_Right(_Ptr) != *_Nil())
                    _Ptr = _Min(_Right(_Ptr));
                else
                {
                    FATreeNode* _P;
                    while (_Ptr == _Right(_P = _Parent(_Ptr)))
                        _Ptr = _P;
                    if (_Right(_Ptr) != _P)
                        _Ptr = _P;
                }
            }
            FATreeNode* _Mynode()
            {
                return _Ptr;
            }

        protected:
            FATreeNode* _Ptr;
        };;

        using _Pairib = pair<iterator, bool>;
        using _Pairii = pair<iterator, iterator>;

        FATree(noinit_t) {}
        explicit FATree(const comparator_type& _Parg, bool _Marg = true,
            const allocator_type& _Al = allocator_type())
            : _Allocator(_Al),
            _Comparator(_Parg), _Multi(_Marg)
        {
            _Init();
        }
        FATree(const value_type* _first, const value_type* _last,
            const comparator_type& _Parg, bool _Marg = true,
            const allocator_type& _Al = allocator_type())
            : _Allocator(_Al),
            _Comparator(_Parg), _Multi(_Marg)
        {
            _Init();
            insert(_first, _last);
        }
        FATree(const _MyType& _X)
            : _Allocator(_X._Allocator),
            _Comparator(_X._Comparator), _Multi(_X._Multi)
        {
            _Init();
            _Copy(_X);
        }
        FATree(_MyType&& _X)
            : _Allocator(_X._Allocator),
            _Comparator(_X._Comparator), 
            _Multi(_X._Multi),
            _Size(_X._Size)
        {
            _Head = std::exchange(_X._Head, nullptr);
            _X._Init();
        }
        ~FATree()
        {
            erase(begin(), end());
            _Freenode(_Head);
            _Head = 0, _Size = 0;
            {
                _Lockit _Lk;
            if (--*_Nilrefs() == 0)
            {
                _Freenode(*_Nil());
                *_Nil() = 0;
            }}
        }
        _MyType& operator=(const _MyType& _X)
        {
            if (this != &_X)
            {
                erase(begin(), end());
                _Comparator = _X._Comparator;
                _Copy(_X);
            }
            return (*this);
        }
        iterator begin()
        {
            return (iterator(_Lmost()));
        }
        iterator end()
        {
            return (iterator(_Head));
        }
        size_t size() const
        {
            return (_Size);
        }
        size_t max_size() const
        {
            return (_Allocator.max_size());
        }
        bool empty() const
        {
            return (size() == 0);
        }
        allocator_type get_allocator() const
        {
            return (_Allocator);
        }
        comparator_type key_comp() const
        {
            return (_Comparator);
        }
        _Pairib insert(const value_type& _V)
        {
            FATreeNode* _X = _Root();
            FATreeNode* _Y = _Head;
            bool _Ans = true;
            {
                _Lockit _Lk;
            while (_X != *_Nil())
            {
                _Y = _X;
                _Ans = _Comparator(key_getter_type()(_V), _Key(_X));
                _X = _Ans ? _Left(_X) : _Right(_X);
            }
            }
            if (_Multi)
                return (_Pairib(_Insert(_X, _Y, _V), true));
            iterator _P = iterator(_Y);
            if (!_Ans)
                ;
            else if (_P == begin())
                return (_Pairib(_Insert(_X, _Y, _V), true));
            else
                --_P;
            if (_Comparator(_Key(_P._Mynode()), key_getter_type()(_V)))
                return (_Pairib(_Insert(_X, _Y, _V), true));
            return (_Pairib(_P, false));
        }
        iterator insert(iterator _P, const value_type& _V)
        {
            if (size() == 0)
                ;
            else if (_P == begin())
            {
                if (_Comparator(key_getter_type()(_V), _Key(_P._Mynode())))
                    return (_Insert(_Head, _P._Mynode(), _V));
            }
            else if (_P == end())
            {
                _Lockit _Lk;
                if (_Comparator(_Key(_Rmost()), key_getter_type()(_V)))
                    return (_Insert(*_Nil(), _Rmost(), _V));
            }
            else
            {
                iterator _Pb = _P;
                if (_Comparator(_Key((--_Pb)._Mynode()), key_getter_type()(_V))
                    && _Comparator(key_getter_type()(_V), _Key(_P._Mynode())))
                {
                    _Lockit _Lk;
                    if (_Right(_Pb._Mynode()) == *_Nil())
                        return (_Insert(*_Nil(), _Pb._Mynode(), _V));
                    else
                        return (_Insert(_Head, _P._Mynode(), _V));
                }
            }
            return (insert(_V).first);
        }
        void insert(iterator _F, iterator _L)
        {
            for (; _F != _L; ++_F)
                insert(*_F);
        }
        void insert(const value_type* _F, const value_type* _L)
        {
            for (; _F != _L; ++_F)
                insert(*_F);
        }
        iterator erase(iterator _P)
        {
            FATreeNode* _X;
            FATreeNode* _Y = (_P++)._Mynode();
            FATreeNode* _Z = _Y;
            _Lockit _Lk;
            if (_Left(_Y) == *_Nil())
                _X = _Right(_Y);
            else if (_Right(_Y) == *_Nil())
                _X = _Left(_Y);
            else
                _Y = _Min(_Right(_Y)), _X = _Right(_Y);
            if (_Y != _Z)
            {
                _Parent(_Left(_Z)) = _Y;
                _Left(_Y) = _Left(_Z);
                if (_Y == _Right(_Z))
                    _Parent(_X) = _Y;
                else
                {
                    _Parent(_X) = _Parent(_Y);
                    _Left(_Parent(_Y)) = _X;
                    _Right(_Y) = _Right(_Z);
                    _Parent(_Right(_Z)) = _Y;
                }
                if (_Root() == _Z)
                    _Root() = _Y;
                else if (_Left(_Parent(_Z)) == _Z)
                    _Left(_Parent(_Z)) = _Y;
                else
                    _Right(_Parent(_Z)) = _Y;
                _Parent(_Y) = _Parent(_Z);
                std::swap(_Color(_Y), _Color(_Z));
                _Y = _Z;
            }
            else
            {
                _Parent(_X) = _Parent(_Y);
                if (_Root() == _Z)
                    _Root() = _X;
                else if (_Left(_Parent(_Z)) == _Z)
                    _Left(_Parent(_Z)) = _X;
                else
                    _Right(_Parent(_Z)) = _X;
                if (_Lmost() != _Z)
                    ;
                else if (_Right(_Z) == *_Nil())
                    _Lmost() = _Parent(_Z);
                else
                    _Lmost() = _Min(_X);
                if (_Rmost() != _Z)
                    ;
                else if (_Left(_Z) == *_Nil())
                    _Rmost() = _Parent(_Z);
                else
                    _Rmost() = _Max(_X);
            }
            if (_Color(_Y) == _Black)
            {
                while (_X != _Root() && _Color(_X) == _Black)
                    if (_X == _Left(_Parent(_X)))
                    {
                        FATreeNode* _W = _Right(_Parent(_X));
                        if (_Color(_W) == _Red)
                        {
                            _Color(_W) = _Black;
                            _Color(_Parent(_X)) = _Red;
                            _Lrotate(_Parent(_X));
                            _W = _Right(_Parent(_X));
                        }
                        if (_Color(_Left(_W)) == _Black
                            && _Color(_Right(_W)) == _Black)
                        {
                            _Color(_W) = _Red;
                            _X = _Parent(_X);
                        }
                        else
                        {
                            if (_Color(_Right(_W)) == _Black)
                            {
                                _Color(_Left(_W)) = _Black;
                                _Color(_W) = _Red;
                                _Rrotate(_W);
                                _W = _Right(_Parent(_X));
                            }
                            _Color(_W) = _Color(_Parent(_X));
                            _Color(_Parent(_X)) = _Black;
                            _Color(_Right(_W)) = _Black;
                            _Lrotate(_Parent(_X));
                            break;
                        }
                    }
                    else
                    {
                        FATreeNode* _W = _Left(_Parent(_X));
                        if (_Color(_W) == _Red)
                        {
                            _Color(_W) = _Black;
                            _Color(_Parent(_X)) = _Red;
                            _Rrotate(_Parent(_X));
                            _W = _Left(_Parent(_X));
                        }
                        if (_Color(_Right(_W)) == _Black
                            && _Color(_Left(_W)) == _Black)
                        {
                            _Color(_W) = _Red;
                            _X = _Parent(_X);
                        }
                        else
                        {
                            if (_Color(_Left(_W)) == _Black)
                            {
                                _Color(_Right(_W)) = _Black;
                                _Color(_W) = _Red;
                                _Lrotate(_W);
                                _W = _Left(_Parent(_X));
                            }
                            _Color(_W) = _Color(_Parent(_X));
                            _Color(_Parent(_X)) = _Black;
                            _Color(_Left(_W)) = _Black;
                            _Rrotate(_Parent(_X));
                            break;
                        }
                    }
                _Color(_X) = _Black;
            }
            _Destval(&_Value(_Y));
            _Freenode(_Y);
            --_Size;
            return (_P);
        }
        iterator erase(iterator _F, iterator _L)
        {
            if (size() == 0 || _F != begin() || _L != end())
            {
                while (_F != _L)
                    erase(_F++);
                return (_F);
            }
            else
            {
                _Lockit _Lk;
                _Erase(_Root());
                _Root() = *_Nil(), _Size = 0;
                _Lmost() = _Head, _Rmost() = _Head;
                return (begin());
            }
        }
        size_t erase(const key_type& _X)
        {
            _Pairii _P = equal_range(_X);
            size_t _N = 0;
            for (iterator itr = _P.first; itr != _P.second; ++itr)
                ++_N;
            erase(_P.first, _P.second);
            return (_N);
        }
        void erase(const key_type* _F, const key_type* _L)
        {
            for (; _F != _L; ++_F)
                erase(*_F);
        }
        void clear()
        {
            erase(begin(), end());
        }
        iterator find(const key_type& _Kv)
        {
            iterator _P = lower_bound(_Kv);
            return (_P == end()
                || _Comparator(_Kv, _Key(_P._Mynode()))
                ? end() : _P);
        }
        size_t count(const key_type& _Kv) const
        {
            _Pairii _Ans = equal_range(_Kv);
            size_t _N = 0;
            for (iterator itr = _Ans.first(); itr != _Ans.second; ++itr)
                ++_N;
            return (_N);
        }
        iterator lower_bound(const key_type& _Kv)
        {
            return (iterator(_Lbound(_Kv)));
        }
        iterator upper_bound(const key_type& _Kv)
        {
            return (iterator(_Ubound(_Kv)));
        }
        _Pairii equal_range(const key_type& _Kv)
        {
            return (_Pairii(lower_bound(_Kv), upper_bound(_Kv)));
        }
        void swap(_MyType& _X)
        {
            std::swap(_Comparator, _X._Comparator);
            if (_Allocator == _X._Allocator)
            {
                std::swap(_Head, _X._Head);
                std::swap(_Multi, _X._Multi);
                std::swap(_Size, _X._Size);
            }
            else
            {
                _MyType _Ts = *this; *this = _X, _X = _Ts;
            }
        }
        friend void swap(_MyType& _X, _MyType& _Y)
        {
            _X.swap(_Y);
        }

    protected:
        static FATreeNode** _Nil()
        {
            return reinterpret_cast<FATreeNode**>(nil_addr);
        }
        static size_t* _Nilrefs()
        {
            return reinterpret_cast<size_t*>(nilrefs_addr);
        }
        //static FATreeNode* _Nil;
        //static size_t _Nilrefs;

        void _Copy(const _MyType& _X)
        {
            _Lockit _Lk;
            _Root() = _Copy(_X._Root(), _Head);
            _Size = _X.size();
            if (_Root() != *_Nil())
            {
                _Lmost() = _Min(_Root());
                _Rmost() = _Max(_Root());
            }
            else
                _Lmost() = _Head, _Rmost() = _Head;
        }
        FATreeNode* _Copy(FATreeNode* _X, FATreeNode* _P)
        {
            _Lockit _Lk;
            FATreeNode* _R = _X;
            for (; _X != *_Nil(); _X = _Left(_X))
            {
                FATreeNode* _Y = _Buynode(_P, _Color(_X));
                if (_R == _X)
                    _R = _Y;
                _Right(_Y) = _Copy(_Right(_X), _Y);
                _Consval(&_Value(_Y), _Value(_X));
                _Left(_P) = _Y;
                _P = _Y;
            }
            _Left(_P) = *_Nil();
            return (_R);
        }
        void _Erase(FATreeNode* _X)
        {
            _Lockit _Lk;
            for (FATreeNode* _Y = _X; _Y != *_Nil(); _X = _Y)
            {
                _Erase(_Right(_Y));
                _Y = _Left(_Y);
                _Destval(&_Value(_X));
                _Freenode(_X);
            }
        }
        void _Init()
        {
            _Lockit _Lk;
            if (*_Nil() == 0)
            {
                *_Nil() = _Buynode(0, _Black);
                _Left(*_Nil()) = 0, _Right(*_Nil()) = 0;
            }
            ++*_Nilrefs();
            _Head = _Buynode(*_Nil(), _Red), _Size = 0;
            _Lmost() = _Head, _Rmost() = _Head;
        }
        iterator _Insert(FATreeNode* _X, FATreeNode* _Y, const value_type& _V)
        {
            _Lockit _Lk;
            FATreeNode* _Z = _Buynode(_Y, _Red);
            _Left(_Z) = *_Nil(), _Right(_Z) = *_Nil();
            _Consval(&_Value(_Z), _V);
            ++_Size;
            if (_Y == _Head || _X != *_Nil()
                || _Comparator(key_getter_type()(_V), _Key(_Y)))
            {
                _Left(_Y) = _Z;
                if (_Y == _Head)
                {
                    _Root() = _Z;
                    _Rmost() = _Z;
                }
                else if (_Y == _Lmost())
                    _Lmost() = _Z;
            }
            else
            {
                _Right(_Y) = _Z;
                if (_Y == _Rmost())
                    _Rmost() = _Z;
            }
            for (_X = _Z; _X != _Root()
                && _Color(_Parent(_X)) == _Red; )
                if (_Parent(_X) == _Left(_Parent(_Parent(_X))))
                {
                    _Y = _Right(_Parent(_Parent(_X)));
                    if (_Color(_Y) == _Red)
                    {
                        _Color(_Parent(_X)) = _Black;
                        _Color(_Y) = _Black;
                        _Color(_Parent(_Parent(_X))) = _Red;
                        _X = _Parent(_Parent(_X));
                    }
                    else
                    {
                        if (_X == _Right(_Parent(_X)))
                        {
                            _X = _Parent(_X);
                            _Lrotate(_X);
                        }
                        _Color(_Parent(_X)) = _Black;
                        _Color(_Parent(_Parent(_X))) = _Red;
                        _Rrotate(_Parent(_Parent(_X)));
                    }
                }
                else
                {
                    _Y = _Left(_Parent(_Parent(_X)));
                    if (_Color(_Y) == _Red)
                    {
                        _Color(_Parent(_X)) = _Black;
                        _Color(_Y) = _Black;
                        _Color(_Parent(_Parent(_X))) = _Red;
                        _X = _Parent(_Parent(_X));
                    }
                    else
                    {
                        if (_X == _Left(_Parent(_X)))
                        {
                            _X = _Parent(_X);
                            _Rrotate(_X);
                        }
                        _Color(_Parent(_X)) = _Black;
                        _Color(_Parent(_Parent(_X))) = _Red;
                        _Lrotate(_Parent(_Parent(_X)));
                    }
                }
            _Color(_Root()) = _Black;
            return (iterator(_Z));
        }
        FATreeNode* _Lbound(const key_type& _Kv) const
        {
            _Lockit _Lk;
            FATreeNode* _X = _Root();
            FATreeNode* _Y = _Head;
            while (_X != *_Nil())
                if (_Comparator(_Key(_X), _Kv))
                    _X = _Right(_X);
                else
                    _Y = _X, _X = _Left(_X);
            return (_Y);
        }
        FATreeNode*& _Lmost()
        {
            return (_Left(_Head));
        }
        FATreeNode*& _Lmost() const
        {
            return (_Left(_Head));
        }
        void _Lrotate(FATreeNode* _X)
        {
            _Lockit _Lk;
            FATreeNode* _Y = _Right(_X);
            _Right(_X) = _Left(_Y);
            if (_Left(_Y) != *_Nil())
                _Parent(_Left(_Y)) = _X;
            _Parent(_Y) = _Parent(_X);
            if (_X == _Root())
                _Root() = _Y;
            else if (_X == _Left(_Parent(_X)))
                _Left(_Parent(_X)) = _Y;
            else
                _Right(_Parent(_X)) = _Y;
            _Left(_Y) = _X;
            _Parent(_X) = _Y;
        }
        static FATreeNode* _Max(FATreeNode* _P)
        {
            _Lockit _Lk;
            while (_Right(_P) != *_Nil())
                _P = _Right(_P);
            return (_P);
        }
        static FATreeNode* _Min(FATreeNode* _P)
        {
            _Lockit _Lk;
            while (_Left(_P) != *_Nil())
                _P = _Left(_P);
            return (_P);
        }
        FATreeNode*& _Rmost()
        {
            return (_Right(_Head));
        }
        FATreeNode*& _Rmost() const
        {
            return (_Right(_Head));
        }
        FATreeNode*& _Root()
        {
            return (_Parent(_Head));
        }
        FATreeNode*& _Root() const
        {
            return (_Parent(_Head));
        }
        void _Rrotate(FATreeNode* _X)
        {
            _Lockit _Lk;
            FATreeNode* _Y = _Left(_X);
            _Left(_X) = _Right(_Y);
            if (_Right(_Y) != *_Nil())
                _Parent(_Right(_Y)) = _X;
            _Parent(_Y) = _Parent(_X);
            if (_X == _Root())
                _Root() = _Y;
            else if (_X == _Right(_Parent(_X)))
                _Right(_Parent(_X)) = _Y;
            else
                _Left(_Parent(_X)) = _Y;
            _Right(_Y) = _X;
            _Parent(_X) = _Y;
        }
        FATreeNode* _Ubound(const key_type& _Kv) const
        {
            _Lockit _Lk;
            FATreeNode* _X = _Root();
            FATreeNode* _Y = _Head;
            while (_X != *_Nil())
                if (_Comparator(_Kv, _Key(_X)))
                    _Y = _X, _X = _Left(_X);
                else
                    _X = _Right(_X);
            return (_Y);
        }
        FATreeNode* _Buynode(FATreeNode* _Parg, FATreeNodeColor _Carg)
        {
            auto&& realAloc =_Alnode(_Allocator);
            //_Alloc_construct_ptr<allocator_type> _Newnode(_Allocator);
            FATreeNode* _Newnode = realAloc.allocate(1);
            _Parent(_Newnode) = _Parg;
            _Color(_Newnode) = _Carg;
            return (_Newnode);
        }
        void _Consval(value_type* _P, const value_type& _V)
        {
            new (_P) value_type( _V);
        }
        void _Destval(value_type* _P)
        {
            _P->~value_type();
        }
        void _Freenode(FATreeNode* _S)
        {
            auto&& realAloc = _Alnode(_Allocator);
            //allocator_traits<allocator_type>::deallocate(realAloc, _S, 1);
            realAloc.deallocate(_S, 1);
        }

    public:
        allocator_type _Allocator;
        comparator_type _Comparator;
        FATreeNode* _Head;
        bool _Multi;
        size_t _Size;
    };

    /*template<
        typename key_type,
        typename value_type,
        typename key_getter_type,
        typename comparator_type,
        typename allocator_type,
        DWORD nil_addr,
        DWORD nilrefs_addr
    >
        typename FATree<typename key_type, typename value_type,
        typename key_getter_type, typename comparator_type,
        typename allocator_type, nil_addr, nilrefs_addr >::FATreeNode*
        FATree<typename key_type, typename value_type,
        typename key_getter_type, typename comparator_type,
        typename allocator_type, nil_addr, nilrefs_addr>::_Nil = nullptr;

    template<
        typename key_type,
        typename value_type,
        typename key_getter_type,
        typename comparator_type,
        typename allocator_type,
        DWORD nil_addr,
        DWORD nilrefs_addr
    >
        size_t
        FATree<typename key_type, typename value_type,
        typename key_getter_type, typename comparator_type,
        typename allocator_type, nil_addr, nilrefs_addr>::_Nilrefs = 0;*/
}

#ifdef  _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */