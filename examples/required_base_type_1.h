template<int required_width>
struct required_base_type {
};

template<>
struct required_base_type<std::numeric_limits<uint8_t>::digits> {
    typedef uint8_t base_type;
};

template<>
struct required_base_type<std::numeric_limits<uint16_t>::digits> {
    typedef uint16_t base_type;
};

template<>
struct required_base_type<std::numeric_limits<uint32_t>::digits> {
    typedef uint32_t base_type;
};

template<>
struct required_base_type<std::numeric_limits<uint64_t>::digits> {
    typedef uint64_t base_type;
};

typedef typename required_base_type<required_base_size<12>()>::base_type base_type_12;