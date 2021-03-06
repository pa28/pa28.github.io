---
layout: post
comments: true
title:  "Template Programming"
date:   2018-08-24 14:04:06 -0400
categories: C++ Templates STL
excerpt_separator: <!--more-->
---
I have described early generic programming in Cfront, which used templates as a way to introduce type-safe
generic typing to the language, as gruesome. There were probably several reasons, but the one I remember
best was due to Cfront being a pre-processor.
<!--more-->
Cfront translated C++ into C which was then compiled by the C
compiler. This was quite an efficient way to get people working with C++. The C compiler, of course, knew
nothing about C++. So, unfortunately when template support was added, Cfront would have to emit all of the
possible variations of structures or functions that the C++ code body could possibly need. This could be
a huge volume of code which then had to be compiled. It wasn't very pretty. That problem was quickly fixed,
and eventually C++ got its own native compiler, and things have been improving ever since.

As the template programming situation improved, and features were added to the language over time, eventually
the C++ template language was proven to be
[Turing Complete](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.14.3670)<sup>1</sup>, which led to some
[interesting developments](http://matt.might.net/articles/c++-template-meta-programming-with-lambda-calculus/).
Those interesting developments aside, what is important about this proof is that we know that when programming in
C++ we are actually using two languages to specify our program. One is procedural<sup>2</sup> which we use to
specify and reason about the program algorithm we are coding. The other is declarative<sup>3</sup> which we
use to specify and reason about how the program should be built. This paring is an incredibly powerful tool for
the programmer who knows how to use
them effectively together. Being effective can be difficult if you are unfamiliar with the general
principles of declarative programming and the C++ template specification language in particular. But things
are getting better. There is more and more literature and training available, but it does help if you have
had some experience with a declarative language. An equally important point which must not be overlooked is
that the program that is written in the templating language is run entirely at compile time. This means that
whatever features or checks you introduce into the program through templates will have no run-time overhead.
This is also a very valuable tool for the programmer.

### Getting to the Code

I will be getting to the code shortly, but when providing sample code it is useful to have a goal in
mind. People often use made-up trivial examples. There is nothing wrong with that, but it does allow one to
gloss over the kind of very specific requirement that would really demonstrate the feature under descussion.
So I'm going to use one of my hobby projects, PiDP-8-sim, which is hosted here in my repository. I'm going
to provide some background, but if you aren't really interested feel free to [skip ahead](#getting_to_code).
You can always come back if you find you are missing something.

PiDP-8-sim is a derivative of [sim-h](http://simh.trailing-edge.com/), an historical simulator for old computer
systems. PiDP-8-sim is designed to work with the [PiDP-8](http://obsolescence.wixsite.com/obsolescence/pidp-8)
DEC PDP-8I emulator that uses
a Raspbery-Pi running sim-h. Since sim-h is written in C, much of PiDP-8-sim is C mixed with C++, all of
which is several (if not many) years old. C hasn't changed much over that time, but C++ certainly has.
Looking at the code today it is obvious that the program could be made better by using features available
from C++17. In this article I will go into the development and final implementation of the hardware
registers and memory storage.

One approach would be to write everything to directly support the PDP-8 architecture. But Obsolescence
Guaranteed is producing a PiDP-11/70 emulator; it would be nice to be able to use the same code for that or
other projects. That is why I have decided to make a generic header only library that I can use for the
PiDP-8 and easily extend to other computer types.

### Hardware Registers<a name="getting_to_code"></a>

So writing software to implement generic hardware registers. What might our requirements be? I quickly
developed the following list:
* Able to specify register size, keeping in mind that the PDP-8 has strange (for today) register sizes.
* Map efficiently to native hardware storage. Efficiency is ambiguous, I should be able to specify either
efficient storage size or efficient computation speed.
* They should be type &mdash; including size &mdash; safe.

Let's start small, if naive. This will allow us to discuss some basic principles, then we can move on.
Consider this structure:

{% highlight cpp linenos %}
template <typename T, size_t wdith>
struct hw_register {
    typedef T base_type;
    static constexpr int storage_bits = width;

    hw_register() : value{0} {}

    base_type value;
};
{% endhighlight %}

This allows us to set up a PDP-8 register by declaring `hw_register<uint16_t,12>pc;`. We have set the register size
to 12 bits, but it falls short of our type safety and storage
efficiency. But it does demonstrate a template programming technique. Lines 3 and 4 show how "variables" are
stored in classes and structs as type definitions or static constexper integral values. We don't need this
inside the struct declaration because we could use the actual template parameters. But with these values
"stored" in the struct we can access the type with `pc.base_type` and the
numeric value with `pc.width`. However we need some whay to go from the
register size to the most efficient storage for speed or size; and we have to do this at compile time. This
is where template meta programming shines through the arcane syntax. First lets compute the standard C++
type size that we need to store the register value in.

{% highlight cpp linenos %}
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
{% endhighlight %}

`required_base_size()` is a function that takes the register size in bits as
a template parameter (ensuring it is constant and defined at compile time). Line 3 ensures the code won't
try to declare registers larger than 64 bits, which is currently the largest integral type supported by C++.
Lines 4 and 5 determine if an 8 bit value is sufficiently large, and if so returns 8. Lines 6 and 7 do the
same for 16 bits, and so on. Line 13 will never be reached, but it keeps the compiler happy. Yes, more work
is needed. But now we can go from a PDP-8 register size to a size that is supported now we just need to
generate a type. If you aren't used to declarative programming this may look strange, but this is how we do
it.

{% highlight cpp linenos %}
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
{% endhighlight %}

Template meta programmers create lots of structs. Lines 1 to 3 specify the base template which will satisfy
all invokations not satisfied by an more specific matching templates. Lines 5 through 23 are the four
specialized themplates that will satisfy invocations with template parameters of 8, 16, 32 or 64. Line 25 is
a sample invocation of a 12 bit register. But this still doesn't allow us to select type optimization. For
that we need to add some more code.

{% highlight cpp linenos %}
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
{% endhighlight %}

This code takes advantage of the `uint_fastN_t` and `uint_leastN_t` types which will provide
the implementation determined type with the fastest operation or smallest sufficient size respectively.
With this we can refactor the hardware register declaration. There two mores tool we will want before we
start that process.

One of the issues with having many complex template types is controlling when a particular type should be
handled by a function. A practice has evolved that deals with this and also makes code more readable. One
tool is type support<sup>4</sup>, the other is policy based design
using policy classes. First type support:

{% highlight cpp linenos %}
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
{% endhighlight %}

Lines 1 to 3 are the generic template definition. It says that unless otherwise indicated the answer to "is
this a hw_register type?" is false. It is invoked like this: `is_hw_register<some_type>::value`. But that can
be a little cumbersome, so we add the syntactic sugar at lines 5 and 6 which allow us to accomplish the same
thing with `is_hw_register_v<some_type>`. Lines 8 to 10 are a forward declaration
of my hw_register. You don't need to forward declare to use type support, but we don't need the full
declaration for this example. Lines 12 to 14 tell the compiler that a template signature of
`hw_register<int,typename,typname>` is a hardware register. This
will let us define a copy constructor at lines 16 and 17 that will only accept hw_register types; and that
intent is clear. This may seem like a lot of work for this simple situation, but this is a powerful syntax
that you should be familiar with. You will have noticed the my hw_register declaration has two policy
classes. Let's look at those now.

Policy classes are structures which contain types or values that are used to control some behavior of the
class using them. My hw_register class has an output and storage policy classes:

{% highlight cpp linenos %}
template<int _radix = 16, int _width = 0, char _fill = '\0',
    bool _show_base = false>
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

struct default_register_store_policy :
    register_store_policy<true,true> {};
{% endhighlight %}

The output policy is controls how register contents are printed to streams. Doing it this way encapsulates
the printing function in the library but also allows users of the library some control. The storage policy
has a flags that controll storage size checking for size safety, and one to allow registers to be
_fast_ or _least_ types. So now we can fully declare hw_register. I have left out a couple of
supporting types, but those are in the repository and use these techniques.

{% highlight cpp %}
template<int _storage_bits, typename _output_policy, typename _store_policy>
struct hw_register {
    typedef _output_policy output_policy;
    typedef _store_policy store_policy;
    typedef typename required_base_type<required_base_size<_storage_bits>(),
            store_policy::optimize_speed>::base_type base_type;
    static constexpr int base_type_bits =
                    std::numeric_limits<base_type>::digits;
    static constexpr int storage_bits = _storage_bits;
    static constexpr base_type mask =
                    static_cast<base_type>((1 << storage_bits) - 1);

    hw_register() : value{0} {}

    template<typename T, typename = typename
        std::enable_if_t<is_hw_register_v < T>>>

    explicit hw_register(T const &other) :
        value{static_cast<base_type>(other() & mask)} {}

    template<typename T, typename = typename
        std::enable_if_t<is_hw_register_v < T>>>

    hw_register &operator=(T const &other) {
        static_assert(not store_policy::check_storage_size ||
            (T::storage_bits <= storage_bits),
                      "Slice too wide for register storage.");
        static_assert(T::base_type_bits <= base_type_bits,
                      "Slice too wide for register base type.");
        value = static_cast<base_type>(other.value);
        return *this;
    }

    template<typename T, typename = typename
        std::enable_if_t<std::is_integral_v < T>>>
    explicit hw_register(T init_value) :
        value{static_cast<base_type>(init_value & mask)} {}

    template<typename T, typename =
        typename std::enable_if_t<std::is_integral_v < T>>>
    hw_register &operator=(T v) {
        value = static_cast<base_type>(v & mask);
        return *this;
    }

    base_type operator()() const {
        return value;
    }

    template<typename T, typename =
        typename std::enable_if_t<is_hw_slice_spec_v < T>>>
    auto operator[](T const &slice_spec) const {
        static_assert(
                not store_policy::check_storage_size ||
                    ((T::width + T::offset) <= storage_bits),
                "Slice too wide for register storage.");
        static_assert((T::width + T::offset) <= base_type_bits,
                      "Slice too wide for register base type.");
        typedef typename required_base_type<required_base_size<
                T::width + T::offset>()>::base_type new_base_type;

        return hw_slice < new_base_type, T::width, T::offset > {value};
    }

    template<typename T, typename =
        typename std::enable_if_t<is_hw_slice_v < T>>>
    hw_register &operator<<(T const &slice) {
        static_assert(
                not store_policy::check_storage_size ||
                    ((T::width + T::offset) <= storage_bits),
                "Slice too wide for register storage.");
        static_assert((T::width + T::offset) <= base_type_bits,
                "Slice too wide for register base type.");
        value = (value & ~static_cast<base_type>(T::mask)) |
            (slice.value_in_place());
    }

    std::ostream &print_on(std::ostream &ostream) const {
        return ostream << std::setbase(output_policy::radix)
                       << std::setw(output_policy::width)
                       << std::setfill(output_policy::fill)
                       << value
                       << std::dec;
    }

    template<typename T, typename =
        typename std::enable_if_t<is_hw_register_v < T>>>
    bool operator==(T const &other) const {
        return value == other();
    }

    template<typename T, typename =
        typename std::enable_if_t<is_hw_register_v < T>>>
    bool operator!=(T const &other) const {
        return value != other();
    }

    template<typename T, typename =
        typename std::enable_if_t<is_hw_register_v < T>>>
    bool operator<(T const &other) const {
        return value < other();
    }

    template<typename T, typename =
        typename std::enable_if_t<is_hw_register_v < T>>>
    bool operator>(T const &other) const {
        return value > other();
    }

    template<typename T, typename =
        typename std::enable_if_t<is_hw_register_v < T>>>
    bool operator<=(T const &other) const {
        return value <= other();
    }

    template<typename T, typename =
        typename std::enable_if_t<is_hw_register_v < T>>>
    bool operator>=(T const &other) const {
        return value >= other();
    }

    explicit operator bool() {
        return value != 0;
    }

protected:
    base_type value;
};
{% endhighlight %}

So there you have it; how I wrote my hardware register type to be flexible, type save and easy to read. But
also notice, no **new**, no **delete**, no garbage collection and no memory leaks. More on that in another article.

## References

1. [Turing Complete](https://en.wikipedia.org/wiki/Turing_completeness): In computability theory, a system of
data-manipulation rules (such as a computer's instruction set, a programming language, or a cellular automaton)
is said to be Turing complete or computationally universal if it can be used to simulate any Turing machine.
2. [Procedural Programming](https://en.wikipedia.org/wiki/Procedural_programming): Procedural programming is a
programming paradigm, derived from structured programming, based upon the concept of the procedure call.
3. [Declarative Programming](https://en.wikipedia.org/wiki/Declarative_programming): In computer science,
declarative programming is a programming paradigm that expresses the logic of a computation without describing
its control flow.
4. [Type Support](https://en.cppreference.com/w/cpp/types): (basic types, RTTI, type traits)

[Stopping the cascade of errors](https://quuxplusone.github.io/blog/2018/08/23/stop-cascading-errors/): Arthur O???Dwyer
