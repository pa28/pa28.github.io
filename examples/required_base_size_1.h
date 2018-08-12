template<size_t bits>
constexpr int required_base_size() {
    static_assert(bits <= std::numeric_limits<uint64_t>::digits, "Register size not supported.");
    if constexpr (bits <= std::numeric_limits<uint8_t>::digits)
        return std::numeric_limits<uint8_t>::digits;
    if constexpr (bits <= std::numeric_limits<uint16_t>::digits)
        return std::numeric_limits<uint16_t>::digits;
    if constexpr (bits <= std::numeric_limits<uint32_t>::digits)
        return std::numeric_limits<uint32_t>::digits;
    if constexpr (bits <= std::numeric_limits<uint64_t>::digits)
        return std::numeric_limits<uint64_t>::digits;
    else
        return 0;
}
