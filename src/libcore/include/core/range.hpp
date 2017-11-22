#ifndef _CORE_RANGE_HPP_
#define _CORE_RANGE_HPP_
# include <iterator>
namespace Core {
    template < long From, long To, long Step=1 >
    class range {
    public:
        class iterator {
        public:
            using difference_type = long;
            using value_type      = long;
            using pointer         = const long*;
            using reference       = const long&;
            using iterator_category = std::forward_iterator_tag;
            explicit iterator( long num = 0L ) : m_num( num ) {}
            iterator& operator++( ) {
                m_num = ( ( To - From ) * Step > 0 ? m_num + Step : To );
                return *this;
            }
            iterator operator ++ ( int ) {
                int n = m_num; m_num = ( ( To - From ) * Step > 0 ? m_num + Step : To );
                return iterator(n);
            }
            value_type operator * () {
                return m_num;
            }
            bool operator == ( iterator other ) const { return m_num == other.m_num; }
            bool operator != ( iterator other ) const { return m_num*Step < other.m_num*Step; }
        private:
            long m_num = From;
        };
        iterator begin() { return iterator(From); }
        iterator end  () { return iterator(To);   }
    };
}
#endif