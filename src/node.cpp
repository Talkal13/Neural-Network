#include "neuron.h"

namespace std {
    template<> struct hash<edb::neuron> {
        typedef edb::neuron argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept
        {
            result_type const h1 ( std::hash<int>()(s._id) );
            return h1 << 1; // or use boost::hash_combine (see Discussion)
        }
    };
}