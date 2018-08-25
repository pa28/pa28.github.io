template<int required_width, bool optimize_speed = true>
struct required_base_type {
};

template<>
struct required_base_type<std::numeric_limits<uint8_t>::digits, true> {
    typedef uint_fast8_t base_type;
};

template<>
struct required_base_type<std::numeric_limits<uint8_t>::digits, false> {
    typedef uint_least8_t base_type;
};

template<>
struct required_base_type<std::numeric_limits<uint16_t>::digits, true> {
    typedef uint_fast16_t base_type;
};

template<>
struct required_base_type<std::numeric_limits<uint16_t>::digits, false> {
    typedef uint_least16_t base_type;
};

template<>
struct required_base_type<std::numeric_limits<uint32_t>::digits, true> {
    typedef uint_fast32_t base_type;
};

template<>
struct required_base_type<std::numeric_limits<uint32_t>::digits, false> {
    typedef uint_least32_t base_type;
};

template<>
struct required_base_type<std::numeric_limits<uint64_t>::digits, true> {
    typedef uint_fast64_t base_type;
};

template<>
struct required_base_type<std::numeric_limits<uint64_t>::digits, false> {
    typedef uint_least64_t base_type;
};

typedef typename required_base_type<required_base_size<12>(),true>::base_type base_type_fast_12;

typedef typename required_base_type<required_base_size<12>(),false>::base_type base_type_least_12;