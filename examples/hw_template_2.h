template<int _storage_bits, typename _output_policy, typename _store_policy>
struct hw_register {
    typedef _output_policy output_policy;
    typedef _store_policy store_policy;
    typedef typename required_base_type<required_base_size<_storage_bits>(),store_policy::optimize_speed>::base_type base_type;
    static constexpr int base_type_bits = std::numeric_limits<base_type>::digits;
    static constexpr int storage_bits = _storage_bits;
    static constexpr base_type mask = static_cast<base_type>((1 << storage_bits) - 1);

    hw_register() : value{0} {}

    template<typename T, typename = typename std::enable_if_t<is_hw_register_v<T>>>
    explicit hw_register(T const &other) : value{static_cast<base_type>(other() & mask)} {}

    template <typename T, typename = typename std::enable_if_t<is_hw_register_v<T>>>
    hw_register &operator=(T const &other) {
        static_assert(not store_policy::check_storage_size || (T::storage_bits <= storage_bits),
                      "Slice too wide for register storage.");
        static_assert(T::base_type_bits <= base_type_bits, "Slice too wide for register base type.");
        value = static_cast<base_type>(other.value);
        return *this;
    }

    template<typename T, typename = typename std::enable_if_t<std::is_integral_v<T>>>
    explicit hw_register(T init_value) : value{static_cast<base_type>(init_value & mask)} {}

    template<typename T, typename = typename std::enable_if_t<std::is_integral_v<T>>>
    hw_register &operator=(T v) {
        value = static_cast<base_type>(v & mask);
        return *this;
    }

    base_type operator()() const {
        return value;
    }

    template<typename T, typename = typename std::enable_if_t<is_hw_slice_spec_v<T>>>
    auto operator[](T const &slice_spec) const {
        static_assert(not store_policy::check_storage_size || ((T::width + T::offset) <= storage_bits),
                      "Slice too wide for register storage.");
        static_assert((T::width + T::offset) <= base_type_bits, "Slice too wide for register base type.");
        typedef typename required_base_type<required_base_size<T::width + T::offset>()>::base_type new_base_type;

        return hw_slice<new_base_type, T::width, T::offset>{value};
    }

    template<typename T, typename = typename std::enable_if_t<is_hw_slice_v<T>>>
    hw_register &operator<<(T const &slice) {
        static_assert(not store_policy::check_storage_size || ((T::width + T::offset) <= storage_bits),
                      "Slice too wide for register storage.");
        static_assert((T::width + T::offset) <= base_type_bits, "Slice too wide for register base type.");
        value = (value & ~static_cast<base_type>(T::mask)) | (slice.value_in_place());
    }

    std::ostream &print_on(std::ostream &ostream) const {
        return ostream << std::setbase(output_policy::radix)
                       << std::setw(output_policy::width)
                       << std::setfill(output_policy::fill)
                       << value
                       << std::dec;
    }

    template <typename T, typename = typename std::enable_if_t<is_hw_register_v<T>>>
    bool operator==(T const &other) const {
        return value == other.value;
    }

    template <typename T, typename = typename std::enable_if_t<is_hw_register_v<T>>>
    bool operator!=(T const &other) const {
        return value != other.value;
    }

    template <typename T, typename = typename std::enable_if_t<is_hw_register_v<T>>>
    bool operator<(T const &other) const {
        return value < other.value;
    }

    template <typename T, typename = typename std::enable_if_t<is_hw_register_v<T>>>
    bool operator>(T const &other) const {
        return value > other.value;
    }

    template <typename T, typename = typename std::enable_if_t<is_hw_register_v<T>>>
    bool operator<=(T const &other) const {
        return value <= other.value;
    }

    template <typename T, typename = typename std::enable_if_t<is_hw_register_v<T>>>
    bool operator>=(T const &other) const {
        return value >= other.value;
    }

    explicit operator bool() {
        return value != 0;
    }

protected:
    base_type value;
};