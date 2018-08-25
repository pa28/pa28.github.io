template<typename T>
struct is_hw_register : std::false_type {
};

template<typename T>
inline constexpr bool is_hw_register_v = is_hw_register<T>::value;

template<int _storage_bits, typename _output_policy = default_register_output_policy,
        typename _store_policy = default_register_store_policy>
struct hw_register;

template<int bits, typename out_pol, typename store_pol>
struct is_hw_register<hw_register<bits, out_pol, store_pol>> : std::true_type {
};

template<typename T, typename = typename std::enable_if_t<is_hw_register_v<T>>>
explicit hw_register(T const &other);