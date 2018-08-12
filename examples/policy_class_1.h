template<int _radix = 16, int _width = 0, char _fill = '\0', bool _show_base = false>
struct register_output_policy {
    static constexpr int radix = _radix;
    static constexpr int width = _width;
    static constexpr char fill = _fill;
    static constexpr bool show_base = _show_base;
};

struct default_register_output_policy : register_output_policy<> {
};

template <bool _check_storage_size, bool _optimize_speed>
struct register_store_policy {
    static constexpr bool check_storage_size = _check_storage_size;
    static constexpr bool optimize_speed = _optimize_speed;
};

struct default_register_store_policy : register_store_policy<true,true> {};