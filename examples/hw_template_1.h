template <typename T, size_t wdith>
struct hw_register {
    typedef T base_type;
    static constexpr int storage_bits = width;

    hw_register() : value{0} {}

    base_type value;
};
